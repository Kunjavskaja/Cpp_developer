#include <iostream>
#include <random>
#include "TrafficLight.h"

/* Implementation of class "MessageQueue" */

template <typename T>
T MessageQueue<T>::receive()
{

    std::unique_lock<std::mutex> uLock(_mutex);
    _cond.wait(uLock, [this] { return !_queue.empty(); });

    auto value = std::move(_queue.back());
    _queue.pop_back();

    return value;
}

template <typename T>
void MessageQueue<T>::send(T &&msg)
{
    std::lock_guard<std::mutex> uLock(_mutex);
    _queue.push_back(std::move(msg));
    _cond.notify_one();
}

/* Implementation of class "TrafficLight" */

TrafficLight::TrafficLight()
{
    _currentPhase = TrafficLightPhase::red;
}

void TrafficLight::waitForGreen()
{
    while (true)
    {
        auto phase = _queuePhases.receive();
        if (phase == green)
        {
            return;
        }
    }
}

TrafficLightPhase TrafficLight::getCurrentPhase()
{
    return _currentPhase;
}

void TrafficLight::simulate()
{
    threads.emplace_back(std::thread(&TrafficLight::cycleThroughPhases, this));
}

// virtual function which is executed in a thread
void TrafficLight::cycleThroughPhases()
{
    //Random number generated
    std::chrono::time_point<std::chrono::system_clock> lastestUpdate = std::chrono::system_clock::now();
    std::random_device rd;
    std::mt19937 eng(rd());
    std::uniform_int_distribution<> distr(4000, 6000);
    double cycleThreshold = distr(eng); 
  
    while (true)

    {   std::this_thread::sleep_for(std::chrono::milliseconds(1));
        long timePassed= std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - lastestUpdate).count();
        
    if (timePassed >= cycleThreshold)
    {


        // toggle between lights
        _currentPhase = _currentPhase == TrafficLightPhase::red ? TrafficLightPhase::green : TrafficLightPhase::red;

        // send update 
        _queuePhases.send(std::move(_currentPhase));

        // reset stop watch
        lastestUpdate = std::chrono::system_clock::now();
        }
    }
}