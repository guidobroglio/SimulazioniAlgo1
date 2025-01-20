/* vim: set tabstop=4 expandtab shiftwidth=4 softtabstop=4: */

/******************************************************************************/
/*** NOME: Guido                                                            ***/
/*** COGNOME: Broglio                                                       ***/
/*** MATRICOLA: 20043973                                                    ***/
/*** DATA: Prova d'esame 02                                                 ***/
/******************************************************************************/

#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/upo/bst.h"
#include "include/upo/hashtable.h"

/**
 * Implementare un algoritmo che ritorni il numero di nodi di un sottoalbero in un albero binario
 * di ricerca BST che si trovano a una profondità pari. Dato un BST e una chiave k,
 * il numer di nodi del sottoalbero radicato in k che si trovano a una profondità pari. Si
 * ottiene contando tutti i nodi che si trovano a prodontà pari a partire dal nodo radice del
 * sottoalbero radicato in k. Si noti che la radice dell'intero BST si trova a profondità 0.
 * Il conteggio dei nodi include anche la radice del sottoalbero se si trova a una profondità pari
 * Se la chiave k non è presente nell'albero, la funzione deve ritornare 0. così come se il sottoalbero
 * radicato in k è vuoto.
 *
 * Per esempio, dato il seguente BST:
 * conteggio nodi a prondoità pari nel sottoalbero radicato in 8: 4 (8, 1, 6, 14)
 * conteggio nodi a profondità pari nel sottoalbero radicato in 3: 2 (1, 6)
 * conteggio nodi a profondità pari nel sottoalbero radicato in 1: 1 (1)
 * conteggio nodi a profondità pari nel sottoalbero radicato in 4: 0 (non ci sono nodi a profondità pari)
 * Conteggio nodi a profondità pari nel sotto-albero radicato in 5: 0 (la chiave non esiste).
 *
 * L’algoritmo implementato dev’essere ottimo, nel senso che deve visitare l’albero una sola volta e non
 * deve visitare sotto-alberi inutili ai fini dell’esercizio, e la complessità temporale nel caso peggiore
 * dev’essere O(n), dove n è il numero di chiavi nel BST.
 *
 * Si implementi la funzione:
 * size_t upo_bst_subtree_count_even(const upo_bst_t bst, const void *key)
 * con parametri:
 * - bst: il BST su cui operare
 * - key: la chiave del nodo radice del sottoalbero di cui si vuole contare i nodi a profondità pari.
 * valori di ritorno:
 * - Se il BST non è vuoto e la chiave key è contenuta nel BST: numero intero rappresentante il
 *      numero di nodi a profondità pari nel sotto-albero radicato nel nodo avente come chiave il
 *      valore puntato da key
 * - Se il BST è vuoto o la chiave key non è contenuta nel BST o il sotto-albero radicato in
 *      key non ha nodi a profondità pari: il valore intero 0
 */
/**** BEGIN of EXERCISE #1 ****/

upo_bst_node_t*upo_bst_get_depth(upo_bst_node_t*node, void*key, upo_bst_comparator_t key_cmp, size_t*depth);

upo_bst_node_t*upo_bst_get_depth(upo_bst_node_t*node, void*key, upo_bst_comparator_t key_cmp, size_t*depth)
{
    if(node==NULL)
    {
        return NULL;
    }
    int compare=key_cmp(key, node->key);

    if(compare<0)
    {
        (*depth)++;
        return upo_bst_get_depth(node->left, key, key_cmp, depth);
    }
    else if(compare>0)
    {
        (*depth)++;
        return upo_bst_get_depth(node->right, key, key_cmp, depth);
    }
    else
    {
        return node;
    }
}

size_t upo_bst_subtree_count_even_imp(upo_bst_node_t*node, size_t depth);

size_t upo_bst_subtree_count_even_imp(upo_bst_node_t*node, size_t depth)
{
    if(node==NULL)
    {
        return 0;
    }

    int count;

    if(depth%2==0)
    {
        count=1;
    }
    else
    {
        count=0;
    }

    count+=upo_bst_subtree_count_even_imp(node->left, depth+1);
    count+=upo_bst_subtree_count_even_imp(node->right, depth+1);

    return count;
}

size_t upo_bst_subtree_count_even(const upo_bst_t bst, const void *key)
{
    if(upo_bst_is_empty(bst) || key==NULL)
    {
        return 0;
    }

    size_t depth=0;
    upo_bst_node_t*node=upo_bst_get_depth(bst->root, (void*)key, bst->key_cmp, &depth);

    if(node==NULL)
    {
        return 0;
    }
    return upo_bst_subtree_count_even_imp(node, depth);
}


/**** END of EXERCISE #1 ****/

