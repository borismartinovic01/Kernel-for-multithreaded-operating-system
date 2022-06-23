
#ifndef _kscheduler_hpp
#define _kscheduler_hpp

#include "List.hpp"

class KThread;

class KScheduler
{
private:
    static List<KThread> readyThreadQueue;
public:
    static KThread* get();
    static void put(KThread* kthread);
};


#endif //_kscheduler_hpp

