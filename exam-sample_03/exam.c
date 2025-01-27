/* vim: set tabstop=4 expandtab shiftwidth=4 softtabstop=4: */

/******************************************************************************/
/*** NOME:   Guido                                                          ***/
/*** COGNOME:  Broglio                                                      ***/
/*** MATRICOLA:   20043973                                                  ***/
/*** DATA: 14 febbraio 2024                                                 ***/
/******************************************************************************/


#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/upo/bst.h"
#include "include/upo/hashtable.h"

/**** BEGIN of EXERCISE #1 ****/
/*
Implementare un algoritmo che, dato un albero binario di ricerca (BST), una chiave k (non necessariamente contenuta nel BST) e
un intero n, restituisca:
•l’ n-esima chiave più piccola del sottoalbero la cui radice ha come chiave k, se esiste e se k è contenuta nel BST;
•NULL, se l’n-esima chiave più piccola non esiste, se k non è contenuta nel BST, o se il BST è vuoto.

Si noti che l’n-esima chiave più piccola è la chiave che si troverebbe nell’n-esima posizione se le chiavi fossero disposte in
ordine di grandezza. Per esempio, dato l’albero di Figura 1, si ha:
•n = 3 e k = 3: 4
•n = 2 e k = 6: 6
•n = 2 e k = 8: 3
•n = 4 e k = 14: NULL
•n = 3 e k = 17: NULL
L’algoritmo implementato dev’essere ottimo, nel senso che deve visitare l’albero una sola volta e non deve visitare sotto-alberi
inutili ai fini dell’esercizio, e la complessità temporale nel caso peggiore dev’essere O(n), dove n è il numero di chiavi nel BST

La funzione da implementare si trova nel file exam.c e ha il seguente prototipo:
void *upo_bst_nmin(const upo_bst_t bst, const void *key, const int n)
Parametri:
•bst: BST.
•key: puntatore alla chiave della radice del sottoalbero di BST in cui si vuole trovare l’n-esima chiave più piccola.
•n: intero che definisce n-esima chiave più piccola.
Valore di ritorno:
•Se il BST non è vuoto, la chiave key è contenuta nel BST e l’n-esima chiave più piccola esiste: il puntatore all’n-esima
chiave più piccola.
•Se il BST è vuoto o la chiave key non è contenuta nel BST o non esiste l’n-esima chiave più piccola: NULL.
Il tipo upo_bst_t è dichiarato in include/upo/bst.h. Per confrontare il valore di due chiavi (qualora fosse necessario) si utilizzi
la funzione di comparazione memorizzata nel campo key_cmp del tipo upo_bst_t, la quale ritorna un valore <, =, o >di zero se il
valore puntato dal primo argomento è minore, uguale o maggiore del valore puntato dal secondo argomento, rispettivamente.
Nella propria implementazione è possibile utilizzare tutte le funzioni dichiarate in include/upo/bst.h. Nel caso s’implementino
nuove funzioni, i prototipi e le definizioni devono essere inserite nel file exam.c.
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

void*upo_bst_nmin_imp(upo_bst_node_t*node, int*count, int n);

void*upo_bst_nmin_imp(upo_bst_node_t*node, int*count, int n)
{
    if(node==NULL)
    {
        return NULL;
    }
    void*min=upo_bst_nmin_imp(node->left, count, n);

    if(min!=NULL)
    {
        return min;
    }
    else
    {
        (*count)++;
        if(*count==n)
        {
            return node->key;
        }
    }
    return upo_bst_nmin_imp(node->right, count, n);
}

void *upo_bst_nmin(const upo_bst_t bst, const void *key, const int n)
{
  if(upo_bst_is_empty(bst) || key==NULL)
  {
    return NULL;
  }

  upo_bst_node_t*subtree=upo_bst_get(bst->root, (void*)key, bst->key_cmp);
  if(subtree==NULL)
  {
    return NULL;
  }
  int count=0;
  return upo_bst_nmin_imp(subtree, &count, n);
}

/**** END of EXERCISE #1 ****/


