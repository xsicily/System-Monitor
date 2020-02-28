#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

// TODO: Return this process's ID
int Process::Pid() {
    return pid_;
}

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() {
    float uptime = LinuxParser::UpTime();
    long total_time = LinuxParser::ActiveJiffies(pid_);
    float starttime;

    std::string line;
    std::ifstream filestream(LinuxParser::kProcDirectory + std::to_string(pid_) + LinuxParser::kStatFilename);
    if (filestream.is_open()) {
        while (std::getline(filestream, line)) {
            std::istringstream linestream(line);
            std::istream_iterator<std::string> beg(linestream), end;
            std::vector<std::string> values(beg, end); // index starts from 0
            starttime = std::stof(values[21]);
            break;
        }
    }

    float seconds = uptime - starttime/sysconf(_SC_CLK_TCK);
    float cpu = total_time/sysconf(_SC_CLK_TCK)/seconds;
    return cpu;
}

// TODO: Return the command that generated this process
string Process::Command() {
    return this->cmd = LinuxParser::Command(pid_);
}

// TODO: Return this process's memory utilization
string Process::Ram() {
    return this->ram = LinuxParser::Ram(pid_);
}

// TODO: Return the user (name) that generated this process
string Process::User() {
    return this->user = LinuxParser::User(pid_);
}

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() {
    return this->uptime = LinuxParser::UpTime(pid_);
}

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const {
    return this->ram < a.ram;
}