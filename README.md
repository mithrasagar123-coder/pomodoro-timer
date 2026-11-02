# Pomodoro Timer

## Overview
Classic study timer using the Pomodoro technique, built with an OLED screen and 4 buttons to cycle between focus and breaks.

<img width="663" height="592" alt="Screenshot 2026-11-01 at 7 49 46 PM" src="https://github.com/user-attachments/assets/98add364-ed0b-446b-8d78-39258a876172" />

<img width="723" height="581" alt="Screenshot 2026-11-01 at 8 43 37 PM" src="https://github.com/user-attachments/assets/84259150-cadd-4375-928f-75449d6a5f85" />


I designed this for my Week 4 project of halflife, a 10-week program where you can design and build hardware projects free of cost!

## Parts
* Seeed Studio XIAO-ESP32-C3 as the main microcontroller
* 0.96 inch OLED Display with the timer
* Four buttons: first enters focus mode, second enters short break, third enters long break, and the last resets the timer.
* Keycaps built with CAD

## Schematic 

<img width="735" height="658" alt="Screenshot 2026-11-01 at 7 56 29 PM" src="https://github.com/user-attachments/assets/138906c3-b969-460b-bf65-e92ab9aa0f94" />

## Board

<img width="762" height="612" alt="Screenshot 2026-11-01 at 7 56 55 PM" src="https://github.com/user-attachments/assets/622ea752-983e-458f-b3d4-594ce9ddaf25" />

*(The character shown twice is Kanade Yoisaki from Project Sekai. The first image depicts her with her phone, representing break time, and the second image shows her intensely reading a book, representing focus time.)* 

[![View PCB on KiCanvas](https://hack.club/pcb-badge)](https://kicanvas.org/?repo=https://github.com/<mithrasagar123>/<pomodoro-timer>/tree/main/pcb)

## Code

| Control | What it does |
| --- | --- |
| Button 1 | Starts focus mode for 25 minutes. Once timer goes to 0, short break automatically starts. |
| Button 2 | Pauses focus timer and starts short break for 5 minutes. Focus mode will continue when short break is over. |
| Button 3 | Pauses focus timer and starts long break for 15 minutes.  |
| Button 4 | Resets all timers. |

View the entire thing at [sketch_pomodorotimer.ino](https://github.com/mithrasagar123-coder/pomodoro-timer/blob/main/sketch_pomodorotimer.ino).

## Keycaps

To be printed and placed on the buttons:
<img width="718" height="543" alt="Screenshot 2026-11-01 at 8 44 59 PM" src="https://github.com/user-attachments/assets/411ffca9-9660-4f86-a608-9a08b0d792c8" />


## BOM
