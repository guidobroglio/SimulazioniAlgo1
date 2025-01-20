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
static int test_empty_bst();
static int test_int_bst(int *keys, double *values, size_t n);
static int test_str_bst(char **keys, double *values, size_t n);
static int test_bst(void *keys, size_t key_sz, void *values, size_t value_sz, size_t n, upo_bst_comparator_t cmp, char* (*to_string)(const void*));
extern const void* upo_bst_xyz_(const upo_bst_t, const void*, int*);
extern void upo_bst_dump(const upo_bst_t, FILE *, char* (*)(const void*), int);
extern void* upo_bst_put(upo_bst_t, void*, void*);


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


int test_bst1()
{
    int keys[] = {3, 2, 1};
    double values[] = {-1.0, -2.0, -3.0};
    size_t n = sizeof keys/sizeof keys[0];

    assert( n == sizeof keys/sizeof keys[0] );
    assert( n == sizeof values/sizeof values[0] );

    return test_int_bst(keys, values, n);
}

int test_bst2()
{
    char *keys[] = {"x", "y", "z"};
    double values[] = {-3.0, -2.0, -1.0};
    size_t n = sizeof keys/sizeof keys[0];

    assert( n == sizeof keys/sizeof keys[0] );
    assert( n == sizeof values/sizeof values[0] );

    return test_str_bst(keys, values, n);
}

int test_empty_bst()
{
    return test_bst(NULL, 0, NULL, 0, 0, upo_test_int_cmp, NULL);
}

int test_int_bst(int *keys, double *values, size_t n)
{
    assert( keys );
    assert( values );

    return test_bst(keys, sizeof keys[0], values, sizeof values[0], n, upo_test_int_cmp, upo_test_int_to_string);
}

int test_str_bst(char **keys, double *values, size_t n)
{
    assert( keys );
    assert( values );

    return test_bst(keys, sizeof keys[0], values, sizeof values[0], n, upo_test_str_cmp, upo_test_str_to_string);
}

int test_bst(void *keys, size_t key_sz, void *values, size_t value_sz, size_t n, upo_bst_comparator_t cmp, char* (*to_string)(const void*))
{
    upo_bst_t bst = NULL;
    unsigned char *pck = keys;
    unsigned char *pcv = values;
    int ret = TEST_OK;

    /* Creates BST */

    bst = upo_bst_create(cmp);

    assert( bst != NULL );

    if (n > 0)
    {
        // Non-empty BST

        // Populate the BST

        for (size_t i = 0; i < n; ++i)
        {
            upo_bst_put(bst, pck + i*key_sz, pcv + i*value_sz);
        }

        if (!upo_bst_is_not_empty(bst))
        {
            fprintf(stderr, "[file: %s, line: %d] ERROR: Non-empty BST -> Expected: TRUE, got: FALSE.\n", __FILE__, __LINE__);
            fprintf(stderr, "[file: %s, line: %d]: <BST>\n", __FILE__, __LINE__);
            upo_bst_dump(bst, stderr, to_string, 1);
            fprintf(stderr, "[file: %s, line: %d]: </BST>\n", __FILE__, __LINE__);
            fflush(stderr);
            ret = TEST_FAIL;
        }
    }
    else
    {
        // Empty BST

        if (upo_bst_is_not_empty(bst))
        {
            fprintf(stderr, "[file: %s, line: %d] ERROR: Empty BST -> Expected: FALSE, got: TRUE.\n", __FILE__, __LINE__);
            fflush(stderr);
            ret = TEST_FAIL;
        }
    }

    /* Destroy */
 
    upo_bst_destroy(bst, 0);

    return ret;
}
