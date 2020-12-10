#include <string>

#include "format.h"

using std::string;

// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS

string Format::ElapsedTime(long seconds) { 
    
    string H, M, S, h, m, s;

    h = std::to_string(seconds/3600);
    m = std::to_string((seconds%3600)/60);
    s = std::to_string(seconds%60);

    //Prepend additional '0' to always return the HH:MM:SS time format
    //For uptime >= 100 hours formatting adjusts to HHH, HHHH, etc..
    h.length()<2 ? (H = string(2 - h.length(), '0') + h) : H = h;
    M = string(2 - m.length(), '0') + m;
    S = string(2 - s.length(), '0') + s;

    return (H+":"+M+":"+S);
}
