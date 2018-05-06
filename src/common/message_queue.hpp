#ifndef INC_433_QUEUE_H
#define INC_433_QUEUE_H

#import <queue>
#import <mutex>


template <typename T>
class message_queue {

public:
    message_queue(int capacity) : capacity(capacity) {
      q = std::queue<T>();
    }

    bool empty() const {
      std::lock_guard<std::mutex> l(m);
      return q.empty();
    }

    size_t size() const  {
      std::lock_guard<std::mutex> l(m);
      return q.size();
    }

    void push(const T& v) {
      std::unique_lock<std::mutex> lock {m};
      not_full.wait(lock, [this](){return q.size() != capacity; });
      q.push(v);
      lock.unlock();
      not_empty.notify_one();
    }

    T pop() {
      std::unique_lock<std::mutex> lock {m};
      not_empty.wait(lock, [this](){return q.size() != 0; });
      auto v = q.front();
      q.pop();
      lock.unlock();
      not_full.notify_one();
      return v;
    }

private:
    int capacity;
    std::queue<T> q;
    std::mutex m;
    std::condition_variable not_full;
    std::condition_variable not_empty;
};


#endif //INC_433_QUEUE_H
