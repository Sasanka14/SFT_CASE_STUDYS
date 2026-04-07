#include "TimerManager.h"
#include <iostream>
#include <chrono>

void TimerManager::startExamTimer(int seconds) {
    timeUp = false;
    remainingSeconds = seconds;

    timerThread = std::thread([this]() {
        while (remainingSeconds > 0 && !timeUp) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            --remainingSeconds;
        }

        if (remainingSeconds <= 0) {
            timeUp = true;
            std::cout << "\n⏰ Time is up! Auto-submitting...\n";
        }
    });
}

bool TimerManager::isTimeUp() const {
    return timeUp.load();
}

int TimerManager::getRemainingSeconds() const {
    return remainingSeconds.load();
}

void TimerManager::stop() {
    timeUp = true;
    if (timerThread.joinable())
        timerThread.join();
}

TimerManager::~TimerManager() {
    stop();
}