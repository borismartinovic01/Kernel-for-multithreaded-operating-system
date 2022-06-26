
#ifndef _kthread_hpp
#define _kthread_hpp
#include "../lib/hw.h"
#include "MemoryAllocator.hpp"
#include "KSleepList.hpp"

extern KThread* idleThread;
extern KThread* mainThread;
extern KThread* transferOutThread;

class KThread {
public:

    ~KThread() {
        if(this != mainThread) MemoryAllocator::instance().kmem_free(stack);
    }

    bool isFinished() const { return finished; }

    void setFinished(bool value) { finished = value; }

    bool isStarted() const { return started; }

    void setStarted(bool value) { started = value; }

    bool isBlocked() const { return blocked; }

    void setBlocked(bool value) { blocked = value; }

    bool isSleep() const { return asleep; }

    void setSleep(bool value) { asleep = value; }

    void start();

    uint64 getTimeSlice() const { return timeSlice; }

    using Body = void (*)(void*);

    static KThread* createThread(Body body, void* arg, void* stack_space);

    static void yield();

    static int exit();

    static KThread *running;

    void *operator new(size_t n);

    void *operator new[](size_t n);

    void operator delete(void *p);

    void operator delete[](void *p);

private:
    KThread(Body body, void* arg, void* stack_space, uint64 timeSlice)
    :
    body(body),
    stack((uint64*)stack_space),
    context({(uint64) &threadWrapper, (uint64) stack + DEFAULT_STACK_SIZE}),
    timeSlice(timeSlice),
    finished(false),
    started(false),
    arg(arg),
    blocked(false),
    asleep(false){}

    struct Context
    {
        uint64 ra;
        uint64 sp;
    };

    Body body;
    uint64* stack;
    Context context;
    uint64 timeSlice;
    bool finished;
    bool started;
    void* arg;
    bool blocked;
    bool asleep;

    friend class Riscv;
    friend class Controller;

    static void threadWrapper();

    static void contextSwitch(Context *oldContext, Context *runningContext);

    static void dispatch();

    static int ksleep(time_t time);

    static uint64 timeSliceCounter;

    static int activeThreads;

};


#endif //_kthread_hpp
