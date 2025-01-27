/* vim: set tabstop=4 expandtab shiftwidth=4 softtabstop=4: */

/******************************************************************************/
/*** NOME: Guido                                                            ***/
/*** COGNOME: Broglio                                                       ***/
/*** MATRICOLA: 20043973                                                    ***/
/*** DATA: Prova d'esame novembre 2024                                      ***/
/******************************************************************************/

#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/upo/bst.h"
#include "include/upo/hashtable.h"

/**** START of EXERCISE #1 ****/
/**
                        8
                    /       \
                   3          10
                 /  \          \
                1    6         14
                   /   \       /
                 4      7    13

Implementare un algoritmo che dato un albero di ricerca BST e due chiavi k1 e k2
restituisca 1 se il sottoalbero con radice k1 contiene la chiave k2
altrimenti restituisca 0 se k2 non è contenuta, se k1 non è nell'albero o se il
bst è vuoto.

Per esempio, dato l'albero:
k1=6 e k2=4, risultato=1
k1=8 e k2=15, risultato=0
k1=14 e k2=14, risultato=1
k1=9 e k2=13, risultato=0
L'algoritmo deve visitare l'albero una sola volta e non deve visitare parti inutili

la funzione da implementare è la funzione:
int upo_bst_subtree_countains(const upo_bst_t tree, const void*key1, const void*key2)
dove tree è l'albero, key1 la radice da cui cercare e key2 il nodo da ricercare dalla radice.
*/
////////////////////////////INIZIO PROTOTIPI////////////////////////////////////
int upo_bst_subtree_countains(const upo_bst_t tree, const void*key1, const void*key2);
upo_bst_node_t*upo_bst_get(upo_bst_node_t*node, void*key, upo_bst_comparator_t key_cmp);
int upo_bst_subtree_countains_imp(upo_bst_node_t*subtree, void*key2, upo_bst_comparator_t key_cmp);
/////////////////////////////FINE PROTOTIPI/////////////////////////////////////
upo_bst_node_t*upo_bst_get(upo_bst_node_t*node, void*key, upo_bst_comparator_t key_cmp)
{
    if(node==NULL)
    {
        return NULL;
    }
    int compare=key_cmp(key, node->key);
    if(compare<0)
    {
        return upo_bst_get(node->left, key, key_cmp);
    }
    else if(compare>0)
    {
        return upo_bst_get(node->right, key, key_cmp);
    }
    else
    {
        return node;
    }
}

int upo_bst_subtree_countains_imp(upo_bst_node_t*subtree, void*key2, upo_bst_comparator_t key_cmp)
{
    if(subtree==NULL)
    {
        return 0;
    }
    int compare=key_cmp(key2, subtree->key);

    if(compare==0)
    {
        return 1;
    }
    else if(compare<0)
    {
        return upo_bst_subtree_countains_imp(subtree->left, key2, key_cmp);
    }
    else
    {
        return upo_bst_subtree_countains_imp(subtree->right, key2, key_cmp);
    }
}

int upo_bst_subtree_countains(const upo_bst_t tree, const void*key1, const void*key2)
{
    if(upo_bst_is_empty(tree) || key1==NULL || key2==NULL)
    {
        return 0;
    }

    upo_bst_node_t*subtree=upo_bst_get(tree->root, (void*)key1, tree->key_cmp);

    if(subtree==NULL)
    {
        return 0;
    }
    return upo_bst_subtree_countains_imp(subtree, (void*)key2, tree->key_cmp);
}
/**** END of EXERCISE #1 ****/

/**** START of EXERCISE #2 ****/
/**
Indice H   | Catena (key, value)
---------------------------------
0          | (I, 7) -> (A, 9) -> (E, 5) -> (L, 3)
1          | (F, 5) -> (B, 1)
2          | (C, 8) -> (G, 6) -> (M, 3)
3          | (D, 1) -> (H, 4)
4          |

Implementare un algoritmo che, data una tabella hash H con gestione delle collisioni,
basata su concatenazioni separate (HT-SC-separete chaching) e una chiave k, conti il
numero di chiavi della tabella che collidono con k, tutte le chiavi nella lista
concatenata vanno conteggiate nel calcolo ad eccezione di k. Se k non è contenuta
in H, o se H è vuota l'algoritmo restituisce 0
Per esempio, dati la tabella hash e la chiave:
 - k=E, il risultato è 3
 - k=B, il risultato è 1
 - k=C, il risultato è 2
 - k=Z, il risultato è 0
L'algoritmo implementato deve visitare la HT-SC una sola volta e non deve visitare
parti della HT-SC inutili ai fini dell'esercizio

La funzione da implementare è:
size_t upo_ht_sepchain_count_collisions_gt(const upo_ht_sepchain_t ht, const void*key)
dove ht è la tabella Hash e key è la chiave.
**/
////////////////////////////INIZIO PROTOTIPI////////////////////////////////////
size_t upo_ht_sepchain_count_collisions_gt(const upo_ht_sepchain_t ht, const void*key);
/////////////////////////////FINE PROTOTIPI/////////////////////////////////////
size_t upo_ht_sepchain_count_collisions_gt(const upo_ht_sepchain_t ht, const void* key)
{
    if (upo_ht_sepchain_is_empty(ht) || key == NULL)
    {
        return 0;
    }

    // Calcola l'indice hash per la chiave
    size_t hash_index = ht->key_hash(key, ht->capacity);
    upo_ht_sepchain_list_node_t* node = ht->slots[hash_index].head;

    // Verifica se la chiave è presente nella lista concatenata
    int key_found = 0;
    size_t collision_count = 0;
    while (node != NULL)
    {
        if (ht->key_cmp(node->key, key) == 0)
        {
            key_found = 1;
        }
        else
        {
            collision_count++;
        }
        node = node->next;
    }

    // Se la chiave non è stata trovata, restituisci 0
    if (key_found==0)
    {
        return 0;
    }

    return collision_count;
}


/**** END of EXERCISE #2 ****/
