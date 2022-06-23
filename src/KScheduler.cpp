
#include "../h/KScheduler.hpp"

List<KThread> KScheduler::readyThreadQueue;

KThread* KScheduler::get()
{
    return readyThreadQueue.removeFirst();
}

void KScheduler::put(KThread* kthread)
{
    readyThreadQueue.addLast(kthread);
}

