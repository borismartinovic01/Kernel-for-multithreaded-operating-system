
#ifndef _kconsole_hpp
#define _kconsole_hpp
#include "../h/KBoundedBuffer.hpp"
#include "../h/Riscv.hpp"

class KConsole{
private:
    KConsole();

//    KSemaphore consoleMutex; //mutex za testiranje

    KBoundedBuffer inputBuffer;
    KBoundedBuffer outputBuffer;
    static void transferIn(); //prenos podataka iz kontrolera u inputBuffer
    static void transferOut(void* arg); //prenos podataka iz outputBuffer-a na kontroler

    friend class Controller;
    friend class Riscv;
public:
    KConsole(const KConsole&) = delete;
    KConsole& operator=(const KConsole&) = delete;

    static KConsole& instance();
    char kgetc(); //prenos podataka iz inputBuffer-a u okviru obrade sistemskog poziva
    void kputc(char c); //prenos podataka u outputBuffer u okviru obrade sistemskog poziva
};

#endif //_kconsole_hpp
