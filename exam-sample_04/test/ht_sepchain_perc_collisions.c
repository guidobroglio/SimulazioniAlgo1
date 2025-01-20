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
#include <math.h>


#define TEST_OK 1
#define TEST_FAIL 0
#define COLLISION_TOLERANCE 0.00001



static int test_ht1(void);
static int test_ht2(void);
static int test_ht3(void);
static int test_ht4(void);
static int test_ht5(void);
static int test_ht6(void);
static int test_empty_ht(void);
static int test_int_ht(int *keys, int *values, size_t n, int *no_keys, size_t m);
static int test_str_ht(char **keys, int *values, size_t n, char **no_keys, size_t m);
static int test_ht(void *keys, size_t key_sz, void *values, size_t value_sz, size_t n, void *no_keys, size_t m, upo_ht_hasher_t key_hasher, upo_ht_comparator_t key_cmp, char* (*key_to_string)(const void*), char* (*value_to_string)(const void*));
extern void upo_ht_sepchain_dump(const upo_ht_sepchain_t, FILE *, char* (*)(const void*), int, char* (*)(const void*), int);
extern void *upo_ht_sepchain_put(upo_ht_sepchain_t ht, void *key, void *value);
extern float upo_ht_sepchain_xyz(const upo_ht_sepchain_t, const void *);


int main(void)
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

    ret = test_empty_ht();
    printf("Test Empty HT => %s\n", (ret == TEST_OK) ? "[OK]" : "[FAIL]");

    return 0;
}


int test_ht1(void)
{
    int keys[] = {0,1,2,3,4,5,6,7,8,9,1000,2000,3000,4000,5000,6000,7000,8000,9000};
    int values[] = {0,1,2,3,4,5,6,7,8,9,1000,2000,3000,4000,5000,6000,7000,8000,9000};
    int no_keys[] = {1001,1002,1003,1004,1005,1006,1009,1010,2010,3010,4010,5010};
    size_t n = sizeof keys/sizeof keys[0];
    size_t m = sizeof no_keys/sizeof no_keys[0];

    assert( n == (sizeof keys/sizeof keys[0]) );
    assert( n == (sizeof values/sizeof values[0]) );
    assert( m == (sizeof no_keys/sizeof no_keys[0]) );

    return test_int_ht(keys, values, n, no_keys, m);
}

int test_ht2(void)
{
    int keys[] = {0,10,20,30,40,50,60,70,80,90,100,200,300,400,500,600,700,800,900};
    int values[] = {0,1,2,3,4,5,6,7,8,9,1000,2000,3000,4000,5000,6000,7000,8000,9000};
    int no_keys[] = {1,22,43,64,85,106,307,508,709,901,1100,1102,1300,1303,1500,1504};
    size_t n = sizeof keys/sizeof keys[0];
    size_t m = sizeof no_keys/sizeof no_keys[0];

    assert( n == (sizeof keys/sizeof keys[0]) );
    assert( n == (sizeof values/sizeof values[0]) );
    assert( m == (sizeof no_keys/sizeof no_keys[0]) );

    return test_int_ht(keys, values, n, no_keys, m);
}

int test_ht3(void)
{
    int keys[] = {0,1,2,3,4,10,11,12,13,14,1000,2000,3000,4000,5000,6000,7000,8000,9000};
    int values[] = {0,1,2,3,4,5,6,7,8,9,1000,2000,3000,4000,5000,6000,7000,8000,9000};
    int no_keys[] = {30,131,232,333};
    size_t n = sizeof keys/sizeof keys[0];
    size_t m = sizeof no_keys/sizeof no_keys[0];

    assert( n == (sizeof keys/sizeof keys[0]) );
    assert( n == (sizeof values/sizeof values[0]) );
    assert( m == (sizeof no_keys/sizeof no_keys[0]) );

    return test_int_ht(keys, values, n, no_keys, m);
}

int test_ht4(void)
{
    char *keys[] = {"AA","AB","AQ","Aa","Aq","Ba","Ca","Da","Ea","Fa","AAA", "Ab"};
    int values[] = {0,1,2,3,4,5,6,7,8,9,10,11};
    char *no_keys[] = {"xAA","xAB","xAQ","xAa","xAq","xBa","xCa","xDa","xEa","xFa","xAAA", "xAb", "yAA", "yAB"};
    size_t n = sizeof keys/sizeof keys[0];
    size_t m = sizeof no_keys/sizeof no_keys[0];

    assert( n == (sizeof keys/sizeof keys[0]) );
    assert( n == (sizeof values/sizeof values[0]) );
    assert( m == (sizeof no_keys/sizeof no_keys[0]) );

    return test_str_ht(keys, values, n, no_keys, m);
}

int test_ht5(void)
{
    char *keys[] = {"aaa","aba","aca","ada","aea","afa","aga","aha","aia","aja"};
    int values[] = {0,1,2,3,4,5,6,7,8,9};
    char *no_keys[] = {"a1a","a2a","a3a","a4a","a5a","a6a","a7a","a8a","a9a","a0a"};
    size_t n = sizeof keys/sizeof keys[0];
    size_t m = sizeof no_keys/sizeof no_keys[0];

    assert( n == (sizeof keys/sizeof keys[0]) );
    assert( n == (sizeof values/sizeof values[0]) );
    assert( m == (sizeof no_keys/sizeof no_keys[0]) );

    return test_str_ht(keys, values, n, no_keys, m);
}

