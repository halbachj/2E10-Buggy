#ifndef MOVING_MEAN_FILTER_HPP
#define MOVING_MEAN_FILTER_HPP

#include "Arduino.h"
#include "EmbeddedLogger.hpp"

using EmbeddedLogger::logger;

template<typename T, size_t N>
class MoveMeanFilter {
private:
  T buffer[N];
  size_t index = 0;

public:
  MoveMeanFilter() {
    for (int i=0; i<N; ++i) {
      buffer[i] = T();
    }
  }

  void push(T value) {
    ++this->index;
    this->index %= N;
    this->buffer[this->index] = value;
    //logger << "Writing buffer at " << this->index << EmbeddedLogger::endl;
  }

  float getMean() {
    float sum = 0;
    for (int i=0; i<N; i++) {
      sum += this->buffer[i];
      //logger << this->buffer[i] << ",";
    }
    //logger << EmbeddedLogger::endl;  
    //logger << "size is " << N << " Sum is " << sum << EmbeddedLogger::endl;
    return sum/N;
  }
};




#endif //MOVING_MEAN_FILTER_HPP