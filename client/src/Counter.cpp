#include "../include/Counter.h"

Counter::Counter() : value(0)  {} 

void Counter::increment()
{
                  value.fetch_add(1);
}

int Counter::get() const
{
                  return value.load();
}
