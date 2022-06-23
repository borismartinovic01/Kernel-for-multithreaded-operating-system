//Klasa uvedena radi modularnosti
//Ime je uzeto iz MVC arhitekture samo da asocira na nesto sto obavlja glavni dio posla
#ifndef _controller_hpp
#define _controller_hpp
#include "MemoryAllocator.hpp"
#include "KThread.hpp"
#include "KSemaphore.hpp"
#include "KConsole.hpp"

class Controller {
public:
    static void idle(void* arg);
    static void createIdle();
    static void createMain();
    static void createTransferOut();
    static void exitIdle();
    static void exitMain();
    static void exitTransferOut();
private:
    friend class Riscv;
    static void kmem_alloc();
    static void kmem_free();
    static void* allocateStack();
    static void kthread_create();
    static void kthread_init();
    static void kthread_dispatch();
    static void kthread_start();
    static void kthread_exit();

    static void ksem_open();
    static void ksem_close();
    static void ksem_wait();
    static void ksem_signal();
    static void ktime_sleep();

    static void kgetc();
    static void kputc();
};

#endif //_controller_hpp
