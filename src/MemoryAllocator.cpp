
#include "../h/MemoryAllocator.hpp"


void MemoryAllocator::kbusy_init() {
    busyListHeader = 0;
}

MemoryAllocator::MemoryAllocator() {
    kbusy_init();
}

MemoryAllocator& MemoryAllocator::instance() {
    static MemoryAllocator allocator;
    return allocator;
}

void* MemoryAllocator::kmem_alloc(size_t size) { //size param. je velicina u blokovima

    BusyNode* tmp = busyListHeader;
    BusyNode* prev = 0;
    char* left = (char*)HEAP_START_ADDR;

    while(tmp != 0 && ((size_t)tmp - (size_t)left) < (size*MEM_BLOCK_SIZE + (size_t)(sizeof(BusyNode)))){
        left = (char*)((size_t)tmp + tmp->size + (size_t)(sizeof(BusyNode)));
        prev = tmp;
        tmp = tmp->next;
    }

    if(tmp == 0 && prev != 0 && ((size_t)HEAP_END_ADDR - (size_t)left) < (size*MEM_BLOCK_SIZE + (size_t)(sizeof(BusyNode)))){
        return 0; // Nema dovoljno slobodne memorije
    }

    BusyNode* newNode;
    if(prev == 0) {
        newNode = (BusyNode*)HEAP_START_ADDR;
        busyListHeader = newNode;
    }
    else {
        newNode = (BusyNode*)left;
        prev->next = newNode;
    }
    newNode->size = size*MEM_BLOCK_SIZE;
    newNode->next = tmp;

    left = (char*)((size_t)newNode + newNode->size + (size_t)(sizeof(BusyNode)));
    size_t minSize = MEM_BLOCK_SIZE + sizeof(BusyNode);
    size_t chunk;
    if(tmp == 0) {
        chunk = (size_t)HEAP_END_ADDR - (size_t)left;
    }
    else{
        chunk = (size_t)tmp - (size_t)left;
    }
    if(chunk < minSize){
        newNode->size += chunk;
    }
    return (char*)newNode + sizeof(BusyNode);
}

int MemoryAllocator::kmem_free(void* addr){

    BusyNode* tmp = busyListHeader;
    BusyNode* prev = 0;

    while(tmp != 0 && ((char*)addr != (char*)tmp + sizeof(BusyNode))){
        prev = tmp;
        tmp = tmp->next;
    }

    if(tmp == 0){
        return -1; //exception
    }
    
    if(prev != 0){
        prev->next = tmp->next;
    }
    else{
        busyListHeader = tmp->next;
    }

    return 0;
}
