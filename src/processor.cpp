#include "processor.h"
#include <unistd.h>

#include "linux_parser.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization()
{ 
    long total_jiffies1 = LinuxParser::Jiffies(name_);
    long active_jiffies1 = LinuxParser::ActiveJiffies(name_);

    float load1 = (active_jiffies1*1.0f)/total_jiffies1;

    sleep(1);

    long total_jiffies2 = LinuxParser::Jiffies(name_);
    long active_jiffies2 = LinuxParser::ActiveJiffies(name_);

    float load2 = (active_jiffies2*1.0f)/total_jiffies2;

    float proc_utilization = (load2 - load1);

    return proc_utilization;
}