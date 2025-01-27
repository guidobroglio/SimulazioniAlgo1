/******************************************************************************/
/*** NOME: Guido Lorenzo                                                    ***/
/*** COGNOME: Broglio                                                       ***/
/*** MATRICOLA: 20043973                                                    ***/
/*** DATA: 7 Novembre 2020                                                  ***/
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
Dati in input un albero binario di ricerca (BST) e una chiave k, implementare un algoritmo che restituisca
il numero di nodi interni del sotto-albero radicato in k. Si noti che:
• per nodo interno s’intende un nodo che ha almeno un figlio;
• il conteggio dei nodi può includere anche la radice del sotto-albero (cioè, il nodo contenente la chiave k)
se essa è un nodo interno. Se la chiave k non è presente nel BST o se il BST è vuoto o se il sotto-albero
non contiene nodi interni, l’algoritmo deve ritornare il valore 0.
*/
//Metodo responsabile del conteggio dei nodi interni
void upo_bst_subtree_count_inner_imp(upo_bst_node_t *node, void *key, size_t *inners);

//Metodo responsabile della selezione del nodo con valore key
upo_bst_node_t*upo_bst_get(upo_bst_node_t*node, void*key, upo_bst_comparator_t key_cmp);

//Metodo responsabile del conteggio dei nodi interni
upo_bst_node_t*upo_bst_get(upo_bst_node_t*node, void*key, upo_bst_comparator_t key_cmp)
{
    //Controllo se il nodo è NULL, in tal caso ritorno NULL
    if(node==NULL)
    {
        return NULL;
    }
    //inizializzo la variabile di confronto, confrontando la chiave con la chiave del nodo
    int compare=key_cmp(key, node->key);
    //Controllo se la chiave è minore di quella del nodo
    if(compare<0)
    {
        //Se la chiave è minore, la ricerca continua nel sottoalbero sinistro
        return upo_bst_get(node->left, key, key_cmp);
    }
    //Controllo se la chiave è maggiore di quella del nodo
    else if(compare>0)
    {
        //Se la chiave è maggiore, la ricerca continua nel sottoalbero destro
        return upo_bst_get(node->right, key, key_cmp);
    }
    //Altrimenti, la chiave è stata trovata
    else
    {
        return node;
    }
}

//Metodo responsabile dell'implementazione del conteggio dei nodi interni
void upo_bst_subtree_count_inner_imp(upo_bst_node_t *node, void *key, size_t *inners)
{
    //Controllo se il nodo è NULL, in tal caso esco dal metodo
    if(node==NULL)
    {
        return;
    }
    //Controllo se il nodo ha almeno un figlio, in tal caso incremento il contatore
    if(node->left!=NULL || node->right!=NULL)
    {
        (*inners)++;
    }
    //Chiamata ricorsiva sul sottoalbero sinistro e destro
    upo_bst_subtree_count_inner_imp(node->left, key, inners);
    upo_bst_subtree_count_inner_imp(node->right, key, inners);
}

//Metodo responsabile del conteggio dei nodi interni
size_t upo_bst_subtree_count_inner(const upo_bst_t bst, const void *key)
{
    //Controllo se l'albero è vuoto o la chiave è NULL, in tal caso ritorno 0
    if(upo_bst_is_empty(bst) || key==NULL)
    {
        return 0;
    }
    //Inizializzo il nodo con la chiave key
    upo_bst_node_t*node=upo_bst_get(bst->root, (void*)key, bst->key_cmp);
    //Controllo se il nodo è NULL, in tal caso ritorno 0
    if(node==NULL)
    {
        return 0;
    }
    //Inizializzo il contatore
    int count=0;
    //Chiamata al metodo responsabile del conteggio dei nodi interni, passando il nodo, la chiave e il contatore
    upo_bst_subtree_count_inner_imp(node, key, &count);
    //Ritorno il contatore
    return count;
}

/* ESERCIZIO 2 */
/* Implementare un algoritmo che, data una tabella hash H con gestione delle collisioni basata su
concatenazioni separate (HT-SC) e una chiave k, conti il numero di collisioni di k in H. Se k non è contenuta
in H, l’algoritmo deve ritornare il valore 0.
*/

//Metodo responsabile del conteggio delle collisioni
size_t upo_ht_sepchain_count_collisions(const upo_ht_sepchain_t ht, const void *key);

//Implementazione del metodo responsabile del conteggio delle collisioni
size_t upo_ht_sepchain_count_collisions(const upo_ht_sepchain_t ht, const void *key)
{
    if(ht==NULL || key==NULL || upo_ht_sepchain_is_empty(ht))
    {
        return 0;
    }
    size_t hash_index=ht->key_hash(key, ht->capacity);
    upo_ht_sepchain_list_node_t*current=ht->slots[hash_index].head;

    size_t collisions=0;
    int keys_found=0;

    while(current!=NULL)
    {
        int compare=ht->key_cmp(key, current->key);
        if(compare==0)
        {
            keys_found=1;
        }
        else
        {
            collisions++;
        }
        current=current->next;
    }
    if(keys_found==0)
    {
        return 0;
    }
    return collisions;
}
