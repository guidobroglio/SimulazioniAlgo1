#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/upo/bst.h"
#include "include/upo/sort.h"
#include "include/upo/hashtable.h"
#include "include/upo/bst_key_list.h"


/* Ritorna la chiave con valore minore */
void* upo_bst_min(const upo_bst_t tree)
{
    if(tree == NULL)
    {
      return NULL;
    }
    upo_bst_node_t *n = upo_bst_min_impl(tree->root);
    if(n == NULL)
    {
      return NULL;
    }
    return n->key;
}

upo_bst_node_t *upo_bst_min_impl(upo_bst_node_t *n)
{
    if(n == NULL)
    {
      return NULL;
    }
    else if(n->left == NULL)
    {
      return n;
    }
    else return upo_bst_min_impl(n->left);
}

/* Ritorna la chiave con valore maggiore */
void* upo_bst_max(const upo_bst_t tree)
{
    if(tree == NULL)
    {
      return NULL;
    }
    upo_bst_node_t *n = upo_bst_max_impl(tree->root);
    if(n == NULL)
    {
      return NULL;
    }
    else
    {
      return n->key;
    }
}

upo_bst_node_t *upo_bst_max_impl(upo_bst_node_t *n) {
    if(n == NULL)
    {
      return NULL;
    }
    else if(n->right == NULL)
    {
      return n;
    }
    else
    {
      return upo_bst_max_impl(n->right);
    }
}

/* Elimina la chiave di valore minimo */
void upo_bst_delete_min(upo_bst_t tree, int destroy_data)
{
    if(tree == NULL)
    {
      return;
    }
    upo_bst_delete(tree, upo_bst_min(tree), destroy_data);
}

/* Elimina la chiave di valore massimo */
void upo_bst_delete_max(upo_bst_t tree, int destroy_data)
{
    if(tree == NULL)
    {
      return;
    }
    upo_bst_delete(tree, upo_bst_max(tree), destroy_data);
}

/* Torna la chiave più grande tra quelle minori che passi alla funzione */
void* upo_bst_floor(const upo_bst_t tree, const void *key)
{
    if(tree == NULL)
    {
      return NULL;
    }
    upo_bst_node_t *n = upo_bst_floor_impl(tree->root, key, tree->key_cmp);
    if(n == NULL)
    {
      return NULL;
    }
    return n->key;
}

upo_bst_node_t *upo_bst_floor_impl(upo_bst_node_t *n, void *key, upo_bst_comparator_t comp)
{
    if(n == NULL)
    {
      return NULL;
    }
    if(comp(key, n->key) < 0)
    {
      upo_bst_floor_impl(n->left, key, comp);
    }
    else if(comp(key, n->key) > 0)
    {
        upo_bst_node_t *fNode = upo_bst_floor_impl(n->right, key, comp);
        if(fNode != NULL)
        {
          return fNode;
        }
        else
        {
          return n;
        }
    }
    else
    {
      return n;
    }
    return NULL;
}

/* Torna la chiave più grande tra quelle maggiori che passi alla funzione */
void* upo_bst_ceiling(const upo_bst_t tree, const void *key)
{
    if(tree == NULL)
    {
      return NULL;
    }
    upo_bst_node_t *n = upo_bst_ceiling_impl(tree->root, key, tree->key_cmp);
    if(n == NULL)
    {
      return NULL;
    }
    return n->key;
}

upo_bst_node_t *upo_bst_ceiling_impl(upo_bst_node_t *n, void *key, upo_bst_comparator_t comp)
{
    if(n == NULL)
    {
      return NULL;
    }
    if(comp(key, n->key) > 0)
    {
      upo_bst_ceiling_impl(n->right, key, comp);
    }
    else if(comp(key, n->key) < 0)
    {
        upo_bst_node_t *cNode = upo_bst_ceiling_impl(n->left, key, comp);
        if(cNode != NULL)
        {
          return cNode;
        }
        else
        {
          return n;
        }
    }
    else
    {
      return n;
    }
    return NULL;
}

/* Ritorna le chiavi che sono nel range passato come parametro */
upo_bst_key_list_t upo_bst_keys_range(const upo_bst_t tree, const void *low_key, const void *high_key)
{
    if(tree == NULL)
    {
      return NULL;
    }
    upo_bst_key_list_t keys = NULL;
    upo_bst_keys_range_impl(tree->root, low_key, high_key, tree->key_cmp, &keys);
    return keys;
}

void upo_bst_keys_range_impl(upo_bst_node_t *n, void *low, void *high, upo_bst_comparator_t comp, upo_bst_key_list_t *keys)
{
    if(n == NULL)
    {
      return;
    }
    upo_bst_keys_range_impl(n->left, low, high, comp, keys);
    if(comp(n->key, low) >= 0 && comp(n->key, high) <= 0)
    {
        upo_bst_key_list_node_t *node = malloc(sizeof(upo_bst_key_list_node_t));
        if(node == NULL)
        {
          return;
        }
        node->key = n->key;
        node->next = *keys;
        *keys = node;
    }
    upo_bst_keys_range_impl(n->right, low, high, comp, keys);
}

/* Ritorna tutte le chiavi dell'albero di ricerca */
upo_bst_key_list_t upo_bst_keys(const upo_bst_t tree)
{
    if(tree == NULL)
    {
      return NULL;
    }
    upo_bst_key_list_t keys = NULL;
    upo_bst_keys_impl(tree->root, tree->key_cmp, &keys);
    return keys;
}

