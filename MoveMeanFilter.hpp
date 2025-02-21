#ifndef MOVING_MEAN_FILTER_HPP
#define MOVING_MEAN_FILTER_HPP

#include "Arduino.h"


template<typename T>
class MovingMeanFilter {
private:
  const size_t size;
  T buffer[size];
  size_t index;

public:
  MovingMeanFilter(const size_t size);

  void push(T element);
  double getMean();
}




#endif //MOVING_MEAN_FILTER_HPP