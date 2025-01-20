#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/upo/bst.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Esame01
/**
 * Implementare un algoritmo che ritorni il predecessore di una chiave in un albero binario
 * di ricerca (BST). Dati in input un BST e una chiave k, il predecessore di k nel BST è la più
 * grande chiave k0 contenuta nel BST tale che k0 < k.
 * Se il predecessore di k non esiste o se il BST è vuoto, l’algoritmo deve ritornare
 * il valore NULL. Dato un albero si ha:
 * Predecessore di 8: 7
 * Predecessore di 1: NULL (non esiste)
 * Predecessore di 5: 4
 * Predecessore di 14: 13
 * Predecessore di 0: NULL (non esiste)
 * Predecessore di 11: 10
 * Predecessore di 100: 14
 *
 * L’algoritmo implementato dev’essere ottimo, nel senso che deve visitare l’albero una sola volta
 * e non deve visitare parti del BST inutili ai fini dell’esercizio, e la complessità temporale
 * nel caso peggiore dev’essere O(n), dove n è il numero di chiavi nel BST.
 */

 //Funzione che restituisce il predecessore di un nodo in un albero binario di ricerca.
 static const void *upo_bst_predecessor_impl(const upo_bst_node_t *node, const void *key, upo_bst_comparator_t key_cmp);

 //Funzione che restituisce il predecessore di un nodo in un albero binario di ricerca.
 const void *upo_bst_predecessor(const upo_bst_t bst, const void *key)
 {
     //Se il predecessore non esiste, ritorna null.
     if (key == NULL)
     {
         return NULL;
     }

     // Se l'albero è vuoto, ritorna null.
     if (upo_bst_is_empty(bst))
     {
         return NULL;
     }

     // Chiama la funzione di supporto per trovare il predecessore.
     return upo_bst_predecessor_impl(bst->root, key, bst->key_cmp);
 }

 //Trova il predecessore di un nodo in un albero binario di ricerca.
static const void *upo_bst_predecessor_impl(const upo_bst_node_t *node, const void *key, upo_bst_comparator_t key_cmp)
 {
     // Se il nodo non è NULL
     if (node != NULL)
     {
         // Confronta la chiave del nodo con la chiave specificata
         int cmp = key_cmp(key, node->key);
         // Se la chiave è minore o uguale a 0, il predecessore è nel sottoalbero sinistro
         if (cmp <= 0)
         {
             //ritorna il predecessore del sottoalbero sinistro
             return upo_bst_predecessor_impl(node->left, key, key_cmp);
         }
         // Se la chiave è maggiore di 0, il predecessore è nel sottoalbero destro
         const void *pred_key = upo_bst_predecessor_impl(node->right, key, key_cmp);
         // Se il predecessore non è NULL, ritorna il predecessore
         if (pred_key != NULL)
         {
             //ritorna il predecessore
             return pred_key;
         }
         // Se la chiave del nodo è maggiore della chiave specificata, il predecessore è il nodo stesso
         else
         {
             //ritorna la chiave del nodo
             return node->key;
         }
     }
     // Se il nodo è NULL, ritorna NULL
     return NULL;
 }

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Esame02
/**
 * Implementare un algoritmo che ritorni il numero di nodi di un sottoalbero in un albero binario
 * di ricerca BST che si trovano a una profondità pari. Dato un BST e una chiave k,
 * il numer di nodi del sottoalbero radicato in k che si trovano a una profondità pari. Si
 * ottiene contando tutti i nodi che si trovano a prodontà pari a partire dal nodo radice del
 * sottoalbero radicato in k. Si noti che la radice dell'intero BST si trova a profondità 0.
 * Il conteggio dei nodi include anche la radice del sottoalbero se si trova a una profondità pari
 * Se la chiave k non è presente nell'albero, la funzione deve ritornare 0. così come se il sottoalbero
 * radicato in k è vuoto.
 *
 * Per esempio, dato il seguente BST:
 * conteggio nodi a prondoità pari nel sottoalbero radicato in 8: 4 (8, 1, 6, 14)
 * conteggio nodi a profondità pari nel sottoalbero radicato in 3: 2 (1, 6)
 * conteggio nodi a profondità pari nel sottoalbero radicato in 1: 1 (1)
 * conteggio nodi a profondità pari nel sottoalbero radicato in 4: 0 (non ci sono nodi a profondità pari)
 * Conteggio nodi a profondità pari nel sotto-albero radicato in 5: 0 (la chiave non esiste).
 *
 * L’algoritmo implementato dev’essere ottimo, nel senso che deve visitare l’albero una sola volta e non
 * deve visitare sotto-alberi inutili ai fini dell’esercizio, e la complessità temporale nel caso peggiore
 * dev’essere O(n), dove n è il numero di chiavi nel BST.
 *
 * Si implementi la funzione:
 * size_t upo_bst_subtree_count_even(const upo_bst_t bst, const void *key)
 * con parametri:
 * - bst: il BST su cui operare
 * - key: la chiave del nodo radice del sottoalbero di cui si vuole contare i nodi a profondità pari.
 * valori di ritorno:
 * - Se il BST non è vuoto e la chiave key è contenuta nel BST: numero intero rappresentante il
 *      numero di nodi a profondità pari nel sotto-albero radicato nel nodo avente come chiave il
 *      valore puntato da key
 * - Se il BST è vuoto o la chiave key non è contenuta nel BST o il sotto-albero radicato in
 *      key non ha nodi a profondità pari: il valore intero 0
 */

 /**
  * Trova il nodo nell'albero di ricerca binaria (BST) che corrisponde alla chiave fornita.
  *
  * @param node Il nodo radice del BST.
  * @param key La chiave da cercare.
  * @param cmp La funzione di comparazione utilizzata per confrontare le chiavi.
  * @param depth Un puntatore a una variabile che conterrà la profondità del nodo trovato.
  * @return Il nodo che corrisponde alla chiave, o NULL se nessuna corrispondenza viene trovata.
  */
