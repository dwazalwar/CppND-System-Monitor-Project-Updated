#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iostream>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;
using std::stol;

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
  string os, kernel, version;
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

string LinuxParser::GetValue(std::string filename, std::string keyname)
{
  string key, value;
  string line;

  if( keyname == "" )
    return value;

  std::ifstream stream(filename);
  if( stream.is_open())
  {
    while(std::getline(stream, line))
    {
      std::stringstream linestream(line);
      linestream >> key >> value;

      if(key == keyname)
      {
        return value;
      }
    }
  }

  return value;
}

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization()
{ 
  string filename = kProcDirectory + kMeminfoFilename;
  int total_memory = std::stoi(GetValue(filename, "MemTotal:"));
  int free_memory = std::stoi(GetValue(filename, "MemFree:"));

  float memory_utilization = 0.0f;

  if( total_memory != 0)
    memory_utilization = ((total_memory - free_memory)*1.0f)/total_memory;

  return memory_utilization;
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime()
{ 
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  string uptime, idletime;

  if( stream.is_open())
  {
    while(std::getline(stream, line) )
    {
      std::stringstream linestream(line);
      linestream >> uptime >> idletime;
    }
  }

  return std::stol(uptime);
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies(std::string processor_name)
{ 
  return ActiveJiffies(processor_name) + IdleJiffies(processor_name);
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies(std::string processor_name)
{ 
  long num_active_jiffies = 0;
  vector<std::string> cpu_details = CpuUtilization(processor_name);
  if( cpu_details.size() == 10 )
  {
      num_active_jiffies += stol(cpu_details[kUser_]) + stol(cpu_details[kNice_]) +
                            stol(cpu_details[kSystem_]) + stol(cpu_details[kIRQ_]) +
                            stol(cpu_details[kSoftIRQ_]) + stol(cpu_details[kSteal_]) +
                            stol(cpu_details[kGuest_]) + stol(cpu_details[kGuestNice_]);
  }

  return num_active_jiffies;
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies(std::string processor_name)
{
   long num_idle_jiffies = 0;
    vector<std::string> cpu_details = CpuUtilization(processor_name);
    if( cpu_details.size() == 10 )
    {
        num_idle_jiffies += stol(cpu_details[kIdle_]) + stol(cpu_details[kIOwait_]);
    }

    return num_idle_jiffies;
}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization(std::string processor_name)
{ 
  string filename = kProcDirectory + kStatFilename;
  string cpu_value =  GetValue(filename, processor_name);

  vector<string> cpu_details;
  std::stringstream ss(cpu_value);

  for(string word; ss >> word; )
  {
    cpu_details.push_back(word);
  }

  return cpu_details;
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses()
{ 
  string filename = kProcDirectory + kStatFilename;
  int total_processes = stoi(GetValue(filename, "processes"));
  
  return total_processes; 
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses()
{ 
  string filename = kProcDirectory + kStatFilename;
  int running_processes = stoi(GetValue(filename, "procs_running"));
  
  return running_processes; 
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid[[maybe_unused]]) { return 0; }