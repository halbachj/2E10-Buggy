#include "MoveMeanFilter.hpp"


template<typename T, size_t N>
void MovingMeanFilter::push(T value) {
  this->index %= N;
  this->buffer[this->index] = value;
}


template<typename T, size_t N>
float MovingMeanFilter::getMean() {
  double sum = 0;
  for (int i=0; i<N; i++) {
    sum += this->buffer[i];
  }
  return sum/N;
}