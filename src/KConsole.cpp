#include "../h/KConsole.hpp"

KConsole::KConsole(){}


KConsole& KConsole::instance() {
    static KConsole console;
    return console;
}

char KConsole::kgetc(){
    char c = inputBuffer.take();
    return c;
}
void KConsole::kputc(char c){
    outputBuffer.append(c);
}

void KConsole::transferOut(void* arg){
    while(1){
        while(*(uint8*)(CONSOLE_STATUS) & CONSOLE_TX_STATUS_BIT){
            char c = KConsole::instance().outputBuffer.take();
            if(c == '\r') continue;
            *(uint8*)(CONSOLE_TX_DATA) = c;
        }
        KThread::yield();
    }
}

void KConsole::transferIn(){
    while(*(uint8*)(CONSOLE_STATUS) & CONSOLE_RX_STATUS_BIT){
        char c = *(uint8*)(CONSOLE_RX_DATA);
        if(!KConsole::instance().inputBuffer.isBlckd()){ //ne moze se blokirati proizvodjac
            KConsole::instance().inputBuffer.append(c);
        }
    }
}