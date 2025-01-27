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
#include "include/upo/sort.h"

/**
 * Implementare una funzione che calcoli il numero di foglie partendo da un nodo key in un BST.
 *
 * La funzione:
 * - Prende in ingresso un BST e una chiave `key`.
 * - Restituisce il numero di nodi foglia nel sottoalbero radicato in `key`.
 * - Se il nodo `key` non è presente o il BST è vuoto, restituisce 0.
 *
 * Si implementi la funzione:
 * size_t upo_bst_subtree_count_leaves(const upo_bst_t bst, const void *key)
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

size_t upo_bst_subtree_count_leaves_imp(upo_bst_node_t*node);

size_t upo_bst_subtree_count_leaves_imp(upo_bst_node_t*node)
{
  if(node==NULL)
  {
    return 0;
  }
  if(node->left==NULL && node->right==NULL)
  {
    return 1;
  }
  return upo_bst_subtree_count_leaves_imp(node->left)+(upo_bst_subtree_count_leaves_imp(node->right));
}

size_t upo_bst_subtree_count_leaves(const upo_bst_t bst, const void *key)
{
    if(upo_bst_is_empty(bst) || key==NULL)
    {
      return 0;
    }

    upo_bst_node_t*subtree=upo_bst_get(bst->root, (void*)key, bst->key_cmp);

    if(subtree==NULL)
    {
      return 0;
    }
    return upo_bst_subtree_count_leaves_imp(subtree);
}

/**** END of EXERCISE #1 ****/
