/**
 * Display5641AS Test Sketch
 * Display Integer
 */

#include <Display5641AS.h>

// Pin definitions (adjust for your board)
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

int toDisplay = 3414; // change this as you wish
Display5641AS display;

void setup(){
    display.begin(A, B, C, D, E, F, G, DP, D1, D2, D3, D4);
    display.setNumber(toDisplay); 
}

void loop(){
    display.update();
}
