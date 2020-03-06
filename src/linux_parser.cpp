#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os;
  string version;
  string number;

  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> number;
  }
  return number;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {  
  string line;
  string key;
  string value1;
  string value2;

  float memTotal;
  float memFree;
  float buffers;

  std::ifstream ifstream(kProcDirectory + kMeminfoFilename);

  if (ifstream.is_open()) {
    while (std::getline(ifstream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value1 >> value2){
        if (key == "MemTotal:"){
          memTotal = std::stof(value1);
        }
        if (key == "MemFree:"){
          memFree = std::stof(value1);
        }
        if (key == "Buffers:"){
          buffers = std::stof(value1);
        }
      }                
    }
    float utilization = 1.0-(memFree/(memTotal-buffers));
    return utilization;
  }
  ifstream.close();
  return 0;
}




// TODO: Read and return the system uptime
long LinuxParser::UpTime() {
  string line;
  float upTime;
  float idleTime;
  std::ifstream ifstream(kProcDirectory + kUptimeFilename);
  if (ifstream.is_open()) {
    std::getline(ifstream, line);
    std::istringstream linestream(line);
    linestream >> upTime >> idleTime;
  }
  ifstream.close();
  return upTime;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { 
  return LinuxParser::ActiveJiffies() + LinuxParser::IdleJiffies(); 
  }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) {
  string line;
  std::ifstream ifstream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (ifstream.is_open()) {
    while (std::getline(ifstream, line)) {
      std::istringstream linestream(line);
      std::istream_iterator<std::string> beg(linestream), end;
      std::vector<std::string> values(beg, end); // index starts from 0
      return (std::stof(values[13]) + 
              std::stof(values[14]) +
              std::stof(values[15]) +
              std::stof(values[16]));
      }
    }
    ifstream.close();
    return 0;
  }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
  return (std::stof(LinuxParser::CpuUtilization()[kUser_]) + 
          std::stof(LinuxParser::CpuUtilization()[kNice_]) + 
          std::stof(LinuxParser::CpuUtilization()[kSystem_]) + 
          std::stof(LinuxParser::CpuUtilization()[kIRQ_]) +
          std::stof(LinuxParser::CpuUtilization()[kSoftIRQ_]) +
          std::stof(LinuxParser::CpuUtilization()[kSteal_]) +
          std::stof(LinuxParser::CpuUtilization()[kGuest_]) +
          std::stof(LinuxParser::CpuUtilization()[kGuestNice_]));
}



// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
  return (std::stof(LinuxParser::CpuUtilization()[kIdle_]) + 
          std::stof(LinuxParser::CpuUtilization()[kIOwait_]));
}


// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  string line;
  string name = "cpu";
  std::ifstream ifstream(kProcDirectory + kStatFilename);
  if (ifstream.is_open()) {
    while (std::getline(ifstream, line)) {
      std::istringstream linestream(line);
      if (line.find(name) == 0) {
        std::istream_iterator<std::string> beg(linestream), end;
        std::vector<std::string> values(beg, end); // index starts from 0
        std::vector<std::string> subvector = {values.begin() + 1, values.end()};
        return subvector;
      }
    }
  }
  ifstream.close();
  return (vector<string>());
} 

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  string line;
  string key;
  float value;
  std::ifstream ifstream(kProcDirectory + kStatFilename);
  if (ifstream.is_open()) {
    while (std::getline(ifstream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "processes") {
          return value;
        }
      }
    }
  }
  ifstream.close();
  return value;
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  string line;
  string key;
  string value;
  std::ifstream ifstream(kProcDirectory + kStatFilename);
  if (ifstream.is_open()) {
    while (std::getline(ifstream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "procs_running") {
          return std::stoi(value);
        }
      }
    }
  }
  ifstream.close();
  return std::stoi(value);
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {
  string line;
  string key;
  string value;
  std::ifstream ifstream(kProcDirectory  + std::to_string(pid) + kCmdlineFilename);
  if (ifstream.is_open()) {
    std::getline(ifstream, line);
    return line;
  }
  ifstream.close();
  return line;
}
  

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) {
  string line;
  string key;
  string value;
  std::ifstream ifstream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (ifstream.is_open()) {
    while (std::getline(ifstream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "VmSize:") {
          return std::to_string(int(std::stof(value)/float(1024))); // convert into megabytes
        }
      }
    }
  }
  ifstream.close();
  return "";
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {
  string line;
  string name = "Uid:";
  std::ifstream ifstream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (ifstream.is_open()) {
    while (std::getline(ifstream, line)) {
      std::istringstream linestream(line);
      if (line.find(name) == 0) {
        std::istream_iterator<std::string> beg(linestream), end;
        std::vector<std::string> values(beg, end); // index starts from 0
        return values[2];
      }
    }
  }
  ifstream.close();
  return "";
;
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) { 
  string line;
  string key;
  string value;
  std::ifstream ifstream(kPasswordPath);
  string findString;
  findString = "x:" + LinuxParser::Uid(pid);

  if (ifstream.is_open()) {
    std::getline(ifstream, line);
    std::istringstream linestream(line);
    ///etc/passwd contains the information necessary to match the UID to a username.
    if (line.find(findString) != std::string::npos){
      return line.substr(0, line.find(":"));
    }
  }
  ifstream.close();
  return 0;
}
// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) {
  string line;
  long int pidTime;

  std::ifstream ifstream(kProcDirectory  + std::to_string(pid) + kStatFilename);

  if (ifstream.is_open()) {
    std::getline(ifstream, line);
    std::istringstream linestream(line);
    std::istream_iterator<std::string> beg(linestream), end;
    std::vector<std::string> values(beg, end); // index starts from 0
    pidTime = long(std::stof(values[21])/sysconf(_SC_CLK_TCK));
    return pidTime;
  }
  ifstream.close();
  return 0;
}