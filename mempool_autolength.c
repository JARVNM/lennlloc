
#include <stdio.h>
#include <stdlib.h>

#define MEM_PAGE_SIZE 4096

typedef struct mp_node_s {
    char* free_ptr;
    char* end;
    struct mp_node_s* next;
} mp_node_t;

typedef struct mp_pool_s {
    mp_node_t* current;
    mp_node_t* first;
    int max;
} mp_pool_t;

int mp_init(mp_pool_t* m, int size)
{
    if(!m)  return -1;

    void* addr = malloc(size);
    mp_node_t* node = (mp_node_t*)addr;
    node->free_ptr = (char*)(addr + 1);
    node->end = (char*)(addr + size);
    node->next = NULL;
    
    m->first = node;
    m->current = node;
    m->max = size;

    return 0;
}

int mp_dest(mp_pool_t* m)
{
    if(!m) return -1;

    while(!m->first)
    {
        void* addr = m->first;
        mp_node_t* node = (mp_node_t*)addr;
        m->first = node->next;
        free(addr);
    }

    return 0;
}

void* mp_alloc(mp_pool_t* m, int size)
{
    void* addr = m->current;
    mp_node_t* node = (mp_node_t*)addr;

    do
    {
        if(size < (node->end - node->free_ptr))
        {
            char* ptr = node->free_ptr;
            node->free_ptr += size;

            return ptr;
        }

        node = node->next;

    }while(node);

    addr = malloc(m->max);
    node = (mp_node_t*)addr;

    node->free_ptr = (char*)(addr + 1);
    node->end = addr + m->max;
    node->next = m->current;
    m->current = node;

    char* ptr = node->free_ptr;
    node->free_ptr += size;

    return ptr;
}

int mp_free(mp_pool_t* m, void* ptr)
{
    if(!m || !ptr) return -1;

    mp_node_t* cur = m->current;
    mp_node_t* prev = m->current;

    while(cur != ptr)
    {
        prev = cur;
        cur = cur->next;
        if(cur == ptr)
        {
            mp_node_t* tmp = cur;
            prev->next = cur->next;
            cur->next = NULL;
            free(tmp);
            return 0;
        }
    }

    return 0;
}

int main() {

	
	mp_pool_t m;

	mp_init(&m, MEM_PAGE_SIZE);


	void *p1 = mp_alloc(&m, 16);
	printf("1: mp_alloc: %p\n", p1);

	void *p2 = mp_alloc(&m, 32);
	printf("2: mp_alloc: %p\n", p2);

	void *p3 = mp_alloc(&m, 64);
	printf("3: mp_alloc: %p\n", p3);

	void *p4 = mp_alloc(&m, 128);
	printf("4: mp_alloc: %p\n", p4);

	void *p5 = mp_alloc(&m, 256);
	printf("5: mp_alloc: %p\n", p5);

	mp_dest(&m);

}