/**** BEGIN of EXERCISE #2 ****/
/*
Implementare un algoritmo che, data una tabella hash H, con gestione delle collisioni basata su indirizzamento aperto (i.e. open
addressing) e scansione lineare (i.e. linear probing) con uso di tombstone (HT-LP), e una lista di chiavi lkeys, calcoli la media del
numero di collisioni delle chiavi k contenute in lkeys in H. In particolare:
•Se una chiave k in lkeys non è contenuta in H, non dev’essere considerata nel calcolo della media del numero di collisioni.
•Se H è vuota, o se lkeys è vuota, o se nessuna chiave di lkeys è contenuta in H, l’algoritmo deve restituire il valore −1.
Si noti che nel calcolare il numero di collisioni di una chiave k non si deve tenere conto dello slot in cui k è memorizzata e che
uno slot tombstone, se attraversato, è considerato una collisione.
Per esempio, date la tabella e la lista di chiavi in Figura 2, e supponendo che il valore hash delle chiavi A,C, e E sia
rispettivamente 0,0, e 10, il numero medio di collisioni è 3 in quanto:
•il numero di collisioni di A è 4;
•il numero di collisioni di C è 5;
•il numero di collisioni di E è 0;
•Z non è contenuta in H quindi non viene conteggiata nel calcolo della media.
L’algoritmo implementato deve essere ottimo, nel senso che non deve visitare parti di HT-LP inutili ai fini dell’esercizio.

La funzione da implementare si trova nel file exam.c e ha il seguente prototipo:
double upo_ht_linprob_avg_collisions(const upo_ht_linprob_t ht, const upo_ht_key_list_t key_list)
Parametri:
•ht: HT-LP
•key_list: lista concatenata di chiavi.
I tipi upo_ht_linprob_t e upo_ht_key_list_t sono dichiarati in include/upo/hashtable.h. Le chiavi di cui calcolare la media
del numero di collisioni sono memorizzate nel campo key del tipo upo_ht_key_list_t. Per scorrere la lista di chiavi key_list si
utilizzi il puntatore memorizzato nel campo next del tipo upo_ht_key_list_t. Per confrontare il valore di due chiavi si utilizzi la
funzione di comparazione memorizzata nel campo key_cmp del tipo upo_ht_linprob_t, la quale ritorna un valore <, =, o >di zero
se il valore puntato dal primo argomento è minore, uguale o maggiore del valore puntato dal secondo argomento, rispettivamente.
Per calcolare il valore hash di una chiave si utilizzi la funzione di hash memorizzata nel campo key_hash del tipo upo_ht_linprob_t,
la quale richiede come parametri il puntatore alla chiave di cui si vuole calcolare il valore hash e la capacità totale della HT-LP
(memorizzata nel campo capacity del tipo upo_ht_linprob_t). Infine, gli slot della HT-LP sono memorizzati nel campo slots del
tipo upo_ht_linprob_t, che è una sequenza di slot, ciascuno dei quali di tipo upo_ht_linprob_slot_t.
Nella propria implementazione è possibile utilizzare tutte le funzioni dichiarate in include/upo/hashtable.h. Nel caso si
implementino nuove funzioni, i prototipi e le definizioni devono essere inserite nel file exam.c
*/

// Funzione per calcolare la media del numero di collisioni delle chiavi contenute in una lista di chiavi
double upo_ht_linprob_avg_collisions(const upo_ht_linprob_t ht, const upo_ht_key_list_t key_list)
{
    if(upo_ht_linprob_is_empty(ht))
    {
        return -1;
    }
    upo_ht_key_list_node_t*current=key_list;
    size_t total_collisions=0;
    size_t keys_count=0;

    while(current!=NULL)
    {
        size_t hash_index=upo_ht_linprob_get_hasher(ht)(current->key, upo_ht_linprob_capacity(ht));
        size_t collisions=0;
        int keys_found=0;

        while(ht->slots[hash_index].key!=NULL && collisions<upo_ht_linprob_capacity(ht) && keys_found==0)
        {
            if(upo_ht_linprob_get_comparator(ht)(ht->slots[hash_index].key, current->key)==0)
            {
                total_collisions+=collisions;
                keys_count++;
                keys_found=1;
            }
            hash_index=(hash_index+1)%upo_ht_linprob_capacity(ht);
            collisions++;
        }
        current=current->next;
    }
    if(keys_count==0)
    {
        return -1;
    }
    return (double)total_collisions/keys_count;
}

/**** END of EXERCISE #2 ****/
