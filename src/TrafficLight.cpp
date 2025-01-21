#include "TrafficLight.h"
// #include "MessageQueue.h"
#include <cstdlib>
#include <ctime>
#include <iostream>

// Generate random number
float generateRandomNumber(float min, float max) {
  srand(static_cast<unsigned int>(time(0)));

  float random = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);

  return min + random * (max - min);
}

TrafficLight::TrafficLight() { _currentPhase = TrafficLightPhase::Red; }

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
  float randomCycleTime_min = 4;
  float randomCycleTime_max = 6;
  float randomCycleTime =
      generateRandomNumber(randomCycleTime_min, randomCycleTime_max);
  int sleepTime = 1;

  auto now = std::chrono::system_clock::now();

  while (true) {
    long delta = std::chrono::duration_cast<std::chrono::seconds>(
                     std::chrono::system_clock::now() - now)
                     .count();

    if (delta > randomCycleTime) {
      if (_currentPhase == TrafficLightPhase::Red) {
        _currentPhase = TrafficLightPhase::Green;
      } else {
        _currentPhase = TrafficLightPhase::Red;
      }

      _queue.send(std::move(_currentPhase));

      now = std::chrono::system_clock::now();

      randomCycleTime = generateRandomNumber(4, 6);
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));
  }
}