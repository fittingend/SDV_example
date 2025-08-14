#ifndef __MESSAGE_QUEUE_H__
#define __MESSAGE_QUEUE_H__

#include <Common.h>
#include <SoaDataType.h>
#include <iostream>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>

template<typename T>
class MessageQueue {
private:
    std::queue<T> queue_;
    std::mutex mutex_;
    std::condition_variable cv_;

public:
    MessageQueue() {}
    ~MessageQueue() {}
    void Push(T msg)
    {
        std::unique_lock<std::mutex> lock(mutex_);
        queue_.push(msg);
        cv_.notify_one();
    }

    T Pop()
    {
        std::unique_lock<std::mutex> lock(mutex_);
        cv_.wait(lock, [this]{ return !queue_.empty(); });
        T msg = queue_.front();
        queue_.pop();
        return msg;
    }

    int PopTimeout(T *ptMsg, int timeout_ms)
    {
        std::unique_lock<std::mutex> lock(mutex_);
        if (cv_.wait_for(lock, std::chrono::milliseconds(timeout_ms), [this]{ return !queue_.empty(); }))
        {
            T msg = queue_.front();
            queue_.pop();
            *ptMsg = std::move(msg);
            return 0;
        }

        return -1;
    }
};


#endif