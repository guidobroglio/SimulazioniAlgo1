/******************************************************************************/
/*** NOME: Guido Lorenzo                                                    ***/
/*** COGNOME: Broglio                                                       ***/
/*** MATRICOLA: 20043973                                                    ***/
/*** DATA: 12 Settembre 2022                                                ***/
/******************************************************************************/


//Esame 12/09/2022
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/upo/bst.h"
#include "include/upo/hashtable.h"


/** Esercizio 1
 *
 * Implementare un algoritmo che, dato un albero binario di ricerca BST e una chiave k, non necessariamente
 * contenuta nel BST, ritorni il succcessore di k, cioè la più piccola chiave k' nel BST tale che
 * k'>k. Se il successore di k non esiste o se il BST è vuoto, l'algoritmo deve ritornare il valore NULL.
 *
 * Esempio albero
 *
 *              8
 *      3               10
 *  1       6                   14
 *      4       7           13
 *
 * Per esempio, dato l'albero, si ha:
 *  - Successore di 8: 10
 *  - Successore di 1: 3
 *  - Successore di 5: 6
 *  - Successore di 14: NULL (non esiste)
 *  - Successore di 0: 1
 *  - Successore di 11: 13
 *  - Successore di 100: NULL (non esiste)
 *
 * L'algoritmo implementato dev'essere ottimo, nel senso che deve visitare l'albero una sola volta e non
 * deve visitare parti del BST inutili ai fini dell'esercizio, e la complessità temporale nel caso
 * peggiore dev'essere O(n), dove n è il numero di chiavi nel BST.
 *
 * Il prototipo è:
 * void*upo_bst_predecessor(const upo_bst_t bst, const void*key);
 * Con parametri:
 *  - bst: bst
 *  - key: puntatore alla chiave per cui si vuole ottenere il successore
 * Come valori di ritorno abbiamo:
 *  - Se il BST non è vuoto e il successore esiste, ritorna un puntatore alla chiave rappresentante il
 *    successore di key
 *  - Se il BST è vuoto o il successore non esiste, ritorna NULL.
 */

//Metodo che restituisce il successore di key
void*upo_bst_predecessor_imp(upo_bst_node_t*node, void*key, upo_bst_comparator_t key_cmp, upo_bst_node_t*node_predecessor);

//Implementazione del metodo che restituisce il successore di key
void*upo_bst_predecessor_imp(upo_bst_node_t*node, void*key, upo_bst_comparator_t key_cmp, upo_bst_node_t*node_predecessor)
{
    //Se il nodo è NULL
    if(node==NULL)
    {
        //Se il predecessore è diverso da NULL, restituisci la chiave del predecessore
        if(node_predecessor!=NULL)
        {
            return (node_predecessor)->key;
        }
        //Altrimenti restituisce NULL
        else
        {
            return NULL;
        }
    }
    //Altrimenti, initializza la variabile compare con il risultato della comparazione tra la chiave del nodo e la chiave key
    int compare=key_cmp(key, node->key);
    //Se compare è minore di 0, restituisci il successore del sottoalbero sinistro
    if(compare<0)
    {
        node_predecessor=node;
        return upo_bst_predecessor_imp(node->left, key, key_cmp, node_predecessor);
    }
    //Se compare è maggiore di 0, restituisci il successore del sottoalbero destro
    else if(compare>0)
    {
        return upo_bst_predecessor_imp(node->right, key, key_cmp, node_predecessor);
    }
    //Altrimenti
    else
    {
        //Se il nodo ha un figlio destro, restituisci il minimo del sottoalbero destro
        if(node->right!=NULL)
        {
            upo_bst_node_t*current=node->right;
            //Finchè il figlio sinistro non è NULL, scorri a sinistra
            while(current->left!=NULL)
            {
                current=current->left;
            }
            //Restituisci la chiave del nodo corrente
            return current->key;
        }
        //Altrimenti, restituisci il predecessore
        if(node_predecessor!=NULL)
        {
            //Restituisci la chiave del predecessore
            return (node_predecessor)->key;
        }
        //Altrimenti restituisci NULL
        else
        {
            return NULL;
        }
    }
}

