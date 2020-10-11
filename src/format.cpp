#include <string>
#include<sstream>
#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long iseconds)
{   
    int hours = iseconds / 3600;
    int minutes = (iseconds - 3600*hours) / 60;
    int seconds = iseconds % 60;

    char out[1024];
    sprintf(out, "%02d:%02d:%02d", hours, minutes, seconds);

    return out;
}