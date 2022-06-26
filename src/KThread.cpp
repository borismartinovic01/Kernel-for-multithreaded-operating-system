
#include "../h/KThread.hpp"
#include "../h/Riscv.hpp"
#include "../h/syscall_cpp.hpp"

KThread *KThread::running = nullptr;

uint64 KThread::timeSliceCounter = 0;
int KThread::activeThreads = 0;

KThread* KThread::createThread(Body body, void* arg, void* stack_space)
{
    return new KThread(body, arg, stack_space, DEFAULT_TIME_SLICE);
}

void KThread::yield()
{
    __asm__ volatile("mv a0, %[code]" : : [code] "r" (cthread_dispatch));
    __asm__ volatile ("ecall");
}

void KThread::dispatch()
{
    KThread *old = running;
    if (!old->isFinished() && !old->isBlocked() && !old->isSleep() && !(old->body == nullptr && old->arg==nullptr)){
        KScheduler::put(old);
    }
    running = KScheduler::get();

    if(running == nullptr){
        if(activeThreads <= 1 && !mainThread->isFinished() && !mainThread->isBlocked() && !mainThread->isSleep()){
            running = mainThread;
        }
        else{
            running = idleThread;
        }
    }
    Riscv::ms_sstatus(Riscv::SSTATUS_SIE);
    Riscv::ms_sstatus(Riscv::SSTATUS_SPP);
    KThread::contextSwitch(&old->context, &running->context);
}

void KThread::threadWrapper()
{
    Riscv::popSppSpie();
    if(running->body == nullptr){ //pozovi run metod
        Thread* t = (Thread*)running->arg;
        t->run();
    }
    else{ //pozovi funkciju koja je prosledjena
        running->body(running->arg);
    }
    if(!running->isFinished()){ //ako u korisnickom kodu nije pozvan exit
        KThread::exit();
    }
    KThread::yield();
}

void KThread::start(){

    if(this->isStarted()) return;
    this->setStarted(true);
    if(body == nullptr && arg == nullptr) return; //ako je body nullptr i arg nullptr kreirala se nit koja odgovara mainu
    activeThreads++;
    KScheduler::put(this);
}

int KThread::exit(){
    KThread::running->setFinished(true);
    activeThreads--;
    delete KThread::running;
    return 0;
}

int KThread::ksleep(time_t time){
    KSleepList::put(KThread::running, time);
    KThread::yield();
    return 0;
}

void* KThread::operator new(size_t size) {
    size_t numOfBlocks = size/MEM_BLOCK_SIZE;
    if(size>numOfBlocks*MEM_BLOCK_SIZE) numOfBlocks++;
    return MemoryAllocator::instance().kmem_alloc(numOfBlocks);
}

void* KThread::operator new[](size_t size){
    size_t numOfBlocks = size/MEM_BLOCK_SIZE;
    if(size>numOfBlocks*MEM_BLOCK_SIZE) numOfBlocks++;
    return MemoryAllocator::instance().kmem_alloc(numOfBlocks);
}

void KThread::operator delete(void *p){
    MemoryAllocator::instance().kmem_free(p);
}

void KThread::operator delete[](void *p){
    MemoryAllocator::instance().kmem_free(p);
}