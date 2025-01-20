/******************************************************************************/
/*** NOME: Guido Lorenzo                                                    ***/
/*** COGNOME: Broglio                                                       ***/
/*** MATRICOLA: 20043973                                                    ***/
/*** DATA: 19 Giugno 2024                                                   ***/
/******************************************************************************/

#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/upo/hashtable.h"
#include "include/upo/bst.h"

/** Esercizio 1
 *
 * Implementare un algoritmo che dato un albero binario di ricerca BST e un intervallo [Klow, Khigh],
 * restituisca la somma dei valori dei nodi le cui chiavi sono contenute nell'intervallo o 0 se nessuna
 * chiave è contenuta nell'intervallo o se il BST è vuoto.
 *
 * Si noti che l'intervallo di chiavi è da intendersi con estremi inclusi e il tipo dei valori dei nodi
 * è da considerarsi intero (int). Dato un albero, possiamo dire di avere:
 *
 * esempio di albero (primo valore key, secondo value)
 *
 *                                          k:8 v:0
 *                      k:3 v:1                                 k:10 v:6
 *           k:1 v:2            k:6 v:3                                     k:14 v:7
 *                      k:4 v:4        k:7 v:5                      k:13 v:8
 *
 *  - Somma dei valori di nodi con chiavi in [1, 8] = 15
 *  - Somma dei valori di nodi con chiavi in [6, 16] = 29
 *  - Somma dei valori di nodi con chiavi in [15, 19] = 0
 *  - Somma dei valori di nodi con chiavi in [8, 12] = 6
 *
 * L'algoritmo implementato dev'essere ottimo, nel senso che deve visitare l'albero una sola volta e la
 * complessità termporale del caso peggiore dev'Essere O(n), dove n è il numero di chiavi del BST.
 *
 * La funzione da implementare si trova nel file ed ha il seguente prototipo:
 * int upo_bst_ksum_range(const upo_bst_t bst, const void*low, const void*high)
 * Con parametri:
 *  - bst: BST
 *  - low: puntatore al limite inferiore dell'intervallo
 *  - high: puntatore al limite superiore dell'intervallo
 * Come valore di ritorno:
 *  - Se il BST non è vuoto ed esistono nodi le cui chiavi sono contenute nell'intervallo specificato,
 *    restituisce la somma dei valori dei nodi le cui chiavi sono contenute nell'intervallo
 *  - Se il BST è vuoto o non ci sono chiavi contenute nell'intervallo specificato, restituisce 0
 *
 */

int upo_bst_ksum_range_imp(upo_bst_node_t*subtree, void*low, void*high, upo_bst_comparator_t key_cmp);

int upo_bst_ksum_range_imp(upo_bst_node_t*subtree, void*low, void*high, upo_bst_comparator_t key_cmp)
{
    if(subtree==NULL)
    {
        return 0;
    }
    int sum=0;
    int compare_low=key_cmp(subtree->key, low);
    int compare_high=key_cmp(subtree->key, high);

    if(compare_low>=0 && compare_high<=0)
    {
        sum+=subtree->value;
    }

    if(compare_low>0)
    {
        sum+=upo_bst_ksum_range_imp(subtree->left, low, high, key_cmp);
    }
    else if(compare_high<0)
    {
        sum+=upo_bst_ksum_range_imp(subtree->right, low, high, key_cmp);
    }

    return sum;
}

int upo_bst_ksum_range(const upo_bst_t bst, const void*low, const void*high)
{
    if(upo_bst_is_empty(bst))
    {
        return 0;
    }
    return upo_bst_ksum_range_imp(bst->root, low, high, bst->key_cmp);
}

/** Esercizio 2
 *
 * Implementare un algoritmo che, data una tabella hash H, con gestione delle collisioni basata su
 * indirizzaento aperto e scansione lineare con uso di tombstone (HT-LP), e una lista di chiavi Ikey,
 * calcoli il massimo numero di collisioni delle chiavi k contenute in Ikey in H.
 * In particolare, se H è vuota o se Ikey è vuota, o se nessuna chiave di Ikey è contenuta in H,
 * l'algoritmo deve restituire il valore -1.
 * Si noti che nel calcolare il numero di collisioni di una chiave k non si deve tenere conto dello slot
 * in cui k è memorizzato e che uno slot tombstone, se attraversato, è considerato una collisione.
 * Per esempio, prendendo in analisi la seguente tabella e la lista di chiavi:
 *
 *  H       key         value       tombstone
 *  0       P           10          false
 *  1       M           9           false
 *  2                               true
 *  3                               true
 *  4       A           8           false
 *  5       C           4           false
 *  6       S           0           false
 *  7       H           5           false
 *  8       L           11          false
 *  9                               true
 *  10      E           12          false
 *  11                              false
 *
 *  key_list
 *  key     next    key     next    key     next     key     next
 *  a   -   /   --   c   -   /   -   z  -   /     -   e   -   /   -
 *
 * Supponendo che il valore hash delle chiavi A, C, E, sia rispettivamente 0, 0 e 10, il numero massimo
 * di collisioni è 5 in quanto:
 *  - Il numero di collisioni di A è 4
 *  - il numero di collisioni di C è 5
 *  - il numero di collisioni di E è 0
 * L'algoritmo implementato deve essere ottimo nel senso che non deve visitare parti di HT-LP inutili
 * ai fini dell'esercizio.
 *
 * La funzione da implementare ha il seguente prototipo:
 * long upo_ht_linprob_max_collisions(const upo_ht_linprob_t ht, const upo_ht_key_list_t key_list)
 * Con parametri:
 *  - ht: HT-LP
 *  - key_list: lista concatenata di chiavi
 */

//Implementazione del metodo upo_ht_linprob_max_collisions
long upo_ht_linprob_max_collisions(const upo_ht_linprob_t ht, const upo_ht_key_list_t key_list)
{
    //Se la tabella è vuota o la lista è vuota, restituisci -1
    if(upo_ht_linprob_is_empty(ht)==1 || key_list==NULL)
    {
        return -1;
    }
    //Altrimenti
    else
    {
        //Inizializzo la variabile max a 0
        long max=0;
        //Inizializzo la variabile current alla lista di chiavi
        upo_ht_key_list_t current=key_list;

        //Finchè la lista non è vuota
        while(current!=NULL)
        {
            //Calcolo l'hasher della chiave
            size_t hash=upo_ht_linprob_get_hasher(ht)(current->key, upo_ht_linprob_capacity(ht));
            //Inizializzo i e collisions a 0
            size_t i=0;
            //inizializzo la variabile collisions a 0
            long collisions=0;
            //Finchè la chiave non è NULL e i è minore della capacità
            while(ht->slots[hash].key!=NULL && i<upo_ht_linprob_capacity(ht))
            {
                //Se la chiave è uguale alla chiave corrente, interrompo il ciclo
                if(ht->slots[hash].key==current->key)
                {
                    break;
                }
                //Altrimenti incremento il numero di collisioni, incremento l'indice e calcolo il prossimo hash
                else
                {
                    collisions++;
                    hash=(hash+1)%upo_ht_linprob_capacity(ht);
                    i++;
                }
            }
            //Se il numero di collisioni è maggiore di max, aggiorno max
            if(collisions>max)
            {
                max=collisions;
            }
            //Passo al nodo successivo
            current=current->next;
        }
        //Restituisco max
        return max;
    }
}
