# BitMath-Game
A binary-logic reflex game built with Arduino.

### Disclaimer
This project was developed in the scope of the subject of Informatic Technologies (TI) in my first year of the Informatics Engineering degree. This accounts for the mixed use of portuguese and english in the comments, variables and function names.

# Overview
BitMath is an educational Arduino game that teaches binary logic (AND, OR, XOR) using LEDs and a button.
The player must:
- View two 4-bit numbers displayed on LEDs
- Check which logic operator (via RGB LED) will be applied
- Compute the result in decimal
- Press the button exactly that many times within the time limit
If the number of button presses matches the correct result → WIN
Otherwise → LOSE
A long button press resets the game.


# 🔌 Hardware Setup
Required Components:
- 1 × Arduino Uno (or compatible)
- 1 × Push button
- 1 × RGB LED (common cathode or anode, depending on wiring)
- 8 × Standard LEDs (to show the bits of the two numbers)
- 11 × Resistors (≈220Ω)
- Breadboard + jumper wires

The components must be placed like the image below:
<img width="932" height="607" alt="image" src="https://github.com/user-attachments/assets/2fae9d8c-6859-471f-aa11-e7b88e5823c6" />

# 🎮 How to Play
## 1) Start the game
- Upload the code to the Arduino
- Open the Serial Monitor at 9600 baud
- Press the button once
Two binary numbers light up, and the RGB LED indicates the operation

## 2) Interpret the display
- Look at the 4 LEDs for Number 1
- Look at the 4 LEDs for Number 2
- Check the RGB LED color to see the operator:
  - Green → XOR
  - Blue → OR
  - Red → AND

Example:
Number1 = 1010 (10)
Number2 = 0111 (7)
Operator = AND (red)

Compute:
1010 AND 0111 = 0010 → decimal 2

## 3) Enter your answer
Press the button exactly the number of times equal to the result.

## 4) Beat the timer
Total time: 7 seconds (with midpoint and 75% warnings printed on Serial) 

### Messages:
- “Passou metade do tempo!”
- “Passou 75% do tempo!”

## 5) Win or Lose
If clicks == correct result → WIN! (LED celebration animation)
Otherwise → LOSE (RGB blinking)

## 6) Reset the game
Hold the button >1 second to reset.
The LEDs run a sweep animation, and new random values are generated.

# 🛠️ Uploading & Running
1) Make sure all hardware requirements are met
2) Doenload the Arduino IDE (https://www.arduino.cc/en/software/)
3) Clone the repository or download the zip file
   ```
   git clone <copied_url_from_code_button>
   ```
4) Open Arduino IDE
5) Load bit_math_game.cpp
6) Select your Arduino board and COM port
7) Click Upload
8) Open Serial Monitor @ 9600 baud
9) Press button to start playing!


