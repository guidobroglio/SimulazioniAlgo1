#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/upo/bst.h"
#include "include/upo/sort.h"
#include "include/upo/hashtable.h"
#include "include/upo/bst_key_list.h"

/* CONCATENAZIONE SEPARATA */

upo_ht_sepchain_t upo_ht_sepchain_create(size_t m, upo_ht_hasher_t key_hash, upo_ht_comparator_t key_cmp)
{

    assert(key_hash != NULL);
    assert(key_cmp != NULL);

    upo_ht_sepchain_t ht = NULL;
    size_t i = 0;

    ht = malloc(sizeof(struct upo_ht_sepchain_s));
    if(ht == NULL)
    {
      abort();
    }
    ht->slots = malloc(m*sizeof(upo_ht_sepchain_slot_t));
    if(ht->slots == NULL)
    {
      abort();
    }
    for(i = 0; i < m; i++)
    {
        ht->slots[i].head = NULL;
    }
    ht->capacity = m;
    ht->size = 0;
    ht->key_hash = key_hash;
    ht->key_cmp = key_cmp;
    return ht;
}

void upo_ht_sepchain_destroy(upo_ht_sepchain_t ht, int destroy_data)
{
    if(ht != NULL)
    {
        upo_ht_sepchain_clear(ht, destroy_data);
        free(ht->slots);
        free(ht);
    }
}

void upo_ht_sepchain_clear(upo_ht_sepchain_t ht, int destroy_data)
{
    if(ht != NULL && ht->slots != NULL)
    {
        for(size_t i = 0; i < ht->capacity; i++)
        {
            upo_ht_sepchain_list_node_t *list = NULL;
            list = ht->slots[i].head;
            while(list != NULL)
            {
                upo_ht_sepchain_list_node_t *node = list;
                list = list->next;
                if(destroy_data)
                {
                    free(node->key);
                    free(node->value);
                }
                free(node);
            }
            ht->slots[i].head = NULL;
        }
        ht->size = 0;
    }
}

void* upo_ht_sepchain_put(upo_ht_sepchain_t ht, void *key, void *value)
{
    void *old_value = NULL;

    if(ht != NULL && ht->slots != NULL)
    {
        size_t h = ht->key_hash(key, ht->capacity);
        upo_ht_sepchain_list_node_t *n = ht->slots[h].head;
        while(n != NULL && ht->key_cmp(key, n->key) != 0)
        {
          n = n->next;
        }
        if(n == NULL)
        {
            n = malloc(sizeof(upo_ht_sepchain_list_node_t));
            n->key = key;
            n->value = value;
            n->next = ht->slots[h].head;
            ht->slots[h].head = n;
        }
        else
        {
            old_value = n->value;
            n->value = value;
        }
    }
    return old_value;
}

void upo_ht_sepchain_insert(upo_ht_sepchain_t ht, void *key, void *value)
{
    if(ht != NULL && ht->slots != NULL)
    {
        size_t h = ht->key_hash(key, ht->capacity);
        upo_ht_sepchain_list_node_t *n = ht->slots[h].head;
        while(n != NULL && ht->key_cmp(key, n->key) != 0)
        {
          n = n->next;
        }
        if(n == NULL)
        {
            n = malloc(sizeof(upo_ht_sepchain_list_node_t));
            n->key = key;
            n->value = value;
            n->next = ht->slots[h].head;
            ht->slots[h].head =n;
        }
    }
}

void* upo_ht_sepchain_get(const upo_ht_sepchain_t ht, const void *key)
{
    if(ht != NULL && ht->slots != NULL)
    {
        size_t h = ht->key_hash(key, ht->capacity);
        upo_ht_sepchain_list_node_t *n = ht->slots[h].head;
        while(n != NULL && ht->key_cmp(key, n->key) != NULL)
        {
          n = n->next;
        }
        if(n != NULL)
        {
          return n->value;
        }
        else
        {
          return NULL;
        }
    }
}

int upo_ht_sepchain_contains(const upo_ht_sepchain_t ht, const void *key)
{
    if(ht != NULL && ht->slots != NULL)
    {
        size_t h = ht->key_hash(key, ht->capacity);
        upo_ht_sepchain_list_node_t *n =  ht->slots[h].head;
        while(n != NULL && ht->key_cmp(key, n->key) != 0)
        {
          n = n->next;
        }
        if(n != NULL)
        {
          return 1;
        }
        else
        {
          return 0;
        }
    }
}

