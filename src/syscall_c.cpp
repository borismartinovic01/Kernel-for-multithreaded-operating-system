#include "../h/syscall_c.hpp"
#include "../lib/hw.h"
#include "../h/ABI.hpp"

void* mem_alloc(size_t size){
    size_t numOfBlocks = size/MEM_BLOCK_SIZE;
    if(size>numOfBlocks*MEM_BLOCK_SIZE) numOfBlocks++;
    return amem_alloc(numOfBlocks);
}
int mem_free(void* addr){
    return amem_free(addr);
}

int thread_create(thread_t* handle, void(*start_routine)(void*), void* arg){
    int code;
    void* stack_space = nullptr;
    if(start_routine!=nullptr || arg!=nullptr){ // ako nije u pitanju main thread
        stack_space = mem_alloc(sizeof(uint64[DEFAULT_STACK_SIZE]));
        if(stack_space == 0) return -2; //can't allocate stack
    }
    code = athread_create(handle, start_routine, arg, stack_space);
    if(code == 0) return code;
    mem_free(stack_space);
    return -1;
}

int thread_init(thread_t* handle, void(*start_routine)(void*), void* arg){
    int code;
    void* stack_space = nullptr;
    if(start_routine!=nullptr || arg!=nullptr){ // ako nije u pitanju main thread
        stack_space = mem_alloc(sizeof(uint64[DEFAULT_STACK_SIZE]));
        if(stack_space == 0) return -2; //can't allocate stack
    }
    code = athread_init(handle, start_routine, arg, stack_space);
    if(code == 0) return code;
    mem_free(stack_space);
    return -1;
}

void thread_dispatch(){
    athread_dispatch();
}

void thread_start(thread_t handle){
    athread_start(handle);
}

int thread_exit(){
    int code = athread_exit();
    return code;
}

int sem_open(sem_t* handle, unsigned init){
    int code = asem_open(handle, init);
    return code;
}

int sem_close(sem_t handle){
    int code = asem_close(handle);
    return code;
}
int sem_wait(sem_t id){
    int code = asem_wait(id);
    return code;
}
int sem_signal(sem_t id){
    int code = asem_signal(id);
    return code;
}

int time_sleep(time_t time){
    int code = atime_sleep(time);
    return code;
}

char getc(){
    return agetc();
}
void putc(char c){
    aputc(c);
}