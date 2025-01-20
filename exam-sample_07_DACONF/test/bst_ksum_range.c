/* vim: set tabstop=4 expandtab shiftwidth=4 softtabstop=4: */

/** Esercizio 1
 * 
 * Implementare un algoritmo che dato un albero binario di ricerca BST e un intervallo [Klow, Khigh],
 * restituisca la somma dei valori dei nodi le cui chiavi sono contenute nell'intervallo o 0 se nessuna
 * chiave è contenuta nell'intervallo o se il BST è vuoto.
 * 
 * Si noti che l'intervallo di chiavi è da intendersi con estremi inclusi e il tipo dei valori dei nodi
 * è da considerarsi intero (int). Dato un albero, possiamo dire di avere:
 * 
 * esempio di albero (primo valore key, secondo value)
 * 
 *                                          k:8 v:0        
 *                      k:3 v:1                                 k:10 v:6
 *           k:1 v:2            k:6 v:3                                     k:14 v:7
 *                      k:4 v:4        k:7 v:5                      k:13 v:8
 * 
 *  - Somma dei valori di nodi con chiavi in [1, 8] = 15
 *  - Somma dei valori di nodi con chiavi in [6, 16] = 29
 *  - Somma dei valori di nodi con chiavi in [15, 19] = 0
 *  - Somma dei valori di nodi con chiavi in [8, 12] = 6
 * 
 * L'algoritmo implementato dev'essere ottimo, nel senso che deve visitare l'albero una sola volta e la
 * complessità termporale del caso peggiore dev'Essere O(n), dove n è il numero di chiavi del BST.
 * 
 * La funzione da implementare si trova nel file ed ha il seguente prototipo:
 * int upo_bst_ksum_range(const upo_bst_t tree, const void*low, const void*high)
 * Con parametri:
 *  - tree: BST
 *  - low: puntatore al limite inferiore dell'intervallo
 *  - high: puntatore al limite superiore dell'intervallo
 * Come valore di ritorno:
 *  - Se il BST non è vuoto ed esistono nodi le cui chiavi sono contenute nell'intervallo specificato, 
 *    restituisce la somma dei valori dei nodi le cui chiavi sono contenute nell'intervallo
 *  - Se il BST è vuoto o non ci sono chiavi contenute nell'intervallo specificato, restituisce 0
 *    
 */

#include <assert.h>
#include <limits.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <upo/bst.h>
#include <upo/test.h>

#define TEST_OK 1
#define TEST_FAIL 0

static int test_bst1();
static int test_bst2();
static int test_empty_bst();
static int test_int_bst_range(int *keys, int *values, size_t n, int low, int high, int expected_sum);
extern int upo_bst_ksum_range(const upo_bst_t tree, const void *low, const void *high);

// Funzione di comparazione per chiavi intere
int int_comparator(const void *a, const void *b) {
    return (*(int *)a - *(int *)b);
}

int main()
{
    size_t n = 2;
    size_t i = 0;
    int ret = 0;

    printf("*** [DISCLAIMER] ***************************************************************\n");
    printf("The following tests are provided as is, to allow you to quickly test your code.\n");
    printf("However, passing these tests is a necessary but not sufficient condition,\n");
    printf("meaning that they do not guarantee that your code is correct.\n");
    printf("In fact, your code may be wrong even it passes all these tests\n");
    printf("********************************************************************************\n\n");

    for (i = 1; i <= n; ++i)
    {
        switch (i)
        {
            case 1:
                ret = test_bst1();
                break;
            case 2:
                ret = test_bst2();
                break;
            default:
                fprintf(stderr, "ERROR: Unexpected test case number");
                abort();
        }
        printf("Test BST #%lu => %s\n", i, (ret == TEST_OK) ? "[OK]" : "[FAIL]");
    }

    ret = test_empty_bst();
    printf("Test Empty BST => %s\n", (ret == TEST_OK) ? "[OK]" : "[FAIL]");

    return 0;
}

// Test su intervallo [1, 8] con albero intero
int test_bst1()
{
    int keys[] = {8, 3, 10, 1, 6, 4, 7, 14, 13};
    int values[] = {0, 1, 6, 2, 3, 4, 5, 7, 8};
    size_t n = sizeof(keys) / sizeof(keys[0]);
    int low = 1, high = 8, expected_sum = 15;

    return test_int_bst_range(keys, values, n, low, high, expected_sum);
}

// Test su intervallo [6, 16] con albero intero
int test_bst2()
{
    int keys[] = {8, 3, 10, 1, 6, 4, 7, 14, 13};
    int values[] = {0, 1, 6, 2, 3, 4, 5, 7, 8};
    size_t n = sizeof(keys) / sizeof(keys[0]);
    int low = 6, high = 16, expected_sum = 29;

    return test_int_bst_range(keys, values, n, low, high, expected_sum);
}

// Test su albero vuoto
int test_empty_bst()
{
    upo_bst_t tree = upo_bst_create(int_comparator);
    int low = 1, high = 10;
    int sum = upo_bst_ksum_range(tree, &low, &high);
    upo_bst_destroy(tree, 0);

    return (sum == 0) ? TEST_OK : TEST_FAIL;
}

// Funzione di supporto per testare un intervallo in un BST con chiavi e valori interi
int test_int_bst_range(int *keys, int *values, size_t n, int low, int high, int expected_sum)
{
    upo_bst_t tree = upo_bst_create(int_comparator);
    
    // Inserimento delle chiavi e dei valori nell'albero
    for (size_t i = 0; i < n; i++) {
        upo_bst_insert(tree, &keys[i], &values[i]);
    }

    // Calcolo della somma dei valori nell'intervallo specificato
    int sum = upo_bst_ksum_range(tree, &low, &high);
    upo_bst_destroy(tree, 0);

    // Verifica della somma
    return (sum == expected_sum) ? TEST_OK : TEST_FAIL;
}
