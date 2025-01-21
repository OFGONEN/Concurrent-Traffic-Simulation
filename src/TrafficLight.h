#ifndef TRAFFICLIGHT_H
#define TRAFFICLIGHT_H

#include "MessageQueue.h"
#include "TrafficObject.h"
#include <condition_variable>
#include <deque>
#include <mutex>

class Vehicle;

enum TrafficLightPhase { Red, Green };

class TrafficLight : public TrafficObject {
public:
  TrafficLight();

  TrafficLightPhase getCurrentPhase();

  void waitForGreen();
  void simulate();

private:
  void cycleThroughPhases();

  std::condition_variable _condition;
  std::mutex _mutex;

  TrafficLightPhase _currentPhase;
  MessageQueue<TrafficLightPhase> _queue;
};

#endif