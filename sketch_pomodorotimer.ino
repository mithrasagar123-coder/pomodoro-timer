#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1 
#define SCREEN_ADDRESS 0x3C 

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Hardware Pin Definitions (GPIOs)
const int btnOne   = 2;   // KiCad Pin D0  -> Focus Mode
const int btnTwo   = 10;  // KiCad Pin D10 -> Short Break Mode
const int btnThree = 3;   // KiCad Pin D1  -> Long Break Mode
const int btnFour  = 20;  // KiCad Pin D7  -> Reset

// Timer Duration Targets (Converted to Seconds)
const unsigned long FOCUS_DURATION = 25 * 60;
const unsigned long SHORT_DURATION = 5 * 60;
const unsigned long LONG_DURATION  = 15 * 60;

// State Machine Variables
enum TimerState { IDLE, FOCUS, SHORT_BREAK, LONG_BREAK };
TimerState currentState = IDLE;

unsigned long timeRemaining = 0; // Total seconds left in active cycle
unsigned long lastTickTime = 0;  // Tracks millisecond ticks for 1-second counting

void setup() {
  Serial.begin(115200);

  // Initialize Input Matrix with Internal Pull-Ups
  pinMode(btnOne, INPUT_PULLUP);
  pinMode(btnTwo, INPUT_PULLUP);
  pinMode(btnThree, INPUT_PULLUP);
  pinMode(btnFour, INPUT_PULLUP);

  // Initialize Hardware I2C (SDA=GPIO6 / Pad 5, SCL=GPIO7 / Pad 6)
  Wire.begin(6, 7); 

  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); 
  }

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  updateDisplay();
}

void loop() {
  // --- SECTION 1: USER INPUT HANDLING ---
  
  // BUTTON ONE: Start / Continue Focus Mode
  if (digitalRead(btnOne) == LOW) {
    currentState = FOCUS;
    timeRemaining = FOCUS_DURATION;
    lastTickTime = millis();
    delay(250); // Debounce
  }
  // BUTTON TWO: Start Short Break
  else if (digitalRead(btnTwo) == LOW) {
    currentState = SHORT_BREAK;
    timeRemaining = SHORT_DURATION;
    lastTickTime = millis();
    delay(250); // Debounce
  }
  // BUTTON THREE: Start Long Break
  else if (digitalRead(btnThree) == LOW) {
    currentState = LONG_BREAK;
    timeRemaining = LONG_DURATION;
    lastTickTime = millis();
    delay(250); // Debounce
  }
  // BUTTON FOUR: System Reset
  else if (digitalRead(btnFour) == LOW) {
    currentState = IDLE;
    timeRemaining = 0;
    delay(250); // Debounce
  }

  // --- SECTION 2: TIMER ENGINE & TICK ACCOUNTING ---
  
  if (currentState != IDLE) {
    // Check if 1 true physical second (1000 ms) has passed
    if (millis() - lastTickTime >= 1000) {
      lastTickTime += 1000; // Increment baseline tracking
      
      if (timeRemaining > 0) {
        timeRemaining--;
      }
      
      // Handle the automatic transition logic when a timer hits zero
      if (timeRemaining == 0) {
        if (currentState == FOCUS) {
          // Transition rule: Focus -> Short Break
          currentState = SHORT_BREAK;
          timeRemaining = SHORT_DURATION;
        } 
        else if (currentState == SHORT_BREAK) {
          // Transition rule: Short Break -> Focus Mode
          currentState = FOCUS;
          timeRemaining = FOCUS_DURATION;
        } 
        else if (currentState == LONG_BREAK) {
          // Transition rule: Long break stops entirely and returns to standard state
          currentState = IDLE;
        }
      }
      
      // Update screen once per second while timer is running
      updateDisplay();
    }
  } else {
    // Keep updating the static idle display frame
    updateDisplay();
  }
}

// --- SECTION 3: THREE-LINE SCREEN RENDERING ---
void updateDisplay() {
  display.clearDisplay();

  // LINE 1: Header (Current Operational Mode)
  display.setTextSize(1);
  display.setCursor(0, 0);
  switch (currentState) {
    case IDLE:        display.println("     SYSTEM IDLE     "); break;
    case FOCUS:       display.println("==== FOCUS MODE ===="); break;
    case SHORT_BREAK: display.println("--- SHORT BREAK --- "); break;
    case LONG_BREAK:  display.println("... LONG BREAK ...  "); break;
  }

  // LINE 2: Middle (Large Digital Clock Readout)
  display.setTextSize(3);
  display.setCursor(20, 20); // Margins padded to center text string horizontally
  
  unsigned int minutes = timeRemaining / 60;
  unsigned int seconds = timeRemaining % 60;
  
  // Format Minute digits with leading zero padding
  if (minutes < 10) display.print("0");
  display.print(minutes);
  display.print(":");
  // Format Second digits with leading zero padding
  if (seconds < 10) display.print("0");
  display.print(seconds);

  // LINE 3: Footer (Status context helper)
  display.setTextSize(1);
  display.setCursor(0, 54);
  if (currentState == IDLE) {
    display.println("Press Button 1-3 to start");
  } else {
    display.println("Btn 4: Reset Target");
  }

  display.display();
}
