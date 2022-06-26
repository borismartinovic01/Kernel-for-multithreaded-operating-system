
#ifndef _list_hpp
#define _list_hpp
#include "MemoryAllocator.hpp"

template<typename T>
class List
{
private:
    struct Elem
    {
        T *data;
        Elem *next;

        Elem(T *data, Elem *next) : data(data), next(next) {}

        void* operator new(size_t size) {
            size_t numOfBlocks = size/MEM_BLOCK_SIZE;
            if(size>numOfBlocks*MEM_BLOCK_SIZE) numOfBlocks++;
            return MemoryAllocator::instance().kmem_alloc(numOfBlocks);
        }

        void* operator new[](size_t size){
            size_t numOfBlocks = size/MEM_BLOCK_SIZE;
            if(size>numOfBlocks*MEM_BLOCK_SIZE) numOfBlocks++;
            return MemoryAllocator::instance().kmem_alloc(numOfBlocks);
        }

        void operator delete(void *p){
            MemoryAllocator::instance().kmem_free(p);
        }

        void operator delete[](void *p){
            MemoryAllocator::instance().kmem_free(p);
        }
    };

    Elem *head, *tail, *cur;
public:
    List() : head(0), tail(0) {}

    List(const List<T> &) = delete;

    List<T> &operator=(const List<T> &) = delete;

    void addFirst(T *data)
    {
        Elem *elem = new Elem(data, head);
        head = elem;
        if (!tail) { tail = head; }
    }

    void setFirst(){
        cur = head;
    }

    void next(){
        if(cur) cur = cur->next;
    }

    bool isCur(){
        return cur != 0;
    }

    void addLast(T *data)
    {
        Elem *elem = new Elem(data, 0);
        if (tail)
        {
            tail->next = elem;
            tail = elem;
        } else
        {
            head = tail = elem;
        }
    }

    void addBefore(T *data){ //tricky, samo se zameni sadrzaj ne i sami cvorovi
        Elem *elem = new Elem(cur->data, cur->next);
        cur->data = data;
        cur->next = elem;
        if(cur == tail) tail = elem; // i po potrebi se azurira tail
    }

    T *removeFirst()
    {
        if (!head) { return 0; }

        Elem *elem = head;
        head = head->next;
        if (!head) { tail = 0; }

        T *ret = elem->data;
        delete elem;
        return ret;
    }

    T *peekFirst()
    {
        if (!head) { return 0; }
        return head->data;
    }

    T *peekCur(){
        if (!cur) { return 0; }
        return cur->data;
    }

    T *removeLast()
    {
        if (!head) { return 0; }

        Elem *prev = 0;
        for (Elem *curr = head; curr && curr != tail; curr = curr->next)
        {
            prev = curr;
        }

        Elem *elem = tail;
        if (prev) { prev->next = 0; }
        else { head = 0; }
        tail = prev;

        T *ret = elem->data;
        delete elem;
        return ret;
    }

    T *peekLast()
    {
        if (!tail) { return 0; }
        return tail->data;
    }

};


#endif //_list_hpp
