#include "TrafficLight.h"
// #include "MessageQueue.h"
#include <random>
#include <future>
#include <iostream>

// Generate random number
long generateRandomNumber(long min, long max) {
  std::random_device rd;
  std::mt19937 mt(rd());
  std::uniform_real_distribution<double> dist(min, max);
  return dist(mt);
}

TrafficLight::TrafficLight() { _currentPhase = TrafficLightPhase::Green; }

void TrafficLight::waitForGreen() {
  while (true) {
    if (_queue.receive() == TrafficLightPhase::Green) {
      return;
    }
  }
};

TrafficLightPhase TrafficLight::getCurrentPhase() { return _currentPhase; }

void TrafficLight::simulate() {
  std::thread t = std::thread(&TrafficLight::cycleThroughPhases, this);
  threads.emplace_back(std::move(t));
}

// virtual function which is executed in a thread
void TrafficLight::cycleThroughPhases() {
  long randomCycleTime_min = 4000;
  long randomCycleTime_max = 6000;
  long randomCycleTime =
      generateRandomNumber(randomCycleTime_min, randomCycleTime_max);
  int sleepTime = 1;

  auto now = std::chrono::system_clock::now();

  while (true) {
    long delta = std::chrono::duration_cast<std::chrono::milliseconds>(
                     std::chrono::system_clock::now() - now)
                     .count();

    if (delta > randomCycleTime) {
      _currentPhase = _currentPhase == TrafficLightPhase::Red
                          ? TrafficLightPhase::Green
                          : TrafficLightPhase::Red;

      auto task =
          std::async(std::launch::async, &MessageQueue<TrafficLightPhase>::send,
                     &_queue, std::move(_currentPhase));

      now = std::chrono::system_clock::now();

      randomCycleTime = generateRandomNumber(randomCycleTime_min, randomCycleTime_max);

      std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));
    }
  }
}