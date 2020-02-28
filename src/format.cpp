#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long int input_seconds) {
    long minutes = input_seconds / 60;
    long hours = minutes /60;
    long seconds = input_seconds % 60;
    minutes = minutes % 60;
    string time;

    time = std::to_string(hours) + ":" + std::to_string(minutes) + ":" + std::to_string(seconds);

    return time; 
}