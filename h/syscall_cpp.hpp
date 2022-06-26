
#ifndef _syscall_cpp_hpp
#define _syscall_cpp_hpp
#include "syscall_c.hpp"

void* operator new (size_t);
void operator delete (void*);

class Thread {
public:
        Thread(void (*body)(void*), void* arg);
        virtual ~Thread();

        void start();

        static void dispatch();
        static void sleep(time_t time);

protected:
        Thread();
        virtual void run() {}
        friend class KThread;
private:
        thread_t myHandle;
};

class Semaphore {
public:
        Semaphore (unsigned init = 1);
        virtual ~Semaphore ();

        void wait ();
        void signal ();
private:
        sem_t myHandle;
};

class PeriodicThread : public Thread {
protected:
        PeriodicThread (time_t period);
        virtual void periodicActivation () {}
private:
        struct periodicInfo{
            PeriodicThread* thread;
            time_t period;
            periodicInfo(PeriodicThread* thread, time_t period) : thread(thread), period(period) {}
        };
        static void periodicWrapper(void* data);
};

class Console {
public:
        static char getc();
        static void putc(char c);
};

#endif //_syscall_cpp_hpp
