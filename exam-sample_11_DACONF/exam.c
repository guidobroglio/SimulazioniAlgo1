/******************************************************************************/
/*** NOME: Guido Lorenzo                                                    ***/
/*** COGNOME: Broglio                                                       ***/
/*** MATRICOLA: 20043973                                                    ***/
/*** DATA: 23 Giugno 2022                                                   ***/
/******************************************************************************/

#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/upo/bst.h"
#include "include/upo/hashtable.h"
#include "include/upo/sort.h"

/** Esercizio 1
 *
 * Implementare un algoritmo che ritorni l'altezza di un nodo associato ad una chiave di un albero BST.
 * Dato un BST e una chiave k, l'altezza del nodo contenente la chiave k e la lunghezza del più lungo
 * cammino che parte da quel nodo e termina su una foglia del suo albero. Se il BST è vuoto o la chiave
 * k non è presente nel BST, l'algoritmo deve ritornare il valore -1.
 *
 * esempio di albero
 *                  8
 *          3               10
 *      1       6               14
 *          4       7       13
 * Dato l'albero abbiamo:
 *  - Altezza del nodo con chiave 8: 3
 *  - Altezza del nodo con chiave 3: 2
 *  - Altezza del nodo con chiave 1: 0
 *  - Altezza del nodo con chiave 14: 1
 *  - Altezza del nodo con chiave 5: -1
 *  - Altezza del nodo con chiave 20: -1
 *
 * L'algoritmo deve essere ottimo, nel senso che deve visitare l'albero una sola volta e non deve visitare
 * sotto alberi inutili ai fini dell'esercizio.
 *
 * La funzione da implementare è:
 * long upo_bst_key_height(const upo_bst_t_bst, const void*key)
 *
 * con parametri:
 *  - bst: bst
 *  - key: puntatore alla chieve per cui si vuole calcolare l'altezza del nodo corrispondente.
 *
 * Come valore di ritorno avremo:
 *  - Se il BST non è vuoto e la chiave key è contenuta nel BST, restituisce un numero intero
 *    rappresentante l'altezza del nodo avente come chiave il valore puntato da key
 *  - Se il BST è vuoto o la chiave key non è nel BST, restituisce -1.
 */

//Metodo che restituisce l'altezza del nodo con chiave key
upo_bst_node_t*upo_bst_get_imp(upo_bst_node_t*node, void*key, upo_bst_comparator_t key_cmp);

//Metodo che restituisce l'altezza del nodo con chiave key
upo_bst_node_t*upo_bst_get_imp(upo_bst_node_t*node, void*key, upo_bst_comparator_t key_cmp)
{
    //Se il nodo è NULL, restituisci NULL
    if(node==NULL)
    {
        return NULL;
    }
    //Altrimenti, confronta la chiave del nodo con la chiave key
    else
    {
        //inizializzo la variabile compare con il risultato del confronto tra la chiave del nodo e la chiave key
        int compare=key_cmp(key, node->key);
        //Se compare è minore di 0, restituisci il sottoalbero sinistro
        if(compare<0)
        {
            return upo_bst_get_imp(node->left, key, key_cmp);
        }
        //Se compare è maggiore di 0, restituisci il sottoalbero destro
        else if(compare>0)
        {
            return upo_bst_get_imp(node->right, key, key_cmp);
        }
        //Altrimenti, restituisci il nodo
        else
        {
            return node;
        }
    }
}

//Metodo che restituisce l'altezza del nodo
long upo_bst_key_height_imp(upo_bst_node_t*node)
{
    //Se il nodo è NULL, restituisci -1
    if(node==NULL)
    {
        return -1;
    }
    //Altrimenti, calcola l'altezza del sottoalbero sinistro e destro
    long left_height=upo_bst_key_height_imp(node->left);
    long right_height=upo_bst_key_height_imp(node->right);

    //Se l'altezza del sottoalbero sinistro è maggiore dell'altezza del sottoalbero destro
    if(left_height>right_height)
    {
        //Restituisci l'altezza del sottoalbero sinistro
        return 1+left_height;
    }
    //Altrimenti, restituisci l'altezza del sottoalbero destro
    else
    {
        return 1+right_height;
    }
}

