#include <string>
#include <iostream>

#include "format.h"

using std::string;
using std::to_string;
using std::cout;

// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long elapsedTime) {
    long hours = elapsedTime / 3600;
    float mins = ((elapsedTime / 3600.0) - hours) * 60;
    int minutes = (int) mins;
    int seconds = (int) ((mins - minutes) * 60);
    string timeExpressed;
    timeExpressed.append(to_string(hours));
    timeExpressed.append(":");
    timeExpressed.append(to_string(minutes));
    timeExpressed.append(":");
    timeExpressed.append(to_string(seconds));
    return timeExpressed;
}