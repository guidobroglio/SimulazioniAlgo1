/* vim: set tabstop=4 expandtab shiftwidth=4 softtabstop=4: */


/******************************************************************************/
/*** NOME: Guido Lorenzo                                                    ***/
/*** COGNOME: Broglio                                                       ***/
/*** MATRICOLA: 20043973                                                    ***/
/*** DATA: Prova 03                                                         ***/
/******************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<stddef.h>
#include<assert.h>
#include "include/upo/bst.h"
#include "include/upo/hashtable.h"
#include "include/upo/test.h"

/**** BEGIN of EXERCISE #1 ****/
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

/**** END of EXERCISE #1 ****/

/**** BEGIN of EXERCISE #2 ****/
/*
Implementare un algoritmo che, data una tabella hash con gestione delle collisioni basata su concatenazioni separate (HT-SC),
dica se la HT-SC non è vuota.
Una HT-SC non è vuota se contiene almeno una chiave. Se la HT-SC non è vuota, l’algoritmo deve ritornare un valore diverso
da 0, altrimenti deve ritornare 0.
L’algoritmo implementato deve essere ottimo, nel senso che non deve visitare parti della HT-SC inutili ai fini dell’esercizio, e
la complessità temporale nel caso peggiore dev’essere O(1).

La funzione da implementare si trova nel file exam.c e ha il seguente prototipo:
int upo_ht_sepchain_is_not_empty(const upo_ht_sepchain_t ht);
Parametri:
•ht: HT-SC.
Valore di ritorno:
•Se la HT-SC non è vuota: valore diverso da 0.
•Se la HT-SC è vuota: valore 0.
Il tipo upo_ht_sepchain_t è dichiarato in include/upo/hashtable.h. Per confrontare il valore di due chiavi si utilizzi la funzione
di comparazione memorizzata nel campo key_cmp del tipo upo_ht_sepchain_t, la quale ritorna un valore <, =, o >di zero se il
valore puntato dal primo argomento è minore, uguale o maggiore del valore puntato dal secondo argomento, rispettivamente. Per
calcolare il valore hash di una chiave si utilizzi la funzione di hash memorizzata nel campo key_hash del tipo upo_ht_sepchain_t,
la quale richiede come parametri il puntatore alla chiave di cui si vuole calcolare il valore hash e la capacità totale della HT-SC
(memorizzata nel campo capacity del tipo upo_ht_sepchain_t). Per tenere traccia della dimensione della HT-SC, si utilizzi il campo
size del tipo upo_ht_sepchain_t. Infine, gli slot della HT-SC sono memorizzati nel campo slots del tipo upo_ht_sepchain_t, che è
una sequenza di slot, ciascuno dei quali di tipo upo_ht_sepchain_slot_t e contenente il puntatore alla propria lista delle collisioni.
Nella propria implementazione è possibile utilizzare tutte le funzioni dichiarate in include/upo/hashtable.h. Nel caso si
implementino nuove funzioni, i prototipi e le definizioni devono essere inserite nel file exam.c.
*/

//Definizione della funzione upo_ht_sepchain_is_not_empty
int upo_ht_sepchain_is_not_empty(const upo_ht_sepchain_t ht)
{
    // Se la HT-SC è vuota, restituisci 0
    if (ht == NULL || ht->slots == NULL || ht->size == 0)
    {
        return 0;
    }
    // Altrimenti, restituisci un valore diverso da 0
    return 1;
}

/**** END of EXERCISE #2 ****/
