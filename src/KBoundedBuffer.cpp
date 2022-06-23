#include "../h/KBoundedBuffer.hpp"

KBoundedBuffer::KBoundedBuffer()
    :mutex(1), spaceAvailable(N), itemAvailable(0), head(0), tail(0){}

void KBoundedBuffer::append(char c){
    spaceAvailable.wait();
    mutex.wait();
    buffer[tail] = c;
    tail = (tail + 1)%N;
    mutex.signal();
    itemAvailable.signal();
}

char KBoundedBuffer::take(){
    itemAvailable.wait();
    mutex.wait();
    char c = buffer[head];
    head = (head + 1)%N;
    mutex.signal();
    spaceAvailable.signal();
    return c;
}