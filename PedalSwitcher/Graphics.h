#pragma once

/*
print(variable,HEX) - this will print a hexidecimal number, from 0000 up to FFFF
print(variable,DEC) or print(variable) - this will print a decimal integer, from 0000 up to 9999
If you need more control, you can call writeDigitNum(location, number) - this will write the number (0-9) to a single location. 
Location #0 is all the way to the left, location #2 is the colon dots so you probably want to skip it, location #4 is all the way to the right. 
If you want a decimal point, call writeDigitNum(location, number, true) which will paint the decimal point. 
To draw the colon, usedrawColon(true or false)
P M TL BL B BR TR T
*/

int letterA = 119;
int letterB = 124;
int letterC = 57;
int letterD = 94;
int letterE = 121;
int letterF = 113;
int letterG = 125;
int letterH = 118;
int letterI = 6;
int letterJ = 30;
int letterL = 56;
int letterN = 84;
int letterO = 63;
int letterP = 115;
int letterQ = 103;
int letterR = 80;
int letterS = 109;
int letterU = 62;
int letterV = 28;
int letterZ = 91;
