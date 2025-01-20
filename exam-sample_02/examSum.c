/* vim: set tabstop=4 expandtab shiftwidth=4 softtabstop=4: */


/******************************************************************************/
/*** NOME: Guido                                                            ***/
/*** COGNOME: Broglio                                                       ***/
/*** MATRICOLA: 20043973                                                    ***/
/******************************************************************************/

#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/upo/bst.h"
#include "include/upo/hashtable.h"

/**
 * Implementare un algoritmo che, dato un albero binario di ricerca BST ed una chiave k, restituisca la somma dei valori
 * dei nodi a profondità pari nel sottoalbero radicato in k. Si noti che la radice dell'intero BST si trova a profondità 0.
 * Se la chiave k non è presente nell'albero, la funzione deve ritornare 0. così come se il sottoalbero radicato in k è vuoto.
 * La complessità temporale nel caso peggiore dev'essere O(n), dove n è il numero di chiavi nel BST.
 *
 * Esempio di bst
     *       8
     *     /   \
     *    3     10
     *   / \     \
     *  1   6     14
     *     / \   /
     *    4   7 13

* - Somma dei valori del sottoalbero radicato in 8: 8+1+7+13=29
* - Somma dei valori del sottoalbero radicato in 3: 3+4=7
* - Somma dei valori del sottoalbero radicato in 1: 1=1
* - Somma dei valori del sottoalbero radicato in 5: 0 (la chiave non esiste)
* - Somma dei valori del sottoalbero radicato in 10: 10+13=23

* La funzione da implementare si trova nel file exam.c e ha il seguente prototipo:
* int upo_bst_subtree_sum_even(const upo_bst_t bst, const void *key)

* Parametri:
* - bst: BST
* - key: la chiave del nodo radice del sottoalbero di cui si vuole calcolare la somma dei valori a profondità pari

* Valore di ritorno:
* - Se il BST non è vuoto e la chiave key è contenuta nel BST: la somma dei valori dei nodi a profondità pari
* - Se il BST è vuoto o la chiave key non è contenuta nel BST o il sottoalbero radicato in key non ha nodi a profondità pari: 0
 */
/**** BEGIN of EXERCISE #1 ****/
//Dichiarazione del metodo che restituisce la somma dei valori dei nodi a profondità pari
upo_bst_node_t*upo_bst_get_and_depth(upo_bst_node_t*node, void*key, upo_bst_comparator_t key_cmp, size_t*depth);
//Dichiarazione del metodo che restituisce la somma dei valori dei nodi a profondità pari
int upo_bst_subtree_sum_even_imp(upo_bst_node_t*node, size_t depth);
//Implementazione del metodo che restituisce la somma dei valori dei nodi a profondità pari
upo_bst_node_t*upo_bst_get_and_depth(upo_bst_node_t*node, void*key, upo_bst_comparator_t key_cmp, size_t*depth)
{
  //Se il nodo è NULL, restituisci NULL
  if(node==NULL)
  {
    return NULL;
  }
  //Inizializzo la variabile di confronto, confrontando la chiave con la chiave del nodo
  int compare=key_cmp(key, node->key);
  //Se il confronto è minore a 0, il nodo è nel sottoalbero sinistro, incremento la prodontità
  if(compare<0)
  {
    (*depth)++;
    return upo_bst_get_and_depth(node->left, key, key_cmp, depth);
  }
  //Se il confronto è maggiore a 0, il nodo è nel sottoalbero destro, incremento la profondità
  else if(compare>0)
  {
    (*depth)++;
    return upo_bst_get_and_depth(node->right, key, key_cmp, depth);
  }
  //Altrimenti restituisce il nodo.
  else
  {
    return node;
  }
}

//Implementazione del metodo che restituisce la somma dei valori dei nodi a profondità pari
int upo_bst_subtree_sum_even_imp(upo_bst_node_t*node, size_t depth)
{
  //Se il nodo è NULL, restituisci 0
  if(node==NULL)
  {
    return 0;
  }
  //Inizializzo la variabile somma
  int somma=0;
  //Se la profondità è pari, incremento la somma
  if(depth%2==0)
  {
    somma+=node->key;
  }
  //Chiamata ricorsiva sul sottoalbero sinistro e destro
  somma+=upo_bst_subtree_sum_even_imp(node->left, depth+1);
  somma+=upo_bst_subtree_sum_even_imp(node->right, depth+1);
  //Restituisco la somma
  return somma;
}

//Implementazione del metodo che restituisce la somma dei valori dei nodi a profondità pari
int upo_bst_subtree_sum_even(const upo_bst_t bst, const void* key)
{
  //Se l'albero è vuoto o la chiave è NULL, restituisci 0
  if(upo_bst_is_empty(bst) || key==NULL)
  {
    return 0;
  }
  //Inizializzo la variabile depth a 0
  size_t depth=0;
  //Richiamo il metodo che restituisce il nodo con la chiave key, passando la radice dell'albero, la chiave key, il comparatore e la profondità
  upo_bst_node_t*node=upo_bst_get_and_depth(bst->root, key, bst->key_cmp, &depth);
  //Se il nodo è NULL, restituisci 0
  if(node==NULL)
  {
    return 0;
  }
  //Richiamo il metodo che restituisce la somma dei valori dei nodi a profondità pari, passando il nodo e la profondità
  return upo_bst_subtree_sum_even_imp(node, depth);
}


/**** END of EXERCISE #1 ****/