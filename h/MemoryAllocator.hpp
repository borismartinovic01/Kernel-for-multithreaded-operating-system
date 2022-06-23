

#ifndef _memoryAllocator_hpp
#define _memoryAllocator_hpp
#include "../lib/hw.h"

class MemoryAllocator{
public:
    static MemoryAllocator& instance();
    void* kmem_alloc(size_t size);
    int kmem_free(void* paddr);
    MemoryAllocator(const MemoryAllocator&) = delete;
    MemoryAllocator& operator=(const MemoryAllocator&) = delete;
    struct BusyNode{
       BusyNode* next;
       size_t size; //velicina zauzetog prostora bez zaglavlja
    };

private:
   BusyNode* busyListHeader;
   MemoryAllocator();
   void kbusy_init();
};
#endif
