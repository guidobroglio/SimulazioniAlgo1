/* vim: set tabstop=4 expandtab shiftwidth=4 softtabstop=4: */

/*
 * Copyright 2016 University of Piemonte Orientale, Computer Science Institute
 *
 * This file is part of UPOalglib.
 *
 * UPOalglib is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * UPOalglib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with UPOalglib.  If not, see <http://www.gnu.org/licenses/>.
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
static int test_bst3();
//static int test_empty_bst();
static int test_int_bst(int *keys, char **values1, char **value2, size_t n);
static int test_str_bst(char **keys, int *values1, int *values2, size_t n);
static int test_bst(void *keys, size_t key_sz, void *values1, void *values2, size_t value_sz, size_t n, upo_bst_comparator_t key_cmp, upo_bst_comparator_t value_cmp, char* (*key_to_string)(const void*), char* (*value_to_string)(const void*));
extern upo_bst_value_list_node_t* upo_bst_get(const upo_bst_t, const void*);
extern void upo_bst_dump(const upo_bst_t, FILE *, char* (*)(const void*), int, char* (*)(const void*), int);
extern int upo_bst_contains(const upo_bst_t, const void*);


int main()
{
    size_t n = 3;
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
            case 3:
                ret = test_bst3();
                break;
            default:
                fprintf(stderr, "ERROR: Unexpected test case number\n");
                abort();
        }
        printf("Test BST #%lu => %s\n", i, (ret == TEST_OK) ? "[OK]" : "[FAIL]");
    }

    return 0;
}


int test_bst1()
{
    /*
     * BST:
     *      11
     *     / \
     *    3   13
     *   / \   \
     *  1   9   19
     *     /   /
     *    7   15
     *   /     \
     *  5       17
     */

    int keys[] = {11, 3, 1, 9, 7, 5, 13, 19, 15, 17};
    char *values1[] = {"a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1", "i1", "j1"};
    char *values2[] = {"a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2", "i2", "j2"};
    size_t n = sizeof keys/sizeof keys[0];

    assert( n == sizeof keys/sizeof keys[0] );
    assert( n == sizeof values1/sizeof values1[0] );
    assert( n == sizeof values2/sizeof values2[0] );

    return test_int_bst(keys, values1, values2, n);
}

int test_bst2()
{
    /*
     * BST:
     *       8
     *     /   \
     *    3     10
     *   / \     \
     *  1   6     14
     *     / \   /
     *    4   7 13
     */

    int keys[] = {8, 3, 1, 6, 4, 7, 10, 14, 13};
    char *values1[] = {"a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1", "i1"};
    char *values2[] = {"a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2", "i2"};
    size_t n = sizeof keys/sizeof keys[0];

    assert( n == sizeof keys/sizeof keys[0] );
    assert( n == sizeof values1/sizeof values1[0] );
    assert( n == sizeof values2/sizeof values2[0] );

    return test_int_bst(keys, values1, values2, n);
}

int test_bst3()
{
    /*
     * BST:
     *         "aae"
     *          / \
     *      "aac" "aaj"
     *       / \      \
     *   "aaa" "aad" "aak"
     */

    char *keys[] = {"aae", "aaj", "aac", "aad", "aaa", "aak"};
    int values1[] = {11, 12, 13, 14, 15, 16};
    int values2[] = {21, 22, 23, 24, 25, 26};
    size_t n = sizeof keys/sizeof keys[0];

    assert( n == sizeof keys/sizeof keys[0] );
    assert( n == sizeof values1/sizeof values1[0] );
    assert( n == sizeof values2/sizeof values2[0] );

    return test_str_bst(keys, values1, values2, n);
}

//int test_empty_bst()
//{
//    return test_bst(NULL, 0, NULL, NULL, 0, 0, upo_test_int_cmp, upo_test_int_cmp, NULL, NULL);
//}

int test_int_bst(int *keys, char **values1, char **values2, size_t n)
{
    assert( keys );
    assert( values1 );
    assert( values2 );
    assert( sizeof values1[0] == sizeof values2[0] );

    return test_bst(keys, sizeof keys[0], values1, values2, sizeof values1[0], n, upo_test_int_cmp, upo_test_str_cmp, upo_test_int_to_string, upo_test_str_to_string);
}

int test_str_bst(char **keys, int *values1, int *values2, size_t n)
{
    assert( keys );
    assert( values1 );
    assert( values2 );
    assert( sizeof values1[0] == sizeof values2[0] );

    return test_bst(keys, sizeof keys[0], values1, values2, sizeof values1[0], n, upo_test_str_cmp, upo_test_int_cmp, upo_test_str_to_string, upo_test_int_to_string);
}

