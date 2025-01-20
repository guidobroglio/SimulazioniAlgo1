#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/upo/bst.h"
#include "include/upo/sort.h"
#include "include/upo/hashtable.h"
//#include "include/upo/bst_key_list.h"

/*data una tabella hash con gestione delle collisioni basata su concatenazioni separate
Se la HT-SC non è vuota, l’algoritmo deve ritornare un valore diverso
da 0, altrimenti deve ritornare 0.
Per confrontare il valore di due chiavi si utilizzi la funzione
di comparazione memorizzata nel campo key_cmp del tipo upo_ht_sepchain_t,
la quale ritorna un valore <, =, o > di zero se il
valore puntato dal primo argomento è minore, uguale o maggiore del valore puntato dal secondo argomento, 
rispettivamente*/
int upo_ht_sepchain_is_not_empty(const upo_ht_sepchain_t ht)
{	
	return (ht == NULL || ht->size == 0) ? 0 : 1; 
}
 
/*date due tabelle hash dest_ht e src_ht con gestione delle collisioni basata
su concatenazioni separate (HT-SC), effettui il “merge” (cioè, la fusione)
di src_ht in dest_ht.*/
void upo_ht_sepchain_insert(upo_ht_sepchain_t ht, void* key, void* value);

void upo_ht_sepchain_merge(upo_ht_sepchain_t dest_ht, const upo_ht_sepchain_t src_ht) 
{
	if(dest_ht == NULL || src_ht == NULL)
    {
        return;
    }
	for(size_t i = 0; i < src_ht->capacity; i++) ù
    {
        upo_ht_sepchain_list_node_t *n = src_ht->slots[i].head;
    	while(n != NULL) 
        {
            upo_ht_sepchain_insert(dest_ht, n->key, n->value);
        	n = n->next;
    	}	
    }  
}

void upo_ht_sepchain_insert(upo_ht_sepchain_t ht, void* key, void* value) 
{
	size_t h = ht->key_hash(key, ht->capacity);
	upo_ht_sepchain_list_node_t *n = ht->slots[h].head;
	while(n!=NULL && ht->key_cmp(key, n->key) != 0)
    {
        n = n->next;
    }
	if(n == NULL) 
    {
	    upo_ht_sepchain_list_node_t *node = calloc(1, sizeof(upo_ht_sepchain_list_node_t));
    	if(node == NULL)
        {
            abort();
        }
    	node->key = key;
    	node->value = value;
    	node->next = ht->slots[h].head;
    	ht->slots[h].head = node;
  	    ht->size += 1;
	}     	
}
 
/* Implementare un algoritmo che, data una tabella hash H con gestione delle collisioni basata su
concatenazioni separate (HT-SC) e una chiave k, conti il numero di collisioni di k in H.
Se k non è contenuta in H, l’algoritmo deve ritornare il valore 0. */
size_t upo_ht_sepchain_count_collisions(const upo_ht_sepchain_t ht, const void *key) 
{
	if(ht == NULL)
    {
        return 0;
    }
	int valid = 0;
	size_t count = 0;
	for(size_t i = 0; i < ht->capacity; i++) 
    {
        upo_ht_sepchain_list_node_t *n = ht->slots[i].head;
    	while(n != NULL) 
        {
        	count += 1;
        	if(n->key == key) 
            {   
            	valid = 1;
            	return count - 1;
        	}
        	else
            {
                valid = 0;
            }
        	n= n->next;
    	}
	}
	return 0;
}
/* Funzione che ritorna una lista di tutte le chiavi */
upo_ht_key_list_t upo_ht_sepchain_keys(const upo_ht_sepchain_t ht) 
{
    upo_ht_key_list_t list = NULL;
    while(ht != NULL && ht->slots != NULL) 
    {
        if(!upo_ht_sepchain_is_empty(ht)) 
        {
        	for(int i = 0; i < ht->capacity; i++) 
            {
            	upo_ht_sepchain_list_node_t *node = NULL;
            	for(node = ht->slots[i].head; node != NULL; node = node->next) 
                {
                	upo_ht_sepchain_list_node_t *list_node = malloc(sizeof(upo_ht_sepchain_list_node_t));
                	if(list_node == NULL)
                    {
                        abort();
                    }
                	list_node->key = key;
                	list_node->next = list;
                	list = list_node;
            	}  
            }   
        }     
    }
    return list; 
}
