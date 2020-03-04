#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

// TODO: Return the system's CPU
Processor& System::Cpu() {return cpu_;}

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() {
    vector<int> pidlist = LinuxParser::Pids();
    bool sorted = true;
    
    if (sorted) {std::sort(processes_.begin(), processes_.end());}

    for (auto pid: pidlist){
        if (std::find_if(processes_.begin(), processes_.end(),[pid](Process& n) {return n.Pid() == pid;}) == processes_.end()) {
            sorted = true;
            Process process(pid);
            processes_.emplace_back(process);
        }
    }
    return processes_; 
}

    

// TODO: Return the system's kernel identifier (string)
std::string System::Kernel() {
    return LinuxParser::Kernel();
}

// TODO: Return the system's memory utilization
float System::MemoryUtilization() {
    return LinuxParser::MemoryUtilization();
}

// TODO: Return the operating system name
std::string System::OperatingSystem() {
    return LinuxParser::OperatingSystem();
}

// TODO: Return the number of processes actively running on the system
int System::RunningProcesses() {
    return LinuxParser::RunningProcesses();
}

// TODO: Return the total number of processes on the system
int System::TotalProcesses() {
    return LinuxParser::TotalProcesses();
}

// TODO: Return the number of seconds since the system started running
long int System::UpTime() {
    return LinuxParser::UpTime();
}