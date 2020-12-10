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


int Process::Pid() { return pid_;}    
                    
float Process::CpuUtilization() { return LinuxParser::CpuUtilization(pid_); }

string Process::Command() { return LinuxParser::Command(pid_); }

string Process::Ram() { return LinuxParser::Ram(pid_); }

string Process::User() { return LinuxParser::User(pid_);}
    
long int Process::UpTime() { return (LinuxParser::UpTime() - LinuxParser::UpTime(pid_)); }

// Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process & a) { 
   return a.CpuUtilization() < this-> CpuUtilization();
}