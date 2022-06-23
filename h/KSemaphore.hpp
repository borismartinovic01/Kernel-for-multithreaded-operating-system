
#ifndef _ksemaphore_hpp
#define _ksemaphore_hpp
#include "KScheduler.hpp"

class KSemaphore {
public:
    KSemaphore(unsigned init = 1)
        :val(init){}

    void wait();
    void signal();

    int value() const { return val; }
protected:
    void block();
    void unblock();
private:
    int val;
    List<KThread> blocked;

    friend class Controller;

    void *operator new(size_t n);

    void *operator new[](size_t n);

    void operator delete(void *p);

    void operator delete[](void *p);
};


#endif //_ksemaphore_hpp