//Implementazione del metodo che restituisce il successore di key
void*upo_bst_predecessor(const upo_bst_t bst, const void*key)
{
    //Se l'albero è vuoto o la chiave è NULL, restituisci NULL
    if(upo_bst_is_empty(bst) || key==NULL)
    {
        return NULL;
    }
    //Altrimenti, initializza il nodo predecessore a NULL
    upo_bst_node_t*node_predecessor=NULL;
    //Richiama il metodo che restituisce il successore di key
    return upo_bst_predecessor_imp(bst->root, key, bst->key_cmp, &node_predecessor);
}

/** Esercizio 2
 *
 * Si consideri una tabella hash H con gestione delle collisioni basata su concatenazioni separate (HT-SC)
 * in cui le liste delle collisioni sono ordinate secondo il valore della chiave. Implementare un
 * algoritmo che realizzi l'operazioni "oput" la quale inserisce la coppia chiave-valore (k, v) in H,
 * preservando l'ordinamento delle liste delle collisioni. In particolare, l'operazione "oput" funziona
 * nel seguente modo:
 *  - Se la chiave k non è già contenuta in H, "oput" inserisce in H la coppia chiave-Valore (k, v) nella
 *    lista delle collisioni in maniera ordinata secondo il valore della chiave, aggiorna la dimensione
 *    di H, e ritorna il valore NULL.
 *  - Se la chiave k è già contenuta in H come coppia (k, vold), "oput" sostiuisce il vecchio valore
 *    vold con v e ritorna vold.
 *
 * In pratica, l'operazione oput è simile alla classica operazione pt ma con la differenza che oput in caso
 * di inserimento di una nuova coppia chiave-valore, deve preservare l'ordinamento della lista delle
 * collisioni in cui la nuova coppia viene inserita.
 *
 * esempio tabella hash
 *
 *  0 -> A 8 -> P 10
 *  1 ->
 *  2 -> C 4 -> H 5 -> M 9 -> R 3
 *  3 -> S 0 -> X 7
 *  4 -> E 12 -> T 11
 *
 * Nella tabella abbiamo 10 coppie chiave-valore (k, v), abbiamo ad esempio:
 *  - oput di <F,2>: la chiave F non è contenuta nella tabella hash, supponendo che la funzione di
 *    hashing restituisca 0, verrà quindi inserita la nuova coppia-valore <F,2> nello slot 0 tra le
 *    coppie <A,8> e <P,10>, in modo da perseverare l'ordinamento della lista secondo la chiave.
 *    Si aggiornerà la dimensione della tabella hash a 11, e si ritornerà NULL.
 *  - oput di <C, 3>: la chiave C è già presente nella tabella hash come coppia-valore <C,4>. Verrà quindi
 *    rimpiazzato il vecchio valore 4 con 3 e si ritornerà 4.
 *
 * L'algoritmo implementato deve essere ottimo, nel senso che non deve visitare parti del HT-SC inutili-
 * La funzione da implementare ha il seguente prototipo:
 * void*upo_ht_sepchain_oput(upo_ht_sepchain_t ht, void*key, void*value)
 * Con parametro:
 *  - ht: HT-SC
 *  - key: Puntatore alla chiave
 *  - value: puntatore al valore
 * Come valore di ritorno avremo:
 *  - Se la chiave key è contenuta nella HT-SC, restituisce il puntatore vecchio valore memorizzato nella
 *    HT-SC e associato a key.
 *  - Se la chiave key non è contenuta nella HT-SC, restituisce il valore NULL.
 */

//Metodo che restituisce il nodo con chiave key
void* upo_ht_sepchain_oput(upo_ht_sepchain_t ht, void* key, void* value)
{
    if (ht == NULL || key == NULL)
    {
        return NULL;
    }

    size_t hash_index = ht->key_hash(key, ht->capacity);
    upo_ht_sepchain_list_node_t* current = ht->slots[hash_index].head;
    upo_ht_sepchain_list_node_t* previous = NULL;

    while (current != NULL && ht->key_cmp(key, current->key) > 0)
    {
        previous = current;
        current = current->next;
    }

    if (current != NULL && ht->key_cmp(key, current->key) == 0)
    {
        void* old_value = current->value;
        current->value = value;
        return old_value;
    }

    upo_ht_sepchain_list_node_t* new_node = malloc(sizeof(upo_ht_sepchain_list_node_t));
    if (new_node == NULL)
    {
        return NULL;
    }

    new_node->key = key;
    new_node->value = value;
    new_node->next = current;

    if (previous == NULL)
    {
        ht->slots[hash_index].head = new_node;
    }
    else
    {
        previous->next = new_node;
    }

    ht->size++;
    return NULL;
}
