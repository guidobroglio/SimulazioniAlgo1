/******************************************************************************/
/*** NOME: Guido Lorenzo                                                    ***/
/*** COGNOME: Broglio                                                       ***/
/*** MATRICOLA: 20043973                                                    ***/
/*** DATA: 25 Settembre 2020                                                ***/
/******************************************************************************/

#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/upo/bst.h"
#include "include/upo/hashtable.h"

/* ESERCIZIO 1*/
/*
Dati in input un albero binario di ricerca BST e una chiave k, implementare un algoritmo che dica se la
chiave k e' contenuta nel BST e che calcoli la profondita' del nodo che contiene k. In particolare:
 - se la chiave k e' contenuta nel BST, l'algoritmo deve ritornare True (valore intero diverso da zero) e
    deve calcolare la profondità del nodo in cui è memorizzata la chiave k.
- se la chiave k non e' contenuta nel BST o se il BST e' vuoto, l'algoritmo deve ritornare False
    (il valore intero 0) e come profondita' deve ritornare il valore intero -1.
*/

int upo_bst_contains_depth_imp(upo_bst_node_t*node, void*key, long*depth, upo_bst_comparator_t key_cmp);

int upo_bst_contains_depth_imp(upo_bst_node_t*node, void*key, long*depth, upo_bst_comparator_t key_cmp)
{
    if(node==NULL)
    {
        *depth=-1:
        return 0;
    }
    int compare=key_cmp(key, node->key);

    if(compare<0)
    {
        *(depth)++;
        return upo_bst_contains_depth_imp(node->left, depth, key_cmp);
    }
    else if(compare>0)
    {
        *(depth)++;
        return upo_bst_contains_depth_imp(node->right, depth, key_cmp);
    }
    else
    {
        return depth;
    }
}

int upo_bst_contains_depth(const upo_bst_t bst, void *key, long *depth)
{
    if(upo_bst_is_empty(bst) || key==NULL)
    {
        *depth=-1;
        return 0;
    }
    *depth=0;
    return upo_bst_contains_depth_imp(bst->root, key, depth, bst->key_cmp);
}

/*FINE ESERCIZIO 1*/

/* ESERCIZIO 2 */
/*
Implementare un algoritmo che, data una tabella hash H con gestione delle collisioni basata su indirizzamento
aperto a scansione lineare con tecnica del "tombstone", una chiave k ed un valore booleano d, cancelli da H
la coppia chiave-valore identificata da k e ritorni True per identificare l'avvenuta cancellazione. Inoltre,
se d è True, l'algoritmo deve deallocare la memoria allocata per la coppia chiave-valore rimossa. Se k non
e' contenuta in H, l'algoritmo deve ritonare False (cioè il valore 0) per indicare che non è avvenuta alcuna
cancellazione. ATTENZIONE: non e' necessario utilizzare la tecnica del resizing-rehashing
*/

//Metodo responsabile della cancellazione della coppia chiave-valore
int upo_ht_linprob_deletex(upo_ht_linprob_t ht, const void *key, int destroy_data);

//Implementazione del metodo responsabile della cancellazione della coppia chiave-valore
int upo_ht_linprob_deletex(upo_ht_linprob_t ht, const void *key, int destroy_data)
{
    if(upo_ht_linprob_is_empty(ht) || ht==NULL || key==NULL)
    {
        return 0;
    }
    size_t hash_index=ht->key_hash(key, ht->capacity);

    while(ht->slots[hash_index].key!=NULL)
    {
        if(ht->slots[hash_index].tombstone==0 && ht->key_cmp(ht->slots[hash_index].key, key)==0)
        {
            if(destroy_data==1)
            {
                free(ht->slots[hash_index].key);
                free(ht->slots[hash_index].value);
            }
            ht->slots[hash_index].key=NULL;
            ht->slots[hash_index].value=NULL;
            ht->slots[hash_index].tombstone=1;

            ht->size--;
            return 1;
        }
        hash_index=(hash_index+1)%ht->capacity;
    }
    return 0;
}
/*FINE ESERCIZIO 2*/
