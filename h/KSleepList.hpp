#ifndef _sleepList_hpp
#define _sleepList_hpp

#include "List.hpp"
#include "KScheduler.hpp"

class KThread;

class KSleepList
{
private:
    struct sleepInfo{
        KThread* thread;
        time_t time;

        sleepInfo(KThread* thread, time_t time) : thread(thread), time(time){}

        void* operator new(size_t size) {
            size_t numOfBlocks = size/MEM_BLOCK_SIZE;
            if(size>numOfBlocks*MEM_BLOCK_SIZE) numOfBlocks++;
            return MemoryAllocator::instance().kmem_alloc(numOfBlocks);
        }

        void* operator new[](size_t size){
            size_t numOfBlocks = size/MEM_BLOCK_SIZE;
            if(size>numOfBlocks*MEM_BLOCK_SIZE) numOfBlocks++;
            return MemoryAllocator::instance().kmem_alloc(numOfBlocks);
        }

        void operator delete(void *p){
            MemoryAllocator::instance().kmem_free(p);
        }

        void operator delete[](void *p){
            MemoryAllocator::instance().kmem_free(p);
        }
    };
    static List<sleepInfo> sleepThreadQueue;

public:
    static void put(KThread* thread, time_t time);
    static void update();
};

#endif //_sleepList_hpp
