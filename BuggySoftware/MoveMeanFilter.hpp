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
  bool filled = false;

public:
  MoveMeanFilter() = default;

  void push(T value) {
    ++this->index;
    this->index = this->index % N;
    this->buffer[this->index] = value;
    //logger << logLevel::DEBUG << "Writing buffer at " << this->index << " value " << value << EmbeddedLogger::endl;
  }

  float getMean() {
    //logger << logLevel::DEBUG;
    int size = N;
    float sum = 0;
    if (!this->filled) {   // if the buffer is not filled only consider the values up until index
      size = this->index;
      this->filled = size/(N-1); // will be 1 when size == N, then it will consider the buffer to be full
    }
    for (int i=0; i<size; i++) {
      sum += this->buffer[i];
      //logger << this->buffer[i] << ",";
    }
    //logger << EmbeddedLogger::endl;
    //logger << logLevel::DEBUG << "size is " << size << " Sum is " << sum << EmbeddedLogger::endl;
    return sum/size;
  }

  float update(T value) {
    this->push(value);
    return this->getMean();
  }
};




#endif //MOVING_MEAN_FILTER_HPP