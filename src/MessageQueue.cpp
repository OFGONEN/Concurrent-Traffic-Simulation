#include "MessageQueue.h"

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