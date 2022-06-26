#include "../h/KSleepList.hpp"
#include "../h/KThread.hpp"

List<KSleepList::sleepInfo> KSleepList::sleepThreadQueue;

void KSleepList::put(KThread* thread, time_t time){
    if(time == 0) return; //bitno je uraditi ovo jer bi se u nastavku desio overflow i nit bi bila uspavana jako dugo
    thread->setSleep(true);
    sleepInfo* newInfo = new sleepInfo(thread, time);

    for(sleepThreadQueue.setFirst(); sleepThreadQueue.isCur(); sleepThreadQueue.next()) {
        sleepInfo* curInfo = sleepThreadQueue.peekCur();
        if (newInfo->time <= curInfo->time) { //<= je bolje od < zbog umetanja
            curInfo->time -= newInfo->time;
            sleepThreadQueue.addBefore(newInfo);
            return;
        }
        newInfo->time -= curInfo->time;

    }
    sleepThreadQueue.addLast(newInfo);
}

void KSleepList::update(){
    sleepInfo* first = sleepThreadQueue.peekFirst();
    if(first == 0) return;

    first->time--;

    while(first && first->time == 0){
        first = sleepThreadQueue.removeFirst();
        first->thread->setSleep(false);
        KScheduler::put(first->thread);
        delete first;
        first = sleepThreadQueue.peekFirst();
    }
}