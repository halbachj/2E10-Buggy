#ifndef MOVING_MEAN_FILTER_HPP
#define MOVING_MEAN_FILTER_HPP

#include "Arduino.h"


template<typename T, size_t N>
class MovingMeanFilter {
private:
  T buffer[N];
  size_t index = 0;

public:
  MovingMeanFilter() = default;

  void push(T element);
  double getMean();
};




#endif //MOVING_MEAN_FILTER_HPP