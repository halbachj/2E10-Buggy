#ifndef MOVING_MEAN_FILTER_HPP
#define MOVING_MEAN_FILTER_HPP

#include "Arduino.h"
#include "EmbeddedLogger.hpp"

using EmbeddedLogger::logger;
using logLevel = EmbeddedLogger::LogLevel;

template<typename T, size_t N>
class MoveMeanFilter {
private:
  T buffer[N]{};
  size_t index = 0;

public:
  MoveMeanFilter() = default;

  void push(T value) {
    ++this->index;
    this->index %= N;
    this->buffer[this->index] = value;
    //logger << logLevel::DEBUG << "Writing buffer at " << this->index << EmbeddedLogger::endl;
  }

  float getMean() {
    float sum = 0;
    for (int i=0; i<N; i++) {
      sum += this->buffer[i];
      //logger << logLevel::DEBUG << this->buffer[i] << ",";
    }
    logger << EmbeddedLogger::endl;
    //logger << logLevel::DEBUG << "size is " << N << " Sum is " << sum << EmbeddedLogger::endl;
    return sum/N;
  }

  float update(T value) {
    this->push(value);
    return this->getMean();
  }
};




#endif //MOVING_MEAN_FILTER_HPP