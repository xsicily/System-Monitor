#include "processor.h"
#include "linux_parser.h"
#include<thread>


// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
    float totalTime = LinuxParser::Jiffies();
    float activeTime = LinuxParser::ActiveJiffies();
    float cpu_ = activeTime/totalTime;
    return cpu_;
}

