#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/upo/hashtable.h"
#include "include/upo/bst.h"
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Esame02
/**
 * Si consideri una tabella hash H con gestione delle collisioni basata su concatenazioni separate
 * (HT-SC) in cui le liste delle collisioni sono ordinate secondo il valore della chiave.
 * Implementare un algoritmo che realizzi l'operazione "odelete" la quale, data una HT-SC H,
 * una chiave k e un flag destroy_data, elimina l'elemento con chiave k dalla tabella hash H,
 * mantenendo l'ordinamento delle liste delle collisioni. Nel caso in cui destroy_data sia
 * true, la funzione deve deallocare la memoria occupata dalla chiave e dal valore associato
 *
 * Se la chiave k è contenuta in H, l'algoritmo deve rimuovere la coppia chiave-valore associata.
 * in maniera tale che la lista delle collisioni rimanga ordinata. Se la chiave k non è presente
 * nella tabella hash, la funzione non deve fare nulla.
 * Una volta rimosso l'elemento, bisogna aggiornare la dimensione di H.
 *
 * Esempio:
 * - Cancellazione di A →la coppia chiave-valore identificata da questa chiave viene rimossa,
 *      preservando l’ordine dei nodi nella lista delle collisioni associata; in particolare, dopo
 *      la cancellazione, la lista conterrà le seguenti chiavi: [P].
 * - Cancellazione di M →la coppia chiave-valore identificata da questa chiave viene rimossa,
 *      preservando l’ordine dei nodi nella lista delle collisioni associata; in particolare,
 *      dopo la cancellazione, la lista conterrà le seguenti chiavi: [C,H,R].
 * - Cancellazione di B →non si effettua alcuna cancellazione in quanto non esiste una
 *      coppia-chiave valore identificata da questa chiave.
 * - Cancellazione di Y →non si effettua alcuna cancellazione in quanto non esiste una
 *      coppia-chiave valore identificata da questa chiave
 *
 * L’algoritmo implementato dev’essere ottimo, nel senso che deve visitare la HT-SC una sola volta e
 * non deve visitare parti della HT-SC inutili ai fini dell’esercizio, e la complessità temporale
 * nel caso medio dev’essere Θ(1)
 *
 * Si implementi la funzione:
 * void upo_ht_sepchain_odelete(upo_ht_sepchain_t ht, const void *key, int destroy_data)
 * con parametri:
 * - ht: la tabella hash su cui operare
 * - key: la chiave dell'elemento da eliminare
 * - destroy_data: flag che indica se deallocare o meno la memoria occupata dalla chiave e dal
 *                  valore associato all'elemento da eliminare (1 per deallocare, 0 altrimenti).
 * valori di ritorno:
 * - la funzione è void e non ritorna alcun valore
 *
 * La deallocazione della memoria deve essere effettuata tramite la funzione free() della
 * libreria standard del C.
 * Il tipo upo_ht_sepchain_t è dichiarato in include/upo/hashtable.h. Per confrontare il valore di
 * due chiavi si utilizzi la funzione di comparazione memorizzata nel campo key_cmp del tipo
 *  upo_ht_sepchain_t, la quale ritorna un valore <, =, o >di zero se il valore puntato dal primo
 * argomento è minore, uguale o maggiore del valore puntato dal secondo argomento, rispettivamente.
 * Per calcolare il valore hash di una chiave si utilizzi la funzione di hash memorizzata nel
 * campo key_hash del tipo upo_ht_sepchain_t, la quale richiede come parametri il puntatore
 * alla chiave di cui si vuole calcolare il valore hash e la capacità totale della HT-SC
 * (memorizzata nel campo capacity del tipo upo_ht_sepchain_t). Per tenere traccia della dimensione
 * della HT-SC, si utilizzi il campo size del tipo upo_ht_sepchain_t. Infine, gli slot della HT-SC
 * sono memorizzati nel campo slots del tipo upo_ht_sepchain_t, che è una sequenza di slot,
 * ciascuno dei quali di tipo upo_ht_sepchain_slot_t e contenente il puntatore alla propria
 * lista delle collisioni
 */

 