void upo_bst_keys_impl(upo_bst_node_t *n, upo_bst_comparator_t comp, upo_bst_key_list_t *keys)
{
    if(n == NULL)
    {
      return;
    }
    upo_bst_keys_impl(n->left, comp, keys);
    upo_bst_key_list_node_t *node = malloc(sizeof(upo_bst_key_list_node_t));
    node->key = n->key;
    node->next = *keys;
    *keys = node;
    upo_bst_keys_impl(n->right, comp, keys);
}

/* Verifica se la chiave data soddisfa le proprietà dell'albero di ricerca */
int upo_bst_is_bst(const upo_bst_t tree, const void *min_key, const void *max_key)
{
    if(tree = NULL)
    {
      return 1;
    }
    return upo_bst_is_bst_impl(tree->root, min_key, max_key, tree->key_cmp);
}

int upo_bst_is_bst_impl(upo_bst_node_t *n, void *min, void *max, upo_bst_comparator_t comp)
{
    if(n == NULL)
    {
      return 1;
    }
    if(comp(n->key, min) < 0 || comp(n->key, max) > 0)
    {
      return 0;
    }
    return upo_bst_is_bst_impl(node->left, min_key, node->key, key_cmp) && upo_bst_is_bst_impl(node->right, node->key, max_key, key_cmp);
}

/* ESERCIZI EXTRA */
/* Esercizio 8 - Calcolare il rango di k*/
size_t upo_bst_rank(const upo_bst_t tree, const void *key)
{
    if(tree == NULL)
    {
      return 0;
    }
    size_t res = 0;
    upo_bst_rank_impl(tree->root, key, tree->key_cmp, &res);
    return res;
}

void upo_bst_rank_impl(upo_bst_node_t *n, void *key, upo_bst_comparator_t comp, size_t *rank) {
    if(n == NULL)
    {
      return;
    }
    if(comp(n->key, key) < 0)
    {
        *rank += 1;
        upo_bst_rank_impl(n->left, key, comp, rank);
        if(n->right != NULL && comp(n->right->key, key) < 0)
        {
            upo_bst_rank_impl(n->right, key, comp, rank);
        }
    }
}

/* Esercizio 9 - calcolare il predecessore */
void* upo_bst_predecessor(const upo_bst_t tree, const void *key)
{
    if(tree == NULL)
    {
      return NULL;
    }
    upo_bst_node_t *n = NULL;
    upo_bst_predecessor_impl(tree->root, n, key, tree->key_cmp)
    return n->key;
}

upo_bst_node_t *upo_bst_node_max(upo_bst_node_t *n)
{
    while(n->right != NULL)
    {
      n = n->right;
    }
    return n;
}

void upo_bst_predecessor_impl(upo_bst_node_t *n, upo_bst_node_t *pred, void *key, upo_bst_comparator_t comp)
{
    if(n == NULL)
    {
      return;
    }
    if(comp(n->key, key) == 0)
    {
      pred = upo_bst_node_max(n->left);
    }
    else if(comp(n->key, key) > 0)
    {
        upo_bst_predecessor_impl(n->left, pred, key, comp);
    }
    else
    {
        pred = n;
        upo_bst_predecessor_impl(n->right, pred, key, comp);
    }
}

/* Esercizio 10 - calcolare valore e profondità di un nodo */
void* upo_bst_get_value_depth(const upo_bst_t tree,const void*key, long*depth)
{
    if(tree == NULL)
    {
      return NULL;
    }
    void *res = upo_bst_get_value_depth_impl(tree->root, key, depth, tree->key_cmp);
    *depth = res == NULL ? -1 : *depth;
    return res;
}

void* upo_bst_get_value_depth_impl (upo_bst_node_t *n, void *key, long*depth, upo_bst_comparator_t cmp)
{
    if(n == NULL)
    {
      return NULL;
    }
    if(comp(n->key, key) == 0)
    {
      return n->value;
    }
    else if(comp(n->key, key) > 0)
    {
        *depth += 1;
        return upo_bst_get_value_depth_impl (n->left, key, depth, cmp);
    }
    else
    {
        *depth += 1;
        return upo_bst_get_value_depth_impl (n->right, key, depth, cmp);
    }
}

/* Esercizio 11 - calcolare le chiavi dell'albero*/
upo_bst_key_list_t upo_bst_keys_le(const upo_bst_t tree,const void*key)
{
    if(upo_bst_is_empty(tree) key);
    {
      upo_bst_key_list_t list = NULL;
    }
    upo_bst_keys_le_impl(tree->root, key, tree->key_cmp, &list);
    return list;
}

void upo_bst_keys_le_impl (upo_bst_node_t *n, void *key, comparator_t comp, upo_bst_key_list_t keys)
{
    if(n == NULL)
    {
      return;
    }
    upo_bst_keys_le_impl(n->left, key, comp, keys);
    if(comp(n->key, key) < 0)
    {
        upo_bst_key_list_node_t *node = malloc(sizeof(struct upo_bst_key_list_node_s));
        if(node == NULL)
        {
          return;
        }
        node->key = n->key;
        node->next = *keys;
        *keys = node;
    }
    upo_bst_keys_le_impl(n->right, key, comp, keys);
}
