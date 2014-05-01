#pragma once
#include <boost/thread.hpp>
#include <iostream>
#include <string>
#include <atomic>
#include <mutex>


template <int size = 100>
class TLog {
public:
    TLog(std::ostream& _out) : out(_out), Buffer(new std::string[size]), BufPtr(0)  {}
    ~TLog() {
        std::call_once(once, [this]() {Flush(Buffer.load());});
    }
        
    TLog& operator<<(const std::string& message) {
        SpLock.lock();
        std::string* currentBuffer = Buffer.load();
        int index = BufPtr.fetch_add(1);
        SpLock.unlock();
        if (index < size) {
            Buffer[index] = message;
        } else {
            std::string* nBuf = new std::string[size];
            if (Buffer.compare_exchange_strong(currentBuffer, nBuf)) {
                nBuf[0] = message;
                BufPtr.store(1);
                Flush(currentBuffer);
            } else {
                delete[] nBuf;
            }
        }
    }

private:
    void Flush(std::string* buf) {
        std::lock_guard<std::mutex> lock (mtx);
        for (int i = 0; i < size; ++i) {
            if (!buf[i].empty())
                out << buf[i] << std::endl;
        }
        delete[] buf;
    }
    std::mutex mtx;
    std::ostream& out;
    boost::detail::spinlock SpLock;
    std::once_flag once;
    std::atomic<std::string*> Buffer;
    std::atomic<int> BufPtr;
};