// Rimuove un elemento dalla tabella hash a catena separata.
void upo_ht_sepchain_odelete(upo_ht_sepchain_t ht, const void *key, int destroy_data)
 {
     // Verifica che la tabella hash non sia vuota
     if (ht != NULL && ht->slots != NULL)
     {
         // Calcola l'hash della chiave
         size_t h = ht->key_hash(key, ht->capacity);
         // Cerca la chiave nella lista delle collisioni
         upo_ht_sepchain_list_node_t *prev_node = NULL;
         // scorro la lista delle collisioni
         upo_ht_sepchain_list_node_t *node = ht->slots[h].head;
         // il ciclo termina quando il nodo è NULL o la chiave è minore o uguale a quella del nodo
         while (node != NULL && ht->key_cmp(key, node->key) > 0)
         {
             // aggiorno il nodo precedente
             prev_node = node;
             // scorro il nodo successivo
             node = node->next;
         }
         // Se la chiave è stata trovata, rimuovi l'elemento
         if (node != NULL && ht->key_cmp(key, node->key) == 0)
         {
             // Se il nodo precedente è NULL, allora il nodo da eliminare è in testa alla lista
             if (prev_node == NULL)
             {
                 // Eliminazione all'inizio della lista
                 ht->slots[h].head = node->next;
             }
             else
             {
                 // Eliminazione nel mezzo / alla fine della lista
                 prev_node->next = node->next;
             }
             // Se destroy_data è true, dealloca la memoria occupata dalla chiave e dal valore
             if (destroy_data)
             {
                 // dealloca la chiave e il valore
                 free(node->key);
                 // se il valore è diverso da NULL
                 free(node->value);
             }
             // dealloca il nodo
             free(node);
             // Aggiorna la dimensione della tabella hash
             ht->size -= 1;
         }
         // altrimenti: chiave non trovata
         return;
     }
 }

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Esame03
/*Implementare un algoritmo che, data una tabella hash H, con gestione delle collisioni basata su indirizzamento aperto (i.e. open
addressing) e scansione lineare (i.e. linear probing) con uso di tombstone (HT-LP), e una lista di chiavi lkeys, calcoli la media del
numero di collisioni delle chiavi k contenute in lkeys in H. In particolare:
•Se una chiave k in lkeys non è contenuta in H, non dev’essere considerata nel calcolo della media del numero di collisioni.
•Se H è vuota, o se lkeys è vuota, o se nessuna chiave di lkeys è contenuta in H, l’algoritmo deve restituire il valore −1.
Si noti che nel calcolare il numero di collisioni di una chiave k non si deve tenere conto dello slot in cui k è memorizzata e che
uno slot tombstone, se attraversato, è considerato una collisione.
Per esempio, date la tabella e la lista di chiavi in Figura 2, e supponendo che il valore hash delle chiavi A,C, e E sia
rispettivamente 0,0, e 10, il numero medio di collisioni è 3 in quanto:
•il numero di collisioni di A è 4;
•il numero di collisioni di C è 5;
•il numero di collisioni di E è 0;
•Z non è contenuta in H quindi non viene conteggiata nel calcolo della media.
L’algoritmo implementato deve essere ottimo, nel senso che non deve visitare parti di HT-LP inutili ai fini dell’esercizio.

La funzione da implementare si trova nel file exam.c e ha il seguente prototipo:
double upo_ht_linprob_avg_collisions(const upo_ht_linprob_t ht, const upo_ht_key_list_t key_list)
Parametri:
•ht: HT-LP
•key_list: lista concatenata di chiavi.
I tipi upo_ht_linprob_t e upo_ht_key_list_t sono dichiarati in include/upo/hashtable.h. Le chiavi di cui calcolare la media
del numero di collisioni sono memorizzate nel campo key del tipo upo_ht_key_list_t. Per scorrere la lista di chiavi key_list si
utilizzi il puntatore memorizzato nel campo next del tipo upo_ht_key_list_t. Per confrontare il valore di due chiavi si utilizzi la
funzione di comparazione memorizzata nel campo key_cmp del tipo upo_ht_linprob_t, la quale ritorna un valore <, =, o >di zero
se il valore puntato dal primo argomento è minore, uguale o maggiore del valore puntato dal secondo argomento, rispettivamente.
Per calcolare il valore hash di una chiave si utilizzi la funzione di hash memorizzata nel campo key_hash del tipo upo_ht_linprob_t,
la quale richiede come parametri il puntatore alla chiave di cui si vuole calcolare il valore hash e la capacità totale della HT-LP
(memorizzata nel campo capacity del tipo upo_ht_linprob_t). Infine, gli slot della HT-LP sono memorizzati nel campo slots del
tipo upo_ht_linprob_t, che è una sequenza di slot, ciascuno dei quali di tipo upo_ht_linprob_slot_t.
Nella propria implementazione è possibile utilizzare tutte le funzioni dichiarate in include/upo/hashtable.h. Nel caso si
implementino nuove funzioni, i prototipi e le definizioni devono essere inserite nel file exam.c
*/