const upo_bst_node_t* upo_bst_find_impl(const upo_bst_node_t* node, const void* key, upo_bst_comparator_t cmp, size_t* depth);

 /**
  * Conta il numero di nodi nel sottoalbero radicato nel nodo dato, dove la profondità del nodo è pari.
  *
  * @param node Il nodo radice del sottoalbero.
  * @param depth La profondità del nodo radice.
  * @return Il numero di nodi nel sottoalbero con profondità pari.
  */
 static size_t upo_bst_subtree_count_even_impl(const upo_bst_node_t* node, size_t depth);

 //Conta il numero di nodi nel BST con profondità pari a partire dal nodo che corrisponde alla chiave data.
 size_t upo_bst_subtree_count_even(const upo_bst_t bst, const void *key)
 {
     // Se l'albero è vuoto, restituisci 0
     if (upo_bst_is_empty(bst))
     {
         return 0;
     }
     // imposto la profondità iniziale a 0
     size_t depth = 0;
     //upo_bst_find_impl restituisce il nodo corrispondente alla chiave e incrementa la profondità
     const upo_bst_node_t* node = upo_bst_find_impl(bst->root, key, upo_bst_get_comparator(bst), &depth);
     // Se la chiave non è presente, restituisci 0
     if (node == NULL)
     {
         return 0;
     }
     // Altrimenti, calcola il numero di nodi a profondità pari nel sottoalbero radicato nel nodo trovato
     return upo_bst_subtree_count_even_impl(node, depth);
 }

 
//Trova il nodo nell'albero di ricerca binaria (BST) che corrisponde alla chiave fornita.
 const upo_bst_node_t* upo_bst_find_impl(const upo_bst_node_t* node, const void* key, upo_bst_comparator_t cmp, size_t* depth)
 {
     // Se il nodo è NULL, restituisci NULL
     if (node == NULL)
     {
         return NULL;
     }
     // Confronta la chiave del nodo con la chiave fornita
     int cmp_res = cmp(key, node->key);
     // Se le chiavi corrispondono, restituisci il nodo
     if (cmp_res == 0)
     {
         return node;
     }
     // Altrimenti, cerca ricorsivamente nel sottoalbero sinistro o destro
     if (cmp_res < 0)
     {
         // Se la chiave è minore, cerca nel sottoalbero sinistro
         *depth += 1;
         return upo_bst_find_impl(node->left, key, cmp, depth);
     }
     // Altrimenti, cerca nel sottoalbero destro
     *depth += 1;
     return upo_bst_find_impl(node->right, key, cmp, depth);
 }