void upo_ht_sepchain_delete(upo_ht_sepchain_t ht, const void *key, int destroy_data)
{
    if(ht != NULL && ht->slots != NULL)
    {
        size_t h = ht->key_hash(key, ht->capacity);
        upo_ht_sepchain_list_node_t *n = ht->slots[h].head;
        upo_ht_sepchain_list_node_t *p = NULL;
        while(n != NULL && ht->key_cmp(key, n->key) != 0)
        {
            p = n;
            n = n->next;
        }
        if(n != NULL)
        {
            if(p  == NULL)
            {
                ht->slots[h].head = n->next;
            }
            else
            {
              p->next = n->next;
            }
            if(destroy_data)
            {
                free(n->key);
                free(n->value);
            }
            free(n);
            ht->size--;
        }
    }
}

size_t upo_ht_sepchain_size(const upo_ht_sepchain_t ht)
{
    /* TO STUDENTS:
     *  Remove the following two lines and put here your implementation. */
    /* fprintf(stderr, "To be implemented!\n");
    abort(); */
    return ht != NULL ? ht->size : 0;
}

int upo_ht_sepchain_is_empty(const upo_ht_sepchain_t ht)
{
    return upo_ht_sepchain_size(ht) == 0 ? 1 : 0;
}

size_t upo_ht_sepchain_capacity(const upo_ht_sepchain_t ht)
{
    return (ht != NULL) ? ht->capacity : 0;
}

double upo_ht_sepchain_load_factor(const upo_ht_sepchain_t ht)
{
    return upo_ht_sepchain_size(ht) / (double) upo_ht_sepchain_capacity(ht);
}

upo_ht_comparator_t upo_ht_sepchain_get_comparator(const upo_ht_sepchain_t ht)
{
    return ht->key_cmp;
}

upo_ht_hasher_t upo_ht_sepchain_get_hasher(const upo_ht_sepchain_t ht)
{
    return ht->key_hash;
}

/* INDIRIZZAMENTO APERTO */
upo_ht_linprob_t upo_ht_linprob_create(size_t m, upo_ht_hasher_t key_hash, upo_ht_comparator_t key_cmp)
{
    upo_ht_linprob_t ht = NULL;
    size_t i = 0;
    assert(key_hash != NULL);
    assert(key_cmp != NULL);
    ht = malloc(sizeof(struct upo_ht_linprob_s));
    if(ht == NULL)
    {
      abort();
    }
    if(m > 0)
    {
        ht-slots = malloc(m*sizeof(upo_ht_linprob_slot_t));
        if(ht->slots == NULL)
        {
          abort();
        }
        for(i = 0; i < m; i++)
        {
            ht->slots[i].key = NULL;
            ht->slots[i].value = NULL;
            ht->slots[i].tombstone = 0;
        }
    }
    ht->capacity = m;
    ht->size = 0;
    ht->key_hash = key_hash;
    ht->key_cmp = key_cmp;

    return ht;
}

void upo_ht_linprob_destroy(upo_ht_linprob_t ht, int destroy_data)
{
    if(ht!= NULL)
    {
        upo_ht_linprob_clear(ht, destroy_data);
        free(ht->slots);
        free(ht);
    }
}

void upo_ht_linprob_clear(upo_ht_linprob_t ht, int destroy_data)
{
    if(ht != NULL && ht->slots != NULL)
    {
        size_t i = 0;
        for(i = 0; i < ht->capacity; i++)
        {
            if(ht-slots[i].key != NULL)
            {
                if(destroy_data)
                {
                    free(ht->slots[i].key);
                    free(ht->slots[i].value);
                }
                ht->slots[i].key = NULL;
                ht->slots[i].value = NULL;
                ht->slots[i].tombstone = 0;
            }
        }
        ht->size = 0;
    }
}

