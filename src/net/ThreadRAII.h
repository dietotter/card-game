#pragma once

#include <thread>

// WIP
// Questions: how to correctly initialize this object? Should be somehow zero-initialized in Server and Client?
// (maybe there is some special case of std::thread for delayed thread launch, idk)
// Do i join/detach/don't do this at all in destructor? Will the program not terminate if main has ended, but this thread
// is endlessly running in a loop?
// (I guess I at least have to try it out in practise. Also, maybe it's not worth it to do this thread wrapper stuff at all)
namespace nik {

    class ThreadRAII
    {
    private:
        std::thread m_thread;

    public:
        ThreadRAII(std::thread &thread): m_thread{ std::move(thread) } {}
        ~ThreadRAII()
        {
            if (m_thread.joinable())
            {
                // not sure this is correct or safe
                m_thread.detach();
            }
        }

        // these methods signature is copied from std::thread's signatures
        bool joinable() const noexcept { return m_thread.joinable(); }
        void join() { return m_thread.join(); }

        ThreadRAII(const ThreadRAII &threadRaii) = delete;
        ThreadRAII& operator=(const ThreadRAII &threadRaii) = delete;
    };

}