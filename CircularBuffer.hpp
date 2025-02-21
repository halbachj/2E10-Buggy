#ifndef CIRCULAR_BUFFER_HPP
#define CIRCULAR_BUFFER_HPP


template<typename T>
class CircularBuffer {
private:
  const size_t size;
  T buffer[size];
  size_t index;

public:
  CircularBuffer<T>(const size_t size);

  void push(T element);
  T get(size_t index);

}




#endif //CIRCULAR_BUFFER_HPP