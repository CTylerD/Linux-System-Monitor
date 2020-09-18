#include <string>
#include <iostream>
#include <iomanip>

#include "format.h"

using std::string;
using std::to_string;

string Format::ElapsedTime(long seconds) {
    string formatted_time;
    int secs, mins, hrs;
    string secs_string, mins_string, hrs_string;

    secs = seconds % 60;
    mins = (seconds / 60) % 60;
    hrs = seconds / 3600;

    secs_string = LeadingZero(secs);
    mins_string = LeadingZero(mins);
    hrs_string = LeadingZero(hrs);

    formatted_time = hrs_string + ":" + mins_string + ":" + secs_string;
    return formatted_time;
}

string Format::LeadingZero(int timeunit) {
    string timeunit_string;

    if (timeunit < 10) {
        timeunit_string = "0" + to_string(timeunit);
        return timeunit_string;
    } else {
        timeunit_string = to_string(timeunit);
        return to_string(timeunit);
    }
}