#ifndef MOVING_MEAN_FILTER_HPP
#define MOVING_MEAN_FILTER_HPP

#include "Arduino.h"


template<typename T, size_t N>
class MoveMeanFilter {
private:
  T buffer[N];
  size_t index = 0;

public:
  MoveMeanFilter() = default;

  void push(T value) {
    this->index %= N;
    this->buffer[this->index] = value;
  }

  double getMean() {
    double sum = 0;
    for (int i=0; i<N; i++) {
      sum += this->buffer[i];
    }
    return sum/N;
  }
};




#endif //MOVING_MEAN_FILTER_HPP