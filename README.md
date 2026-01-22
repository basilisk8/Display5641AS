# Display5641AS
# Author : basilisk8

# Features
 - Non blocking multiplexing
 - Display integers from 0 - 9999
 - Display float from 0.000 - 9999.
 - Constant Refresh Rate of 125Hz (When update is called every 2ms or faster)
 - Simple API

# Hardware Setup
5641AS pinout
```javascript
                      D1             a             f            D2            D3              b
                      /              /             /            /             /               /
                      /              /             /            /             /               /
,.............................@.............................#............................/,............................@
,.                            @                             #                            /,                            @
,.           Digit 1          @           Digit 2           #           Digit 3          /,           Digit 4          @
,.                            @                             #                            /,                            @
,.        -@@@@@@@@@@@@.      @        -@@@@@@@@@@@@.       #        -@@@@@@@@@@@@.      /,        -@@@@@@@@@@@@.      @
,.       *@,     a    #@@.    @       &@@           @@@     #       @@%           @@(    /,      ,@@*          (@@.    @
,.      &@@           @@@  b  @       @@%           @@(     #      *@@*          (@@.    /,      #@@           &@@     @
,.  f   @@%           @@(     @      *@@*          (@@.     #      #@@           &@@     /,      @@&           @@%     @
,.     *@@*          (@@      @      %@@           @@@      #      @@&           @@%     /,     .@@*          *@@,     @
,.        *@@@@@@@@@*         @         *@@@@@@@@@*         #        *@@@@@@@@@*         /,        *@@@@@@@@@*         @
,.     @@&,.   g   .,@@#      @     @@&,.       .,@@#       #      @@&,.       .,@@#     /,     @@&,.       .,@@#      @
,.     @@*          *@@,      @     #@@           &@@       #     @@@           @@%      /,     @@(          ,@@*      @
,. e  #@@           &@@   c   @     @@@           @@%       #     @@(          ,@@*      /,    /@@.          #@@       @
,.    @@&           @@%       @    .@@#          ,@@*       #    /@@.          #@@       /,    @@@           @@&       @
,.   .@@#          *@@*       @    /@@.          #@@        #    @@@           @@&       /,    @@%          .@@/       @
,.      ;@@@@cle@@@@@@;   .@@&   @       ;@@@@@@@@@@;   .@@&   #      ;@@@@@@@@@@;   .@@&   /,      ;@@@@@@@@@@;   .@@&   @
,.            d         dot   @                             #                            /,                            @
,.                            @                             #                            /,                            @
,,............................@.............................%............................/,............................@
                      /              /             /            /             /               /
                      /              /             /            /             /               /
                      e              d            dot           c             g               D
```
Pinout diagram from : https://github.com/adrian200223/Simple5641AS

 - Connect each pin on the 5641AS to a pin on your microcontroller

# Installation
## Arduino IDE
 - Download the repo as a .ZIP file
 - Sketch -> Include Library -> Add .ZIP library
 - Select the downloaded .ZIP file

# Setup
```cpp
#include <Display5641AS.h>
Display5641AS display;

// Adjust for your board
const int A = 22;
const int B = 5;
const int C = 26;
const int D = 33;
const int E = 32;
const int F = 21;
const int G = 27;
const int DP = 25;
const int D1 = 23;
const int D2 = 19;
const int D3 = 18;
const int D4 = 14;

void setup() {
    Serial.begin(115200); // Change baud rate according to your board
    display.begin(A, B, C, D, E, F, G, DP, D1, D2, D3, D4);
    // You can use these methods anywhere in the code, I am including it in the setup
    display.setNumber(1234);
    // display.setFloat(12.34, 2);
    // display.clearScreen();
}

void loop(){
    display.update();
}
```
# API
| Method | Description |
|--------|--------------|
| begin(A, B, C, D, E, F, G, DP, D1, D2, D3, D4) | Initialize every pin for the 5641AS. All these pins are set to OUTPUT
| setNumber(number)                              | Sets a number to be displayed. NOTE : This does NOT render the number, it simply sets the number to be shown |
| setFloat(number, numbersAfterDecimal) | Sets the float to be displayed NOTE : This does NOT render the number, it simply sets the number to be shown | 
| update() | Renders the display to display the digits set. Will display either 0000 or random values if no int or number is set using setNumber or setFloat
| clearDisplay() | Clears the display |

# Limitations
 - Shows any integer between 0 - 9999
 - Shows any float between 0.000 - 9999.
 - Does not support negative numbers
 - Numbers greater than 9999 will crash the program with indexOutOfBounds error or automatically truncate
 - Can only show 3 numbers after decimal point
 - leading zeros are not automatically added, but any number with leading zeros, like 089 will display 089

# License
MIT
