#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/upo/bst.h"
#include "include/upo/sort.h"
#include "include/upo/hashtable.h"

upo_bst_node_t* upo_bst_node_create(void*key, void*value):

void upo_bst_insert(upo_bst_t bst, void*key, void*value);

upo_bst_node_t*upo_bst_insert_impl(upo_bst_node_t*node, void*key, void*value, upo_bst_comparator_t comp);

upo_bst_value_list_node_t* upo_bst_add_list(upo_bst_value_list_node_t*list, upo_bst_node_t*node);

upo_bst_node_t*upo_bst_node_create(void*key, void*value)
{
  upo_bst_node_t*node =calloc(1, sizeof(upo_bst_node_t));

  if(node==NULL)
  {
    perror("errore nella creazione");
    abort();
  }
  node->key=key;
  node->values=NULL;
  node->values=upo_bst_add_list(node->values, value);
  node->left=NULL;
  node->right=NULL;

  return node;
}

void upo_bst_insert(upo_bst_t bst, void*key, void*value)
{
  if(bst==NULL)
  {
    abort();
  }
  else
  {
    bst->root=upo_bst_insert_impl(bst->root, key, bst, bst->key_cmp);
  }
}

upo_bst_node_t*upo_bst_insert_impl(upo_bst_node_t*node, void*key, void*value, upo_bst_comparator_t comp)
{
  if(node==NULL)
  {
    return upo_bst_node_create(key, value);
  }
  else if(comp(key, node->key)<0)
  {
    node->left=upo_bst_insert_impl(node->left, key, value, comp);
  }
  else if(comp(node->key)>0)
  {
    node->right=upo_bst_insert_impl(node->right, key, value, comp);
  }
  else
  {
    node->values=upo_bst_add_list(node->values, value);
  }
  return node;
}

upo_bst_value_list_node_t*upo_bst_add_list(upo_bst_value_list_node_t*list, upo_bst_node_t*nodeList)
{
  upo_bst_value_list_node_t*listNode=calloc(1, sizeof(upo_bst_value_list_node_t));
  listNode->value=value;
  listNode->next=list;
  list=nodeList;
  return nodeList;
}



//////////////////////////////////////////////////////////////////////////////////////////////////////////

void upo_ht_sepchain_insert(upo_ht_sepchain_t ht, void*key, void*value);

void upo_ht_sepchain_insert(upo_ht_sepchain_t ht, void*key, void*value)
{
  if(ht!=NULL && ht->slots!=NULL)
  {
    size_t h = ht->key_hash(key, ht->capacity);

    upo_ht_sepchain_list_node_t *node=ht->slots[h].head;

    while(node!=NULL && ht->key_cmp(key, node->key)!=0)
    {
      node=node->next;
    }
    if(node==NULL)
    {
      upo_ht_sepchain_list_node_t*node=calloc(1, sizeof(upo_ht_sepchain_list_node_t));

      if(node==NULL)
      {
        abort();
      }
      else
      {
        node->key=key;
        node->value=value;
        node->next=ht->slots[h].head;
        ht->slots[h].head=node;
        ht->size+=1;
      }
    }
  }
}

void upo_ht_sepchain_merge(upo_ht_sepchain_t dest_ht, const upo_ht_sepchain_t src_ht)
{
  if(src_ht!=NULL && dest_ht!=NULL)
  {
    for(size_t i=0; i<src_ht->capacity; i++)
    {
      upo_ht_sepchain_list_node_t*node=calloc(1, sizeof(upo_ht_sepchain_list_node_t));
      while(node!=NULL)
      {
        upo_ht_sepchain_insert(dest_ht, node->key, node->value);
        node=node->next;
      }
    }
  }
}
