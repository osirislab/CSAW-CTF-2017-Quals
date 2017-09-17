
#ifndef MALLOC_H
#define MALLOC_H

#include <stdint.h>

struct node_t_ {
    uint16_t size;
    struct node_t_ *next;
    struct node_t_ *prev;
};
typedef struct node_t_ node_t;

void* malloca(uint32_t size);
void freea(void *ptr);
void memseta(void* buf, char c, int cnt);
void strcpya(char* buf, char* str);
unsigned int strlena(const char* buf);

#endif