int test_ht6(void)
{
    char *keys[] = {"aaa","aab","aac","aad","aba","abb","abc","abd","abe","abf"};
    int values[] = {0,1,2,3,4,5,6,7,8,9};
    char *no_keys[] = {"AAA","AAX","AAC","AAY","ABA","ABZ"};
    size_t n = sizeof keys/sizeof keys[0];
    size_t m = sizeof no_keys/sizeof no_keys[0];

    assert( n == (sizeof keys/sizeof keys[0]) );
    assert( n == (sizeof values/sizeof values[0]) );
    assert( m == (sizeof no_keys/sizeof no_keys[0]) );

    return test_str_ht(keys, values, n, no_keys, m);
}

int test_empty_ht(void)
{
    int dummy1 = 0;
    int dummy2 = 1;

    return test_int_ht(&dummy1, &dummy1, 0, &dummy2, 1);
}

int test_int_ht(int *keys, int *values, size_t n, int *no_keys, size_t m)
{
    assert( keys );
    assert( values );
    assert( no_keys );

    return test_ht(keys, sizeof keys[0], values, sizeof values[0], n, no_keys, m, upo_ht_hash_int_div, upo_test_int_cmp, upo_test_int_to_string, upo_test_int_to_string);
}

int test_str_ht(char **keys, int *values, size_t n, char **no_keys, size_t m)
{
    assert( keys );
    assert( values );
    assert( no_keys );

    return test_ht(keys, sizeof keys[0], values, sizeof values[0], n, no_keys, m, upo_ht_hash_str_kr2e, upo_test_str_cmp, upo_test_str_to_string, upo_test_int_to_string);
}

int test_ht(void *keys, size_t key_sz, void *values, size_t value_sz, size_t n, void *no_keys, size_t m, upo_ht_hasher_t key_hasher, upo_ht_comparator_t key_cmp, char* (*key_to_string)(const void*), char* (*value_to_string)(const void*))
{
    upo_ht_sepchain_t ht = NULL;
    size_t base_cap = 3;
    size_t caps[4];
    size_t nc;
    unsigned char *pck = keys;
    unsigned char *pcv = values;
    unsigned char *pcnk = no_keys;
    int ret = TEST_OK;

/*
    caps[0] = 2*n+UPO_HT_SEPCHAIN_DEFAULT_CAPACITY;
    caps[1] = n*UPO_HT_SEPCHAIN_DEFAULT_CAPACITY;
    caps[2] = UPO_HT_SEPCHAIN_DEFAULT_CAPACITY;
*/
    caps[0] = 3*n+base_cap;
    caps[1] = 2*n+base_cap;
    caps[2] = n*base_cap;
    caps[3] = base_cap;
    nc = sizeof caps/sizeof caps[0];

    for (size_t k = 0; k < nc && ret == TEST_OK; ++k)
    {
        ht = upo_ht_sepchain_create(caps[k], key_hasher, key_cmp);

        assert( ht != NULL );

        if (n > 0)
        {
            // Non-empty HT

            // Populate the source HT
            for (size_t i = 0; i < n; ++i)
            {
                upo_ht_sepchain_put(ht, pck + i*key_sz, pcv + i*value_sz);
            }

            // Check the number of collisions for existing keys
            for (size_t i = 0; i < n && ret == TEST_OK; ++i)
            {
                float perc = 0;
                float check_perc = 0;

                /* Use a different pointer for key to catch comparison-by-pointers issues */

                void *key = NULL;

                key = malloc(key_sz);
                if (key == NULL)
                {
                    perror("Unable to allocate memory for key");
                    abort();
                }

                memcpy(key, pck + i*key_sz, key_sz);

                perc = upo_ht_sepchain_perc_collisions(ht, key);
                check_perc = upo_ht_sepchain_xyz(ht, pck + i*key_sz);

                if (fabs(perc - check_perc) > COLLISION_TOLERANCE)
                {
                    char *key_str = key_to_string(key);
                    fprintf(stderr, "[file: %s, line: %d] ERROR: HT Alternative #: %lu, Key: '%s' -> Expected percentage %f, got %f.\n", __FILE__, __LINE__, k, key_str, check_perc, perc);
                    fprintf(stderr, "[file: %s, line: %d]: <HT>\n", __FILE__, __LINE__);
                    upo_ht_sepchain_dump(ht, stderr, key_to_string, 1, value_to_string, 1);
                    fprintf(stderr, "[file: %s, line: %d]: </HT>\n", __FILE__, __LINE__);
                    fflush(stderr);
                    free(key_str);
                    ret = TEST_FAIL;
                }

                free(key);
            }

            // Check the percentage of collisions for non existing keys
            for (size_t i = 0; i < m && ret == TEST_OK; ++i)
            {
                float perc = 0;
                float check_perc = 0;

                void *key = NULL;

                key = malloc(key_sz);
                if (key == NULL)
                {
                    perror("Unable to allocate memory for non key");
                    abort();
                }

                memcpy(key, pcnk + i*key_sz, key_sz);

                perc = upo_ht_sepchain_perc_collisions(ht, key);
                check_perc = upo_ht_sepchain_xyz(ht, key);

                if (fabs(perc - check_perc) > COLLISION_TOLERANCE)
                {
                    char *key_str = key_to_string(pcnk + i*key_sz);
                    fprintf(stderr, "[file: %s, line: %d] ERROR: HT Alternative #: %lu, Non key: '%s' -> Expected percentage %f, got %f.\n", __FILE__, __LINE__, k, key_str, check_perc, perc);
                    fprintf(stderr, "[file: %s, line: %d]: <HT>\n", __FILE__, __LINE__);
                    upo_ht_sepchain_dump(ht, stderr, key_to_string, 1, value_to_string, 1);
                    fprintf(stderr, "[file: %s, line: %d]: </HT>\n", __FILE__, __LINE__);
                    fflush(stderr);
                    free(key_str);
                    ret = TEST_FAIL;
                }

                free(key);
            }
        }

        upo_ht_sepchain_destroy(ht, 0);
    }

    return ret;
}
