//Kernel main
#include "../h/syscall_c.hpp"
#include "../h/Riscv.hpp"

extern int userMain();

int main(){

    __asm__ volatile("csrw stvec, %[vector]" : : [vector] "r" (&Riscv::supervisorTrap));
    Controller::createIdle();
    Controller::createMain();
    Controller::createTransferOut();
//    thread_create(&transferOutThread, KConsole::transferOut, nullptr);
    Riscv::ms_sstatus(Riscv::SSTATUS_SIE);

    userMain();

    Controller::exitTransferOut();
    Controller::exitIdle();
    Controller::exitMain();
    Riscv::mc_sstatus(Riscv::SSTATUS_SIE);
    return 0;
}