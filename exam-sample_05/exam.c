/******************************************************************************/
/*** NOME: Guido Lorenzo                                                    ***/
/*** COGNOME: Broglio                                                       ***/
/*** MATRICOLA: 20043973                                                    ***/
/*** DATA: 25 giugno 2020                                                   ***/
/******************************************************************************/

#include<assert.h>
#include<stdio.h>
#include<stdlib.h>
#include<stddef.h>
#include "include/upo/bst.h"
#include "include/upo/hashtable.h"

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
/**** BEGIN of EXERCISE #1 ****/

//Dichiarazione del metodo responsabile dell'inserimento di un nodo all'interno del BST
void upo_bst_insert(upo_bst_t bst, void *key, void *value);

//Dichiarazione del metodo responsabile dell'inserimento di un nodo all'interno del BST
upo_bst_node_t*upo_bst_insert_imp(upo_bst_node_t*node, void*key, void*value, upo_bst_comparator_t key_cmp);

//Dichiarazione del metodo responsabile della creazione di un nodo all'interno del BST
upo_bst_node_t*upo_bst_node_create(void*key, void*value);

//Dichiarazione del metodo responsabile dell'aggiunta di un nodo alla lista
upo_bst_value_list_node_t*upo_bst_add_list(upo_bst_value_list_node_t*list, void *value);

//Implementazione del metodo responsabile della creazione di un nodo alla lista
upo_bst_value_list_node_t*upo_bst_add_list(upo_bst_value_list_node_t*list, void*value)
{
    //Creo un nuovo nodo
    upo_bst_value_list_node_t* new_node = (upo_bst_value_list_node_t*) malloc(sizeof(upo_bst_value_list_node_t));

    //Controllo se il nodo è stato creato correttamente
    if(new_node==NULL)
    {
        //Se il nodo non è stato creato correttamente, esco dal metodo
        abort();
    }
    //Inizializzo il nodo con i valori passati in input
    new_node->value=value;
    //Aggiungo il nodo alla lista
    new_node->next=list;
    //Ritorno il nodo
    return new_node;
}

//Implementazione del metodo responsabile della creazione di un nodo all'interno del BST
upo_bst_node_t*upo_bst_node_create(void*key, void*value)
{
    //Creo un nuovo nodo
    upo_bst_node_t* new_node = (upo_bst_node_t*) malloc(sizeof(upo_bst_node_t));
    //Controllo se il nodo è stato creato correttamente, altrimenti esco dal metodo
    if(new_node==NULL)
    {
        abort();
    }
    //Inizializzo il nodo con i valori passati in input
    new_node->key=key;
    //Creo la lista dei valori. Inizialmente il nodo contiene un solo valore
    new_node->values=upo_bst_add_list(NULL, value);
    //Inizializzo i puntatori ai nodi figli
    new_node->left=NULL;
    new_node->right=NULL;
    //Ritorno il nodo
    return new_node;
}

//Implementazione del metodo responsabile dell'inserimento di un nodo all'interno del BST
upo_bst_node_t*upo_bst_insert_imp(upo_bst_node_t*node, void*key, void*value, upo_bst_comparator_t key_cmp)
{
    //Controllo se il nodo è nullo
    if(node==NULL)
    {
        //Se il nodo è nullo, creo un nuovo nodo
        return upo_bst_node_create(key, value);
    }
    //Inito la variabile compare con il risultato della comparazione tra la chiave del nodo e la chiave passata in input
    int compare=key_cmp(key, node->key);
    //Controllo se la chiave passata in input è minore della chiave del nodo
    if(compare<0)
    {
        //Se la chiave passata in input è minore della chiave del nodo, inserisco il nodo nel sottoalbero sinistro
        node->left=(upo_bst_insert_imp(node->left, key, value, key_cmp));
    }
    //Controllo se la chiave passata in input è maggiore della chiave del nodo
    else if(compare>0)
    {
        //Se la chiave passata in input è maggiore della chiave del nodo, inserisco il nodo nel sottoalbero destro
        node->right=(upo_bst_insert_imp(node->right, key, value, key_cmp));
    }
    //Controllo se la chiave passata in input è uguale alla chiave del nodo
    else
    {
        //Se la chiave passata in input è uguale alla chiave del nodo, aggiungo il valore alla lista dei valori
        node->values = upo_bst_add_list(node->values, value);
    }
    //Ritorno il nodo
    return node;
}

//Implementazione del metodo responsabile dell'inserimento di un nodo all'interno del BST
void upo_bst_insert(upo_bst_t bst, void*key, void*value)
{
    //Controllo se il BST è vuoto, altrimenti esco dal metodo
    if(bst==NULL)
    {
        return;
    }
    //Controllo se la radice del BST è vuota
    else
    {
        //Se la radice del BST è vuota, inserisco il nodo
        bst->root=upo_bst_insert_imp(bst->root, key, value, bst->key_cmp);
    }
}

/**** END of EXERCISE #1 ****/

/**** BEGIN of EXERCISE #2 ****/ 
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

// Dichiarazione della funzione per il merge delle tabelle hash
void upo_ht_sepchain_merge(upo_ht_sepchain_t dest_ht, const upo_ht_sepchain_t src_ht)
{
    //Controllo se le tabelle hash sono vuote, se non lo sono procedo con il merge
    if(src_ht!=NULL && dest_ht!=NULL)
    {
        //Scorro la tabella hash di origine, per ogni chiave presente nella tabella hash di origine
        for(size_t hash=0; hash<src_ht->capacity; hash++)
        {
            //inizializzo node con il primo nodo della lista di collisioni
            upo_ht_sepchain_list_node_t*node=src_ht->slots[hash].head;
            //Per ogni nodo della lista di collisioni diverso da NULL
            while(node!=NULL)
            {
                //Calcolo l'indice hash della chiave
                size_t hash_index=dest_ht->key_hash(node->key, dest_ht->capacity);
                //Inizializzo current_node con il primo nodo della lista di collisioni
                upo_ht_sepchain_list_node_t*current_node=dest_ht->slots[hash_index].head;
                //Inizializzo key_exist a 0
                int key_exist=0; 
                //Per ogni nodo della lista di collisioni diverso da NULL
                while(current_node!=NULL)
                {
                    //Controllo se la chiave è già presente nella tabella hash di destinazione
                    if(dest_ht->key_cmp(node->key, current_node->key)==0)
                    {
                        //Se la chiave è già presente nella tabella hash di destinazione, setto key_exist a 1 e esco dal ciclo
                        key_exist=1;
                        break;
                    }
                    //Passo al nodo successivo
                    current_node=current_node->next;
                }
                //Se la chiave non è presente nella tabella hash di destinazione
                if(!key_exist)
                {
                    //Creo un nuovo nodo
                    upo_ht_sepchain_list_node_t*new_node=calloc(1, sizeof(upo_ht_sepchain_list_node_t));
                    //Controllo se il nodo è stato creato correttamente
                    if(new_node!=NULL)
                    {
                        //Inizializzo il nodo con i valori del nodo corrente
                        new_node->key=node->key;
                        new_node->value=node->value;
                        //Inserisco il nodo in testa alla lista di collisioni
                        new_node->next=dest_ht->slots[hash_index].head;
                        //Aggiorno la testa della lista di collisioni
                        dest_ht->slots[hash_index].head=new_node;
                        //Incremento la dimensione della tabella hash di destinazione
                        dest_ht->size+=1;
                    }
                }
                //Passo al nodo successivo
                node=node->next;
            }
        }
    }
}

/**** END of EXERCISE #2 ****/