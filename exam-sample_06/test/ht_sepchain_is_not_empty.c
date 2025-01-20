/* vim: set tabstop=4 expandtab shiftwidth=4 softtabstop=4: */

/*
 * Copyright 2015 University of Piemonte Orientale, Computer Science Institute
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
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <upo/hashtable.h>
#include <upo/test.h>


#define TEST_OK 1
#define TEST_FAIL 0


static int test_ht1();
static int test_ht2();
static int test_empty_ht();
static int test_int_ht(int *keys, int *values, size_t n);
static int test_str_ht(char **keys, int *values, size_t n);
static int test_ht(void *keys, size_t key_sz, void *values, size_t value_sz, size_t n, upo_ht_hasher_t key_hasher, upo_ht_comparator_t key_cmp, char* (*key_to_string)(const void*), char* (*value_to_string)(const void*));
extern void upo_ht_sepchain_dump(const upo_ht_sepchain_t, FILE *, char* (*)(const void*), int, char* (*)(const void*), int);
extern void* upo_ht_sepchain_put(upo_ht_sepchain_t ht, void *key, void *value);


int main()
{
    const size_t n = 2;
    size_t i = 0;
    int ret = TEST_FAIL;

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
                ret = test_ht1();
                break;
            case 2:
                ret = test_ht2();
                break;
            default:
                fprintf(stderr, "ERROR: Unexpected test case number");
                abort();
        }
        printf("Test HT #%lu => %s\n", i, (ret == TEST_OK) ? "[OK]" : "[FAIL]");
    }

    ret = test_empty_ht();
    printf("Test Empty HT => %s\n", (ret == TEST_OK) ? "[OK]" : "[FAIL]");

    return 0;
}


int test_ht1()
{
    int keys[] = {1,2,3};
    int values[] = {1,2,3};
    size_t n = sizeof keys/sizeof keys[0];

    assert( n == (sizeof keys/sizeof keys[0]) );
    assert( n == (sizeof values/sizeof values[0]) );

    return test_int_ht(keys, values, n);
}

int test_ht2()
{
    char *keys[] = {"x","y","z"};
    int values[] = {1,2,3};
    size_t n = sizeof keys/sizeof keys[0];

    assert( n == (sizeof keys/sizeof keys[0]) );
    assert( n == (sizeof values/sizeof values[0]) );

    return test_str_ht(keys, values, n);
}

int test_empty_ht()
{
    return test_ht(NULL, 0, NULL, 0, 0, upo_ht_hash_int_div, upo_test_int_cmp, NULL, NULL);
}

int test_int_ht(int *keys, int *values, size_t n)
{
    assert( keys );
    assert( values );

    return test_ht(keys, sizeof keys[0], values, sizeof values[0], n, upo_ht_hash_int_div, upo_test_int_cmp, upo_test_int_to_string, upo_test_int_to_string);
}

int test_str_ht(char **keys, int *values, size_t n)
{
    assert( keys );
    assert( values );

    return test_ht(keys, sizeof keys[0], values, sizeof values[0], n, upo_ht_hash_str_kr2e, upo_test_str_cmp, upo_test_str_to_string, upo_test_int_to_string);
}

int test_ht(void *keys, size_t key_sz, void *values, size_t value_sz, size_t n, upo_ht_hasher_t key_hasher, upo_ht_comparator_t key_cmp, char* (*key_to_string)(const void*), char* (*value_to_string)(const void*))
{
    upo_ht_sepchain_t ht = NULL;
    size_t base_cap = 7;
    size_t caps[3];
    size_t m;
    unsigned char *pck = keys;
    unsigned char *pcv = values;
    int ret = TEST_OK;

/*
    caps[0] = 2*n+UPO_HT_SEPCHAIN_DEFAULT_CAPACITY;
    caps[1] = n*UPO_HT_SEPCHAIN_DEFAULT_CAPACITY;
    caps[2] = UPO_HT_SEPCHAIN_DEFAULT_CAPACITY;
*/
    caps[0] = 2*n+base_cap;
    caps[1] = n*base_cap;
    caps[2] = base_cap;
    m = sizeof caps/sizeof caps[0];

    for (size_t k = 0; k < m && ret == TEST_OK; ++k)
    {
        ht = upo_ht_sepchain_create(caps[k], key_hasher, key_cmp);

        assert( ht != NULL );

        if (n > 0)
        {
            // Non-empty HT

            // Populate the HT

            for (size_t i = 0; i < n; ++i)
            {
                upo_ht_sepchain_put(ht, pck + i*key_sz, pcv + i*value_sz);
            }

            if (!upo_ht_sepchain_is_not_empty(ht))
            {
                fprintf(stderr, "[file: %s, line: %d] ERROR: HT alternative #: %lu, non-empty HT -> Expected: TRUE, got: FALSE.\n", __FILE__, __LINE__, k);
                fprintf(stderr, "[file: %s, line: %d]: <HT>\n", __FILE__, __LINE__);
                upo_ht_sepchain_dump(ht, stderr, key_to_string, 1, value_to_string, 1);
                fprintf(stderr, "[file: %s, line: %d]: </HT>\n", __FILE__, __LINE__);
                fflush(stderr);
                ret = TEST_FAIL;
            }
        }
        else
        {
            // Empty HT

            if (upo_ht_sepchain_is_not_empty(ht))
            {
                fprintf(stderr, "[file: %s, line: %d] ERROR: HT alternative #: %lu, empty HT -> Expected: FALSE, got: TRUE.\n", __FILE__, __LINE__, k);
                fflush(stderr);
                ret = TEST_FAIL;
            }
        }

        upo_ht_sepchain_destroy(ht, 0);
    }

    return ret;
}
