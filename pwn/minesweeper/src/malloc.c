#include "malloc.h"
#include <unistd.h>
#include <stdio.h>
#include <string.h>

// first node of the doubly linked free list
node_t* head = 0;
node_t base;

void strcpya(char* buf, char* str) {
    int cnt;
    for (cnt = 0; str[cnt]; cnt++)
        buf[cnt] = str[cnt];
    buf[cnt] = 0;
}

unsigned int strlena(const char* buf) {
    unsigned int cnt;
    for (cnt = 0; buf[cnt]; cnt++);
    return cnt;
}

void memseta(void* buf, char c, int count) {
    char *buffer = (char*)buf;
    int cnt;
    for (cnt = 0; cnt < count; cnt ++ ) 
        buffer[cnt] = c;
}
void add_node(node_t *node) {
    node_t* currp;
    
    // if only the head node, add in between
    if (head->next == head) {
        node->prev = head;
        node->next = head;
        head->prev = node;
        head->next = node;
    }
    else {
        for (currp = head->next; ; currp = currp->next) {
            // sort nodes from lowest to highest size
            if ((node->size <= currp->size) || currp == head) {
                // insert node before curr
                node->next = currp;
                node->prev = currp->prev;
                currp->prev->next = node;
                currp->prev = node;
                break;
            }
        }
    }
}

void delink(node_t *node) {
    node_t *prev = node->prev;
    node_t *next = node->next;
    node->next->prev = prev;
    node->prev->next = next;
    fprintf(stderr, "delinked!");
    //node->prev = 0;
    //node->next = 0;
}

void* malloca(uint32_t size) {
    node_t *currp = 0, *ret = 0, *new_node = 0;
    void *buf;
    
    // search free list to find empty block of this size
    // or larger
    uint32_t chunks = (((sizeof(node_t) + size) - 1) / (sizeof(node_t))) + 1;

    // initialize first time
    if (!head) {
        head = &base;
        head->size = 0;
        head->next = head;
        head->prev = head;
    }
          
    // loop until you are back at the head again
    // or you find a node with same or larger than
    // requested size
    for (currp=head->next; currp != head; currp = currp->next) {
        if (currp->size >= chunks) {
            ret = currp;
            break;
        }
    }
    
    // if node found, if same size, delink and return
    if (ret && ret->size == chunks) {
        delink(ret);
        return (void*)(ret + 1);
    }

    // if no node, allocate memory, create node
    if (!ret) {
        buf = sbrk(0x1000);
        if (buf == (void*)-1)
            return (void*)-1;
        
        ret = (node_t*)buf;
        ret->size = 0x1000 / sizeof(node_t);
        
    }
    
    // then in both cases (except same size)
    // split node into node1, node2
    // if node1 is valid, unlink node1,
    // add node2 to free list, return node1
    
    if (ret && ret->size > chunks) {
        // split node into node1 <- ret, node2 <- new_node
        new_node = ret + chunks;
        new_node->size = ret->size - chunks;
        ret->size = chunks;
        // if ret is valid, delink it
        if (ret->next && ret->prev)
            delink(ret);
        // add rest of the block to the free list
        add_node(new_node);
        return (void*)(ret + 1);
    }

    // should not reach this case ever
    return (void*)-1;

}

void freea(void *ptr) {
    // get pointer to the head of this node
    // add node to the free list
    node_t* node = (node_t*) (((char*)ptr) - sizeof(node_t));
    add_node(node);
}

