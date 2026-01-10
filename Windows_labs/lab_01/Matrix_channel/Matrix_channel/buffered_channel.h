#ifndef BUFFERED_CHANNEL_H_
#define BUFFERED_CHANNEL_H_
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>
#include <queue>

class MyException : public std::exception {
private:
    std::string message;
public:
    MyException(const std::string& msg) : message(msg) {}

    const char* what() const noexcept override {
        return message.c_str();
    }
};

template<class T>
class BufferedChannel {

public:
    explicit BufferedChannel(int size) : buffered_size(size), closed(false) {}

    void send(T value) {
        std::unique_lock<std::mutex> lk(mtx);
        cv_send.wait(lk, [this]() {
            return q.size() < buffered_size || closed;
            });
        if (closed) {
            throw MyException("writing in closed channel!!!!");
        }
        q.push(value);
        cv_receive.notify_one();
    }

    std::pair<T, bool> receive() {
        std::unique_lock lk(mtx);
        cv_receive.wait(lk, [this] {
            return !q.empty() || (closed && q.empty());
            });
        if (q.empty()) {
            return { T{}, false };
        }
        T res = q.front();
        q.pop();
        cv_send.notify_one();
        return { res, true };
    }

    void close() {
        std::lock_guard<std::mutex> lk(mtx);
        closed = true;
        cv_receive.notify_all();
    }

private:
    int buffered_size;
    std::queue<T> q;
    std::mutex mtx;
    std::condition_variable cv_receive, cv_send;
    bool closed;
};

#endif // BUFFERED_CHANNEL_H_
