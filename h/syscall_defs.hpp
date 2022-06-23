#ifndef _syscall_defs_hpp
#define _syscall_defs_hpp
#include "../lib/hw.h"

const uint64 cmem_alloc = 0x01;
const uint64 cmem_free = 0x02;
const uint64 cthread_create = 0x11;
const uint64 cthread_exit = 0x12;
const uint64 cthread_dispatch = 0x13;
const uint64 cthread_start = 0x14;
const uint64 cthread_init = 0x15;
const uint64 csem_open = 0x21;
const uint64 csem_close = 0x22;
const uint64 csem_wait = 0x23;
const uint64 csem_signal = 0x24;
const uint64 ctime_sleep = 0x31;
const uint64 cgetc = 0x41;
const uint64 cputc = 0x42;

#endif
