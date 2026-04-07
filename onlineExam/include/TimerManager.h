#ifndef TIMERMANAGER_H
#define TIMERMANAGER_H

#include <atomic>
#include <thread>

class TimerManager {
private:
    std::atomic<bool> timeUp{false};
    std::atomic<int> remainingSeconds{0};
    std::thread timerThread;

public:
    void startExamTimer(int seconds);
    bool isTimeUp() const;
    int getRemainingSeconds() const;

    void stop();
    ~TimerManager();
};

#endif