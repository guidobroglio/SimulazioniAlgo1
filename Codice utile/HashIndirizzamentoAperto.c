#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/upo/bst.h"
#include "include/upo/sort.h"
#include "include/upo/hashtable.h"
//#include "include/upo/bst_key_list.h"

/* Implementare un algoritmo che, data una tabella hash H con gestione delle collisioni basata
su indirizzamento aperto a scansione lineare con tecnica del "tombstone",
una chiave k ed un valore booleano d, cancelli da H la coppia chiave-valore identificata da k e ritorni
True per identificare l'avvenuta cancellazione. se d è True, l'algoritmo deve deallocare
la memoria allocata per la coppia chiave-valore rimossa.
Se k non e' contenuta in H, l'algoritmo deve ritonare False (cioè il valore 0)
per indicare che non è avvenuta alcuna cancellazione. */
/* ATTENZIONE: non e' necessario utilizzare la tecnica del resizing-rehashing */
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
    else
    {
        return 0;
    } 
}

/* Funzione che ritorna una lista di tutte le chiavi */
upo_ht_key_list_t upo_ht_linprob_keys(const upo_ht_linprob_t ht) 
{
    upo_ht_key_list_t list = NULL;
    while(ht != NULL && ht->slots != NULL) 
    {
        for(int i = 0; i < ht->capacity; i++) 
        {
        	if(ht->slots[i].tombstone != 0) 
            {
            	upo_ht_sepchain_list_node_t *node = malloc(sizeof(upo_ht_sepchain_list_node_t));
            	if(node == NULL)
                {
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
 
/* Esegue una visita di attraversamento */
void upo_ht_linprob_traverse(const upo_ht_linprob_t ht, upo_ht_visitor_t visit, void *visit_arg) 
{
    if(ht != NULL && ht->slots != NULL) 
    {
        if(!upo_ht_sepchain_is_empty(ht)) 
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
}
 
upo_ht_key_list_t upo_ht_linprob_keys(const upo_ht_linprob_t ht) 
{
    if(ht != NULL && ht->slots != NULL) 
    {
        for(int i = 0; i < ht->capacity; i++) 
        {
            if(ht->slots[i].tombstone != 0)
            {
                visit(ht->slots[i].key, ht->slots[i].value, visit_arg);
            }
        }     
    } 
}