//Conta il numero di nodi nel sottoalbero radicato nel nodo dato, dove la profondità del nodo è pari.
 size_t upo_bst_subtree_count_even_impl(const upo_bst_node_t* node, size_t depth)
 {
     // Se il nodo è NULL, restituisci 0
     if (node == NULL)
     {
         return 0;
     }
     // Se la profondità è pari, restituisci 1 + il numero di nodi nei sottoalberi sinistro e destro
     if (depth % 2 == 0)
     {
         return 1 + upo_bst_subtree_count_even_impl(node->left, depth + 1) + upo_bst_subtree_count_even_impl(node->right, depth + 1);
     }
     return upo_bst_subtree_count_even_impl(node->left, depth + 1) + upo_bst_subtree_count_even_impl(node->right, depth + 1);
 }

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Esame03
/*
Implementare un algoritmo che, dato un albero binario di ricerca (BST), una chiave k (non necessariamente contenuta nel BST) e
un intero n, restituisca:
•l’ n-esima chiave più piccola del sottoalbero la cui radice ha come chiave k, se esiste e se k è contenuta nel BST;
•NULL, se l’n-esima chiave più piccola non esiste, se k non è contenuta nel BST, o se il BST è vuoto.

Si noti che l’n-esima chiave più piccola è la chiave che si troverebbe nell’n-esima posizione se le chiavi fossero disposte in
ordine di grandezza. Per esempio, dato l’albero di Figura 1, si ha:
•n = 3 e k = 3: 4
•n = 2 e k = 6: 6
•n = 2 e k = 8: 3
•n = 4 e k = 14: NULL
•n = 3 e k = 17: NULL
L’algoritmo implementato dev’essere ottimo, nel senso che deve visitare l’albero una sola volta e non deve visitare sotto-alberi
inutili ai fini dell’esercizio, e la complessità temporale nel caso peggiore dev’essere O(n), dove n è il numero di chiavi nel BST

La funzione da implementare si trova nel file exam.c e ha il seguente prototipo:
void *upo_bst_nmin(const upo_bst_t tree, const void *key, const int n)
Parametri:
•tree: BST.
•key: puntatore alla chiave della radice del sottoalbero di BST in cui si vuole trovare l’n-esima chiave più piccola.
•n: intero che definisce n-esima chiave più piccola.
Valore di ritorno:
•Se il BST non è vuoto, la chiave key è contenuta nel BST e l’n-esima chiave più piccola esiste: il puntatore all’n-esima
chiave più piccola.
•Se il BST è vuoto o la chiave key non è contenuta nel BST o non esiste l’n-esima chiave più piccola: NULL.
Il tipo upo_bst_t è dichiarato in include/upo/bst.h. Per confrontare il valore di due chiavi (qualora fosse necessario) si utilizzi
la funzione di comparazione memorizzata nel campo key_cmp del tipo upo_bst_t, la quale ritorna un valore <, =, o >di zero se il
valore puntato dal primo argomento è minore, uguale o maggiore del valore puntato dal secondo argomento, rispettivamente.
Nella propria implementazione è possibile utilizzare tutte le funzioni dichiarate in include/upo/bst.h. Nel caso s’implementino
nuove funzioni, i prototipi e le definizioni devono essere inserite nel file exam.c.
*/

void* upo_bst_nmin(const upo_bst_t tree, const void *key, const int n);

//Metodo ricorsivo per trovare l'n-esima chiave più piccola
void* findNthSmallest(upo_bst_node_t *node, int n, int *count)
{
    // Se il nodo è NULL, restituisci NULL
    if (node == NULL)
    {
        return NULL;
    }
    // Visita il sottoalbero sinistro
    void *leftResult = findNthSmallest(node->left, n, count);
    // Se il risultato è diverso da NULL, restituiscilo
    if (leftResult != NULL)
    {
        return leftResult;
    }
    // Incrementa il contatore per il nodo corrente
    (*count)++;
    // Se il contatore raggiunge n, restituisci la chiave del nodo corrente
    if (*count == n)
    {
        return node->key;
    }
    // Visita il sottoalbero destro
    return findNthSmallest(node->right, n, count);
}

