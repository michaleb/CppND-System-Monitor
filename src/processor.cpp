#include "processor.h"
#include "linux_parser.h"
#include <vector>
#include <string>
#include <iostream>

using std::string;
using std::vector;
using std::cout;

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
    
    vector<string> procData = LinuxParser::CpuUtilization();

    float user = stof(procData[LinuxParser::kUser_]);
    float nice = stof(procData[LinuxParser::kNice_]);
    float system = stof(procData[LinuxParser::kSystem_]);
    float idle = stof(procData[LinuxParser::kIdle_]);
    float iowait = stof(procData[LinuxParser::kIOwait_]);
    float irq = stof(procData[LinuxParser::kIRQ_]);
    float softirq = stof(procData[LinuxParser::kSoftIRQ_]);
    float steal = stof(procData[LinuxParser::kGuest_]);
    /*float guest = stof(procData[LinuxParser::kGuestNice_]);
    float guest_nice = stof(procData[LinuxParser::kNice_]); */

    float Idle = idle + iowait;
    float NonIdle = user + nice + system + irq + softirq + steal;
    float Total = Idle + NonIdle;

    float totald = Total - preTotal();
    float idled = Idle - preIdle();

    preIdle(Idle);
    preNonIdle(NonIdle);
          
    return (totald - idled)/totald;
}

    