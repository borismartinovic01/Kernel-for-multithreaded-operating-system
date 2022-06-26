
#ifndef _kBoundedBuffer_hpp
#define _kBoundedBuffer_hpp
#include "KSemaphore.hpp"

class KBoundedBuffer{

public:
    KBoundedBuffer();
    void append(char c);
    char take();
    bool isBlckd() const { return spaceAvailable.value() == 0 || mutex.value() == 0;}
private:
    static const int N = 256;
    KSemaphore mutex;
    KSemaphore spaceAvailable;
    KSemaphore itemAvailable;

    char buffer[N];
    int head, tail;
};

#endif //_kBoundedBuffer_hpp
