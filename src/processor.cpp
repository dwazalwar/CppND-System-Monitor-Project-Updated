#include "processor.h"
#include <unistd.h>

#include "linux_parser.h"
#include <iostream>


// TODO: Return the aggregate CPU utilization
float Processor::Utilization()
{ 
    long total_jiffies1 = LinuxParser::Jiffies(name_);
    long active_jiffies1 = LinuxParser::ActiveJiffies(name_);

    usleep(1000);

    long total_jiffies2 = LinuxParser::Jiffies(name_);
    long active_jiffies2 = LinuxParser::ActiveJiffies(name_);

    long delta_total_jiffies = total_jiffies2 - total_jiffies1;
    long delta_active_jiffies = active_jiffies2 - active_jiffies1;

    float proc_utilization = 0.0f;
    if( delta_total_jiffies > 0 )
    {
        proc_utilization = (delta_active_jiffies * 1.0f)/delta_total_jiffies;
    }

    return proc_utilization;
}