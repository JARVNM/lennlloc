
#include <stdio.h>
#include <stdlib.h>

#define MEM_PAGE_SIZE   4096

typedef struct mempool_s {
    int block_size;
    int free_count;

    char* free_ptr;
    char* mems;
} mempool_t;

int mp_init(mempool_t* m, int size)
{
    if(!m) return -1;
    if(size < 16)
        size = 16;

    m->block_size = size;
    m->mems = (char*)malloc(MEM_PAGE_SIZE);
    if(!m->mems) return -1;
    m->free_ptr = m->mems;
    m->free_count = MEM_PAGE_SIZE / size;
    
    int i = 0;
    char* ptr = m->free_ptr;
    for(i; i < m->free_count; i++)
    {
        *(char**)ptr = ptr + size;
        ptr += size;
    }

    return 0;
}

int mp_dest(mempool_t* m)
{
    if(!m || !m->mems) return -1;
    free(m->mems);
    return 0;
}

void* mp_alloc(mempool_t* m)
{
    if(!m || m->free_count == 0) return NULL;

    void* ptr = m->free_ptr;
    m->free_ptr = *(char**)ptr;
    m->free_count--;

    return ptr;
}

int mp_free(mempool_t* m, void* ptr)
{
    if(!m || !ptr) return -1;

    *(char**)ptr = m->free_ptr;
    m->free_ptr = (char*)ptr;
    m->free_count++;
}

int main()
{
    mempool_t m;
    mp_init(&m, 16);
    void* p1 = mp_alloc(&m);
    printf("1: mp_alloc: %p\n", p1);

    void* p2 = mp_alloc(&m);
    printf("2: mp_alloc: %p\n", p2);

    void* p3 = mp_alloc(&m);
    printf("3: mp_alloc: %p\n", p3);

    void* p4 = mp_alloc(&m);
    printf("4: mp_alloc: %p\n", p4);

    mp_free(&m, p2);

    void* p5 = mp_alloc(&m);
    printf("5: mp_alloc: %p\n", p5);
}