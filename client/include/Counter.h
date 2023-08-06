#pragma once
#include <atomic>
class Counter
{
                  private:

                  std::atomic <int> value;

                  public:

                  Counter();

                  void increment();
                  
                  int get() const;

};