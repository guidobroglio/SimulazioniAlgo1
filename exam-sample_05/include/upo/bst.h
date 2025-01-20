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
 * - Each node has a key and an associated list of values, to handle duplicate
 *   keys,
 * - The key in node v is greater than the keys in all nodes in the
 *   left subtree of v, and
 * - The key in node v is less than the keys in all nodes in the right
 *   subtree of v.
 * .
 *
 * \author Marco Guazzone (marco.guazzone@uniupo.it)
 *
 * \copyright 2015 University of Piemonte Orientale, Computer Science Institute
 *
 *  This file is part of UPOalglib.
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

/** \brief Alias for node type of a list of values stored in a binary search tree node. */
typedef struct upo_bst_value_list_node_s upo_bst_value_list_node_t;

/** \brief Type for nodes of the list of values stored in a binary search tree node. */
struct upo_bst_value_list_node_s
{
    void *value;
    struct upo_bst_value_list_node_s *next;
};

/** \brief Alias for binary search tree node type. */
typedef struct upo_bst_node_s upo_bst_node_t;

/** \brief Type for nodes of a binary search tree. */
struct upo_bst_node_s
{
    void *key; ///< Pointer to user-provided key.
    upo_bst_value_list_node_t *values; ///< Pointer to user-provided value.
    //size_t num_values; ///< Number of values.
    upo_bst_node_t *left; ///< Pointer to the left child node.
    upo_bst_node_t *right; ///< Pointer to the right child node.
};

/** \brief Defines a binary tree. */
struct upo_bst_s
{
    upo_bst_node_t* root; ///< The root of the binary tree.
    upo_bst_comparator_t key_cmp; ///< Pointer to the key comparison function.
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
 * \brief Returns the comparison function stored in the binary search tree.
 *
 * \param tree The binary search tree.
 * \return The comparison function.
 */
upo_bst_comparator_t upo_bst_get_comparator(const upo_bst_t tree);

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
 * \brief Inserts the given value identified by the provided key in the given
 *  binary search tree handling possible duplicates.
 *
 * \param tree The binary search tree.
 * \param key The key.
 * \param value The value.
 *
 * If the key is already present in the tree, the value provided as argument is
 * added to the list of values already stored in the tree.
 */
void upo_bst_insert(upo_bst_t tree, void *key, void *value);


#endif /* UPO_BST_H */
