/* vim: set tabstop=4 expandtab shiftwidth=4 softtabstop=4: */

/**
 * \file upo/bst.h
 *
 * \brief The Binary Search Tree (BST) abstract data type.
 *
 * Trees are nonlinear containers where data is structured according to
 * hierarchical organization.
 * Trees are made of nodes that are connected to each other according to a
 * parent-child relationship.
 * Binary Search Trees are Binary Trees where:
 * - Each node has a key and an associated value (possibly, the
 *   key itself),
 * - The key in node v is greater than the keys in all nodes in the
 *   left subtree of v, and
 * - The key in node v is less than the keys in all nodes in the right
 *   subtree of v.
 * .
 *
 * \author Anna Sapienza (anna.sapienza@uniupo.it)
 *
 * \copyright 2015 University of Piemonte Orientale, Computer Science Institute
 *
 *  This file is part of UPOalglib developed by Marco Guazzone, Università del Piemonte Orientale.
 *
 *  UPOalglib is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  UPOalglib is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with UPOalglib.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef UPO_BST_H
#define UPO_BST_H


#include <stddef.h>


/**
 * \brief The type for key comparison functions.
 *
 * Declares the type for key comparison functions that are used to compare keys
 * stored in the binary search tree.
 * A comparison function takes two parameters:
 * - The first parameter is a pointer to the first key to compare.
 * - The second parameter is a pointer to the second key to compare.
 * A comparison function returns a number less than, equal to, or greater than
 * zero if the first key (first argument) is less than, equal to, or greater
 * than the second key (second argument), respectively.
 */
typedef int (*upo_bst_comparator_t)(const void*, const void*);

/** \brief Alias for binary search tree node type. */
typedef struct upo_bst_node_s upo_bst_node_t;

/** \brief Type for nodes of a binary search tree. */
struct upo_bst_node_s
{
    void *key; /**< Pointer to user-provided key. */
    void *value; /**< Pointer to user-provided value. */
    upo_bst_node_t *left; /**< Pointer to the left child node. */
    upo_bst_node_t *right; /**< Pointer to the right child node. */
};

/** \brief Defines a binary tree. */
struct upo_bst_s
{
    upo_bst_node_t *root; /**< The root of the binary tree. */
    upo_bst_comparator_t key_cmp; /**< Pointer to the key comparison function. */
};

/** \brief Declares the Binary Search Tree type. */
typedef struct upo_bst_s* upo_bst_t;

/**
 * \brief Creates a new empty binary search tree.
 *
 * \param key_cmp A pointer to the function used to compare keys.
 * \return An empty binary search tree.
 *
 * Worst-case complexity: constant, `O(1)`.
 */
upo_bst_t upo_bst_create(upo_bst_comparator_t key_cmp);

/**
 * \brief Destroys the given binary search tree together with data stored on it.
 *
 * \param tree The binary search tree to destroy.
 * \param destroy_data Tells whether the previously allocated memory for keys
 *  and values stored in this binary search tree must be freed (value `1`) or
 *  not (value `0`).
 *
 * Memory deallocation (if requested) is performed by means of the `free()`
 * standard C function.
 *
 * Worst-case complexity: linear in the number `n` of elements, `O(n)`.
 */
void upo_bst_destroy(upo_bst_t tree, int destroy_data);

/**
 * \brief Removes all elements from the given binary search tree and destroys
 *  all data stored on it.
 *
 * \param tree The binary search tree to clear.
 * \param destroy_data Tells whether the previously allocated memory for keys
 *  and values stored in this binary search tree must be freed (value `1`) or
 *  not (value `0`).
 *
 * Memory deallocation (if requested) is performed by means of the `free()`
 * standard C function.
 *
 * Worst-case complexity: linear in the number `n` of elements, `O(n)`.
 */
void upo_bst_clear(upo_bst_t tree, int destroy_data);

/**
 * \brief Tells if the given binary search tree is empty.
 *
 * \param tree The binary search tree.
 * \return `1` if the binary search tree is empty or `0` otherwise.
 *
 * A binary search tree is empty if it doesn't contain any node.
 *
 * Worst-case complexity: constant, `O(1)`.
 */
int upo_bst_is_empty(const upo_bst_t tree);

/**
 * \brief Returns the number of nodes stored on the given binary search tree.
 *
 * \param tree The binary search tree.
 * \return The number of nodes of the given binary search tree.
 *
 * Worst-case complexity: linear in the number `n` of elements, `O(n)`.
 */
size_t upo_bst_size(const upo_bst_t tree);
/**
 * \brief Find the Lowest Common Ancestor (LCA) of two given keys.
 *
 * \param tree The binary search tree.
 * \param key1 The first key value.
 * \param key2 The second key value.
 * \return The key value of the Lowest Common Ancestor (LCA) of key1 and key2 (if any), 
 * or NULL if the tree is empty or key1 and/or key2 are not present in the tree
 *
 * This function  finds the Lowest Common Ancestor of two given keys, if they are present in the tree.
 * 
 * Worst-case complexity: linear in the number `n` of elements, `O(n)`.
 */
void *upo_bst_nmin(const upo_bst_t tree, const void *key, const int n);

#endif /* UPO_BST_H */
