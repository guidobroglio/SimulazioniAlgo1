/* vim: set tabstop=4 expandtab shiftwidth=4 softtabstop=4: */

/******************************************************************************/
/*** NOME: Guido                                                            ***/
/*** COGNOME: Broglio                                                       ***/
/*** MATRICOLA: 20043973                                                    ***/
/*** DATA: Simulazione prova gennaio 2025                                   ***/
/******************************************************************************/


#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <upo/bst.h>
#include <upo/hashtable.h>

/**** BEGIN of EXERCISE #1 ****/
/**
                            8
                        /       \
                      3          10
                    /   \          \
                  1      6         14
                       /   \      /
                      4     7   13
Implementare un algoritmo che, dato un albero binario di ricerca (BST) e due chiavi k1 e k2, restituisca la chiave del
loro minimo comune antenato (Lowest Common Ancestor - LCA). L’LCA di due nodi n1 e n2 è il nodo più lontano
dalla radice del BST che ha sia n1 che n2 come discendenti (N.B. un nodo può essere discendente di se stesso).
Per esempio, dato l’albero di Figura 1, si ha che l’LCA di 1 e 7 è 3, l’LCA di 4 e 10 è 8,
l’LCA di 10 e 13 è 10 e l’LCA di 6 e 18 è NULL.
L’algoritmo implementato dev’essere ottimo: 1) deve visitare l’albero una sola volta, 2) non deve visitare parti del BST inutili
ai fini dell’esercizio e 3) la complessità temporale nel caso peggiore dev’essere O(n), dove n è il numero di chiavi nel BST.

La funzione da implementare si trova nel file exam.c e ha il seguente prototipo:
void *upo_bst_lca(const upo_bst_t tree, const void *key1, const void *key2)
Parametri:
• tree: BST.
• key1 e key2: puntatori alla prima e seconda chiave di cui si vuole trovare il minimo comune antenato.
Valore di ritorno:
• Se il BST non è vuoto e le chiavi key1 e key2 sono contenute nel BST: il puntatore alla chiave del loro LCA.
• Se il BST è vuoto o le chiavi key1 e/o key2 non sono contenute nel BST: NULL.
Il tipo upo_bst_t è dichiarato in include/upo/bst.h. Per confrontare il valore di due chiavi (qualora fosse necessario) si utilizzi
la funzione di comparazione memorizzata nel campo key_cmp del tipo upo_bst_t.
**/
//////////////////////////Inizio Prototipi//////////////////////////////////////
void *upo_bst_lca(const upo_bst_t tree, const void *key1, const void *key2);
int upo_bst_contains(upo_bst_t tree, const void*key);
upo_bst_node_t*upo_bst_lca_imp(upo_bst_node_t*node, void*key1, void*key2, upo_bst_comparator_t key_cmp);
///////////////////////////Fine Prototipi///////////////////////////////////////

int upo_bst_contains(upo_bst_t tree, const void*key)
{
    if(upo_bst_is_empty(tree) || key==NULL)
    {
        return 0;
    }
    upo_bst_node_t*node=tree->root;

    while(node!=NULL)
    {
        int compare=tree->key_cmp(key, node->key);
        if(compare==0)
        {
            return 1;
        }
        else if(compare<0)
        {
            node=node->left;
        }
        else
        {
            node=node->right;
        }
    }
    return 0;
}

upo_bst_node_t*upo_bst_lca_imp(upo_bst_node_t*node, void*key1, void*key2, upo_bst_comparator_t key_cmp)
{
    if(node==NULL)
    {
        return NULL;
    }
    int cmp1=key_cmp(key1, node->key);
    int cmp2=key_cmp(key2, node->key);

    if(cmp1<0 && cmp2<0)
    {
        return upo_bst_lca_imp(node->left, key1, key2, key_cmp);
    }
    else if(cmp1>0 && cmp2>0)
    {
        return upo_bst_lca_imp(node->right, key1, key2, key_cmp);
    }
    return node;
}