/**
 * Si consideri una tabella hash H con gestione delle collisioni basata su concatenazioni separate
 * (HT-SC) in cui le liste delle collisioni sono ordinate secondo il valore della chiave.
 * Implementare un algoritmo che realizzi l'operazione "odelete" la quale, data una HT-SC H,
 * una chiave k e un flag destroy_data, elimina l'elemento con chiave k dalla tabella hash H,
 * mantenendo l'ordinamento delle liste delle collisioni. Nel caso in cui destroy_data sia
 * true, la funzione deve deallocare la memoria occupata dalla chiave e dal valore associato
 *
 * Se la chiave k è contenuta in H, l'algoritmo deve rimuovere la coppia chiave-valore associata.
 * in maniera tale che la lista delle collisioni rimanga ordinata. Se la chiave k non è presente
 * nella tabella hash, la funzione non deve fare nulla.
 * Una volta rimosso l'elemento, bisogna aggiornare la dimensione di H.
 *
 * Esempio:
 * - Cancellazione di A →la coppia chiave-valore identificata da questa chiave viene rimossa,
 *      preservando l’ordine dei nodi nella lista delle collisioni associata; in particolare, dopo
 *      la cancellazione, la lista conterrà le seguenti chiavi: [P].
 * - Cancellazione di M →la coppia chiave-valore identificata da questa chiave viene rimossa,
 *      preservando l’ordine dei nodi nella lista delle collisioni associata; in particolare,
 *      dopo la cancellazione, la lista conterrà le seguenti chiavi: [C,H,R].
 * - Cancellazione di B →non si effettua alcuna cancellazione in quanto non esiste una
 *      coppia-chiave valore identificata da questa chiave.
 * - Cancellazione di Y →non si effettua alcuna cancellazione in quanto non esiste una
 *      coppia-chiave valore identificata da questa chiave
 *
 * L’algoritmo implementato dev’essere ottimo, nel senso che deve visitare la HT-SC una sola volta e
 * non deve visitare parti della HT-SC inutili ai fini dell’esercizio, e la complessità temporale
 * nel caso medio dev’essere Θ(1)
 *
 * Si implementi la funzione:
 * void upo_ht_sepchain_odelete(upo_ht_sepchain_t ht, const void *key, int destroy_data)
 * con parametri:
 * - ht: la tabella hash su cui operare
 * - key: la chiave dell'elemento da eliminare
 * - destroy_data: flag che indica se deallocare o meno la memoria occupata dalla chiave e dal
 *                  valore associato all'elemento da eliminare (1 per deallocare, 0 altrimenti).
 * valori di ritorno:
 * - la funzione è void e non ritorna alcun valore
 *
 * La deallocazione della memoria deve essere effettuata tramite la funzione free() della
 * libreria standard del C.
 * Il tipo upo_ht_sepchain_t è dichiarato in include/upo/hashtable.h. Per confrontare il valore di
 * due chiavi si utilizzi la funzione di comparazione memorizzata nel campo key_cmp del tipo
 *  upo_ht_sepchain_t, la quale ritorna un valore <, =, o >di zero se il valore puntato dal primo
 * argomento è minore, uguale o maggiore del valore puntato dal secondo argomento, rispettivamente.
 * Per calcolare il valore hash di una chiave si utilizzi la funzione di hash memorizzata nel
 * campo key_hash del tipo upo_ht_sepchain_t, la quale richiede come parametri il puntatore
 * alla chiave di cui si vuole calcolare il valore hash e la capacità totale della HT-SC
 * (memorizzata nel campo capacity del tipo upo_ht_sepchain_t). Per tenere traccia della dimensione
 * della HT-SC, si utilizzi il campo size del tipo upo_ht_sepchain_t. Infine, gli slot della HT-SC
 * sono memorizzati nel campo slots del tipo upo_ht_sepchain_t, che è una sequenza di slot,
 * ciascuno dei quali di tipo upo_ht_sepchain_slot_t e contenente il puntatore alla propria
 * lista delle collisioni
 */
/**** BEGIN of EXERCISE #2 ****/

void upo_ht_sepchain_odelete(upo_ht_sepchain_t ht, const void*key, int destroy_data)
{
    if(upo_ht_sepchain_is_empty(ht))
    {
        return;
    }

    size_t hash_index=ht->key_hash(key, ht->capacity);
    upo_ht_sepchain_list_node_t*current=ht->slots[hash_index].head;
    upo_ht_sepchain_list_node_t*previous=NULL;

    while(current!=NULL)
    {
        int compare=ht->key_cmp(key, current->key);
        if(compare==0)
        {
            if(previous==0)
            {
                ht->slots[hash_index].head=current->next;
            }
            else
            {
                previous->next=current->next;
            }
            if(destroy_data==1)
            {
                free(current->key);
                free(current->value);
            }
            free(current);
            ht->size-=1;
            return;
        }
        previous=current;
        current=current->next;
    }
}


/**** END of EXERCISE #2 ****/
