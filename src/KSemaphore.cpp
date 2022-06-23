
#include "../h/KSemaphore.hpp"
#include "../h/KThread.hpp"

void KSemaphore::wait(){
    if(--val<0) block();
}

void KSemaphore::signal(){
    if(++val<=0) unblock();
}

void KSemaphore::block(){
    KThread::running->setBlocked(true);
    blocked.addLast(KThread::running);
    KThread::yield();
}

void KSemaphore::unblock(){
    KThread* kt = blocked.removeFirst();
    kt->setBlocked(false);
    KScheduler::put(kt);
}

void* KSemaphore::operator new(size_t size) {
    size_t numOfBlocks = size/MEM_BLOCK_SIZE;
    if(size>numOfBlocks*MEM_BLOCK_SIZE) numOfBlocks++;
    return MemoryAllocator::instance().kmem_alloc(numOfBlocks);
}

void* KSemaphore::operator new[](size_t size){
    size_t numOfBlocks = size/MEM_BLOCK_SIZE;
    if(size>numOfBlocks*MEM_BLOCK_SIZE) numOfBlocks++;
    return MemoryAllocator::instance().kmem_alloc(numOfBlocks);
}

void KSemaphore::operator delete(void *p){
    MemoryAllocator::instance().kmem_free(p);
}

void KSemaphore::operator delete[](void *p){
    MemoryAllocator::instance().kmem_free(p);
}