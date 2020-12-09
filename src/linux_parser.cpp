#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <filesystem>
#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;
//namespace fs = std::filesystem;

const int Hertz = sysconf(_SC_CLK_TCK);
const float kbMB = 1024.0;

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
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  //bool is_directory( const fs::path& p );
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
  
  float memTotal, memFree ;
  string key, value, line;

  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "MemTotal:") { memTotal = stof(value);}
        if (key == "MemFree:") { memFree = stof(value);}
      }
    }
  }
  return (memTotal - memFree)/memTotal;
}  

// TODO: Read and return the system uptime
long LinuxParser::UpTime() { 
  
  string uptime;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime;
  }
  return stol(uptime); 
  
}
  

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// TODO: Read and return CPU utilization 
vector<string> LinuxParser::CpuUtilization() { 
  
  string line, key, value;
  vector<string> procStat;

  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line); //only first line in file of interest
    std::istringstream linestream(line);
    while (linestream >> value) {
      if (value != "cpu") {
        procStat.push_back(value);
      }
    }
  }  
  return procStat;
}


// TODO: Read and return CPU utilization (the CPU utilization per process)
 float LinuxParser::CpuUtilization(int pid) {

  string value, line;
  float utime, stime, cutime, cstime, starttime;
  
  std::ifstream stream(kProcDirectory + "/"+ to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) { 
      std::istringstream linestream(line);
      
      for (int i=0; i<22; i++){
        linestream >> value;
        if (i == TimeValues::kUtime_){
          utime = stof(value);
        }
        else if (i == TimeValues::kStime_){
          stime = stof(value);
        }
        else if (i == TimeValues::kCUtime_){
          cutime = stof(value);
        }
        else if (i == TimeValues::kCStime_){
          cstime = stof(value);
        }
        else if (i == TimeValues::kStarttime_){
          starttime = stof(value);
        }
      }
    } 
  }  
    
  long uptime = LinuxParser::UpTime();
  float totalTime = utime + stime + cutime + cstime;
  float tSec = uptime - (starttime / Hertz);

  return ((totalTime / Hertz) / tSec);
}  


// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
    
  string line, key;
  int value;

  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "processes") {
          return value;
      }
    }
  }
}
return value;
  
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  
  string line, key;
  int value;

  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "procs_running") {
          return value;
        }
      }
    }
  }
  return value;
}
  
  
// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) { 
  
  string line, value;
  
  std::ifstream stream(kProcDirectory + "/" + to_string(pid) + kCmdlineFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::replace(line.begin(), line.end(), ' ', '_');
    std::istringstream linestream(line);
    linestream >> value;
    std::replace(value.begin(), value.end(), '_', ' ');
  }

  return value;
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) { 
  
  string key, value, line;
  long ram;
  
  std::ifstream stream(kProcDirectory + "/"+ to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "VmSize:") { ram = stof(value) / kbMB;}
      }
    }
  } 
  ram = ((int)(ram * 100 + 0.5)) / 100;
  return to_string(ram);
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) { 
    
  string key, value, line;
  std::ifstream stream(kProcDirectory + "/"+ to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "Uid:") { return value;}
      }
    }
  }  
  return value;
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) { 
  
  string uid = LinuxParser::Uid(pid);

  string key, value1, value2, line;
  std::ifstream stream(kPasswordPath);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> key >> value1 >> value2;
        if (value2 == uid) { return key;}
      
    }
  } 
  return key;
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) {
  
  string line, value;
  long starttime;
  std::ifstream stream(kProcDirectory + "/"+ to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line); 
    std::istringstream linestream(line);
      for (int i=0; i<22; i++){
        linestream >> value;
        if (i == TimeValues::kStarttime_){
          starttime = stol(value);
        }
      }
  } 
  return (starttime / Hertz); 
}