//Funzione per trovare l'n-esima chiave più piccola
void *upo_bst_nmin(const upo_bst_t tree, const void *key, const int n)
{
    // Se l'albero è vuoto, restituisci NULL
    if (tree == NULL || tree->root == NULL)
    {
        return NULL;
    }
    // Trova il nodo con chiave 'key' nel BST
    upo_bst_node_t *current = tree->root;
    // Scorri l'albero finché non trovi il nodo con chiave 'key'
    while (current != NULL && tree->key_cmp(key, current->key) != 0)
    {
        // Se la chiave è minore della chiave del nodo corrente, vai a sinistra
        if (tree->key_cmp(key, current->key) < 0)
        {
            current = current->left;
        }
        // Altrimenti, vai a destra
        else
        {
            current = current->right;
        }
    }
    // Se il nodo con chiave 'key' non è stato trovato, ritorna NULL
    if (current == NULL)
    {
        return NULL;
    }
    // Contatore per tenere traccia del numero di nodi visitati
    int count = 0;
    // Trova l'n-esima chiave più piccola nel sottoalbero radicato a 'current'
    return findNthSmallest(current, n, &count);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Esame04
/*
Implementare un algoritmo che, dato un albero binario di ricerca (BST) e una chiave k,
restituisca:
    - la più grande chiave del sottoalbero la cui radice è k;
    - NULL, se la chiave non è presente nell’albero o se il BST è vuoto.
Per esempio, dati l’albero in Figura 1, e una chiave:
    • k = 6 il risultato è 7;
    • k = 8 il risultato è 14;
    • k = 13 il risultato è 13;
    • k = 19 il risultato è NULL.
L’algoritmo implementato dev’essere ottimo, nel senso che deve visitare l’albero una sola
volta e la complessità temporale nel caso peggiore dev’essere O(n), dove n è il numero
di chiavi nel BST.

La funzione da implementare si trova nel file exam.c e ha il seguente prototipo:
void* upo_bst_subtree_max(const upo_bst_t tree, const void *key)

Parametri:
    - tree: BST
    - key: la radice del sottoalbero di cui trovare la chiave maggiore

Valore di ritorno
    - Se il BST non è vuoto e la chiave k è contenuta nell'albero, la chiave maggiore
      del sottoalbero la cui radice è k.
    - se il BST è vuoto o k non è contenuta nell'albero: NULL.

Il tipo upo_bst_t è dichiarato in include/upo/bst.h. Per confrontare il valore di due chiavi
(qualora fosse necessario) si utilizzi la funzione di comparazione memorizzata nel campo key_cmp del tipo
upo_bst_t, la quale ritorna un valore <, =, o >di zero se il valore puntato dal primo argomento è minore,
uguale o maggiore del valore puntato dal secondo argomento, rispettivamente.
Nella propria implementazione è possibile utilizzare tutte le funzioni dichiarate in include/upo/bst.h.
Nel caso s’implementino nuove funzioni, i prototipi e le definizioni devono essere presenti e inserite nel
file exam.c.
Il file test/bst_subtree_max.c contiene alcuni casi di test tramite cui è possibile verificare la correttezza
della propria implementazione.
*/

//Definizione della funzione upo_bst_subtree_max
void* upo_bst_subtree_max(const upo_bst_t tree, const void *key);

// Funzione per trovare il nodo con la chiave specificata
upo_bst_node_t* find_node(const upo_bst_t tree, const void *key)
{
  // Verifica se l'albero è vuoto
  upo_bst_node_t* current = tree->root;
  // Itera finché non trova la chiave o raggiunge una foglia
  while (current != NULL)
  {
    // Confronta la chiave attuale con la chiave cercata
    int cmp = tree->key_cmp(key, current->key);
    // Se la chiave è stata trovata, restituisci il nodo corrente
    if (cmp == 0)
    {
      return current;
    }
    // Se la chiave è minore della chiave corrente, vai a sinistra
    else if (cmp < 0)
    {
      current = current->left;
    }
    // Se la chiave è maggiore della chiave corrente, vai a destra
    else
    {
      current = current->right;
    }
  }
  // Chiave non trovata
  return NULL;
}

// Funzione per trovare la chiave massima del sottoalbero radicato in un dato nodo
void* find_subtree_max(upo_bst_node_t* node)
{
  // Se il nodo è NULL, restituisci NULL
  if (node == NULL)
  {
    return NULL;
  }
  // Vai a destra finché non raggiungi la foglia più a destra
  while (node->right != NULL)
  {
    node = node->right;
  }
  // Restituisci la chiave del nodo più a destra
  return node->key;
}

// Funzione principale per trovare la chiave massima del sottoalbero
void* upo_bst_subtree_max(const upo_bst_t tree, const void *key)
{
  // Verifica se l'albero è vuoto
  if (tree == NULL || tree->root == NULL)
  {
    return NULL;
  }
  // Trova il nodo con la chiave specificata
  upo_bst_node_t* node = find_node(tree, key);
  // Se la chiave non è stata trovata, restituisci NULL
  if (node == NULL)
  {
    return NULL;
  }
  // Restituisci la chiave massima del sottoalbero radicato nel nodo trovato
  return find_subtree_max(node);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Esame05
/*
Nella definizione classica di un albero binario di ricerca (BST) non possono esserci chiavi duplicate.
Si consideri la seguente estensione dei BST che permette di gestire chiavi duplicate. Ogni nodo del BST,
anziché contenere una chiave k e il suo valore associato v (come nella versione classica), contiene una
chiave k e una lista di valori [v1,v2,...,vnk ] associati alla chiave k (un nodo della lista per ogni valore).
In questo modo, quando si vuole inserire nel BST una coppia chiave-valore 〈k,v′〉, se il BST non contiene alcun
nodo associato alla chiave k, se ne crea uno nuovo, memorizzandovi la chiave k e la lista [v′] composta da un
solo nodo; invece, se nel BST c’è già un nodo associato alla chiave k con associata la seguente lista di valori
[v1,v2,...,vnk ], lo si aggiorna, aggiungendo il nuovo valore nella lista dei valori (per es., aggiungendo un
nuovo nodo in testa alla lista, ottenendo la nuova lista [v′,v1,v2,...,vnk ])
*/

// Dichiarazione della funzione upo_bst_add_list
upo_bst_value_list_node_t* upo_bst_add_list(upo_bst_value_list_node_t* list, void* value);

// Dichiarazione della funzione upo_bst_node_create
upo_bst_node_t *upo_bst_node_create(void *key, void *value);

// Dichiarazione della funzione upo_bst_insert
void upo_bst_insert(upo_bst_t bst, void *key, void *value);

// Dichiarazione della funzione upo_bst_insert_impl
upo_bst_node_t *upo_bst_insert_impl(upo_bst_node_t *n, void *key, void *value, upo_bst_comparator_t comp);

// Dichiarazione della funzione upo_bst_node_create
upo_bst_node_t *upo_bst_node_create(void *key, void *value)
{
    // Allocazione della memoria per il nodo
    upo_bst_node_t *node = calloc(1, sizeof(upo_bst_node_t));
    // Controllo se l'allocazione è andata a buon fine
    if (node == NULL)
    {
        // Gestione dell'errore di allocazione
        return NULL;
    }
    // Inizializzazione dei campi del nodo
    node->key = key;
    // Aggiunta del valore alla lista
    node->values = upo_bst_add_list(NULL, value);
    // Inizializzazione dei figli del nodo
    if (node->values == NULL)
    {
        // Gestione dell'errore di allocazione
        free(node);
        return NULL;
    }
    // Inizializzazione dei figli del nodo
    node->left = NULL;
    node->right = NULL;
    // Ritorno del nodo
    return node;
}

// Aggiunge un valore alla lista
upo_bst_value_list_node_t* upo_bst_add_list(upo_bst_value_list_node_t* list, void* value)
{
    // Allocazione della memoria per il nodo della lista
    upo_bst_value_list_node_t *nodeList = calloc(1, sizeof(upo_bst_value_list_node_t));
    // Controllo se l'allocazione è andata a buon fine
    if (nodeList == NULL)
    {
        // Gestione dell'errore di allocazione
        return NULL;
    }
    // Inizializzazione dei campi del nodo della lista
    nodeList->value = value;
    // Aggiunta del nodo alla lista
    nodeList->next = list;
    // Ritorno della lista
    return nodeList;
}

//Dichiarazione del metodo responsabile dell'inserimento di un nodo all'interno dell'albero
void upo_bst_insert(upo_bst_t bst, void *key, void *value)
{
    //Controllo se l'albero è vuoto
    if(bst == NULL)
    {
        //Se l'albero è vuoto, esco dal metodo
        return;
    }
    //Se l'albero non è vuoto, chiamo il metodo per l'inserimento del nodo
    bst->root = upo_bst_insert_impl(bst->root, key, value, bst->key_cmp);
}

//Implementazione del metodo responsabile dell'inserimento di un nodo all'interno dell'albero
upo_bst_node_t *upo_bst_insert_impl(upo_bst_node_t *n, void *key, void *value, upo_bst_comparator_t comp)
{
    //Controllo se il nodo è vuoto
    if(n == NULL)
    {
        //Se il nodo è vuoto, creo un nuovo nodo
        return upo_bst_node_create(key, value);
    }
    //Altrimenti, Controllo se la chiave è minore della chiave del nodo
    else if(comp(key, n->key) < 0)
    {
        //Se la chiave è minore della chiave del nodo, chiamo il metodo ricorsivamente sul figlio sinistro
        n->left = upo_bst_insert_impl(n->left, key, value, comp);
    }
    //Altrimenti, Controllo se la chiave è maggiore della chiave del nodo
    else if (comp(key, n->key) > 0)
    {
        //Se la chiave è maggiore della chiave del nodo, chiamo il metodo ricorsivamente sul figlio destro
        n->right = upo_bst_insert_impl(n->right, key, value, comp);
    }
    //Altrimenti, la chiave è uguale alla chiave del nodo
    else
    {
        //Aggiungo il valore alla lista
        n->values = upo_bst_add_list(n->values, value);
    }
    //Ritorno il nodo
    return n;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Esame06
/*
Implementare un algoritmo che, dato un albero binario di ricerca (BST), dica se il BST non è vuoto.
Un BST non è vuoto se contiene almeno un nodo. Se il BST non è vuoto, l’algoritmo deve ritornare un valore diverso da 0,
altrimenti deve ritornare 0.
L’algoritmo implementato deve essere ottimo, nel senso che non deve visitare parti del BST inutili ai fini dell’esercizio, e la
complessità temporale nel caso peggiore dev’essere O(1).

La funzione da implementare si trova nel file exam.c e ha il seguente prototipo:
int upo_bst_is_not_empty(const upo_bst_t bst);
Parametri:
•bst: BST.
Valore di ritorno:
•Se il BST non è vuoto: valore diverso da 0.
•Se il BST è vuoto: valore 0.
Il tipo upo_bst_t è dichiarato in include/upo/bst.h. Per confrontare il valore di due chiavi del BST si utilizzi la funzione di
comparazione memorizzata nel campo key_cmp del tipo upo_bst_t, la quale ritorna un valore <, =, o >di zero se il valore puntato
dal primo argomento è minore, uguale o maggiore del valore puntato dal secondo argomento, rispettivamente.
Nella propria implementazione è possibile utilizzare tutte le funzioni dichiarate in include/upo/bst.h. Nel caso s’implementino
nuove funzioni, i prototipi e le definizioni devono essere inserite nel file exam.c
*/

//Definizione della funzione upo_bst_is_not_empty
int upo_bst_is_not_empty(const upo_bst_t bst)
{
    // Se il BST è vuoto, restituisci 0
    if (bst == NULL || bst->root == NULL)
    {
        return 0;
    }
    // Altrimenti, restituisci un valore diverso da 0
    return 1;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Esame07
/*
Dati in input un albero binario di ricerca (BST) e una chiave k, implementare un algoritmo che restituisca
il numero di nodi interni del sotto-albero radicato in k. Si noti che:
• per nodo interno s’intende un nodo che ha almeno un figlio;
• il conteggio dei nodi può includere anche la radice del sotto-albero (cioè, il nodo contenente la chiave k)
se essa è un nodo interno. Se la chiave k non è presente nel BST o se il BST è vuoto o se il sotto-albero
non contiene nodi interni, l’algoritmo deve ritornare il valore 0.
*/

//Metodo responsabile del conteggio dei nodi interni
size_t upo_bst_subtree_count_inner(const upo_bst_t bst, const void *key);

//Metodo responsabile dell'implementazione del conteggio dei nodi interni
void upo_bst_subtree_count_inner_impl(upo_bst_node_t *n, void *key, size_t *inners);

//Implementazione del metodo responsabile del conteggio dei nodi interni
size_t upo_bst_subtree_count_inner(const upo_bst_t bst, const void *key)
{
    //Controllo se l'albero è vuoto
    if(bst == NULL || bst->root == NULL)
    {
        //Se l'albero è vuoto, ritorno 0
        return 0;
    }
    //Se l'albero non è vuoto, inizializzo la variabile che conterrà il risultato
    size_t res = 0;
    //Chiamo il metodo responsabile del conteggio dei nodi interni
    upo_bst_subtree_count_inner_impl(bst->root, key, &res);
    //Ritorno il risultato
    return res;
}

//Implementazione del metodo responsabile dell'implementazione del conteggio dei nodi interni
void upo_bst_subtree_count_inner_impl(upo_bst_node_t *n, void *key, size_t *inners)
{
    //Controllo se il nodo è vuoto
    if(n == NULL)
    {
        //Se il nodo è vuoto, esco dal metodo
        return;
    }
    //Se il nodo non è vuoto, controllo se il nodo è un nodo interno
    if((n->left != NULL || n->right != NULL) && strcmp(n->key, key) == 0)
    {
        //se il nodo è un nodo interno e la chiave corrisponde, incremento il contatore
        (*inners) += 1;
    }
    //Chiamo il metodo ricorsivamente sui figli del nodo
    upo_bst_subtree_count_inner_impl(n->left, key, inners);
    upo_bst_subtree_count_inner_impl(n->right, key, inners);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Esame08
/*
Dati in input un albero binario di ricerca BST e una chiave k, implementare un algoritmo che dica se la
chiave k e' contenuta nel BST e che calcoli la profondita' del nodo che contiene k. In particolare:
    - se la chiave k e' contenuta nel BST, l'algoritmo deve ritornare True (valore intero diverso da zero) e
      deve calcolare la profondità del nodo in cui è memorizzata la chiave k.
    - se la chiave k non e' contenuta nel BST o se il BST e' vuoto, l'algoritmo deve ritornare False
      (il valore intero 0) e come profondita' deve ritornare il valore intero -1.
*/

//Metodo responsabile della ricerca della chiave e del calcolo della profondità
int upo_bst_contains_depth(const upo_bst_t bst, void *key, long *depth);

//Metodo responsabile dell'implementazione della ricerca della chiave e del calcolo della profondità
int upo_bst_contains_depth_impl(upo_bst_node_t *node, void *key, long *depth, upo_bst_comparator_t comp);

//Implementazione del metodo responsabile della ricerca della chiave e del calcolo della profondità
int upo_bst_contains_depth(const upo_bst_t bst, void *key, long *depth)
{
    //Controllo se l'albero è vuoto
    if(bst == NULL)
    {
        //Se l'albero è vuoto, ritorno -1
        return -1;
    }
    //Altrimenti, inizializzo la variabile che conterrà il risultato
    *depth = 0;
    //Chiamo il metodo responsabile della ricerca della chiave e del calcolo della profondità
    return upo_bst_contains_depth_impl(bst->root, key, depth, bst->key_cmp);
}

//Implementazione del metodo responsabile della ricerca della chiave e del calcolo della profondità
int upo_bst_contains_depth_impl(upo_bst_node_t *node, void *key, long *depth, upo_bst_comparator_t comp)
{
    //Controllo se il nodo è vuoto
    if(node == NULL)
    {
        //Se il nodo è vuoto, ritorno -1
        return -1;
    }
    //Controllo se la chiave corrisponde
    if(comp(node->key, key) < 0)
    {
        //Se la chiave corrisponde, incremento la profondità
        *depth += 1;
        //Chiamo il metodo ricorsivamente sul figlio sinistro
        return upo_bst_contains_depth_impl(node->left, key, depth, comp);
    }
    //Altrimenti, controllo se la chiave è maggiore della chiave del nodo
    else if(comp(node->key, key) > 0)
    {
        //Se la chiave è maggiore della chiave del nodo, incremento la profondità
        *depth += 1;
        //Chiamo il metodo ricorsivamente sul figlio destro
        return upo_bst_contains_depth_impl(node->right, key, depth, comp);
    }
    //Altrimenti, la chiave corrisponde
    else
    {
        //Incremento la profondità
        depth += 1;
        //Ritorno 1
        return 1;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
