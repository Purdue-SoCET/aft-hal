
#include <new>
#include <cstddef>
#include <cstdint>
#include "stdlib.h"

#define K 1024

#ifdef __cplusplus
extern "C" {
#endif
extern uint8_t __heap_start[];
#ifdef __cplusplus
}
#endif

/*uint8_t *heap_ptr = __heap_start;
const uint32_t heap_size = 8*K;
const uint8_t *heap_max = heap_ptr + heap_size;*/


template <size_t N, size_t MemSize>
class SlabAllocator {
private:
    size_t m_slab_count;
    void *m_base_ptr;
    uint8_t freelist[MemSize / 8];
public:
    SlabAllocator(void *base_ptr) : m_slab_count(MemSize / N), m_base_ptr(base_ptr) {
        print("SlabAllocator: Base pointer = ");
        putinthex((uint32_t)m_base_ptr);
        print("\n");
        //__builtin_memset(freelist, 0, m_slab_count / 8);
    }

    void *allocate() {
        for(auto i = 0; i < m_slab_count / 8; i++) {
            for(auto j = 0; j < 8; j++) {
                if((freelist[i] & (1 << j)) == 0) {
                    freelist[i] |= (1 << j);
                    void *ptr = (void *)(((uint32_t)m_base_ptr) + ((i*8) + j)*N);
                    print("SlabAllocator: Returning ");
                    putinthex((uint32_t)ptr);
                    print("\n");
                    return ptr;
                }
            }
        }

        print("SlabAllocator: Unable to allocate\n");
        return nullptr;
    }

    void deallocate(void *ptr) {
        auto offset = (uint32_t)ptr - (uint32_t)m_base_ptr;
        auto arr_idx = offset / 8;
        auto bit_idx = offset % 8;
        freelist[arr_idx] &= ~(1 << bit_idx);
    }

    bool in_range(void *ptr) {
        return ((uint32_t)ptr - (uint32_t)m_base_ptr) < MemSize;
    }
};



/*void __attribute__((constructor)) print_heap_startup() {
    print("Heap start ptr is ");
    putinthex((uint32_t)heap_ptr);
    print("\n");
}*/

class HeapManager {
private:
    uint8_t *m_heap_ptr;
    size_t m_heap_size;
    uint8_t *m_heap_top;
    SlabAllocator<8, 1*K> allocator_8;
    SlabAllocator<16, 1*K> allocator_16;
    SlabAllocator<32, 1*K> allocator_32;
    SlabAllocator<64, 1*K> allocator_64;

public:
    HeapManager(uint8_t *heap_ptr, size_t heap_size) 
        : m_heap_ptr(heap_ptr), m_heap_size(heap_size), m_heap_top(heap_ptr + heap_size),
            allocator_8(sbrk(1*K)), allocator_16(sbrk(1*K)), allocator_32(sbrk(1*K)), allocator_64(sbrk(1*K)) {
        print("HeapManager: starting with ptr = "); putinthex((uint32_t)m_heap_ptr); print("\n");
    }

private:
    bool fits(size_t sz) {
        return (m_heap_ptr + sz) < m_heap_top;
    }

public:
    void *sbrk(size_t sz) {
        if(sz % 16 != 0) {
            sz += (16 - (sz % 16));
        }

        if(fits(sz)) {
            void *ptr = (void *)m_heap_ptr;
            m_heap_ptr += sz;
            print("sbrk: Returning pointer "); putinthex((uint32_t)ptr); print("\n");
            return ptr;
        } else {
            print("sbrk: Returning nullptr!\n");
            return nullptr;
        }
    }
    
    void *slab_allocate(size_t sz) {
        if(sz <= 8) {
            return allocator_8.allocate();
        } else if(sz <= 16) {
            return allocator_16.allocate();
        } else if(sz <= 32) {
            return allocator_32.allocate();
        } else if(sz <= 64) {
            return allocator_64.allocate();
        } else {
            // FIXME: Add ability to alloc contiguous slabs for larger sizes
            print("HeapManager::slab_allocate: Unable to allocate: requested size too large!\n");
            return nullptr; // Cannot alloc that much
        }
    }

    void slab_deallocate(void *ptr) {
        if(allocator_8.in_range(ptr)) {
            allocator_8.deallocate(ptr);
        } else if(allocator_16.in_range(ptr)) {
            allocator_16.deallocate(ptr);
        } else if(allocator_32.in_range(ptr)) {
            allocator_32.deallocate(ptr);
        } else if(allocator_64.in_range(ptr)) {
            allocator_64.deallocate(ptr);
        } else {
            print("HeapManager::slab_deallocate: Warning: Unable to deallocate (bad argument)");
        }
    }
};

static HeapManager heap(__heap_start, 8*K);

/*
static void *sbrk(size_t sz) {
    static uint8_t *heap_ptr = __heap_start;
    const uint32_t heap_size = 8*K;
    static uint8_t *heap_max = heap_ptr + heap_size;
    print("sbrk: ");
    putinthex((uint32_t)heap_ptr);
    print("\n");
    // Alignment check -- always 16B aligned
    if(sz % 16 != 0) {
        sz += (16 - (sz % 16));
    }

    print("sbrk: Allocating ");
    putinthex(sz);
    print(" bytes.\n");

    if(heap_ptr + sz < heap_max) {
        void *rv = (void *)heap_ptr;
        heap_ptr += sz;
        print("Alloc success\n");
        return rv;
    }

    print("Alloc failed\n");
    return nullptr;
}*/


void *malloc(size_t sz) {
    //return heap.sbrk(sz);
    return heap.slab_allocate(sz);
}

void free(void *ptr) {
    heap.slab_deallocate(ptr);

    // FIXME: Should be a panic condition
}

void *operator new(size_t size) noexcept {
    return malloc(size);
}

void operator delete(void *p) noexcept {
    free(p);
}

void *operator new[](size_t size) noexcept {
    return operator new(size);
}

void operator delete[](void *p) noexcept {
    operator delete(p);
}

void *operator new(size_t size, std::nothrow_t) noexcept {
    return operator new(size);
}

void operator delete(void *p, std::nothrow_t) noexcept {
    operator delete(p);
}

void *operator new[](size_t size, std::nothrow_t) noexcept {
    return operator new(size);
}

void operator delete[](void *p, std::nothrow_t) noexcept {
    operator delete(p);
}
