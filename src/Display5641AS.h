#ifndef DISPLAY5641AS_H
#define DISPLAY5641AS_H
#include <Arduino.h>

class Display5641AS {
    public:
        void begin(int A, int B, int C, int D, int E, int F, int G, int DP, int D1, int D2, int D3, int D4Dig); // initialize the 5641AS, pass in pin numbers for segment A to segment G, Decimal point and all the digits
        void setNumber(int number); // Pass in any intetger from 0 to 9999
        void setFloat(float number, int digitsAfterDecimal); // pass in any float from 0.000 to 9999. second parameter is how many decimal digits you want to display
        void update(); // refresh the display, must be called every 2ms for a smooth visual
        void clearDisplay(); // clear the displayed values
    private:
        void showDigit(int digit, int digitPin);
        void setDigit(int digitPin);
        int D1_;
        int D2_;
        int D3_;
        int D4_;
        int A_;
        int B_;
        int C_;
        int D_;
        int E_;
        int F_;
        int G_;
        int DP_;
        int _digitPins[4];
        int _segmentPins[8];
        int _digitNumber[4];
        int _currentDigit;
        int _decimalDigit;
        byte _digitPatterns[12][8];
        unsigned long _prevMillis;
};
#endif