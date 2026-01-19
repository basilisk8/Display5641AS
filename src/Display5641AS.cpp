/**
 * Display5641AS Library
 * Non-blocking driver for 5641AS 4-digit 7-segment display
 * @author Basilisk8
 * @version 1.0
 */

#include "Display5641AS.h"
#include <Arduino.h>

void Display5641AS::begin(int A, int B, int C, int D, int E, int F, int G, int DP, int D1, int D2, int D3, int D4){
    // Pins declaration
    A_ = A;
    B_ = B;
    C_ = C;
    D_ = D;
    E_ = E;
    F_ = F;
    G_ = G;
    DP_ = DP;
    D1_ = D1;
    D2_ = D2;
    D3_ = D3;
    D4_ = D4;

    // add the pins in an array to be able to use a for loop to loop through digits or segements
    _digitPins[0] = D1;
    _digitPins[1] = D2;
    _digitPins[2] = D3;
    _digitPins[3] = D4;

    _segmentPins[0] = A;
    _segmentPins[1] = B;
    _segmentPins[2] = C;
    _segmentPins[3] = D;
    _segmentPins[4] = E;
    _segmentPins[5] = F;
    _segmentPins[6] = G;
    _segmentPins[7] = DP;
    // An array to store the mapping of specific numbers, 1 means the segment turns on, 0 means it's off
    // DP is always off here because it is treated as another 'digit' to multiplex, so it is rendered seperately
    byte digitPatterns[12][8] = {
    //A  B  C  D  E  F  G DP
    {1, 1, 1, 1, 1, 1, 0, 0}, // 0
    {0, 1, 1, 0, 0, 0, 0, 0}, // 1 
    {1, 1, 0, 1, 1, 0, 1, 0}, // 2
    {1, 1, 1, 1, 0, 0, 1, 0}, // 3
    {0, 1, 1, 0, 0, 1, 1, 0}, // 4
    {1, 0, 1, 1, 0, 1, 1, 0}, // 5
    {1, 0, 1, 1, 1, 1, 1, 0}, // 6
    {1, 1, 1, 0, 0, 0, 0, 0}, // 7
    {1, 1, 1, 1, 1, 1, 1, 0}, // 8
    {1, 1, 1, 1, 0, 1, 1, 0}, // 9
    {0, 0, 0, 0, 0, 0, 0, 1}, // decimal
    {0, 0, 0, 0, 0, 0, 0, 0}  // blank
    };

    _prevMillis = 0; // millis timer, to make sure update is called only after 2ms
    _currentDigit = 0; // the current digit to turn on
    _decimalDigit = -1; // the decimal digit, -1 for no decimal

    // copy the mapping variable into the global class variable
    for (int i = 0; i < 12; i++){
        for (int j = 0; j < 8; j++){
            _digitPatterns[i][j] = digitPatterns[i][j];
        }
    }

    // Set every pin to OUTPUT
    for (int i = 0; i < 8; i++){
        pinMode(_segmentPins[i], OUTPUT);
    }
    for (int i = 0; i < 4; i++){
        pinMode(_digitPins[i], OUTPUT);
    }
}

// loop through every segment and turn it Off (LOW) and every digit off (HIGH)
// turns the entire display off
void Display5641AS::clearDisplay(){
    for (int i = 0; i < 4; i++){
        digitalWrite(_digitPins[i], HIGH);
    }
    for (int i = 0; i < 8; i++){
        digitalWrite(_segmentPins[i], LOW);
    }
}

// pass in any int from 0 to 9999 to show on the display, calling this method just sets these numbers in the background
// call update every 2ms to actually turn on the segments to show the number
void Display5641AS::setNumber(int number){
    _decimalDigit = -1; // reset the decimal digit, so the decimal doesn't trail and show up when it is supposed to show a number
    for (int i = 0; i < 4; i++){
        _digitNumber[i] = -1; // reset every value, just so no numbers trail if numbers change
    }
    String numStr = String(number); // convert to string so no leading zeros are rendered

    // for loop to split a given number into it's individual numbers
    for (int i = 0; i < numStr.length(); i++){
        _digitNumber[3-i] = (number / (int)pow(10, i)) % 10; 
        // example : number = 5380
        // _digitNumber[3-0] = (5380) / 10^0 % 10 = 0
        // _digitNumber[3-1] = (5380) / 10^1 % 10 = 538 % 10 = 8
        // _digitNumber[3-2] = (5380) / 10^2 % 10 = 53 % 10 = 3
        // _digitNumber[3-3] = (5380) / 10^3 % 10 = 5 % 10 = 5
    }
    // set every missing value to blank so nothing shows, instead of a 0
    for (int i = 0; i < 4; i++){
        if (_digitNumber[i] == -1){
            _digitNumber[i] = 11; // set to 11 because _digitPattern[11] is storing the mapping for a blank digit
        }
    }
}

// pass in a float from 0.000 to 9999. to show on the display
// this method also just defines the numbers to show, it does not render these nubmers on display
// to render the number call update() every 2ms to show
void Display5641AS::setFloat(float number, int numbersAfterDecimal){
    _decimalDigit = -1; // reset _decimalDigit just to we start with a clean slate
    String numStr = String(number, numbersAfterDecimal); // convert the number into a string for easy string manipulation
    int decimalIndex = numStr.indexOf('.'); // find the index of the decimal point
    _decimalDigit = _digitPins[3-numbersAfterDecimal];
    // example number = 12.56 numbersAfterDecimal = 2
    // _decimalDigit = _digitPins[3-2] = D2
    // so then the 5641AS will show the decimal on the 2nd digit
    numStr.remove(decimalIndex,1); // remove the decimal point from the string
    setNumber(numStr.toInt()); // get the raw numbers using setNumber()
    _decimalDigit = _digitPins[3-numbersAfterDecimal]; // set decimalDigit back because setNumber resets it
}

// just call this method to refresh 1 digit of the display
// refresh rate is a constant 125Hz no matter how fsat this method is called
// calling this method slower than every 2ms will cause visible flickering
void Display5641AS::update(){
    unsigned long currentMillis = millis(); // get current millis
    if (currentMillis - _prevMillis >= 2){ // to ensure that the display is updated every 2ms, just to keep a stable refresh rate
        _prevMillis = currentMillis; 
        showDigit(_digitNumber[_currentDigit], _digitPins[_currentDigit]); // show the number at current index
        _currentDigit++; // move to the next digit
        _currentDigit %= 4; // after 3 back to 0
    }
}


void Display5641AS::setDigit(int digitPin){
    // passed in parameter is for the digit that should be turned on
    // go through the _digitPins loop and turn of every digit that doesn't match digitPin
    // if a digit matches then turn on digitPin
    for (int i = 0; i < 4; i++){
        if(digitPin == _digitPins[i]){
            digitalWrite(digitPin, LOW);
        }
        else{
            digitalWrite(_digitPins[i], HIGH);
        }
    }

}

void Display5641AS::showDigit(int digit, int digitPin){
    setDigit(digitPin); // turn on the digit based on the digitPassed in by update()
    for(int i = 0; i < 8; i++) { // go through the mapping array, and turn a segment on or off based on the mapping
        digitalWrite(_segmentPins[i], _digitPatterns[digit][i]);
    }
    if (digitPin == _decimalDigit){
        digitalWrite(DP_, HIGH); // if the decimal digit matches the digit passed in then turn on the decimal digit
    }
}