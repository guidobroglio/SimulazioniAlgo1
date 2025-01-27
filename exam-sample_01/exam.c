/* vim: set tabstop=4 expandtab shiftwidth=4 softtabstop=4: */

/******************************************************************************/
/*** NOME: Guido                                                            ***/
/*** COGNOME: Broglio                                                       ***/
/*** MATRICOLA: 20043973                                                    ***/
/*** DATA: Prova d'esame 01                                                 ***/
/******************************************************************************/

#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/upo/bst.h"
#include "include/upo/sort.h"

/**** BEGIN of EXERCISE #1 ****/
/**
 * Implementare un algoritmo che ritorni il predecessore di una chiave in un albero binario
 * di ricerca (BST). Dati in input un BST e una chiave k, il predecessore di k nel BST è la più
 * grande chiave k0 contenuta nel BST tale che k0 < k.
 * Se il predecessore di k non esiste o se il BST è vuoto, l’algoritmo deve ritornare
 * il valore NULL. Dato un albero si ha:
 * Predecessore di 8: 7
 * Predecessore di 1: NULL (non esiste)
 * Predecessore di 5: 4
 * Predecessore di 14: 13
 * Predecessore di 0: NULL (non esiste)
 * Predecessore di 11: 10
 * Predecessore di 100: 14
 *
 * L’algoritmo implementato dev’essere ottimo, nel senso che deve visitare l’albero una sola volta
 * e non deve visitare parti del BST inutili ai fini dell’esercizio, e la complessità temporale
 * nel caso peggiore dev’essere O(n), dove n è il numero di chiavi nel BST.
 */

upo_bst_node_t*upo_bst_predecessor_imp(upo_bst_node_t*node, void*key, upo_bst_comparator_t key_cmp);

upo_bst_node_t*upo_bst_predecessor_imp(upo_bst_node_t*node, void*key, upo_bst_comparator_t key_cmp)
{
    if(node==NULL)
    {
        return NULL;
    }
    int compare=key_cmp(key, node->key);

    if(compare<=0)
    {
        return upo_bst_predecessor_imp(node->left, key, key_cmp);
    }
    upo_bst_node_t*pred_node=upo_bst_predecessor_imp(node->right, key, key_cmp);

    if(pred_node!=NULL)
    {
        return pred_node;
    }
    return node;
}

const void* upo_bst_predecessor(const upo_bst_t bst, const void*key)
{
    if(upo_bst_is_empty(bst) || key==NULL)
    {
        return NULL;
    }
    upo_bst_node_t*pred_node=upo_bst_predecessor_imp(bst->root, (void*)key, bst->key_cmp);
    if(pred_node==NULL)
    {
        return NULL;
    }
    return pred_node->key;
}

/**** END of EXERCISE #1 ****/

/**** BEGIN of EXERCISE #2 ****/
/**
 * L'algoritmo bubblesort bidirezionale è un algoritmo di ordinamento di complessità quadratica, in cui
 * si scansiona ripetutamente in avanti e all'indietro l'intera sequenza di elementi, scambiando di posizione
 * quegli elementi adiacenti che si trovano nell'ordine sbagliato. In particolare, l'algoritmo effettua:
 * - una scansione in avanti della sequenza, scambiando di posizione gli elementi adiacenti che si trovano
 *  nell'ordine sbagliato;
 * - una scansione all'indietro della sequenza, scambiando di posizione gli elementi adiacenti che si trovano
 * nell'ordine sbagliato.
 *
 * I passi 1 e 2 vengono ripetuti finché non si effettuano più scambi, cioè finché la sequenza è ordinata.
 * Per esempio, si consideri la sequenza di elementi [5, 1, 4, 2, 8]. L'algoritmo esegue:
 * - una scansione in avanti, ottenendo [1, 5, 4, 2, 8];
 * - una scansione all'indietro, ottenendo [1, 4, 5, 2, 8];
 * - una scansione in avanti, ottenendo [1, 4, 2, 5, 8];
 * - una scansione all'indietro, ottenendo [1, 2, 4, 5, 8];
 * - una scansione in avanti, ottenendo [1, 2, 4, 5, 8].
 * le ultime due scansioni non effettuano scambi, quindi la sequenza è ordinata.
 *
 * La funzione upo_bidi_bubble_sort implementa l'algoritmo bubblesort bidirezionale. La funzione riceve in input:
 * - un puntatore base alla sequenza di elementi da ordinare;
 * - il numero n di elementi nella sequenza;
 * - la dimensione size (in byte) di ciascun elemento della sequenza;
 * - un puntatore cmp alla funzione di confronto utilizzata per ordinare la sequenza in ordine crescente.
 * Come valore di ritorno, non restituisce nulla, ma ordina la sequenza in input.
 *
 */

void upo_bidi_bubble_sort(void*base, size_t n, size_t size, upo_sort_comparator_t cmp)
{
    char*array=base;
    int swapped=1;

    while(swapped==1)
    {
        swapped=0;
        for(size_t i=0; i<n-1; i++)
        {
            if(cmp(array+i*size, array+(i+1)*size)>0)
            {
                char temp[size];
                memcpy(temp, array+i*size, size);
                memcpy(array+i*size, array+(i+1)*size, size);
                memcpy(array+(i+1)*size, temp, size);
                swapped=1;
            }
        }
        for(size_t i=n-1; i>0; i--)
        {
            if(cmp(array+(i-1)*size, array+i*size)>0)
            {
                char temp[size];
                memcpy(temp, array+(i-1)*size, size);
                memcpy(array+(i-1)*size, array+i*size, size);
                memcpy(array+i*size, temp, size);
                swapped=1;
            }
        }
    }
}
/**** END of EXERCISE #2 ****/