void *upo_bst_lca(const upo_bst_t tree, const void *key1, const void *key2)
{
    if(tree==NULL || tree->root==NULL || key1==NULL || key2==NULL)
    {
        return NULL;
    }

    if(upo_bst_contains(tree, key1)==0 || upo_bst_contains(tree, key2)==0)
    {
        return NULL;
    }

    upo_bst_node_t*node=upo_bst_lca_imp(tree->root, (void*)key1, (void*)key2, tree->key_cmp);
    if(node==NULL)
    {
        return NULL;
    }
    return node->key;
}
/**** END of EXERCISE #1 ****/



/**** BEGIN of EXERCISE #2 ****/
/**
Implementare un algoritmo che, data una tabella hash H, con gestione delle collisioni basata su indirizzamento aperto
(i.e. open addressing) e scansione lineare (i.e. linear probing) con uso di tombstone (HT-LP), e una
lista di chiavi lkeys, calcoli la media del numero di collisioni delle chiavi k contenute in lkeys in H. In particolare:
• Se una chiave k in lkeys non è contenuta in H, non dev’essere considerata nel calcolo della media del numero di collisioni.
• Se H è vuota, o se lkeys è vuota, o se nessuna chiave di lkeys è contenuta in H, l’algoritmo deve restituire il valore −1.
Si noti che nel calcolare il numero di collisioni di una chiave k non si deve tenere conto dello slot in cui k è
memorizzata e che uno slot tombstone, se attraversato, è considerato una collisione.
Per esempio, date la tabella e la lista di chiavi in Figura 2, e supponendo che il valore hash delle chiavi A,C, e E sia
rispettivamente 0, 0, e 10, il numero medio di collisioni è 3 in quanto:
• il numero di collisioni di A è 4;
• il numero di collisioni di C è 5;
• il numero di collisioni di E è 0;
• Z non è contenuta in H quindi non viene conteggiata nel calcolo della media.
L’algoritmo implementato deve essere ottimo, nel senso che non deve visitare parti di HT-LP inutili ai fini dell’esercizio
La funzione da implementare si trova nel file exam.c e ha il seguente prototipo:
double upo_ht_linprob_avg_collisions(const upo_ht_linprob_t ht, const upo_ht_key_list_t key_list)
Parametri:
• ht: HT-LP
• key_list: lista concatenata di chiavi.
I tipi upo_ht_linprob_t e upo_ht_key_list_t sono dichiarati in include/upo/hashtable.h.
Nella propria implementazione è possibile utilizzare tutte le funzioni dichiarate in include/upo/hashtable.h. Nel caso si
implementino nuove funzioni, i prototipi e le definizioni devono essere inserite nel file exam.c.
**/
double upo_ht_linprob_avg_collisions(const upo_ht_linprob_t ht, const upo_ht_key_list_t key_list) {
    // Se la tabella hash o la lista di chiavi è vuota, restituisci -1
    if (upo_ht_linprob_is_empty(ht) || key_list == NULL)
    {
        return -1;
    }

    upo_ht_key_list_node_t *current = key_list;
    size_t total_collisions = 0;  // Somma totale delle collisioni
    size_t keys_count = 0;        // Numero di chiavi trovate

    // Itera su tutte le chiavi nella lista
    while (current != NULL)
    {
        size_t hash_index = upo_ht_linprob_get_hasher(ht)(current->key, upo_ht_linprob_capacity(ht));
        size_t collisions = 0;
        int key_found = 0;

        // Scansione lineare nella tabella hash
        while (ht->slots[hash_index].key != NULL && collisions < upo_ht_linprob_capacity(ht) && key_found == 0)
        {
            if (upo_ht_linprob_get_comparator(ht)(ht->slots[hash_index].key, current->key) == 0 && ht->slots[hash_index].tombstone==0)
            {
                // Chiave trovata: aggiungi le collisioni e incrementa il contatore
                total_collisions += collisions;
                keys_count++;
                key_found = 1;
            }
            else
            {
                // Conta una collisione (inclusi i tombstone)
                collisions++;
            }
            hash_index = (hash_index + 1) % upo_ht_linprob_capacity(ht);  // Passa allo slot successivo
        }
        current = current->next;  // Passa alla chiave successiva
    }

    // Se nessuna chiave è stata trovata, restituisci -1
    if (keys_count == 0)
    {
        return -1;
    }

    // Calcola e restituisci la media delle collisioni
    return (double)total_collisions / keys_count;
}


/**** END of EXERCISE #2 ****/
