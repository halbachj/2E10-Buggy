#include "MoveMeanFilter.hpp"

MovingMeanFilter::MovingMeanFilter(const size_t size): size(size) {

}


template<typename T>
void MovingMeanFilter::push(T value) {
  this->index %= this->size;
  this->buffer[this->index] = value;
}

float MovingMeanFilter::getMean() {
  double sum = 0;
  for (int i=0; i<this->size; i++) {
    sum += this->buffer[i];
  }
  return sum/this->size;
}