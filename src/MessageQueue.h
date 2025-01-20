#ifndef MessageQueue_H
#define MessageQueue_H

#include <condition_variable>
#include <deque>
#include <mutex>

template <class T> class MessageQueue {

public:
  void send(T &&);
  T receive();

private:
  std::mutex _mutex;
  std::condition_variable _cond;
  std::deque<T> _queue;
};

#endif