int test_bst(void *keys, size_t key_sz, void *values1, void *values2, size_t value_sz, size_t n, upo_bst_comparator_t key_cmp, upo_bst_comparator_t value_cmp, char* (*key_to_string)(const void*), char* (*value_to_string)(const void*))
{
    const size_t num_alt_values = 2;

    upo_bst_t bst = NULL;
    unsigned char *pck = keys;
    unsigned char *pcv1 = values1;
    unsigned char *pcv2 = values2;
    int ret = TEST_OK;

    /* Creates BST */

    bst = upo_bst_create(key_cmp);

    assert( bst != NULL );

    if (n > 0)
    {
        // Non-empty BST

        for (size_t k = 0; k < num_alt_values; ++k)
        {
            // Populate the BST

            for (size_t i = 0; i < n; ++i)
            {
                switch (k)
                {
                    case 0:
                        upo_bst_insert(bst, pck + i*key_sz, pcv1 + i*value_sz);
                        break;
                    case 1:
                        upo_bst_insert(bst, pck + i*key_sz, pcv2 + i*value_sz);
                        break;
                    default:
                        fprintf(stderr, "[file: %s, line: %d] ERROR: Non-empty BST -> Unexpected number of alternative values.\n", __FILE__, __LINE__);
                        abort();
                        break;
                }
            }

            // Check

            for (size_t i = 0; i < n && ret == TEST_OK; ++i)
            {
                // Check if the key is present

                if (upo_bst_contains(bst, pck + i*key_sz))
                {
                    size_t check_nvals = k+1;
                    size_t nvals = 0;
                    void *values = NULL;
                    upo_bst_value_list_node_t *value_list = upo_bst_get(bst, pck + i*key_sz);
                    upo_bst_value_list_node_t *node = value_list;
                    while (node != NULL)
                    {
                        values = realloc(values, (nvals+1)*value_sz);
                        if (values == NULL)
                        {
                            perror("Unable to (re)allocate memory for values");
                            abort();
                        }
                        memcpy(((unsigned char*) values) + nvals*value_sz, node->value, value_sz);

                        ++nvals;

                        upo_bst_value_list_node_t *del_node = node;
                        node = node->next;
                        free(del_node);
                    }

                    // Check if the number of values associated with the key is correct

                    if (nvals == check_nvals)
                    {
                        // Check if stored values associated with the key are correct

                        qsort(values, nvals, value_sz, value_cmp);

                        for (size_t j = 0; j < k; ++j)
                        {
                            void *check_value = NULL;
                            switch (j)
                            {
                                case 0:
                                    check_value = pcv1 + i*value_sz;
                                    break;
                                case 1:
                                    check_value = pcv2 + i*value_sz;
                                    break;
                                default:
                                    fprintf(stderr, "[file: %s, line: %d] ERROR: Non-empty BST -> Unexpected number of alternative values.\n", __FILE__, __LINE__);
                                    abort();
                                    break;
                            }
                            if (bsearch(check_value, values, nvals, value_sz, value_cmp) == NULL)
                            {
                                char *key_str = key_to_string(pck + i*key_sz);
                                char *value_str = value_to_string(check_value);
                                fprintf(stderr, "[file: %s, line: %d] ERROR: Non-empty BST, Key '%s', Alternative value %lu -> Value '%s' not found.\n", __FILE__, __LINE__, key_str, j, value_str);
                                fprintf(stderr, "[file: %s, line: %d]: <BST>\n", __FILE__, __LINE__);
                                upo_bst_dump(bst, stderr, key_to_string, 1, value_to_string, 1);
                                fprintf(stderr, "[file: %s, line: %d]: </BST>\n", __FILE__, __LINE__);
                                fflush(stderr);
                                free(value_str);
                                free(key_str);
                                ret = TEST_FAIL;
                            }
                        }
                    }
                    else
                    {
                        char *key_str = key_to_string(pck + i*key_sz);
                        fprintf(stderr, "[file: %s, line: %d] ERROR: Non-empty BST, Key '%s' -> Expected %lu values, got %lu.\n", __FILE__, __LINE__, key_str, check_nvals, nvals);
                        fprintf(stderr, "[file: %s, line: %d]: <BST>\n", __FILE__, __LINE__);
                        upo_bst_dump(bst, stderr, key_to_string, 1, value_to_string, 1);
                        fprintf(stderr, "[file: %s, line: %d]: </BST>\n", __FILE__, __LINE__);
                        fflush(stderr);
                        free(key_str);
                        ret = TEST_FAIL;
                    }

                    free(values);
                }
                else
                {
                    char *key_str = key_to_string(pck + i*key_sz);
                    fprintf(stderr, "[file: %s, line: %d] ERROR: Non-empty BST -> Key '%s' not found.\n", __FILE__, __LINE__, key_str);
                    fprintf(stderr, "[file: %s, line: %d]: <BST>\n", __FILE__, __LINE__);
                    upo_bst_dump(bst, stderr, key_to_string, 1, value_to_string, 1);
                    fprintf(stderr, "[file: %s, line: %d]: </BST>\n", __FILE__, __LINE__);
                    fflush(stderr);
                    free(key_str);
                    ret = TEST_FAIL;
                }
            }
        }
    }

    /* Destroy */
 
    upo_bst_destroy(bst, 0);

    return ret;
}