// Funzione per distruggere un nodo di una lista di chiavi
void upo_ht_key_list_destroy(upo_ht_key_list_t key_list);

// Funzione per calcolare la media del numero di collisioni delle chiavi contenute in una lista di chiavi
double upo_ht_linprob_avg_collisions(const upo_ht_linprob_t ht, const upo_ht_key_list_t key_list);

// Funzione per distruggere un nodo di una lista di chiavi
void upo_ht_key_list_destroy(upo_ht_key_list_t key_list)
{
    // Scorri la lista di chiavi e dealloca i nodi
    upo_ht_key_list_node_t *current = key_list;
    // Quando la lista è vuota, dealloca il nodo corrente
    while (current != NULL)
    {
        // Salva il puntatore al prossimo nodo
        upo_ht_key_list_node_t *next = current->next;
        // Dealloca il nodo corrente
        free(current);
        // Passa al prossimo nodo
        current = next;
    }
}

// Funzione per calcolare la media del numero di collisioni delle chiavi contenute in una lista di chiavi
double upo_ht_linprob_avg_collisions(const upo_ht_linprob_t ht, const upo_ht_key_list_t key_list)
{
    // Verifica se la tabella hash è vuota o se la lista di chiavi è vuota
    if (upo_ht_linprob_is_empty(ht) || key_list == NULL)
    {
        // Se la tabella hash è vuota o la lista di chiavi è vuota, restituisci -1
        return -1;
    }
    // Inizializza il numero totale di collisioni e il numero di chiavi
    size_t total_collisions = 0;
    size_t keys_count = 0;

    // Scorri la lista di chiavi
    upo_ht_key_list_node_t *current = key_list;

    //Quando la lista non è vuota
    while (current != NULL)
    {
        // Calcola l'hash della chiave corrente
        size_t hash = upo_ht_linprob_get_hasher(ht)(current->key, upo_ht_linprob_capacity(ht));

        // Utilizza linear probing per trovare il primo slot libero o la chiave
        size_t collisions = 0;
        //Quando la chiave non è NULL e il numero di collisioni è minore della capacità della tabella
        while (ht->slots[hash].key != NULL && collisions < upo_ht_linprob_capacity(ht))
        {
            // Se la chiave corrente è uguale alla chiave nel nodo corrente
            if (upo_ht_linprob_get_comparator(ht)(ht->slots[hash].key, current->key) == 0)
            {
                // Aggiorna il numero totale di collisioni e il numero di chiavi
                total_collisions += collisions;
                // Incrementa il numero di chiavi
                keys_count++;
                // Interrompi il ciclo
                break;
            }
            // Altrimenti, passa al prossimo slot
            hash = (hash + 1) % upo_ht_linprob_capacity(ht);
            // Incrementa il numero di collisioni
            collisions++;
        }
        //Altrimenti passa al prossimo nodo
        current = current->next;
    }
    // Se nessuna chiave è stata trovata, restituisci -1
    if (keys_count == 0)
    {
        return -1;
    }
    // Restituisci la media del numero di collisioni
    return (double)total_collisions / keys_count;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Esame04
/*
Implementare un algoritmo che, data una tabella hash H con gestione delle collisioni basata su concatenazioni
separate (HT-SC - separate chaining) e una chiave k, calcoli la percentuale di elementi della tabella che
collidono con k (Nota: k non è inclusa nel calcolo della percentuale). In particolare:
    • se una chiave k è presente in H, l’algoritmo calcola la percentuale di elementi della tabella che collidono
      con k.
    • se una chiave k non è contenuta in H, o se H è vuota l’algoritmo restituisce -1.
      Data la tabella hash in Figura 2 e una chiave:
    • k = E, il risultato è: 20%;
    • k = B, il risultato è: 10%;
    • k = L, il risultato è: 30%;
    • k = Z, il risultato è: −1.
L’algoritmo implementato deve essere ottimo, nel senso che deve visitare le HT-SC una sola volta e non deve
visitare parti delle HT-SC inutili ai fini dell’esercizio.

La funzione da implementare si trova nel file exam.c e ha il seguente prototipo:
float upo_ht_sepchain_perc_collisions(const upo_ht_sepchain_t ht, const void *key)
Parametri:
    • ht: Tabella Hash.
    • key: chiave.
Il tipo upo_ht_sepchain_t è dichiarato in include/upo/hashtable.h. Per confrontare il valore di due chiavi si
utilizzi la funzione di comparazione memorizzata nel campo key_cmp del tipo upo_ht_sepchain_t, la quale
ritorna un valore <, =, o >di zero se il valore puntato dal primo argomento è minore, uguale o maggiore del
valore puntato dal secondo argomento, rispettivamente. Per calcolare il valore hash di una chiave si utilizzi
la funzione di hash memorizzata nel campo key_hash del tipo upo_ht_sepchain_t, la quale richiede come
parametri il puntatore alla chiave di cui si vuole calcolare il valore hash e la capacità totale della HT-SC
(memorizzata nel campo capacity del tipo upo_ht_sepchain_t). Infine, gli slot della HT-SC sono memorizzati nel
campo slots del tipo upo_ht_sepchain_t, che è una sequenza di slot, ciascuno dei quali di tipo
upo_ht_sepchain_slot_t e contenente il puntatore alla propria lista delle collisioni.
Nella propria implementazione è possibile utilizzare tutte le funzioni dichiarate in include/upo/hashtable.h.
Nel caso si implementino nuove funzioni, i prototipi e le definizioni devono essere presenti e inserite nel file
exam.c.
Il file test/ht_sepchain_perc_collisions.c contiene alcuni casi di test tramite cui è possibile verificare
la correttezza della propria implementazione.
*/

//Definizione della funzione upo_ht_sepchain_perc_collisions
float upo_ht_sepchain_perc_collisions(const upo_ht_sepchain_t ht, const void *key);

//Definizione della funzione upo_ht_sepchain_perc_collisions
float upo_ht_sepchain_perc_collisions(const upo_ht_sepchain_t ht, const void *key)
{
    // Verifica se la tabella hash è vuota o NULL
    if (ht == NULL || ht->size == 0)
    {
        return -1;
    }
    // Calcola l'indice hash della chiave
    size_t hash_index = ht->key_hash(key, ht->capacity);
    // Puntatore al bucket (lista di collisioni) corrispondente
    upo_ht_sepchain_list_node_t *current = ht->slots[hash_index].head;
    //Flag per indicare se la chiave è stata trovata
    int key_found = 0;
    //Contatore delle collisioni
    int collision_count = 0;
    // Itera attraverso la lista nel bucket
    while (current != NULL)
    {
        // Confronta la chiave attuale con la chiave cercata
        if (ht->key_cmp(current->key, key) == 0)
        {
          // Chiave trovata
          key_found = 1;
        }
        // Se la chiave attuale non è uguale alla chiave cercata
        else
        {
          // Incrementa il contatore delle collisioni
          collision_count++;
        }
        // Passa al nodo successivo
        current = current->next;
    }
    // Se la chiave non è stata trovata, restituisci -1
    if (!key_found)
    {
        return -1;
    }
    // Calcola la percentuale di elementi che collidono con la chiave `k`
    // La formula è: (collision_count / (totale_elementi - 1)) * 100
    float percentage = ((float)collision_count / (ht->size - 1)) * 100.0f;
    // Arrotonda la percentuale a 6 cifre decimali
    percentage = roundf(percentage * 1000000) / 1000000;
    return percentage;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Esame05
/*
implementare un algoritmo che, date due tabelle hash dest_ht e src_ht con gestione delle collisioni basata su concatenazioni
separate (HT-SC), effettui il “merge” (cioè, la fusione) di src_ht in dest_ht.
Come mostrato nell’esempio di Figura 1, il “merge” di src_ht in dest_ht consiste nell’inserire tutte le chiavi di src_ht in dest_ht.
Si noti che:
•La HT-SC src_ht non deve essere modificata.
•La HT-SC dest_ht non è necessariamente vuota. In particolare, dopo il “merge” tutte le chiavi (e i valori) precedentemente
presenti in dest_ht, devono rimanere inalterati (per es., nella figura, si vedano le chiavi “A”, “B”, e “X” – e i valori associati –
di dest_ht). Inoltre, se in dest_ht c’è già una chiave contenuta anche in src_ht, quella chiave (e il valore associato) deve
rimanere inalterato in dest_ht (per es., nella figura, si vedano le chiavi “A” e “X” – e i valori associati – di dest_ht).
•Le due HT-SC possono avere capacità differenti (per es., nella figura, src_ht ha 5 slot, mentre dest_ht ha 3 slot).
•Le due HT-SC possono usare funzioni hash differenti.
•Si assume che i tipi delle chiavi e dei valori delle due HT-SC siano uguali.
Si noti che, nell’esempio, la scelta di effettuare l’inserimento di un nuovo nodo nella lista delle collisioni in coda alla lista è del
tutto arbitraria; è possibile utilizzare una strategia differente.
L’algoritmo implementato deve essere ottimo, nel senso che non deve visitare parti della HT-SC inutili ai fini dell’esercizio

La funzione da implementare si trova nel file exam.c e ha il seguente prototipo:
void upo_ht_sepchain_merge(upo_ht_sepchain_t dest_ht, const upo_ht_sepchain_t src_ht);
Parametri:
•dest_ht: HT-SC di destinazione.
•src_ht: HT-SC di origine.
Valore di ritorno: nessuno.
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

//Dichiarazione del metodo responsabile dell'inserimento di un nodo all'interno della tabella hash
void upo_ht_sepchain_insert(upo_ht_sepchain_t ht, void *key, void *value);

//Dichiarazione del metodo responsabile della fusione di due tabelle hash
void upo_ht_sepchain_merge(upo_ht_sepchain_t dest_ht, const upo_ht_sepchain_t src_ht)
{
    // Controllo se le tabelle hash sono vuote
    if (src_ht != NULL && dest_ht != NULL)
    {
        // Itero su tutti gli slot della tabella hash sorgente
        for (size_t i = 0; i < src_ht->capacity; i++)
        {
            // Ottengo il nodo della lista di collisioni
            upo_ht_sepchain_list_node_t *node = src_ht->slots[i].head;
            // Itero sui nodi della lista di collisioni
            while (node != NULL)
            {
                // Inserisco il nodo nella tabella hash di destinazione
                upo_ht_sepchain_insert(dest_ht, node->key, node->value);
                // Passo al nodo successivo
                node = node->next;
            }
        }
    }
}

//Implementazione del metodo responsabile dell'inserimento di un nodo all'interno della tabella hash
void upo_ht_sepchain_insert(upo_ht_sepchain_t ht, void *key, void *value)
{
    //Controllo se la tabella hash è vuota
    if(ht != NULL && ht->slots != NULL)
    {
        //Se la tabella hash non è vuota, calcolo l'hash della chiave
        size_t h = ht->key_hash(key, ht->capacity);
        //Controllo se la chiave è presente nella tabella hash
        upo_ht_sepchain_list_node_t *n = ht->slots[h].head;
        //Quando la chiave non è presente nella tabella hash
        while(n != NULL && ht->key_cmp(key, n->key) != 0)
        {
            //Passo al nodo successivo
            n = n->next;
        }
        //Quando la chiave è presente nella tabella hash
        if(n == NULL)
        {
            //Creo un nuovo nodo
            upo_ht_sepchain_list_node_t *node = calloc(1, sizeof(upo_ht_sepchain_list_node_t));
            //Controllo se il nodo è stato creato correttamente
            if(node == NULL)
            {
                //Se il nodo non è stato creato correttamente, esco dal metodo
                abort();
            }
            //Inizializzo il nodo con i valori passati in input
            node->key = key;
            node->value = value;
            //Aggiungo il nodo alla lista
            node->next = ht->slots[h].head;
            //Aggiorno la testa della lista
            ht->slots[h].head = node;
            //Incremento la dimensione della tabella hash
            ht->size += 1;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Esame06
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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Esame07
/* Implementare un algoritmo che, data una tabella hash H con gestione delle collisioni basata su
concatenazioni separate (HT-SC) e una chiave k, conti il numero di collisioni di k in H. Se k non è contenuta
in H, l’algoritmo deve ritornare il valore 0.
*/

//Metodo responsabile del conteggio delle collisioni
size_t upo_ht_sepchain_count_collisions(const upo_ht_sepchain_t ht, const void *key);

//Metodo responsabile dell'implementazione del conteggio delle collisioni
size_t upo_ht_sepchain_count_collisions_impl(const upo_ht_sepchain_t ht, const void *key, int *valid, size_t count);

//Implementazione del metodo responsabile del conteggio delle collisioni
size_t upo_ht_sepchain_count_collisions(const upo_ht_sepchain_t ht, const void *key)
{
    //Controllo se la tabella hash è vuota
    if(ht == NULL)
    {
        //Se la tabella hash è vuota, ritorno 0
        return 0;
    }
    //Altrimenti, inizializzo la variabile che conterrà il risultato
    int valid = 0;
    size_t count = 0;
    //Chiamo il metodo responsabile del conteggio delle collisioni
    size_t res = upo_ht_sepchain_count_collisions_impl(ht, key, &valid, count);
    //Ritorno il risultato
    return res;
}

//Implementazione del metodo responsabile dell'implementazione del conteggio delle collisioni
size_t upo_ht_sepchain_count_collisions_impl(const upo_ht_sepchain_t ht, const void *key, int *valid, size_t count)
{
    //Controllo se la tabella hash è vuota
    if(ht == NULL)
    {
        //Se la tabella hash è vuota, ritorno 0
        return 0;
    }
    //Altrimenti, inizializzo la variabile che conterrà il risultato
    size_t res = 0;
    //Scorro la tabella hash
    for(size_t i = 0; i < ht->capacity; i++)
    {
        //Controllo se la chiave corrisponde
        if(ht->key_cmp(ht->slots[i].head->key, key) == 0)
        {
            //Se la chiave corrisponde, incremento il contatore
            res += 1;
        }
    }
    //Ritorno il risultato
    return res;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Esame08
/*
Implementare un algoritmo che, data una tabella hash H con gestione delle collisioni basata su indirizzamento
aperto a scansione lineare con tecnica del "tombstone", una chiave k ed un valore booleano d, cancelli da H
la coppia chiave-valore identificata da k e ritorni True per identificare l'avvenuta cancellazione. Inoltre,
se d è True, l'algoritmo deve deallocare la memoria allocata per la coppia chiave-valore rimossa. Se k non
e' contenuta in H, l'algoritmo deve ritonare False (cioè il valore 0) per indicare che non è avvenuta alcuna
cancellazione. ATTENZIONE: non e' necessario utilizzare la tecnica del resizing-rehashing
*/

//Metodo responsabile della cancellazione della coppia chiave-valore
int upo_ht_linprob_deletex(upo_ht_linprob_t ht, const void *key, int destroy_data);

//Implementazione del metodo responsabile della cancellazione della coppia chiave-valore
int upo_ht_linprob_deletex(upo_ht_linprob_t ht, const void *key, int destroy_data)
{
    //Controllo se la tabella hash è vuota
    if(ht == NULL)
    {
        return 0;  //Se la tabella hash è vuota, ritorno 0
    }
    //Altrimenti, calcolo l'hash della chiave
    size_t h = ht->key_hash(key, ht->capacity);
    //Scorro la tabella hash
    while(ht->slots[h].key != NULL && ht->key_cmp(ht->slots[h].key, key) != 0 || ht->slots[h].tombstone == 1)
    {
        h = (h + 1) % ht->capacity; //Incremento l'hash
    }
    //Controllo se la chiave corrisponde
    if(ht->slots[h].key != NULL)
    {
        //Se la chiave corrisponde, controllo se devo deallocare la memoria
        if(destroy_data)
        {
            //Se devo deallocare la memoria, dealloco la memoria
            free(ht->slots[h].key);
            free(ht->slots[h].value);
        }
        //Setto la tombstone a 1
        ht->slots[h].key = NULL;
        ht->slots[h].value = NULL;
        ht->slots[h].tombstone = 0;
        ht->size -= 1; //Decremento la dimensione della tabella hash
        return 1;
    }
    //Altrimenti, ritorno 0
    else
    {
        return 0;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
