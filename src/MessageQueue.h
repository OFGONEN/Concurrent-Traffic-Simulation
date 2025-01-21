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

template <typename T> void MessageQueue<T>::send(T &&msg) {

  std::lock_guard<std::mutex> uLock(_mutex);

  _queue.push_back(std::move(msg));
  _cond.notify_one();
}

template <typename T> T MessageQueue<T>::receive() {

  std::unique_lock<std::mutex> uLock(_mutex);
  _cond.wait(uLock, [this] { return !_queue.empty(); });

  T msg = std::move(_queue.back());
  _queue.pop_back();

  return msg;
}

#endif