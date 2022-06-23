#ifndef _trap_defs_hpp
#define _trap_defs_hpp
#include "../lib/hw.h"

const uint64 timerTrap = 0x01UL << 63 | 0x01;
const uint64 consoleTrap = 0x01UL << 63 | 0x09;
const uint64 instrTrap = 0x02;
const uint64 readTrap = 0x05;
const uint64 writeTrap = 0x07;
const uint64 userTrap = 0x08;
const uint64 systemTrap = 0x09;

#endif
