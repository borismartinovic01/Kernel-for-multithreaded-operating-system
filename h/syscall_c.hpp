#ifndef _syscall_c_hpp
#define _syscall_c_hpp

#include "../lib/hw.h"
#include "syscall_defs.hpp"
#include "basic_defs.hpp"

void* mem_alloc(size_t size);
int mem_free(void*);

int thread_create(thread_t* handle, void(*start_routine)(void*), void* arg);
void thread_dispatch();
void thread_start(thread_t handle);
int thread_init(thread_t* handle, void(*start_routine)(void*), void* arg);
int thread_exit();
int time_sleep(time_t time);

int sem_open(sem_t* handle, unsigned init);
int sem_close(sem_t handle);
int sem_wait(sem_t id);
int sem_signal(sem_t id);

char getc();
void putc(char c);

#endif
