#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
  public:
    float Utilization();
  private:
    long prev_idle_cpu = 0;
    long prev_active_cpu = 0;
    long idle_cpu;
    long active_cpu;
};

#endif