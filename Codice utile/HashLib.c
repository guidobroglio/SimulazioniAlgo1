#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/upo/bst.h"
#include "include/upo/sort.h"
#include "include/upo/hashtable.h"
//#include "include/upo/bst_key_list.h"

upo_ht_linprob_t upo_ht_linprob_create(size_t m, upo_ht_hasher_t key_hash,  upo_ht_comparator_t key_cmp)  
{
	upo_ht_linprob_t ht = NULL;
	size_t i = 0;
	assert( key_hash != NULL );
	assert( key_cmp != NULL );

    ht = malloc(sizeof(struct upo_ht_linprob_s));
    if (ht == NULL)   
    {
    	perror("Unable to allocate memory for Hash Table with Linear Probing");
    	abort();	
    }
    if (m > 0)  
    {
        ht->slots = malloc(m*sizeof(upo_ht_linprob_slot_t))
        if (ht->slots == NULL)   
        {
   	        perror("Unable to allocate memory for slots of the Hash Table with Separate Chaining");
   	        abort();	
        }
        for (i = 0; i < m; ++i)  
        {
            ht->slots[i].key = NULL;
            ht->slots[i].value = NULL;
            ht->slots[i].tombstone = 0;	
        }    
    }
	ht->capacity = m;
	ht->size = 0;
	ht->key_hash = key_hash;
	ht->key_cmp = key_cmp;
	return ht;  
}

void upo_ht_linprob_destroy(upo_ht_linprob_t ht, int destroy_data)
{	
    if (ht != NULL)   
    {
        upo_ht_linprob_clear(ht, destroy_data);
        free(ht->slots);
        free(ht);	
    }  
}
 
void upo_ht_linprob_clear(upo_ht_linprob_t ht, int destroy_data)
{  
    if (ht != NULL && ht->slots != NULL)   
    {
    	size_t i = 0;
        for (i = 0; i < ht->capacity; ++i)	
        {
            if (ht->slots[i].key != NULL)  
            {
  	            if (destroy_data) 
                {
  	                free(ht->slots[i].key);
                    free(ht->slots[i].value);	
                }
                ht->slots[i].key = NULL;
                ht->slots[i].value = NULL;
                ht->slots[i].tombstone = 0;  
            }    
        }
    	ht->size = 0;   
    }  
} 
 
void* upo_ht_linprob_put(upo_ht_linprob_t ht, void *key, void *value)
{ 
    void *old_value = NULL;
    if(ht != NULL && ht->slots != NULL) 
    {
        if(upo_ht_linprob_load_factor(ht) >= 0.5) 
        {
            upo_ht_linprob_resize(ht, (ht->capacity)*2);   
        }
        size_t h = ht->key_hash(key, ht->capacity);
        int foundTomb = 0;
        size_t hTomb;	
        while(ht->slots[h].key != NULL && ht->key_cmp(key, ht->slots[h].key) != 0 || ht->slots[h].tombstone == 1) 
        {
            if(ht->slots[h].tombstone == 1 && foundTomb == 0) 
            {
                foundTomb = 1;
                hTomb = h; 	
            }
            h = (h + 1) % ht->capacity;   
        }
        if(ht->slots[h].key == NULL) 
        {
            if(foundTomb == 1) 
            {
                h = hTomb;   
            }
            ht->slots[h].key = key;
            ht->slots[h].value = value;
            ht->slots[h].tombstone = 0;
            ht->size++;
        } 
        else 
        {
            old_value = ht->slots[h].value;
            ht->slots[h].value = value;   
        }	
    }
	return old_value;   
}

void upo_ht_linprob_insert(upo_ht_linprob_t ht, void *key, void *value)
{   
    if(ht != NULL && ht->slots != NULL) 
    {
        if(upo_ht_linprob_load_factor(ht) >= 0.5) 
        {
            upo_ht_linprob_resize(ht, (ht->capacity)*2);   
        }
        size_t h = ht->key_hash(key, ht->capacity);
        int foundTomb = 0;
        size_t hTomb;
        while(ht->slots[h].key != NULL && ht->key_cmp(key, ht->slots[h].key) != 0 || ht->slots[h].tombstone == 1) 
        {
            if(ht->slots[h].tombstone == 1 && foundTomb == 0) 
            {
                foundTomb = 1;
                hTomb = h;  	
            }
            h = (h + 1) % ht->capacity;
        }
        if(ht->slots[h].key == NULL) 
        {
            if(foundTomb == 1) 
            {
                h = hTomb;	
            }
            ht->slots[h].key = key;
            ht->slots[h].value = value;
            ht->slots[h].tombstone = 0;
            ht->size++;   
        }   
    }   
} 
 
void* upo_ht_linprob_get(const upo_ht_linprob_t ht, const void *key)
{ 
    if(ht != NULL && ht->slots != NULL) 
    {
        size_t h = ht->key_hash(key, ht->capacity);
        while(ht->slots[h].key != NULL && ht->key_cmp(key, ht->slots[h].key) != 0 || ht->slots[h].tombstone == 1) 
        {
            h = (h + 1) % ht->capacity;
        }
        if(ht->slots[h].key != NULL) 
        {
            return ht->slots[h].value;
        } 
        else 
        {   
            return NULL;
        }   
    }  
    return NULL;  
}
 
