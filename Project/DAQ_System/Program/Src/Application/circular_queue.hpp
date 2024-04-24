/*
* Circular Queue
* Author:   Cristian Cruz
*
* Email:    cris14.alex@gmail.com
*
* (c) 2024 Dallas Formula Racing - Embedded Firmware Team
* Formula SAE International Collegiate Chapter
* GPL-3.0 License
*/

#ifndef CIRCULAR_QUEUE_H
#define CIRCULAR_QUEUE_H

// Standard Libraries
#include <cstdint>
#include <vector>

// DFR Custom Dependencies
#include "Mutex/imutex.hpp"

namespace application {

template <class T>
class CircularQueue {
public:
    CircularQueue(const uint8_t size)
        : buffer_(std::vector<T>(size)),
          kMaxSize(size) { }

    CircularQueue(const uint8_t size,
        			  std::shared_ptr<application::IMutex> mutex)
            : buffer_(std::vector<T>(size)),
              kMaxSize(size),
    		  mutex_(mutex) { }
    
    void Enqueue(T data) {
        buffer_[head_] = data;

        if (full_) {
            // Current tail has been overwritten
            IncrementIndex(tail_);
        }

        IncrementIndex(head_);
        full_ = head_ == tail_;
    }

    T Dequeue() {
        if (IsEmpty()) {
            return T();
        }

        T data = buffer_[tail_];
        full_ = false;
        IncrementIndex(tail_);

        return data;
    }

    bool IsEmpty() {
        return !full_ && (head_ == tail_);
    }

    bool IsFull() {
        return full_;
    }

    void Clear() {
    	while (!IsEmpty()) {
    		Dequeue();
    	}
    }

    void Lock() { mutex_->Lock(); }

    void Unlock() { mutex_->Unlock(); }


private:
    void IncrementIndex(uint8_t &index) {
        index = (index + 1) % kMaxSize;   
    }



    std::vector<T> buffer_;
    const uint8_t kMaxSize;
    std::shared_ptr<application::IMutex> mutex_;
    uint8_t head_ = 0;
    uint8_t tail_ = 0;
    bool empty_ = true;
    bool full_ = false;
};

} // namespace application

#endif // CIRCULAR_QUEUE_H
