#include "../h/Riscv.hpp"


void Riscv::popSppSpie()
{
    __asm__ volatile ("csrw sepc, ra");
    __asm__ volatile ("sret");
}

void Riscv::handleSupervisorTrap(){
    uint64 scauseVal = Riscv::r_scause();
    if(scauseVal == consoleTrap){
        handle_console();
    }
    else if(scauseVal == timerTrap){
        KThread::timeSliceCounter++;
        KSleepList::update();
        if (KThread::timeSliceCounter >= KThread::running->getTimeSlice())
        {
            uint64 sepc = r_sepc();
            uint64 sstatus = r_sstatus();
            KThread::timeSliceCounter = 0;
            KThread::dispatch();
            w_sstatus(sstatus);
            w_sepc(sepc);
        }
        mc_sip(SIP_SSIP);
    }
    else if(scauseVal == userTrap || scauseVal == systemTrap){
        uint64 sepc = r_sepc() + 4; //prelazak na sledecu instrukciju posle ecall
        uint64 sstatus = r_sstatus();
        syscall_handler();
        __asm__ volatile("sd a0, 0xa0(sp)"); //prosledjivanje povratne vrednosti na stek 50 + 50 heksa, 50 je pomeraj iz prekidne rutine +50 jer se sp promenio kada se pozvala handleSupervisorTrap()
        w_sstatus(sstatus);
        w_sepc(sepc);
    }
    else {
    }

}

void Riscv::handle_console(){
    int irq = plic_claim();
    if(irq == CONSOLE_IRQ){
        KConsole::transferIn();
    }
    plic_complete(irq);
}


void Riscv::syscall_handler(){

    uint64 code;
    __asm__ volatile("mv %[val], a0" : [val] "=r" (code));

    switch(code){
        case cmem_alloc:
            Controller::kmem_alloc();
            break;
        case cmem_free:
            Controller::kmem_free();
            break;
        case cthread_create:
            Controller::kthread_create();
            break;
        case cthread_exit:
            Controller::kthread_exit();
            break;
        case cthread_dispatch:
            Controller::kthread_dispatch();
            break;
        case cthread_start:
            Controller::kthread_start();
            break;
        case cthread_init:
            Controller::kthread_init();
            break;
        case csem_open:
            Controller::ksem_open();
            break;
        case csem_close:
            Controller::ksem_close();
            break;
        case csem_wait:
            Controller::ksem_wait();
            break;
        case csem_signal:
            Controller::ksem_signal();
            break;
        case ctime_sleep:
            Controller::ktime_sleep();
            break;
        case cgetc:
            Controller::kgetc();
            break;
        case cputc:
            Controller::kputc();
            break;
        default:
            break;
    }

}