int upo_ht_linprob_contains(const upo_ht_linprob_t ht, const void *key)
{   
    return upo_ht_linprob_get(ht, key) == NULL ? 0 : 1; 
}

void upo_ht_linprob_delete(upo_ht_linprob_t ht, const void *key, int destroy_data)
{  
    if(ht != NULL && ht->slots != NULL) 
    {
        size_t h = ht->key_hash(key, ht->capacity);
        while(ht->slots[h].key != NULL && ht->key_cmp(key, ht->slots[h].key) != 0 || ht->slots[h].tombstone == 1) 
        {
            h = (h + 1) % ht->capacity;    
        }
        if(ht->slots[h].key != NULL) 
        {
            ht->slots[h].key = NULL;
            ht->slots[h].value = NULL;
            ht->slots[h].tombstone = 1;
            ht->size -= 1;
            if(upo_ht_linprob_load_factor(ht) <= 0.125) 
            {
                upo_ht_linprob_resize(ht, ht->capacity/2);  
            } 
        }  
    }
}

size_t upo_ht_linprob_size(const upo_ht_linprob_t ht)
{  
    return ht == NULL ? 0 : ht->size; 
}
 
int upo_ht_linprob_is_empty(const upo_ht_linprob_t ht)
{	
    return upo_ht_linprob_size(ht) == 0 ? 1 : 0;  
}
 
size_t upo_ht_linprob_capacity(const upo_ht_linprob_t ht)
{   
    return (ht != NULL) ? ht->capacity : 0; 
}
 
double upo_ht_linprob_load_factor(const upo_ht_linprob_t ht)
{   
    return upo_ht_linprob_size(ht) / (double) upo_ht_linprob_capacity(ht); 
}
 
void upo_ht_linprob_resize(upo_ht_linprob_t ht, size_t n)
{  
    if (ht != NULL)     
    { 
        size_t i = 0;
        upo_ht_linprob_t new_ht = NULL;
        new_ht = upo_ht_linprob_create(n, ht->key_hash, ht->key_cmp);
        if (new_ht == NULL)  
        {
            perror("Unable to allocate memory for slots ");   abort();     
        }
        for (i = 0; i < ht->capacity; ++i)
        {  
            if (ht->slots[i].key != NULL)   
            {
                upo_ht_linprob_put(new_ht, ht->slots[i].key, ht->slots[i].value);   
            }    	
        }
        upo_swap(&ht->slots, &new_ht->slots, sizeof ht->slots);
        upo_swap(&ht->capacity, &new_ht->capacity, sizeof ht->capacity);
        upo_swap(&ht->size, &new_ht->size, sizeof ht->size);
        upo_ht_linprob_destroy(new_ht, 0);   
    }  
}
 
upo_ht_key_list_t upo_ht_linprob_keys(const upo_ht_linprob_t ht)
{	
    upo_ht_key_list_t list = NULL;
    if(ht!= NULL && ht->slots != NULL) 
    {
        for(int i = 0; i < ht->capacity; i++) 
        {
            if(ht->slots[i].tombstone != 0) 
            {
	            upo_ht_key_list_node_t *node = malloc(sizeof(struct upo_ht_list_node_s));
	            if (node == NULL) 
                {
	                perror("Nodo non creato correttamente");
	                abort();   
                }
                node->key = ht->slots[i].key;
                node->next = list;
                list = node; 
            }  
        }  
    }
    return list;  
}
 
void upo_ht_linprob_traverse(const upo_ht_linprob_t ht, upo_ht_visitor_t visit, void *visit_arg)
{  
    if(ht != NULL && ht->slots != NULL) 
    {
        for(int i = 0; i < ht->capacity; i++) 
        {
            upo_ht_sepchain_list_node_t *node = NULL;
            for(node = ht->slots[i].head; node != NULL; node = node->next)
            {
                visit(node->key, node->value, visit_arg);
            }
        }     	
    }     
}
_______________________________________________
 
/*Implementare un algoritmo che, data una tabella hash H con gestione delle collisioni basata
su indirizzamento aperto a scansione lineare con tecnica del "tombstone",
una chiave k ed un valore booleano d, cancelli da H la coppia chiave-valore identificata da k e ritorni
True per identificare l'avvenuta cancellazione.
*/
int upo_ht_linprob_deletex(upo_ht_linprob_t ht, const void *key, int destroy_data) 
{
	if(ht == NULL)
    {
        return 0;
    }
	size_t h = ht->key_hash(key, ht->capacity);
	while(ht->slots[h].key != NULL && ht->key_cmp(ht->slots[h].key, key) != 0 || ht->slots[h].tombstone == 1)
    {
        h = (h + 1) % ht->capacity;
    }
	if(ht->slots[h].key != NULL) 
    {
    	if(destroy_data) 
        {
            free(ht->slots[h].key);
            free(ht->slots[h].value);
    	}
    	ht->slots[h].key = NULL;
    	ht->slots[h].value = NULL;
    	ht->slots[h].tombstone = 0;
    	ht->size -= 1;
  	  return 1;
	}
	else return 0;
}
 