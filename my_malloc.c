#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include "my_malloc.h"

#define HEAP_SIZE 1024  

// structure pour un bloc de mémoire hehe
typedef struct Block {
    size_t size;          // size bloc lol
    bool free;            
    struct Block* next;   // pointeur vers bloc d'après :3
} Block;

#define BLOCK_SIZE sizeof(Block)

static char heap[HEAP_SIZE];  
static Block* freeList = (Block*)heap;  // encore un pointeur mais cette fois sur le 1er bloc libre

// on initialise le tas comme le void l'indique en fait 
void initialize_heap() {
    freeList->size = HEAP_SIZE - BLOCK_SIZE;
    freeList->free = true;
    freeList->next = NULL;
}

// simple fct malloc
void* my_malloc(size_t size) {
    Block* current = freeList;
    
    while (current != NULL) {
        if (current->free && current->size >= size) {
            // trouver un bloc assez grand
            size_t remaining_size = current->size - size - BLOCK_SIZE;
            
            // si le bloc est suffisamment grand pour être divisé dcp 
            if (remaining_size > 0) {
                Block* newBlock = (Block*)((char*)current + BLOCK_SIZE + size);
                newBlock->size = remaining_size;
                newBlock->free = true;
                newBlock->next = current->next;
                current->next = newBlock;
                current->size = size;
            }
            
            // on marque le bloc comme alloué hehe
            current->free = false;
            return (void*)((char*)current + BLOCK_SIZE);
        }
        
        current = current->next;
    }
    
    // par contre si y a pas de mémoire libre
    return NULL; // "c'est nulllllllll - Homer Simpson"
}

// fct de malloc avec stratégie Best-Fit ( ouais je sais je fais genre)
void* my_malloc_best_fit(size_t size) {
    Block* bestBlock = NULL;
    Block* current = freeList;
    
    // on parcours la liste pour trouver le meilleur bloc ( the best bloc ever)
    while (current != NULL) {
        if (current->free && current->size >= size) {
            if (bestBlock == NULL || current->size < bestBlock->size) {
                bestBlock = current;
            }
        }
        current = current->next;
    }
    
    // si on a trouvé un bloc approprié
    if (bestBlock != NULL) {
        size_t remaining_size = bestBlock->size - size - BLOCK_SIZE;

        // on divise si possible
        if (remaining_size > 0) {
            Block* newBlock = (Block*)((char*)bestBlock + BLOCK_SIZE + size);
            newBlock->size = remaining_size;
            newBlock->free = true;
            newBlock->next = bestBlock->next;
            bestBlock->next = newBlock;
            bestBlock->size = size;
        }
        
        bestBlock->free = false;
        return (void*)((char*)bestBlock + BLOCK_SIZE);
    }

    // bon bah lol encore si y a rien t'as capté, c'est nul
    return NULL;
}

// fct de libération de mémoire 
void my_free(void* ptr) {
    if (!ptr) return;
    
    Block* block = (Block*)((char*)ptr - BLOCK_SIZE);
    block->free = true;

    // fusionne avec le bloc suivant si libre
    if (block->next != NULL && block->next->free) {
        block->size += BLOCK_SIZE + block->next->size;
        block->next = block->next->next;
    }

    // fusion avec le bloc précédent si possible
    Block* current = freeList;
    while (current != NULL) {
        if (current->next == block && current->free) {
            current->size += BLOCK_SIZE + block->size;
            current->next = block->next;
            break;
        }
        current = current->next;
    }
}

// fct de réallocation (realloc)
void* my_realloc(void* ptr, size_t new_size) {
    if (!ptr) {
        // si le pointeur est NULL, faire un malloc classique
        return my_malloc(new_size);
    }

    Block* block = (Block*)((char*)ptr - BLOCK_SIZE);

    // si le bloc actuel est suffisamment grand, on le garde
    if (block->size >= new_size) {
        return ptr;
    }

    // si le bloc suivant est libre et assez grand, on l'étend
    if (block->next != NULL && block->next->free && 
        block->size + BLOCK_SIZE + block->next->size >= new_size) {
        block->size += BLOCK_SIZE + block->next->size;
        block->next = block->next->next;
        return ptr;
    }

    // sinon bahhhhh, on doit allouer un nouveau bloc
    void* new_ptr = my_malloc(new_size);
    if (new_ptr) {
        // copie les anciennes données vers le nouveau bloc
        memcpy(new_ptr, ptr, block->size);
        my_free(ptr);  // libère l'ancien bloc
    }

    return new_ptr;
}


