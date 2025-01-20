/* vim: set tabstop=4 expandtab shiftwidth=4 softtabstop=4: */

/*
 * Copyright 2020 University of Piemonte Orientale, Computer Science Institute
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
static int test_ht3();
static int test_ht4();
static int test_ht5();
static int test_ht6();
static int test_int_ht(int *keys, int *values, size_t n, int *keys2, int *values2, size_t n2);
static int test_str_ht(char **keys, int *values, size_t n, char **keys2, int *values2, size_t n2);
static int test_ht(void *keys, size_t key_sz, void *values, size_t value_sz, size_t n, void *alt_keys, void *alt_values, size_t n2, upo_ht_hasher_t key_hasher, upo_ht_comparator_t key_cmp, char* (*key_to_string)(const void*), char* (*value_to_string)(const void*));
extern void upo_ht_sepchain_dump(const upo_ht_sepchain_t, FILE *, char* (*)(const void*), int, char* (*)(const void*), int);
extern size_t upo_ht_sepchain_ht_size(const upo_ht_sepchain_t ht);
extern void *upo_ht_sepchain_put(upo_ht_sepchain_t ht, void *key, void *value);
extern int upo_ht_sepchain_contains(const upo_ht_sepchain_t ht, const void *key);


int main()
{
    const size_t n = 6;
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
            case 3:
                ret = test_ht3();
                break;
            case 4:
                ret = test_ht4();
                break;
            case 5:
                ret = test_ht5();
                break;
            case 6:
                ret = test_ht6();
                break;
            default:
                fprintf(stderr, "ERROR: Unexpected test case number");
                abort();
        }
        printf("Test HT #%lu => %s\n", i, (ret == TEST_OK) ? "[OK]" : "[FAIL]");
    }

    return 0;
}


int test_ht1()
{
    int keys[] = {0,1,2,3,4,5,6,7,8,9,1000,2000,3000,4000,5000,6000,7000,8000,9000};
    int values[] = {0,1,2,3,4,5,6,7,8,9,1000,2000,3000,4000,5000,6000,7000,8000,9000};
    int keys2[] = {0,1,2,3,4,5,6,9,1010,2010,3010,4010,5010};
    int values2[] = {0,1,2,3,4,5,6,7,8,9,1000,2000,3000};
    size_t n = sizeof keys/sizeof keys[0];
    size_t n2 = sizeof keys2/sizeof keys2[0];

    assert( n == (sizeof keys/sizeof keys[0]) );
    assert( n == (sizeof values/sizeof values[0]) );
    assert( n2 == (sizeof keys2/sizeof keys2[0]) );
    assert( n2 == (sizeof values2/sizeof values2[0]) );

    return test_int_ht(keys, values, n, keys2, values2, n2);
}

int test_ht2()
{
    int keys[] = {0,10,20,30,40,50,60,70,80,90,100,200,300,400,500,600,700,800,900};
    int values[] = {0,1,2,3,4,5,6,7,8,9,1000,2000,3000,4000,5000,6000,7000,8000,9000};
    int keys2[] = {1,20,22,40,43,60,64,80,85,100,106,300,307,500,508,700,709,900,901,1100,1102,1300,1303,1500,1504};
    int values2[] = {0,1,2,3,4,5,6,7,8,9,1000,2000,3000,4000,5000,6000,7000,8000,9000,100,200,300,400,500,600};
    size_t n = sizeof keys/sizeof keys[0];
    size_t n2 = sizeof keys2/sizeof keys2[0];

    assert( n == (sizeof keys/sizeof keys[0]) );
    assert( n == (sizeof values/sizeof values[0]) );
    assert( n2 == (sizeof keys2/sizeof keys2[0]) );
    assert( n2 == (sizeof values2/sizeof values2[0]) );

    return test_int_ht(keys, values, n, keys2, values2, n2);
}

int test_ht3()
{
    int keys[] = {0,1,2,3,4,10,11,12,13,14,1000,2000,3000,4000,5000,6000,7000,8000,9000};
    int values[] = {0,1,2,3,4,5,6,7,8,9,1000,2000,3000,4000,5000,6000,7000,8000,9000};
    int keys2[] = {30,131,232,333};
    int values2[] = {-30,-131,-232,-333};
    size_t n = sizeof keys/sizeof keys[0];
    size_t n2 = sizeof keys2/sizeof keys2[0];

    assert( n == (sizeof keys/sizeof keys[0]) );
    assert( n == (sizeof values/sizeof values[0]) );
    assert( n2 == (sizeof keys2/sizeof keys2[0]) );
    assert( n2 == (sizeof values2/sizeof values2[0]) );

    return test_int_ht(keys, values, n, keys2, values2, n2);
}

int test_ht4()
{
    char *keys[] = {"AA","AB","AQ","Aa","Aq","Ba","Ca","Da","Ea","Fa","AAA", "Ab"};
    int values[] = {0,1,2,3,4,5,6,7,8,9,10,11};
    char *keys2[] = {"xAA","xAB","xAQ","xAa","xAq","xBa","xCa","xDa","xEa","xFa","xAAA", "xAb", "yAA", "yAB"};
    int values2[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13};
    size_t n = sizeof keys/sizeof keys[0];
    size_t n2 = sizeof keys2/sizeof keys2[0];

    assert( n == (sizeof keys/sizeof keys[0]) );
    assert( n == (sizeof values/sizeof values[0]) );
    assert( n2 == (sizeof keys2/sizeof keys2[0]) );
    assert( n2 == (sizeof values2/sizeof values2[0]) );

    return test_str_ht(keys, values, n, keys2, values2, n2);
}

int test_ht5()
{
    char *keys[] = {"aaa","aba","aca","ada","aea","afa","aga","aha","aia","aja"};
    int values[] = {0,1,2,3,4,5,6,7,8,9};
    char *keys2[] = {"a1a","a2a","a3a","a4a","a5a","a6a","aga","aha","aia","aja"};
    int values2[] = {10,11,12,13,14,15,16,17,18,19};
    size_t n = sizeof keys/sizeof keys[0];
    size_t n2 = sizeof keys2/sizeof keys2[0];

    assert( n == (sizeof keys/sizeof keys[0]) );
    assert( n == (sizeof values/sizeof values[0]) );
    assert( n2 == (sizeof keys2/sizeof keys2[0]) );
    assert( n2 == (sizeof values2/sizeof values2[0]) );

    return test_str_ht(keys, values, n, keys2, values2, n2);
}

int test_ht6()
{
    char *keys[] = {"aaa","aab","aac","aad","aba","abb","abc","abd","abe","abf"};
    int values[] = {0,1,2,3,4,5,6,7,8,9};
    char *keys2[] = {"aaa","aax","aac","aay","aba","abz"};
    int values2[] = {-10,-11,-12,-13,-14,-15};
    size_t n = sizeof keys/sizeof keys[0];
    size_t n2 = sizeof keys2/sizeof keys2[0];

    assert( n == (sizeof keys/sizeof keys[0]) );
    assert( n == (sizeof values/sizeof values[0]) );
    assert( n2 == (sizeof keys2/sizeof keys2[0]) );
    assert( n2 == (sizeof values2/sizeof values2[0]) );

    return test_str_ht(keys, values, n, keys2, values2, n2);
}

int test_int_ht(int *keys, int *values, size_t n, int *keys2, int *values2, size_t n2)
{
    assert( keys );
    assert( values );
    assert( keys2 );

    return test_ht(keys, sizeof keys[0], values, sizeof values[0], n, keys2, values2, n2, upo_ht_hash_int_div, upo_test_int_cmp, upo_test_int_to_string, upo_test_int_to_string);
}

int test_str_ht(char **keys, int *values, size_t n, char **keys2, int *values2, size_t n2)
{
    assert( keys );
    assert( values );
    assert( keys2 );

    return test_ht(keys, sizeof keys[0], values, sizeof values[0], n, keys2, values2, n2, upo_ht_hash_str_kr2e, upo_test_str_cmp, upo_test_str_to_string, upo_test_int_to_string);
}

int test_ht(void *keys, size_t key_sz, void *values, size_t value_sz, size_t n, void *alt_keys, void *alt_values, size_t n2, upo_ht_hasher_t key_hasher, upo_ht_comparator_t key_cmp, char* (*key_to_string)(const void*), char* (*value_to_string)(const void*))
{
    upo_ht_sepchain_t ht = NULL;
    upo_ht_sepchain_t ht2 = NULL;
    size_t base_cap = 7;
    size_t caps[3];
    size_t m;
    unsigned char *pck = keys;
    unsigned char *pck2 = alt_keys;
    unsigned char *pcv = values;
    unsigned char *pcv2 = alt_values;
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
        ht2 = upo_ht_sepchain_create(caps[k], key_hasher, key_cmp);

        assert( ht != NULL );
        assert( ht2 != NULL );

        if (n > 0)
        {
            // Non-empty HT

            // Populate the source HT
            for (size_t i = 0; i < n; ++i)
            {
                upo_ht_sepchain_put(ht, pck + i*key_sz, pcv + i*value_sz);
            }

            // Populate the alternative source HT
            for (size_t i = 0; i < n2; ++i)
            {
                upo_ht_sepchain_put(ht2, pck2 + i*key_sz, pcv2 + i*value_sz);
            }

            // Merge
            for (size_t i = 0; i < m && ret == TEST_OK; ++i)
            {
                size_t size = 0;
                size_t check_size = 0;

                upo_ht_sepchain_t new_ht = upo_ht_sepchain_create(caps[i], key_hasher, key_cmp);

                assert( new_ht != NULL );

                // Merge into an empty HT

                upo_ht_sepchain_merge(new_ht, ht);

                size = upo_ht_sepchain_size(new_ht);
                check_size = upo_ht_sepchain_size(ht);

                if (size != check_size)
                {
                    fprintf(stderr, "[file: %s, line: %d] ERROR: HT Alternative #: %lu -> Expected size %lu, got %lu. Did you forget to update the 'size' field?\n", __FILE__, __LINE__, k, check_size, size);
                    fprintf(stderr, "[file: %s, line: %d]: <HT>\n", __FILE__, __LINE__);
                    upo_ht_sepchain_dump(new_ht, stderr, key_to_string, 1, value_to_string, 1);
                    fprintf(stderr, "[file: %s, line: %d]: </HT>\n", __FILE__, __LINE__);
                    fflush(stderr);
                    ret = TEST_FAIL;
                }
                else
                {
                    for (size_t j = 0; j < n && ret == TEST_OK; ++j)
                    {
                        if (!upo_ht_sepchain_contains(new_ht, pck + j*key_sz))
                        {
                            char *key_str = key_to_string(pck + j*key_sz);
                            fprintf(stderr, "[file: %s, line: %d] ERROR: HT Alternative #: %lu -> Key: '%s' not found.\n", __FILE__, __LINE__, k, key_str);
                            fprintf(stderr, "[file: %s, line: %d]: <HT>\n", __FILE__, __LINE__);
                            upo_ht_sepchain_dump(new_ht, stderr, key_to_string, 1, value_to_string, 1);
                            fprintf(stderr, "[file: %s, line: %d]: </HT>\n", __FILE__, __LINE__);
                            fflush(stderr);
                            free(key_str);
                            ret = TEST_FAIL;
                        }
                    }
                }

                // Merge into an non empty HT with same keys

                upo_ht_sepchain_merge(new_ht, ht);

                size = upo_ht_sepchain_size(new_ht);
                check_size = upo_ht_sepchain_size(ht);

                if (size != check_size)
                {
                    fprintf(stderr, "[file: %s, line: %d] ERROR: HT Alternative #: %lu -> Expected size %lu, got %lu. Did you forget to update the 'size' field?\n", __FILE__, __LINE__, k, check_size, size);
                    fprintf(stderr, "[file: %s, line: %d]: <HT>\n", __FILE__, __LINE__);
                    upo_ht_sepchain_dump(new_ht, stderr, key_to_string, 1, value_to_string, 1);
                    fprintf(stderr, "[file: %s, line: %d]: </HT>\n", __FILE__, __LINE__);
                    fflush(stderr);
                    ret = TEST_FAIL;
                }
                else
                {
                    for (size_t j = 0; j < n && ret == TEST_OK; ++j)
                    {
                        if (!upo_ht_sepchain_contains(new_ht, pck + j*key_sz))
                        {
                            char *key_str = key_to_string(pck + j*key_sz);
                            fprintf(stderr, "[file: %s, line: %d] ERROR: HT Alternative #: %lu -> Key: '%s' not found.\n", __FILE__, __LINE__, k, key_str);
                            fprintf(stderr, "[file: %s, line: %d]: <HT>\n", __FILE__, __LINE__);
                            upo_ht_sepchain_dump(new_ht, stderr, key_to_string, 1, value_to_string, 1);
                            fprintf(stderr, "[file: %s, line: %d]: </HT>\n", __FILE__, __LINE__);
                            fflush(stderr);
                            free(key_str);
                            ret = TEST_FAIL;
                        }
                    }
                }

                // Merge into an non empty HT

                upo_ht_sepchain_merge(new_ht, ht2);

                size = upo_ht_sepchain_size(new_ht);
                check_size = upo_ht_sepchain_size(ht) + upo_ht_sepchain_size(ht2);
                for (size_t j = 0; j < n; ++j)
                {
                    if (upo_ht_sepchain_contains(ht2, pck + j*key_sz))
                    {
                        assert( check_size > 0 );
                        --check_size;
                    }
                }

                if (size != check_size)
                {
                    fprintf(stderr, "[file: %s, line: %d] ERROR: HT Alternative #: %lu -> Expected size %lu, got %lu. Did you forget to update the 'size' field?\n", __FILE__, __LINE__, k, check_size, size);
                    fprintf(stderr, "[file: %s, line: %d]: <HT>\n", __FILE__, __LINE__);
                    upo_ht_sepchain_dump(new_ht, stderr, key_to_string, 1, value_to_string, 1);
                    fprintf(stderr, "[file: %s, line: %d]: </HT>\n", __FILE__, __LINE__);
                    fflush(stderr);
                    ret = TEST_FAIL;
                }
                else
                {
                    for (size_t j = 0; j < n && ret == TEST_OK; ++j)
                    {
                        if (!upo_ht_sepchain_contains(new_ht, pck + j*key_sz))
                        {
                            char *key_str = key_to_string(pck + j*key_sz);
                            fprintf(stderr, "[file: %s, line: %d] ERROR: HT Alternative #: %lu -> Key: '%s' not found.\n", __FILE__, __LINE__, k, key_str);
                            fprintf(stderr, "[file: %s, line: %d]: <HT>\n", __FILE__, __LINE__);
                            upo_ht_sepchain_dump(new_ht, stderr, key_to_string, 1, value_to_string, 1);
                            fprintf(stderr, "[file: %s, line: %d]: </HT>\n", __FILE__, __LINE__);
                            fflush(stderr);
                            free(key_str);
                            ret = TEST_FAIL;
                        }
                    }
                    for (size_t j = 0; j < n2 && ret == TEST_OK; ++j)
                    {
                        if (!upo_ht_sepchain_contains(new_ht, pck2 + j*key_sz))
                        {
                            char *key_str = key_to_string(pck2 + j*key_sz);
                            fprintf(stderr, "[file: %s, line: %d] ERROR: HT Alternative #: %lu -> Key: '%s' not found.\n", __FILE__, __LINE__, k, key_str);
                            fprintf(stderr, "[file: %s, line: %d]: <HT>\n", __FILE__, __LINE__);
                            upo_ht_sepchain_dump(new_ht, stderr, key_to_string, 1, value_to_string, 1);
                            fprintf(stderr, "[file: %s, line: %d]: </HT>\n", __FILE__, __LINE__);
                            fflush(stderr);
                            free(key_str);
                            ret = TEST_FAIL;
                        }
                    }
                }

                upo_ht_sepchain_destroy(new_ht, 0);
            }
        }
        else
        {
            // Empty HT: no test
        }

        upo_ht_sepchain_destroy(ht2, 0);
        upo_ht_sepchain_destroy(ht, 0);
    }

    return ret;
}
