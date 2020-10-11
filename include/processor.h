#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  Processor() {}
  Processor(std::string n): name_(n) {}
  float Utilization();  // TODO: See src/processor.cpp

  // TODO: Declare any necessary private members
 private:
 std::string name_{""};

};

#endif