void* upo_ht_linprob_put(upo_ht_linprob_t ht, void *key, void *value)
{
    void *old_value = NULL;
    if(ht != NULL && ht->slots != NULL)
    {
        if(upo_ht_linprob_load_factor(ht) >= 0.5)
        {
          upo_ht_linprob_resize(ht, (ht->capacity)*2);
        }
        size_t h = ht->key_hash(key, ht->capacity);
        int foundTomb = 0;
        int hTomb;
        while(ht->slots[h].key != NULL && ht->key_cmp(key, ht->slots[h].key) != 0 || ht->slots[h].tombstone  == 1)
        {
            if(ht->slots[h].tombstone  == 1 && foundTomb == 0)
            {
                foundTomb = 1;
                hTomb = h;
            }
            h = (h + 1) % ht->capacity;
        }
        if(ht->slots[h].key == NULL)
        {
            if(foundTomb == 1)
            {
              h = hTomb;
            }
            ht->slots[h].key = key;
            ht->slots[h].value = value;
            ht->slots[h].tombstone = 0;
        }
        else
        {
            old_value = ht->slots[h].value;
            ht->slots[h].value = old_value;
        }
    }
    return old_value;
}

void upo_ht_linprob_insert(upo_ht_linprob_t ht, void *key, void *value)
{
    if(ht != NULL && ht->slots != NULL)
    {
        if(upo_ht_linprob_load_factor(ht) >= 0.5)
        {
          upo_ht_linprob_resize(ht, (ht->capacity)*2);
        }
        size_t h = ht->key_hash(key, ht->capacity);
        int foundTomb = 0;
        int hTomb;
        while(ht->slots[h].key != NULL && ht->key_cmp(key, ht->slots[h].key) != 0 || ht->slots[h].tombstone == 1)
        {
            if(ht->slots[h].tombstone == 1 && foundTomb == 0)
            {
                foundTomb = 1;
                hTomb = h;
            }
            h = (h + 1) % ht->capacity;
        }
        if(ht->slots[h].key == NULL)
        {
            if(foundTomb == 1)
            {
              h = hTomb;
            }
            ht->slots[h].key = key;
            ht->slots[h].value = value;
            ht->slots[h].tombstone = 0;
        }
    }
}

void* upo_ht_linprob_get(const upo_ht_linprob_t ht, const void *key)
{
    if(ht != NULL && ht->slots != NULL)
    {
        if(upo_ht_linprob_load_factor(ht) >= 0.5)
        {
          upo_ht_linprob_resize(ht, (ht->capacity)*size);
        }

        size_t h = ht->key_hash(key, ht->capacity);
        int foundTomb = 0;
        int hTomb;
        while(ht->slots[h].key != NULL && ht->key_cmp(key, ht->slots[h].key) != 0 || ht->slots[h].tombstone == 1)
        {
            h = (h + 1) % ht->capacity;
        }
        if(ht->slots[h].key != NULL)
        {
          return ht->slots[h].value;
        }
        else
        {
          return NULL;
        }
    }
    return NULL;
}

int upo_ht_linprob_contains(const upo_ht_linprob_t ht, const void *key)
{
    if(ht != NULL && ht->slots != NULL)
    {
        if(upo_ht_linprob_load_factor(ht) >= 0.5)
        {
          upo_ht_linprob_resize(ht, (ht->capacity)*2);
        }
        size_t h = ht->key_hash(key, ht->capacity);
        int foundTomb = 0;
        int hTomb;
        while(ht->slots[h].key != NULL && ht->key_cmp(key, ht->slots[h].key) != 0 || ht->slots[h].tombstone == 1)
        {
          h = (h + 1) % ht->capacity;
        }
        if(ht->slots[h].key != NULL)
        {
          return 1;
        }
        else
        {
          return 0;
        }
    }
    return 0;
}

void upo_ht_linprob_delete(upo_ht_linprob_t ht, const void *key, int destroy_data)
{
    if(ht != NULL && ht->slots != NULL)
    {
        size_t h = ht->key_hash(key, ht->capacity);
        while(ht->slots[h].key != NULL && ht->key_cmp(key, ht->slots[h].key) != 0 || ht->slots[h].tombstone)
        {
          h = (h + 1) % ht->capacity;
        }
        if(ht->slots[h].key != NULL)
        {
            ht->slots[h].key = NULL;
            ht->slots[h].value = NULL;
            ht->slots[h].tombstone = 1;
            ht->size -= 1;
            if(upo_ht_linprob_load_factor(ht) <= 0.125)
            {
              upo_ht_linprob_resize(ht, ht->capacity/2);
            }
        }
    }
}
