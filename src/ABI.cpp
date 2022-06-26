#include "../h/ABI.hpp"

void* amem_alloc(size_t numOfBlocks){
    void* addr;
    __asm__ volatile("mv a1, %[val]" : : [val] "r" (numOfBlocks));
    __asm__ volatile("mv a0, %[code]" : : [code] "r" (cmem_alloc));
    __asm__ volatile("ecall");

    __asm__ volatile("mv %[val], a0" : [val] "=r" (addr));
    return addr;
}
int amem_free(void* addr){
    int code;
    __asm__ volatile("mv a1, %[val]" : : [val] "r" (addr));
    __asm__ volatile("mv a0, %[code]" : : [code] "r" (cmem_free));
    __asm__ volatile("ecall");

    __asm__ volatile("mv %[val], a0" : [val] "=r" (code));
    return code;
}

int athread_create(thread_t* handle, void(*start_routine)(void*), void* arg, void* stack_space){
    int code;

    __asm__ volatile("mv a6, %[val]" : : [val] "r" (stack_space));
    __asm__ volatile("mv a3, %[val]" : : [val] "r" (arg));
    __asm__ volatile("mv a2, %[val]" : : [val] "r" (start_routine));
    __asm__ volatile("mv a1, %[val]" : : [val] "r" (handle));
    __asm__ volatile("mv a0, %[code]" : : [code] "r" (cthread_create));
    __asm__ volatile("ecall");
    __asm__ volatile("mv %[val], a0" : [val] "=r" (code));
    return code;
}

int athread_init(thread_t* handle, void(*start_routine)(void*), void* arg, void* stack_space){
    int code;
    __asm__ volatile("mv a6, %[val]" : : [val] "r" (stack_space));
    __asm__ volatile("mv a3, %[val]" : : [val] "r" (arg));
    __asm__ volatile("mv a2, %[val]" : : [val] "r" (start_routine));
    __asm__ volatile("mv a1, %[val]" : : [val] "r" (handle));
    __asm__ volatile("mv a0, %[code]" : : [code] "r" (cthread_init));
    __asm__ volatile("ecall");
    __asm__ volatile("mv %[val], a0" : [val] "=r" (code));
    return code;
}

void athread_dispatch(){
    __asm__ volatile("mv a0, %[code]" : : [code] "r" (cthread_dispatch));
    __asm__ volatile("ecall");
}

void athread_start(thread_t handle){
    __asm__ volatile("mv a1, %[val]" : : [val] "r" (handle));
    __asm__ volatile("mv a0, %[code]" : : [code] "r" (cthread_start));
    __asm__ volatile("ecall");
}

int athread_exit(){
    int code;
    __asm__ volatile("mv a0, %[code]" : : [code] "r" (cthread_exit));
    __asm__ volatile("ecall");
    __asm__ volatile("mv %[val], a0" : [val] "=r" (code));
    return code;
}

int asem_open(sem_t* handle, unsigned init){
    int code;
    __asm__ volatile("mv a2, %[val]" : : [val] "r" (init));
    __asm__ volatile("mv a1, %[val]" : : [val] "r" (handle));
    __asm__ volatile("mv a0, %[code]" : : [code] "r" (csem_open));
    __asm__ volatile("ecall");
    __asm__ volatile("mv %[val], a0" : [val] "=r" (code));
    return code;
}
int asem_close(sem_t handle){
    int code;
    __asm__ volatile("mv a1, %[val]" : : [val] "r" (handle));
    __asm__ volatile("mv a0, %[code]" : : [code] "r" (csem_close));
    __asm__ volatile("ecall");
    __asm__ volatile("mv %[val], a0" : [val] "=r" (code));
    return code;
}
int asem_wait(sem_t id){
    int code;
    __asm__ volatile("mv a1, %[val]" : : [val] "r" (id));
    __asm__ volatile("mv a0, %[code]" : : [code] "r" (csem_wait));
    __asm__ volatile("ecall");
    __asm__ volatile("mv %[val], a0" : [val] "=r" (code));
    return code;
}
int asem_signal(sem_t id){
    int code;
    __asm__ volatile("mv a1, %[val]" : : [val] "r" (id));
    __asm__ volatile("mv a0, %[code]" : : [code] "r" (csem_signal));
    __asm__ volatile("ecall");
    __asm__ volatile("mv %[val], a0" : [val] "=r" (code));
    return code;
}

int atime_sleep(time_t time){
    int code;
    __asm__ volatile("mv a1, %[val]" : : [val] "r" (time));
    __asm__ volatile("mv a0, %[code]" : : [code] "r" (ctime_sleep));
    __asm__ volatile("ecall");
    __asm__ volatile("mv %[val], a0" : [val] "=r" (code));
    return code;
}

char agetc(){
    char c;
    __asm__ volatile("mv a0, %[code]" : : [code] "r" (cgetc));
    __asm__ volatile("ecall");
    __asm__ volatile("mv %[val], a0" : [val] "=r" (c));
    return c;
}
void aputc(char c){
    __asm__ volatile("mv a1, %[val]" : : [val] "r" (c));
    __asm__ volatile("mv a0, %[code]" : : [code] "r" (cputc));
    __asm__ volatile("ecall");
}