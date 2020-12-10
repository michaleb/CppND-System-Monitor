#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();  
  void preIdle(float x) { preIdle_ = x;}
  void preNonIdle(float y) { preNonIdle_ = y;}

  float preIdle() { return preIdle_;}
  float preNonIdle() { return preNonIdle_;}
  float preTotal() { return (preIdle_ + preNonIdle_);}
    
  
 private: 
 float preIdle_ = 0.0;
 float preNonIdle_ = 0.0;
 float preTotal_ ;   

 };
#endif