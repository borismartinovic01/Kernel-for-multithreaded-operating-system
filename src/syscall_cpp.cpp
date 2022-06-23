#include "../h/syscall_cpp.hpp"

void *operator new(size_t n)
{
    return mem_alloc(n);
}

void operator delete(void *p)
{
    mem_free(p);
}


Thread::Thread(void (*body)(void*), void* arg){
    if(body != nullptr) thread_init(&myHandle, body, arg);
    else thread_create(&myHandle, nullptr, this); //kernelu ce se proslediti this kako bi mogao preko njega da pozove run
}

Thread::~Thread() {}

Thread::Thread(){
    thread_init(&myHandle, nullptr, this);
}

void Thread::start() {
    thread_start(myHandle);
}

void Thread::dispatch() {
    thread_dispatch();
}

void Thread::sleep(time_t time) {
    time_sleep(time);
}

Semaphore::Semaphore(unsigned int init) {
    sem_open(&myHandle, init);
}

Semaphore::~Semaphore (){
    sem_close(myHandle);
}

void Semaphore::wait() {
    sem_wait(myHandle);
}

void Semaphore::signal() {
    sem_signal(myHandle);
}

char Console::getc(){
    return ::getc();
}
void Console::putc(char c){
    return ::putc(c);
}

//void *operator new[](size_t n)
//{
//    return mem_alloc(n);
//}
//
//void operator delete[](void *p) noexcept
//{
//mem_free(p);
//}