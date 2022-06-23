#include "../h/Controller.hpp"

KThread* idleThread;
KThread* mainThread;
KThread* transferOutThread;

void Controller::kmem_alloc() {
    size_t sizeArg;
    __asm__ volatile("mv %[val], a1" : [val] "=r" (sizeArg));
    void* addr = MemoryAllocator::instance().kmem_alloc(sizeArg);
    __asm__ volatile("mv a0, %[val]" : : [val] "r" (addr));
}

void Controller::kmem_free() {
    void* addrArg;
    __asm__ volatile("mv %[val], a1" : [val] "=r" (addrArg));
    int code = MemoryAllocator::instance().kmem_free(addrArg);
    __asm__ volatile("mv a0, %[val]" : : [val] "r" (code));
}

void Controller::kthread_create() {
    KThread** handleP;
    KThread* ret;
    using Body = void(*)(void*);
    Body start_routine;
    void* arg;
    void* stack_space;

    __asm__ volatile("mv %[val], a1" : [val] "=r" (handleP));
    __asm__ volatile("mv %[val], a2" : [val] "=r" (start_routine));
    __asm__ volatile("mv %[val], a3" : [val] "=r" (arg));
    __asm__ volatile("mv %[val], a6" : [val] "=r" (stack_space));
    ret = (KThread::createThread(start_routine, arg, stack_space));
    int code;
    if(ret != 0) { (*handleP) = ret; code = 0; ret->start(); }
    else{
        MemoryAllocator::instance().kmem_free(stack_space);
        code = -1;
    }
    __asm__ volatile("mv a0, %[val]" : : [val] "r" (code));
}

void Controller::kthread_init() {
    KThread** handleP;
    KThread* ret;
    using Body = void(*)(void*);
    Body start_routine;
    void* arg;
    void* stack_space;

    __asm__ volatile("mv %[val], a1" : [val] "=r" (handleP));
    __asm__ volatile("mv %[val], a2" : [val] "=r" (start_routine));
    __asm__ volatile("mv %[val], a3" : [val] "=r" (arg));
    __asm__ volatile("mv %[val], a6" : [val] "=r" (stack_space));
    ret = (KThread::createThread(start_routine, arg, stack_space));
    int code;
    if(ret != 0) { (*handleP) = ret; code = 0; }
    else{
        MemoryAllocator::instance().kmem_free(stack_space);
        code = -1;
    }
    __asm__ volatile("mv a0, %[val]" : : [val] "r" (code));
}

void Controller::kthread_dispatch() {
    KThread::timeSliceCounter = 0;
    KThread::dispatch();
}

void Controller::kthread_start() {
    KThread* handle;
    __asm__ volatile("mv %[val], a1" : [val] "=r" (handle));
    handle->start();
}

void Controller::kthread_exit() {
    int code = 0;
    KThread::exit();
    __asm__ volatile("mv a0, %[val]" : : [val] "r" (code));
}

void Controller::ksem_open() {
    KSemaphore** handleP;
    int init;
    int code;
    KSemaphore* ret;
    __asm__ volatile("mv %[val], a1" : [val] "=r" (handleP));
    __asm__ volatile("mv %[val], a2" : [val] "=r" (init));

    ret = new KSemaphore(init);
    if(ret != 0) { (*handleP) = ret; code = 0; }
    else code = -1;

    __asm__ volatile("mv a0, %[val]" : : [val] "r" (code));
}

void Controller::ksem_close() {
    KSemaphore* handle;
    int code = 0;
    __asm__ volatile("mv %[val], a1" : [val] "=r" (handle));
    KThread* tmp;
    while((tmp = handle->blocked.removeFirst()) != nullptr){
        KScheduler::put(tmp);
    }
    delete handle;
    __asm__ volatile("mv a0, %[val]" : : [val] "r" (code));
}

void Controller::ksem_wait() {
    KSemaphore* handle;
    int code = 0;
    __asm__ volatile("mv %[val], a1" : [val] "=r" (handle));
    handle->wait();
    __asm__ volatile("mv a0, %[val]" : : [val] "r" (code));
}

void Controller::ksem_signal() {
    KSemaphore* handle;
    int code = 0;
    __asm__ volatile("mv %[val], a1" : [val] "=r" (handle));
    handle->signal();
    __asm__ volatile("mv a0, %[val]" : : [val] "r" (code));
}

void Controller::ktime_sleep(){
    time_t time;
    int code;
    __asm__ volatile("mv %[val], a1" : [val] "=r" (time));
    code = KThread::ksleep(time);
    __asm__ volatile("mv a0, %[val]" : : [val] "r" (code));
}

void Controller::kgetc(){
    char c = KConsole::instance().kgetc();
//    KConsole::instance().kputc(c);
    __asm__ volatile("mv a0, %[val]" : : [val] "r" (c));
}
void Controller::kputc(){
    char c;
    __asm__ volatile("mv %[val], a1" : [val] "=r" (c));
    KConsole::instance().kputc(c);
}

void Controller::idle(void* arg){
    while(1){
        Riscv::ms_sstatus(Riscv::SSTATUS_SIE);
        KThread::yield();
    }
}
void Controller::createIdle(){
    void* stack_space = Controller::allocateStack();
    idleThread = KThread::createThread(idle, nullptr, stack_space);
    idleThread->setStarted(true);
    idleThread->setFinished(true);
}
void Controller::createMain(){
    mainThread = KThread::createThread(nullptr, nullptr, nullptr);
    mainThread->start();
    KThread::running = mainThread;
}

void Controller::createTransferOut() {
    void* stack_space = Controller::allocateStack();
    transferOutThread = KThread::createThread(KConsole::transferOut, nullptr, stack_space);
    transferOutThread->start();
}

void* Controller::allocateStack(){
    size_t size = sizeof(uint64[DEFAULT_STACK_SIZE]);
    size_t numOfBlocks = size/MEM_BLOCK_SIZE;
    if(size>numOfBlocks*MEM_BLOCK_SIZE) numOfBlocks++;
    return MemoryAllocator::instance().kmem_alloc(numOfBlocks);
}

void Controller::exitIdle(){
    delete idleThread;
}
void Controller::exitMain(){
    while(!transferOutThread->isBlocked()){//cekanje da se zavrsi transfer ukoliko ga ima
        KThread::yield();
    }
    mainThread->setFinished(true);
    delete mainThread;
}
void Controller::exitTransferOut(){
    transferOutThread->setFinished(true);
    delete transferOutThread;
}