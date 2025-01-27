/* vim: set tabstop=4 expandtab shiftwidth=4 softtabstop=4: */

/******************************************************************************/
/*** NOME: Guido Lorenzo                                                    ***/
/*** COGNOME: Broglio                                                       ***/
/*** MATRICOLA: 20043973                                                    ***/
/*** DATA: 10 giugno 2024                                                   ***/
/******************************************************************************/


#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "include/upo/bst.h"
#include "include/upo/hashtable.h"

/**** BEGIN of EXERCISE #1 ****/
/*
Implementare un algoritmo che, dato un albero binario di ricerca (BST) e una chiave k,
restituisca:
    - la più grande chiave del sottoalbero la cui radice è k;
    - NULL, se la chiave non è presente nell’albero o se il BST è vuoto.
Per esempio, dati l’albero in Figura 1, e una chiave:
    • k = 6 il risultato è 7;
    • k = 8 il risultato è 14;
    • k = 13 il risultato è 13;
    • k = 19 il risultato è NULL.
L’algoritmo implementato dev’essere ottimo, nel senso che deve visitare l’albero una sola
volta e la complessità temporale nel caso peggiore dev’essere O(n), dove n è il numero
di chiavi nel BST.

La funzione da implementare si trova nel file exam.c e ha il seguente prototipo:
void* upo_bst_subtree_max(const upo_bst_t tree, const void *key)

Parametri:
    - tree: BST
    - key: la radice del sottoalbero di cui trovare la chiave maggiore

Valore di ritorno
    - Se il BST non è vuoto e la chiave k è contenuta nell'albero, la chiave maggiore
      del sottoalbero la cui radice è k.
    - se il BST è vuoto o k non è contenuta nell'albero: NULL.

Il tipo upo_bst_t è dichiarato in include/upo/bst.h. Per confrontare il valore di due chiavi
(qualora fosse necessario) si utilizzi la funzione di comparazione memorizzata nel campo key_cmp del tipo
upo_bst_t, la quale ritorna un valore <, =, o >di zero se il valore puntato dal primo argomento è minore,
uguale o maggiore del valore puntato dal secondo argomento, rispettivamente.
Nella propria implementazione è possibile utilizzare tutte le funzioni dichiarate in include/upo/bst.h.
Nel caso s’implementino nuove funzioni, i prototipi e le definizioni devono essere presenti e inserite nel
file exam.c.
Il file test/bst_subtree_max.c contiene alcuni casi di test tramite cui è possibile verificare la correttezza
della propria implementazione.
*/

upo_bst_node_t*upo_bst_get(upo_bst_node_t*node, void*key, upo_bst_comparator_t key_cmp);

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

void*upo_bst_subtree_max_imp(upo_bst_node_t*node);

void*upo_bst_subtree_max_imp(upo_bst_node_t*node)
{
    if(node==NULL)
    {
        return NULL;
    }
    else
    {
        if(node->right==NULL)
        {
            return node->key;
        }
        else
        {
            return upo_bst_subtree_max_imp(node->right);
        }
    }
}

void* upo_bst_subtree_max(const upo_bst_t tree, const void *key)
{
    if(upo_bst_is_empty(tree) || key==NULL)
    {
        return NULL;
    }

    upo_bst_node_t*subtree=upo_bst_get(tree->root, (void*)key, tree->key_cmp);

    if(subtree==NULL)
    {
        return NULL;
    }
    return upo_bst_subtree_max_imp(subtree);
}

/**** END of EXERCISE #1 ****/


/**** BEGIN of EXERCISE #2 ****/
/*
Implementare un algoritmo che, data una tabella hash H con gestione delle collisioni basata su concatenazioni
separate (HT-SC - separate chaining) e una chiave k, calcoli la percentuale di elementi della tabella che
collidono con k (Nota: k non è inclusa nel calcolo della percentuale). In particolare:
    • se una chiave k è presente in H, l’algoritmo calcola la percentuale di elementi della tabella che collidono
      con k.
    • se una chiave k non è contenuta in H, o se H è vuota l’algoritmo restituisce -1.
      Data la tabella hash in Figura 2 e una chiave:
    • k = E, il risultato è: 20%;
    • k = B, il risultato è: 10%;
    • k = L, il risultato è: 30%;
    • k = Z, il risultato è: −1.
L’algoritmo implementato deve essere ottimo, nel senso che deve visitare le HT-SC una sola volta e non deve
visitare parti delle HT-SC inutili ai fini dell’esercizio.

La funzione da implementare si trova nel file exam.c e ha il seguente prototipo:
float upo_ht_sepchain_perc_collisions(const upo_ht_sepchain_t ht, const void *key)
Parametri:
    • ht: Tabella Hash.
    • key: chiave.
Il tipo upo_ht_sepchain_t è dichiarato in include/upo/hashtable.h. Per confrontare il valore di due chiavi si
utilizzi la funzione di comparazione memorizzata nel campo key_cmp del tipo upo_ht_sepchain_t, la quale
ritorna un valore <, =, o >di zero se il valore puntato dal primo argomento è minore, uguale o maggiore del
valore puntato dal secondo argomento, rispettivamente. Per calcolare il valore hash di una chiave si utilizzi
la funzione di hash memorizzata nel campo key_hash del tipo upo_ht_sepchain_t, la quale richiede come
parametri il puntatore alla chiave di cui si vuole calcolare il valore hash e la capacità totale della HT-SC
(memorizzata nel campo capacity del tipo upo_ht_sepchain_t). Infine, gli slot della HT-SC sono memorizzati nel
campo slots del tipo upo_ht_sepchain_t, che è una sequenza di slot, ciascuno dei quali di tipo
upo_ht_sepchain_slot_t e contenente il puntatore alla propria lista delle collisioni.
Nella propria implementazione è possibile utilizzare tutte le funzioni dichiarate in include/upo/hashtable.h.
Nel caso si implementino nuove funzioni, i prototipi e le definizioni devono essere presenti e inserite nel file
exam.c.
Il file test/ht_sepchain_perc_collisions.c contiene alcuni casi di test tramite cui è possibile verificare
la correttezza della propria implementazione.
*/

//Definizione della funzione upo_ht_sepchain_perc_collisions
float upo_ht_sepchain_perc_collisions(const upo_ht_sepchain_t ht, const void *key)
{
    if(upo_ht_sepchain_is_empty(ht))
    {
        return -1;
    }

    size_t hash_index=ht->key_hash(key, ht->capacity);
    upo_ht_sepchain_list_node_t*current=ht->slots[hash_index].head;
    int keys_found=0;
    int collisions_count=0;

    while(current!=NULL && keys_found==0)
    {
        int compare=ht->key_cmp(key, current->key);
        if(compare==0)
        {
            keys_found=1;
        }
        else
        {
            collisions_count++;
            current=current->next;
        }
    }
    if(keys_found==0)
    {
        return -1;
    }
    float percentuale=((float)collisions_count/(ht->size-1))*100.0f;
    return percentuale;
}

/**** END of EXERCISE #2 ****/
