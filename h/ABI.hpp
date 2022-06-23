
#ifndef _abi_hpp
#define _abi_hpp
#include "basic_defs.hpp"
#include "../lib/hw.h"
#include "syscall_defs.hpp"
#include "Riscv.hpp"

void* amem_alloc(size_t size);
int amem_free(void*);

int athread_create(thread_t* handle, void(*start_routine)(void*), void* arg, void* stack_space);
int athread_init(thread_t* handle, void(*start_routine)(void*), void* arg, void* stack_space);
void athread_dispatch();
void athread_start(thread_t handle);
int athread_exit();
int atime_sleep(time_t time);

int asem_open(sem_t* handle, unsigned init);
int asem_close(sem_t handle);
int asem_wait(sem_t id);
int asem_signal(sem_t id);

char agetc();
void aputc(char c);

#endif //_abi_hpp