//Implementazione del metodo che restituisce l'altezza del nodo con chiave key
long upo_bst_key_height(const upo_bst_t bst, const void*key)
{
    //Se il BST è vuoto o la chiave è NULL, restituisci -1
    if(upo_bst_is_empty(bst) || key==NULL)
    {
        return -1;
    }
    //Altrimenti, richiamo il metodo che restituisce il nodo con chiave key
    upo_bst_node_t*node=upo_bst_get_imp(bst->root, key, bst->key_cmp);
    //Se il nodo è NULL, restituisci -1
    if(node==NULL)
    {
        return -1;
    }
    //Altrimenti, restituisci l'altezza del nodo
    return upo_bst_key_height_imp(node);
}

/** Esercizio 2
 * L'algoritmo odd-even sort è un algoritmo di ordinamento di complessità che è una variante di bubblesort.
 * In questo algoritmo si confrontano ripetutamente le coppie di posizioni pari e quelle di posizioni
 * dispari fino a quando tutti gli elementi si trovano nell'ordine corrett. In particolare, l'algoritmo
 * esegue ripetutamente i seguenti due passaggi:
 *
 * - Scansione dispari-pari: scambia le coppie di elementi adiacenti (ei, ei+1) in posizione dipari/pari con
 *   con i=1,3.., che si trovano nell'ordine sbagliato
 * - Scansione pari/dispari: scambia le coppie di elementi adiacenti (ei, ei+1) in posizione pari/dispari con
 *   i=0,2.., che si trovano nell'ordine sbagliato.
 *
 * I passi sudetti sono ripetuti fino a quando non vengono più effettuati scambi in nessuno dei passi, cioè
 * tutti gli elementi si trovano nella posizione corretta. A questo punto, la sequenza è ordinata.
 * Per esempio, si consideri la sequenza da ordinare [5, 1, 4, 2, 8]. L'algoritmo esegue i seguenti passi:
 *
 * 1. nuova iterazione: scansione dispari pari:
 *  - Scansione dispari/pari:
 *      a) controlla 1 e 4 -> nulla da scambiare
 *      b) controlla 2 e 8 -> nulla da scambiare
 *  - Scansione pari/dispari:
 *      a) controlla 5 e 1 -> scambia: [1, 5, 4, 2, 8]
 *      b) controlla 4 e 2 -> scambia: [1, 5, 2, 4, 8]
 *
 * 2. nuova iterazione:
 *  - Scansione dispari/pari:
 *      a) Controlla 5 e 2 -> scambia: [1, 2, 5, 4, 8]
 *      b) controlla 4 e 8 -> Nulla da scambiare
 *  - Scansione pari/dispari:
 *      a) Controlla 1 e 2 -> Nulla da scambiare
 *      b) Controlla 5 e 4 -> Scambia: [1, 2, 4, 5, 8]
 *
 * 3. Nuova iterazione
 *  - Scansione dispari/pari:
 *      a) Controlla 2 e 4 -> Nulla da scambiare
 *      b) Controlla 5 e 8 -> Nulla da scambiare
 *  - Scansione pari/dispari:
 *      a) Controlla 1 e 2 -> Nulla da scambiare
 *      b) Controlla 4 e 5 -> nulla da scambiare
 *
 * La funzione da implementare si trova nel file ed è la funzione:
 * void upo_odd_even_sort(void*base, size_t n, size_t size, upo_sort_comparator_t cmp)
 * Con parametro:
 *  - base: puntatore alla prima cella dell'array da ordinare
 *  - n: numero di elementi dell'array da ordinare
 *  - cmp: puntatore alla funzione di comparazione utilizzata per confrontare due elementi dell'array, la quale
 *         ritorna un valore <, =, > di zero se il valore puntato dal primo argomento è minore, uguale o
 *         maggiore del valore puntato dal secondo argomento, rispettivamente.
 * La funzione non ritorna nulla.
**/

void upo_odd_even_sort(void*base, size_t n, size_t size, upo_sort_comparator_t cmp)
{
    char* array=base;
    int swapped=1;

    while(swapped==1)
    {
        swapped=0;
        for(size_t i=1; i<n-1; i+=2)
        {
            if(cmp(array+i*size, array+(i+1)*size)>0)
            {
                char temp[size];
                memcpy(temp, array+i*size, size)
                memcpy(array+i*size, array+(i+1)*size, size);
                memcpy(array+(i+1)*size, temp, size);
                swapped=1;
            }
        }
        for(size_t i=0; i<n-1; i+=2)
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
    }
}
