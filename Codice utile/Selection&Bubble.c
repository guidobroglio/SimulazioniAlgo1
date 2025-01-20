#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/upo/bst.h"
#include "include/upo/sort.h"
#include "include/upo/hashtable.h"
//#include "include/upo/bst_key_list.h"

/* Funzione di comparazione al fine di eseguire correttamente bubbleSort*/
typedef int (*upo_sort_comparator_t) (const void *a, const void *b);

int upo_sort_comparator (const void *a, const void *b) {
    const unsigned char *aPtr = a;
    const unsigned char *bPtr = b;
    return *aPtr - *bPtr; // Se sono uguali ritorna 0
}

void upo_selection_sort(void *base, size_t n, size_t size, upo_sort_comparator_t cmp) 
{
    assert(base != NULL);
    unsigned char *basePtr = base;
    for(size_t k = 0; k < n-1; k++) 
    {
        size_t m = k;
        for(size_t j = k + 1; j < n; j++) 
        {
            if(cmp(basePtr+j*size, basePtr+m*size)) 
            {
                m = j;
            }
        }
        unsigned char *tmp = *(basePtr+m*size);
        memcpy(basePtr+m*size, basePtr+(k)*size, size);
        *(basePtr+(k)*size) = tmp;
    }
}

void upo_bubble_sort (void *base, size_t n, size_t size, upo_sort_comparator_t cmp) 
{    
    unsigned char *basePtr = base;   
    for(size_t i = 0; i < n; i++) 
    { /* Scorro tutto l'array */
        int scambi = 0;
        for(size_t j = 1; j < n - i; j++) 
        { /* Confrontare due elementi */
            if(cmp(basePtr+(j-1)*size, basePtr+j*size) > 0) 
            {
                unsigned char *tmp;
                /* Inizio swap */
                tmp = *(basePtr + (j-1)*size);
                memcpy(basePtr+(j-1)*size, basePtr+j*size, size); /* Nell'elemento prima metto quello dopo */
                *(basePtr+j*size) = tmp;
                /* Fine swap */
                scambi = 1;
            }
        }
        if(!scambi)
        {
            return;
        }
    }
}

int main(int argc, char const *argv[])
{
    int v[] = {3, 4, 1};
    upo_bubble_sort(v, sizeof(v)/sizeof(int), sizeof(int), upo_sort_comparator);
    for(int i = 0; i < sizeof(v)/sizeof(int); i ++ )
    {
        printf("%d\t", v[i]);
    }
    printf("\n");
    return 0;
}
