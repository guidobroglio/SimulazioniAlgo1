#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/upo/bst.h"
#include "include/upo/sort.h"
#include "include/upo/hashtable.h"
//#include "include/upo/bst_key_list.h"

/*BST non è vuoto. Se il BST non è vuoto, l’algoritmo deve ritornare un valore diverso da 0,
altrimenti deve ritornare 0. caso peggiore dev’essere O(1).
int upo_bst_is_not_empty(const upo_bst_t bst)
{	//fprintf(stderr, "To be implemented!\n");
	//abort();
	return (bst == NULL || bst->root == NULL) ? 0 : 1;
}
---------------------------------------------------
/*25 giugno 20.Ogni nodo del BST, anziché contenere una chiave k e il suo valore associato v,
 contiene una chiave k e una lista di valori [v1; v2; vnk ] associati alla chiave k.
 se il BST non contiene alcun nodo associato alla chiave k, se ne crea uno nuovo,
 memorizzandovi la chiave k e la lista, se nel BST c’è già un nodo associato alla chiave k
lo si aggiorna, aggiungendo il nuovo valore nella lista dei valori
Implementare un algoritmo che, dato un BST con l’estensione suddetta e una coppia
chiave-valore inerisca <k,v>*/
 
upo_bst_node_t *upo_bst_node_create(void *key, void *value);
typedef struct upo_bst_value_list_node_s {
	void *value;
	struct upo_bst_value_list_node_s *next;
} upo_bst_value_list_node_t;

upo_bst_value_list_node_t *upo_bst_add_list(upo_bst_value_list_node_t *list, void *value);
upo_bst_node_t *upo_bst_insert_impl(upo_bst_node_t *n, void *key, void *value, upo_bst_comparator_t comp);
 
void upo_bst_insert(upo_bst_t bst, void *key, void *value) 
{
	if(bst == NULL)
    {
        return;
    }
	bst->root = upo_bst_insert_impl(bst->root, key, value, bst->key_cmp); 
}
 
upo_bst_node_t *upo_bst_node_create(void *key, void *value) 
{
	upo_bst_node_t *node = calloc(1, sizeof(upo_bst_node_t));
	if(node == NULL) //{ perror("Impossibile creare il nodo");
	{
		return NULL;                           	    //abort();
	}
	node->key = key; 	
	node->right = NULL; 	
	node->left = NULL;
	return node;
}

upo_bst_value_list_node_t *upo_bst_add_list(upo_bst_value_list_node_t *list, void *value) 
{
	upo_bst_value_list_node_t *values = calloc(1, sizeof(upo_bst_value_list_node_t));
	values->value = value;
	values->next = list;
	list = values;
	return values;
}
 
upo_bst_node_t *upo_bst_insert_impl(upo_bst_node_t *n, void *key, void *value, upo_bst_comparator_t comp) 
{
	if(n == NULL) return upo_bst_node_create(key, value);
	if(comp(key, n->key) < 0)
    	n->left = upo_bst_insert_impl(n->left, key, value, comp);
	else if(comp(key, n->key) > 0)
    	n->right = upo_bst_insert_impl(n->right, key, value, comp);
	else 
    {    
        n->values = upo_bst_add_list(n->values, value); 	
    }
	return n; 
}


upo_bst_node_t *upo_bst_insert_impl(upo_bst_node_t *n, void* key, void *value, upo_bst_comparator_t cmp);
void upo_bst_insert(upo_bst_t bst, void *key, void *value)
{
    assert(bst != NULL);
    bst->root = upo_bst_insert_impl(bst->root, key, value, bst->key_cmp);
}
 
upo_bst_node_t *upo_bst_node_create(void* key, void* value)
{	
    upo_bst_node_t *n = malloc(sizeof(upo_bst_node_t));
	if(n == NULL) abort();
	n->key = key;
	n->left = NULL;
	n->right = NULL;
	upo_bst_value_list_node_t *list = malloc(sizeof(upo_bst_value_list_node_t));
	if(list == NULL) abort();
	list->value = value;
	list->next = NULL;
	n->values = list; //CHANGED
	return n;
}
 
upo_bst_node_t *upo_bst_insert_impl(upo_bst_node_t *n, void* key, void *value, upo_bst_comparator_t cmp)
{
	if(n == NULL) return upo_bst_node_create(key, value);
	int compare = cmp(key, n->key);
	if(compare < 0) 
    {
    	n->left = upo_bst_insert_impl(n->left, key, value, cmp); 
    }
	else if(compare > 0)
    	n->right = upo_bst_insert_impl(n->right, key, value, cmp);
	else
	{   
        upo_bst_value_list_node_t *l = n->values;
    	while(l != NULL)
    	{ 
            if(l->next == NULL)
        	{ 
                upo_bst_value_list_node_t *newNode = malloc(sizeof(upo_bst_value_list_node_t));
           	    if(newNode == NULL) abort();
            	    newNode->value = value;
            	    newNode->next = NULL;
            	    l->next = newNode;
            	    break;
            } 
        }
    }
	return n;
}
//---------------------------------------------------
 
/* Dati in input un albero binario di ricerca BST e una chiave k, implementare un algoritmo che dica se la chiave k e' contenuta nel BST e che calcoli la profondita' del nodo che contiene k. In particolare:
 * se la chiave k e' contenuta nel BST, l'algoritmo deve ritornare True (valore intero diverso da zero)
e deve calcolare la profondità del nodo in cui è memorizzata la chiave k.
 * se la chiave k non e' contenuta nel BST o se il BST e' vuoto, l'algoritmo deve ritornare False (il valore intero 0)  e come profondita' deve ritornare il valore intero -1.
 */
int upo_bst_contains_depth_impl(upo_bst_node_t *node, void *key, long *depth, upo_bst_comparator_t comp);
 
int upo_bst_contains_depth(const upo_bst_t bst, void *key, long *depth) 
{
	if(bst == NULL) return -1;
	*depth = 0;
	return upo_bst_contains_depth_impl(bst->root, key, depth, bst->key_cmp);
}
 
int upo_bst_contains_depth_impl(upo_bst_node_t *node, void *key, long *depth, upo_bst_comparator_t comp) 
{
	if(node == NULL) 
    {
    	*depth = -1;
    	return 0;
	}
	if(comp(node->key, key) < 0) 
    {
    	*depth += 1;
    	return upo_bst_contains_depth_impl(node->left, key, depth, comp);
	}
	else if(comp(node->key, key) > 0) 
    {
    	*depth += 1;
    	return upo_bst_contains_depth_impl(node->right, key, depth, comp);
	}
	else 
    {
    	depth += 1;
    	return 1; 
    }
}
 
//--------------------------------------------------------------------
/* luglio20.Dati in input un albero binario di ricerca (BST) e una chiave k,
implementare un algoritmo che restituisca il numero di nodi interni
del sotto-albero radicato in k.*/
size_t upo_bst_subtree_count_inner(const upo_bst_t bst, const void *key) 
{
	if(bst == NULL) return 0;
    size_t res = 0;
    upo_bst_subtree_count_inner_impl(bst->root, key, &res);
	return res;
}
 
void upo_bst_subtree_count_inner_impl(upo_bst_node_t *n, void *key, size_t inners) 
{
	if(n == NULL) return;
	if(n->left && n->right == NULL) return;
	(*inners)++;
    upo_bst_subtree_count_inner_impl(n->left, key, inners);
    upo_bst_subtree_count_inner_impl(n->right, key, inners);
}
//si calcoli la profondità di un range di nodi
long upo_bst_min_depth(const upo_bst_t bst) 
{
	if(bst == NULL)
    	return -1;
	long depth = -1;
	even_depth_lh(bst->root, &depth);
	return depth;   
}
 
void even_depth_lh(const upo_bst_node_t *node, long depth) 
{
	if(node == NULL)
    	return;
	(depth)++;
	even_depth_lh(node->left,depth);
}

size_t upo_bst_count_keys_range(const upo_bst_t tree, const void *low_key, const void *high_key)
{ 	
    int cont=0;
	if(tree == NULL)
    	return NULL;
	upo_bst_key_list_t key_list = NULL;
	keys_range_impl(tree->root, low_key, high_key,&cont,tree->key_cmp, &key_list);
	return cont; 
}
 
void keys_range_impl(const upo_bst_node_t *node, const void *lK, const void *hK, int *cont,upo_bst_comparator_t key_cmp, upo_bst_key_list_t key_list)
{	
    if(node == NULL)
    	return;
	if(key_cmp(node->key, hK) > 0) 	{
    	keys_range_impl(node->left, lK, hK, cont,key_cmp, key_list);
	} 
    
    else if(key_cmp(node->key, hK) == 0)	
    {
   	(cont)++;
    	if(key_cmp(node->key, lK) > 0)
        	keys_range_impl(node->left, lK, hK, cont,key_cmp, key_list);
	} else {     	keys_range_impl(node->right, lK, hK,cont, key_cmp, key_list);
    	(*cont)++;
    	if(key_cmp(node->key, lK) > 0)
    	keys_range_impl(node->left, lK, hK,cont, key_cmp, key_list);
	}}
 
/* ESEGUE UNA VISITA IN PROFONDITA' TRASVERSALE DEL NODO */
void upo_bst_traverse_in_order(const upo_bst_t tree, upo_bst_visitor_t visit, void *visit_arg) 
{
	if(tree->root == NULL)
    {
        return;
    }
	else
    {
    	upo_bst_traverse_in_order_impl(tree->root, visit, visit_arg);
    }
}

void upo_bst_traverse_in_order_impl (upo_bst_node_t *n, upo_bst_visitor_t visit, void *visit_arg) 
{
	if(n == NULL)
    {
        return;
    }
	else 
    {        
        upo_bst_traverse_in_order_impl(n->left, visit, visit_arg);
    	visit(n->key, n->value, visit_arg);
    	upo_bst_traverse_in_order_impl(n->right, visit, visit_arg);	
    }   	
}
 
/* RICERCA DELLA CHIAVE MINIMA IN UN ALBERO */
void *upo_bst_min(const upo_bst_t tree) 
{
	if(tree->root == NULL)
    {
        return NULL;
    }
	upo_bst_node_t *n = upo_bst_min_impl(tree->root);
	if(n == NULL)
    {
        return NULL;
    }
	return n->key; 
}

upo_bst_node_t *upo_bst_min_impl(upo_bst_node_t *n) 
{
	if(n == NULL)
    {
        return NULL;
    }
	else if (n->left == NULL)
    {
        return n;
    }
	else
    {
        return upo_bst_min_impl(n->left);
    }  
}
 
/* RICERCA DELLA CHIAVE MASSIMA IN UN ALBERO */
void *upo_bst_max(const upo_bst_t tree) 
{
	if(tree->root == NULL)
    {
        return NULL;
    }
	upo_bst_node_t *n = upo_bst_max_impl(tree->root);
	if(n == NULL)
    {
        return NULL;
    }
	return n->key; 
}

upo_bst_node_t *upo_bst_max_impl(upo_bst_node_t *n) 
{
	if(n == NULL)
    {
        return NULL;
    }
	else if(n->right == NULL)
    {
        return n;
    }
	else
    {
        return upo_bst_max_impl(n->right);
    }              	
}
 
/* ELIMINAZIONE DELLA CHIAVE MINIMA */
void upo_bst_delete_min(upo_bst_t tree, int destroy_data) 
{
	if(tree->root == NULL)
    {
        return;
    }
	upo_bst_delete(tree, upo_bst_min(tree), destroy_data);  
}
 
 
/* ELIMINAZIONE DELLA CHIAVE MASSIMA */
void upo_bst_delete_max(upo_bst_t tree, int destroy_data) 
{
	if(tree->root == NULL)
    {
        return;
    }
	upo_bst_delete(tree, upo_bst_max(tree), destroy_data);
}

/* LA CHIAVE PIU' GRANDE TRA QUELLE MINORI */
void *upo_bst_floor(const upo_bst_t tree, const void *key) 
{
	if(tree->root == NULL)
    {
        return NULL;
    }
	upo_bst_node_t *n = upo_bst_floor_impl(tree->root, key, tree->key_cmp);
	if(n != NULL)
    {
        return n->key;
    }
    return NULL; 
}

upo_bst_node_t *upo_bst_floor_impl(upo_bst_node_t *n, void *key, upo_bst_comparator_t comp) 
{
	if(n == NULL)
    {
        return NULL;
    }
	int cmp = comp(key, n->key);
	if(cmp < 0)
    {
        return upo_bst_floor_impl(n->left, key, comp);
    }
	else if(cmp > 0) 
    {
    	upo_bst_node_t *fNode = upo_bst_floor_impl(n->right, key, comp);
    	if(fNode != NULL)
        {
            return fNode;
        }
    	else
        {
            return n;
        }	
    }
	else
    {
        return n;
    }
	return NULL;  
}
 
/* LA CHIAVE PIU' GRANDE TRA QUELLE MAGGIORI */
void *upo_bst_ceiling(const upo_bst_t tree, int destroy_data) 
{
	if(tree->root == NULL)
    {
        return NULL;
    }
	upo_bst_node_t *n = upo_bst_ceiling_impl(tree->root, key, tree->key_cmp);
	if(n != NULL)
    {
        return n->key;
    }
	return NULL; 
}

upo_bst_node_t * upo_bst_ceiling_impl (upo_bst_node_t *n, void *key, upo_sort_comparator_t comp) 
{
	if(n == NULL)
    {
        return NULL;
    }
	int cmp = comp(key, n->key);
	if(cmp < 0)
    {
        upo_bst_node_t *cNode = upo_bst_ceiling_impl(n->left, key, comp);
    }
	else if(cmp > 0) 
    {
        return upo_bst_ceiling_impl(n->right, key, comp);
    	if(cNode != NULL)
        {
            return cNode;
        }
    	else
        {
            return n;
        }
	}   
    else
    {
        return n;
    }
	return NULL; 
}
 
/* RITORNA LE CHIAVI CHE SONO DENTRO AL RANGE PASSATO COME PARAMETRO */
upo_bst_key_list_t upo_bst_keys_range(const upo_bst_t tree, const void *low_key, const void *high_key) 
{
	if(tree == NULL)
    {
        return NULL;
    }
	upo_bst_key_list_t keys = NULL;
	upo_bst_keys_range_impl(tree->root, low_key, high_key, tree->key_cmp, &keys);
	return keys;  
}

void upo_bst_keys_range_impl(upo_bst_node_t *n, void *low, void *high, upo_sort_comparator_t comp, upo_bst_key_list_t *keys) 
{
	if(n == NULL)
    {
        return;
    }
	upo_bst_keys_range_impl(n->left, low_key, high_key, comp, keys);
	if(comp(n->key, low) >= 0 && comp(n->key, high) <= 0) 
    {
    	upo_bst_key_list_node_t *node = malloc(sizeof(upo_bst_key_list_node_s));
    	if(node == NULL)
        {
            return;
        }
    	node->key = n->key;
    	node->next = *keys;
    	*keys = node; 	
    }
	upo_bst_keys_range_impl(n->right, low_key, high_key, comp, keys);  
}
 
/* RITORNA TUTTE LE CHIAVI DELL'ALBERO*/
upo_bst_key_list_t upo_bst_keys(const upo_bst_t tree) 
{
	if(tree->root == NULL)
    {
        return NULL;
    }
	upo_bst_key_list_t keys = NULL;
	upo_bst_keys_impl(tree->root, tree->key_cmp, &keys);
	return keys;	
}

void upo_bst_keys_impl(upo_bst_node_t *n, upo_bst_comparator_t comp, upo_bst_key_list_t *keys) 
{
	if(n == NULL)
    {
        return NULL;
    }
	upo_bst_keys_impl(n->left, comp, keys);
	upo_bst_key_list_node_t *node = malloc(sizeof(upo_bst_key_list_node_s));
	if(node == NULL)
    {
        return NULL;
    }    
    node->key = n->key;
	node->next = *keys;
	*keys = node;
	upo_bst_keys_impl(n->right, comp, keys); 	
}

/* VERIFICA SE LA CHIAVE DATA SODDISFA LA PROPRIETA' DELL'ALBERO DI RICERCA */
int upo_bst_is_bst(const upo_bst_t tree, const void *min_key, const void *max_key) 
{
	if(upo_bst_is_empty(tree))
    {
        return 1;
    }
	return upo_bst_is_bst_impl(tree->root, min_key, max_key, tree->key_cmp);  
}

int upo_bst_is_bst_impl (upo_bst_node_t *n, const void *min_key, const void *max_key, upo_sort_comparator_t comp) 
{
	if(n == NULL)
    {
        return 1;
    }
	if(comp(n->key, min_key) < 0 || comp(n->key, max_key) > 0)
    {
        return 0;
    }
	return upo_bst_is_bst_impl(n->left, min_key, max_key, comp) && upo_bst_is_bst_impl(n->right, min_key, max_key, comp); 
}
 
/* Calcolare il rango di k*/
size_t upo_bst_rank(const upo_bst_t tree, const void *key) 
{
	if(tree == NULL)
    {
        return 0;
    }
	size_t res = 0;
	upo_bst_rank_impl(tree->root, key, tree->key_cmp, &res);
	return res;  
}

void upo_bst_rank_impl(upo_bst_node_t *n, void *key, upo_bst_comparator_t comp, size_t *rank) 
{
	if(n == NULL) 
    {
    	return;
    }
	if(comp(n->key, key) < 0) 
    {
    	*rank += 1;
    	upo_bst_rank_impl(n->left, key, comp, rank);
    	if(n->right != NULL && comp(n->right->key, key) < 0)
        {
            upo_bst_rank_impl(n->right, key, comp, rank);
        }
    }
}
 
/* calcolare il predecessore */
void* upo_bst_predecessor(const upo_bst_t tree, const void *key) 
{
	if(tree == NULL)
    {
        return NULL;
    }
	upo_bst_node_t *n = NULL;
	upo_bst_predecessor_impl(tree->root, n, key, tree->key_cmp)
	return n->key; 
}

upo_bst_node_t *upo_bst_node_max(upo_bst_node_t *n) 
{
	while(n->right != NULL)
    {
        n = n->right;
    }
	return n;  
}
 
void upo_bst_predecessor_impl(upo_bst_node_t *n, upo_bst_node_t *pred, void *key, upo_bst_comparator_t comp) 
{
	if(n == NULL)
    {
        return;
    }
	if(comp(n->key, key) == 0)
    {
    	pred = upo_bst_node_max(n->left);
    }
	else if(comp(n->key, key) > 0) 
    {
    	upo_bst_predecessor_impl(n->left, pred, key, comp); 	
    }
	else 
    {     	
        pred = n;
    	upo_bst_predecessor_impl(n->right, pred, key, comp); 	
    } 
}
 
/* calcolare valore e PROFONDITà di un nodo */
void* upo_bst_get_value_depth(const upo_bst_t tree,const void*key, long*depth) 
{
	if(tree == NULL)
    {
        return NULL;
    }
    void *res = upo_bst_get_value_depth_impl(tree->root, key, depth, tree->key_cmp);
	*depth = res == NULL ? -1 : *depth;
	return res; 
}

void* upo_bst_get_value_depth_impl (upo_bst_node_t *n, void *key, long*depth, upo_bst_comparator_t cmp) 
{
	if(n == NULL)
    {
        return NULL;
    }
	if(comp(n->key, key) == 0)
    {
        return n->value;
    }
	else if(comp(n->key, key) > 0) 
    {     	
        *depth += 1;
    	return upo_bst_get_value_depth_impl (n->left, key, depth, cmp); 	
    }
	else 
    {     	
        *depth += 1;
        return upo_bst_get_value_depth_impl (n->right, key, depth, cmp);     
    }  
}
 
/* calcolare la lista delle chiavi che sono minori o uguali alla chiave data*/
upo_bst_key_list_t upo_bst_keys_le(const upo_bst_t tree,const void*key)
{
 	if(tree->root == NULL)
    {
        return NULL;
    }
	upo_bst_key_list_t list = NULL;
	upo_bst_keys_le_impl(tree->root, key, tree->key_cmp, &list);
	return list; 
}

void upo_bst_keys_le_impl (upo_bst_node_t *n, void *key, comparator_t comp, upo_bst_key_list_t keys) 
{
	if(n == NULL)
    {
        return NULL;
    }
	upo_bst_keys_le_impl(n->left, key, comp, keys);
	if(comp(n->key, key) < 0) 
    {
    	upo_bst_key_list_node_t *node = malloc(sizeof(struct upo_bst_key_list_node_s));
    	if(node == NULL)
        {
            return NULL;
        }
    	node->key = n->key;
    	node->next = *keys;
    	*keys = node; 	
    }
	upo_bst_keys_le_impl(n->right, key, comp, keys); 
}

upo_bst_t  upo_bst_create(upo_bst_comparator_t key_cmp) 
{
	upo_bst_t tree =  malloc(sizeof(struct upo_bst_s));
	if( tree == NULL)
	{
        perror("Errore");
        about();                          	
	}
	tree-> root = NULL;
	tree->key_cmp;
	return tree; 
}

upo_bst_node_t* upo_bst_node_create(void* k, void* v)  
{  
	upo_bst_node_t *n = malloc (sizeof(struct upo_bast_node_s));
	if(n == NULL)
	{
		retun NULL;
	}
	else 
	{ 
		n->key = k;
		n-> value = v;
		n->left = NULL;
		n-> right = NULL; 
	}   
	return n; 
}
______________________________________________

void upo_bst_destroy(upo_bst_t tree, int destroy_data)
{ 
	if (tree != NULL ) 
	{
		upo_bst_clear(tree, destroy_data);
 		free(tree);         
	}   
}

void upo_bst_clear(upo_bst_t tree, int destroy_data)
{ 
	if (tree != NULL) 
	{
     	upo_bst_clear_impl ( tree-> root, destroy_data);
     	tree->root = NULL;
	}   
}

void upo_bst_clear_impl ( upo_bst_node_t *n, int destroy_data)
{ 
	if( n != NULL) 
	{
     	upo_bst_clear_impl (n->left, destroy_data);
     	upo_bst_clear_impl  (n->left, destroy_data);
     	if(destroy_data) 
		{
     		free(n-> key);
     		free(n->value);  
		}
		free (n); 
	}          	
}
______________________________________________

void* upo_bst_put (upo_bst_t* tree, void* key, void* value) 
{   
	if( tree == NULL)
	{
		return NULL;
	}

	upo_bst_node_t *vold =  NULL;
	tree-> root = upo_bst_put_impl (tree-> root, key, value, vold, tree->key_cmp);
	return vold; 
}

upo_bst_node_t* upo_bst_put_impl (upo_bst_node_t* n, void* k, void* v, void* vold upo_bst_comparator_t comp)
{
 	vold = NULL;
 	if(n == NULL) 
	{
		return upo_bst_node_create(k, v);
	}
	else if( comp( n->key, k) > 0)
	{
     	n -> left = upo_bst_put_impl (n->left, k, v, vold, comp);
	}
	else if( comp( n->key, k) <  0)
    {
		n -> right = upo_bst_put_impl (n->right, k, v, vold, comp);
	}
	else 
	{  	
		vold = n-> value;
     	n-> value = v; 
	}
	return n; 
}
______________________________________________

void* upo_bst_insert (upo_bst_t* tree, void* key, void* value) 
{ 
	if( tree == NULL) return null;
	tree-> root = upo_bst_node_t* upo_bst_insert_imp (tree->root, key, value, tree->key_cmp);
}

Upo_bst_node_t* upo_bst_insert_impl  (upo_bst_node_t* n, void* k, void* v, upo_bst_comparator_t comp)    
{
	if (n == null) 
	{
		return upo_bst_node_create(k, v);
	}
	else if ( comp ( n-> key, k) > 0)  
	{
		n-> left = upo_bst_insert_impl(n->left, k, v, comp);
	}
	else if ( comp ( n-> key, k) < 0)  
	{
		n-> right = upo_bst_insert_impl(n->right, k, v, comp);
	}
	return n; 
}

Void* upo_bst_get (upo_bst_t tree, const void *key) 
{
	If ( tree == NULL) 
	{
		retun NULL;
	}
	upo_bst_node_t *n = upo_bst_get_impl (tree->root, key, tree-> key_cmp);
	return n == NULL ? NULL : n->value; 
}
 
upo_bst_node_t* upo_bst_get_impl (upo_bst_node_t *n, const void *key, upo_bst_comparator_t comp)
{
    if(n == NULL) 
	{
		return NULL;
	}
	else if ( comp (n->key, key) > 0) 	
	{
		return  upo_bst_get_impl (n->left, key, comp);
	}
	else if ( comp (n->key, key) < 0) 	
	{
		return  upo_bst_get_impl (n->right, key, comp);
	}
	else   
	{
		return n;   
	}
}
______________________________________________
int upo_bst_contains(const upo_bst_t tree, const void *key)
{     
	if(tree == NULL) 
	{
		return 0;
	}
    if(upo_bst_get_impl(tree->root, key, tree->key_cmp) != NULL) 
	{
		return 1;
	}
    return 0;      	
}
 
int upo_bst_is_leaf(upo_bst_node_t* n)	
{
    if(n != NULL) 
	{
		return n->left == NULL && n->right == NULL;
	}
    return 1; 	
}

size_t upo_bst_size(const upo_bst_t tree)
{    	 
	if(tree != NULL) 
	{
		return upo_bst_size_impl(tree->root);
	}
    return 0;      	
}

size_t upo_bst_size_impl(upo_bst_node_t* n)
{
    if(n == NULL) 
	{
		return 0;
	}
    return 1 + upo_bst_size_impl(n->left) + upo_bst_size_impl(n->right);	
}

 
void upo_bst_delete(upo_bst_t tree, const void *key, int destroy_data)
{   
	if(tree != NULL)
	{
 		tree->root = upo_bst_delete_impl(tree->root, key, tree->key_cmp, destroy_data);   
	}
 }

upo_bst_node_t* upo_bst_delete_impl(upo_bst_node_t* n, const void* k, upo_bst_comparator_t key_cmp, int destroy_data)         	
{
	if(n == NULL) 
	{
		return NULL;
	}
	else if(key_cmp(k, n->key) < 0)	
	{
		n->left = upo_bst_delete_impl(n->left, k, key_cmp, destroy_data);
	}
	else if(key_cmp(k, n->key) > 0) 
	{
		n->right = upo_bst_delete_impl(n->right, k, key_cmp, destroy_data);
	}
	else if(n->left != NULL && n->right != NULL) 
	{
		n = upo_bst_delete2(n, key_cmp, destroy_data);
	}
	else 
	{
		n = upo_bst_delete1(n, destroy_data);
	}
	return n;
}

upo_bst_node_t* upo_bst_delete1(upo_bst_node_t* n, int destroy_data)
{
    upo_bst_node_t* x = n;
    if(n->left != NULL) 
	{
		n = n->left;
	}
    else 
	{
		n = n->right;
	}
    upo_bst_destroy_node(x, destroy_data);
    return n;      	
}
 
upo_bst_node_t* upo_bst_delete2(upo_bst_node_t* n, upo_bst_comparator_t key_cmp, j destroy_data)
{
    upo_bst_node_t* m =  t(n->left); 
    n->key = m->key;ttyyt
    n->value = m->value;
    n->left = upo_bst_delete_impl(n->left, m->key, key_cmp, destroy_data);
    return n;      	
}

upo_bst_node_t* upo_bst_max_impl(upo_bst_node_t* n)
{
    if(n == NULL) 
	{
		return NULL;
	}

    if(n->right != NULL) 
	{
		return upo_bst_max_impl(n->right);
	}
    else 
	{
		return n;  
	}
}

void upo_bst_destroy_node (upo_bst_node_t* n, int destroy_data ) 
{
	if (destroy_data) 
	{ 
		free (n->key);
     	free (n-> value)  
	}
	free (n);           	 
}

//ALTEZZA RAMO Più LUNGO
size_t upo_bst_height(const upo_bst_t tree)
{   
	if(tree != NULL) 
	{
		return upo_bst_height_impl(tree->root);
	}
    return 0;      	
}

size_t upo_bst_height_impl(upo_bst_node_t* n)
{
	if(n == NULL || upo_bst_is_leaf(n)) 
	{
		return 0;
	}

    if(upo_bst_height_impl(n->left) >= upo_bst_height_impl(n->right))
	{
      	return 1 + upo_bst_height_impl(n->left);
	}
	else 
	{
		return 1 + upo_bst_height_impl(n->right);  
	}
}

void upo_bst_traverse_in_order(const upo_bst_t tree, upo_bst_visitor_t visit, void *visit_arg)
{     
	if(tree != NULL) 
	{
		upo_bst_traverse_in_order_impl(tree->root, visit, visit_arg);  	
	}
}

void upo_bst_traverse_in_order_impl(upo_bst_node_t* n, upo_bst_visitor_t visit, void* visit_arg)
{
	if(n == NULL) 
	{
		return;
	}
    upo_bst_traverse_in_order_impl(n->left, visit, visit_arg);
    visit(n->key, n->value, visit_arg);
    upo_bst_traverse_in_order_impl(n->right, visit, visit_arg);   
}

int upo_bst_is_bst_impl(upo_bst_node_t* n, const void* mnk, const void* mxk, upo_bst_comparator_t key_cmp)
{
	if(n == NULL) 
	{
		return 1;
	}
	int cmp_min = key_cmp(n->key, mnk);
	int cmp_max = key_cmp(n->key, mxk);
	if(cmp_min < 0 || cmp_max > 0) 
	{
		return 0;
	}
	return upo_bst_is_bst_impl(n->left, mnk, n->key, key_cmp) && upo_bst_is_bst_impl(n->right, n->key, mxk, key_cmp);	
}
 
int upo_bst_is_bst(const upo_bst_t tree, const void *min_key, const void *max_key)	
{
    if(upo_bst_is_empty(tree)) 
	{
		return 1;
	}
	return upo_bst_is_bst_impl(tree->root, min_key, max_key, tree->key_cmp);  
 }

//MAX
void* upo_bst_max(const upo_bst_t tree)
{     
	if(tree == NULL) 
	{
		return NULL;
	}
 	upo_bst_node_t* max = upo_bst_max_impl(tree->root);
    return (min == NULL) ? NULL : max->key;	
}

upo_bst_node_t* upo_bst_max_impl(upo_bst_node_t* n)	
{       
	if(n == NULL) 
	{
		return NULL;
	}

    if(n->right != NULL) 
	{
		return upo_bst_max_impl(n->right);
	}
	else 
	{
		return n;
	}
}
 
//MIN
void* upo_bst_min(const upo_bst_t tree)
{     
	if(tree == NULL) 
	{
		return NULL;
	}
    upo_bst_node_t* min = upo_bst_min_impl(tree->root);
    return (min == NULL) ? NULL : min->key;   
}

upo_bst_node_t* upo_bst_min_impl(upo_bst_node_t* n)	
{       
	if(n == NULL) 
	{
		return NULL;
	}
    if(n->left!= NULL) 
	{
		return upo_bst_min_impl(n->left);
	}
	else 
	{
		return n;   
	}
}

//FLOOR   ritorna la più grande chiave minore = di k (k può nn sistere nel Bst)
void* upo_bst_floor(const upo_bst_t tree, const void *key)
{ 
	if (tree == NULL) 
	{
		return NULL;
	}
	upo_bst_node_t* res = upo_bst_floor_imp (tree-> root, key, tree-> key_cmp);
	return res == NULL ? NULL : res-> key;    	
}
 
upo_bst_node_t * upo_bst_floor_impl (upo_bst_node_t *n, const void* k, upo_bst_comparator_t  comp)
{
	if(n == NULL) 
	{
		return NULL;
	}

	else if(comp (n-> key, k) > 0) 
	{
		return upo_bst_floor_impl (n->left, k, comp);
	}
	else if(comp (n-> key, k) < 0)  
	{
        upo_bst_node_t *res = upo_bst_floor_impl (n->right, k, comp);
        if ( res != NULL)	
		{
			return res;
		}
        else	
		{
			return n;
		}
	} 
	else    
	{
		return n;	
	} 
}


//CEILING  ritorna la più piccola chiave >= a k
void* upo_bst_ceiling (const upo_bst_t tree, const void *key)
{ 
	if (tree == NULL) 
	{
		return NULL;
	}
	upo_bst_node_t *res = upo_bst_ceiling_impl (tree->root, key, tree->key_cmp);
	return res == NULL ? NULL : res-> key;  
}
 
upo_bst_node_t* upo_bst_ceiling_impl (upo_bst_node_t *n, const void* k, upo_bst_comparator_t  comp)
{
	if(n == NULL) 
	{
		return NULL;
	}

	else if (comp (n-> key, k) < 0)  
	{
		return upo_bst_ceiling_impl(n-> right , k , comp) ;
	}
	else if (comp (n-> key, k) > 0)  
	{
  		upo_bst_node_t *node = upo_bst_ceiling_impl(n-> left , k , comp) ;
  		if (node != NULL )  
		{
			return node;
		}
  		else   
		{
			return n;
		}
	} 
	else	
	{
		return n;  
	}  
}

 //PREDECESSORE di k, se contenuta, il predecessore è la chiave k0 < k
void* upo_bst_predecessor(const upo_bst_t tree, const void *key)
{	
	if( tree== NULL || tree->root == NULL )  
	{
		return NULL;
	}

	else 
	{   
		upo_bst_node_t *res = NULL;
	}
	upo_bst_predecessor_imp ( tree->root, res,key, tree->key_cmp);
	return res == NULL ? NULL : res-> key;  
}
 
upo_bst_node_t* upo_bst_predecessor_impl (upo_bst_node_t *n, upo_bst_node_t *pred, const void* k, upo_bst_comparator_t  cmp)  
{
	upo_bst_node_t pred = NULL;
	if (n== NULL) 
	{
		return NULL;
	}
	else if(cmp(n->key, k) == 0) 
	{
    	if(n->left != NULL) 
		{
        	pred = upo_node_max(n->left);	
		}
	}
	else if(cmp(n->key, k) > 0) 	
	{
      upo_bst_predecessor_impl(n->left, pred, k, cmp); 	
	} 
	else  
	{ 
		pred = n;
  		upo_bst_predecessor_impl(n->right, pred, k, cmp); 
	} 
}
 
upo_bst_node_t *upo_node_max(upo_bst_node_t *n)
{ 	
	while(n->right != NULL)    	
	{
		n = n->right;
	}
	return n; 
}

//Ritorna la LISTA di key che sono all’interno di un RANGE
upo_bst_key_list* upo_bst_keys_range (upo_bst_t tree, const void* keyL, const void* keyH ) 
{
	if (tree == NULL)  
	{
		return NULL;
	}
	upo_bst_key_list* list = NULL;
	upo_bst_keys_range_impl  (tree-> root, keyL, keyH, &list, tree-> key_cmp);
	return list;	
}
 
void upo_bst_keys_range_impl(upo_bst_node_t* n, const void* low, const void* high, upo_bst_key_list_t* list, upo_bst_comparator_t comp) 	
{
	if(n == NULL) return;
	upo_bst_keys_range_impl (n->left, low, high, list, comp);
	if (comp(n->key, low) >= 0 && comp(n->key, high) <=0 ) 
	{
   		upo_bst_key_list_node_t* node = malloc(sizeof(upo_bst_key_list_node_t));
		node->key = n->key;
		node->next = *list;
		*list = node; 	
	}
	upo_bst_keys_range_imp(n->right, low,high, list, comp); 
}

 //ritorna la LISTA di tutte le chiavi
upo_bst_key_list_t upo_bst_keys(const upo_bst_t tree)
{ 
	if (tree==NULL) return NULL;
	else  
	{ 
		upo_bst_key_list_t lista = NULL;
   		upo_bst_keys_impl (tree->root, tree->key_cmp, &lista);
   		return lista; 	
	} 
}
 
void upo_bst_keys_impl(upo_bst_node_t *n, upo_bst_comparator_t comp, upo_bst_key_list_t *lista)
{  
	if (n!=NULL)  
	{
    	upo_bst_keys_impl (n->left, comp, lista);
		upo_bst_key_list_node_t *nodoL = 
       	malloc(sizeof(upo_bst_key_list_node_s));
		if (nodoL==NULL)  
		{   
			perror("Error");  
			abort();   
		}
		nodoL->key = n->key;
		nodoL->next = *lista;
		*lista = nodoL;
		upo_bst_keys_impl(n->right, comp, lista);   
	}     
}

/**che ritorni la lista delle chiavi in un albero binario di
ricerca (BST) che sono <=  a una data chiave k. **/
 
upo_bst_key_list_t upo_bst_keys_le(const upo_bst_t tree,const void*key) 
{
    if(upo_bst_is_empty(tree) )  return NULL;
	upo_bst_key_list_t *list = NULL;
    upo_bst_keys_le_impl(tree->root, key, tree->key_cmp, &list);
	return list; 
}
 
void upo_bst_keys_le_impl (upo_bst_node_t *n, void *key, comparator_t comp, upo_bst_key_list_t listK) 
{
	if(n == NULL) return;
    upo_bst_keys_le_impl(n->left, key, comp, listK);
	if(comp(n->key, key) < 0) 
	{
        upo_bst_key_list_node_t *node = malloc(sizeof(struct upo_bst_key_list_node_s));
    	if(node == NULL) return;
    	node->key = n->key;
    	node->next = *keys;
    	* listK = node;
	} 	
	upo_bst_keys_range_impl(n->right, key, comp, keys); 
}
 
//RANGO di una chiave
size_t upo_bst_rank(const upo_bst_t tree, const void *key)  
{   
	if( upo_bst_is_empty(tree) ) 
	{
		return 0;
	}
	size_t res = 0;
	upo_bst_rank_impl(tree->root, key, tree->key_cmp, &res);    	
	return res; 
}
 
void upo_bst_rank_impl(upo_bst_node_t *n, void *key, upo_bst_comparator_t cmp, size_t *rank)
{   
	if(n != NULL)  
	{         
		if(cmp(n->key, key) > 0)
		{   
			*rank += 1;
      		upo_bst_rank_impl(n->left, key, cmp, rank);
  			if(n->right != NULL ,cmp(n->right->key , key) < 0)
          	{
				upo_bst_rank_impl(n->right, key, cmp, rank);
 			} 
			else if (comp(n->key, key) < 0)    	
			{
   				upo_bst_rank_impl(n->right, key, comp, rank); 
			}   
		} 
	}
}

/**valore e profondità di una chiave **/
void* upo_bst_get_value_depth(const upo_bst_t tree,const void*key, long*depth) 
{
    if(upo_bst_is_empty(tree)) 
	{
		return NULL;
	}

	*depth =0;  
 	void * res = upo_bst_get_value_depth_impl(tree->root, key, depth, tree->key_cmp);
	*depth = res == NULL ? -1 : *depth;
	return res; 
}
 
void* upo_bst_get_value_depth_impl (upo_bst_node_t *n, void *key, long*depth, upo_bst_comparator_t cmp) 
{     
	if(n == NULL) 
	{
		return;
	}
	else if(cmp(n->key, key) == 0) 
	{
    	return *depth; 	
	}
	else 
	{     	
		if(cmp(n->key, key) > 0) 
		{
        	*depth += 1;
        	return upo_bst_get_value_depth_impl (n->left, key, depth, cmp);	
		} 
		else 
		{ 	
			*depth += 1;
   			return upo_bst_get_value_depth_impl (n->right, key, depth, cmp);       
		}     
	}   
}

///dire se la chiave K è contenuta e calcoli la PROFONDITA’
int upo_bst_contains_depth (upo_bst_t bst, const void* key, const, long *depth)
{
	if(bst == NULL) 
	{
		return 1;
	}
	*depth = 0;
	return upo_bst_contains_depth_impl(bst->root, key, depth, bst->key_cmp);
}
 
int upo upo_bst_contains_depth_impl (const upo_bst_t tree, const void *min_key, const void *max_key)	
{
    if(upo_bst_is_empty(tree)) 
	{
		return 1;
	}
	return upo_bst_is_bst_impl(tree->root, min_key, max_key, tree->key_cmp);   
}

//conta le foglie di un SottoAlbero radicato in K.
Size_ t upo_bst_subtree_count_leaves (upo_bst_t bst, const void * key) 
{
	if(bst == null) 
	{
		return 0;
	}
	upo_bst _node_ t * node = upo_bst_get_impl(bst->root, key, bst->key_cmp);
	size_t count = upo_bst_subtree_count_leaves_impl (node, key)
	return count; 
}

Upo_bst_node_t upo_bst_get_impl(upo_bst_node_t *n, const void * key, upo_bst_comparator_t comp) 
{
	if(n==NULL) 
	{
		return NULL;
	}
	else if( comp(n->key, key) > 0) 
	{
	return  upo_bst_get_impl(n->left, key, comp);
	} 
	else if(comp(n->key, key)<0) 
	{
	return upo_bst_get_impl(n->right, key, comp);
	} 
	return n;
}
 
size_t upo_bst_subtree_count_leaves_impl (upo_bst_node_t *n, const void *key) 
{
	if( n == Null) 
	{
		return 0;
	}
	else if( n->left == NULL && n->right == NULL)
	{
		return 1;
	}
	return (upo_bst_subtree_count_leaves_impl(n-> left, key) + upo_bst_subtree_count_leaves_impl(n->right, key));
}

//Novembre2021 x contare i nodi di un sottoalbero radicato in k;
size_t upo_bst_subtree_count_inner (upo_bst_t bst, const void * key) 
{
	if(bst == null) 
	{
		return 0;
	}
	upo_bst_node_t* node = upo_bst_get_impl(bst->root, key, bst->key_cmp);
	size_t count = upo_bst_subtree_count_ inner_impl (node, key)
	return count;
}

size_t upo_bst_subtree_count_inner_impl(upo_bst_node_t *node, const void *key) 
{
	if(node == NULL) 
	{
		return 0;
	}
	if(node->left == NULL && node->right == NULL) 
	{
		return 0;
	}
	size_t sx = upo_bst_subtree_count_inner_impl(node->left, key);
	size_t dx = upo_bst_subtree_count_inner_impl(node->right, key);
	return 1 + sx + dx;  
}
 
//9luglio2021
//2 chiavi Klow e Khigh (che possono nn esistere nel bst kL<= kH) ritorni il numero di nodi che sono compresi nelle 2 chiavi
 
size_t upo_bst_range_count (upo_bst_t tree, const void *Lkey, const void *Hkey) 
{
	if(tree == NULL) 
	{
		return 0;
	}
	size_t count= 0;
	upo_bst_range_count_impl (tree->root, Lkey, Hkey, tree->key_cmp, &count);
	return count;
}
 
void upo_bst_range_count_imp(upo_bst_node_t *n, void *low, void *high, upo_sort_comparator_t comp, size_t *cont) 
{
	if(n== NULL) 
	{
		return;
	}
	upo_bst_range_count_imp(n->left, low, high, comp, cont);
	if(comp(n->key, low) >= 0 && comp(n->key, high)<= 0) 
	{   
		*cont += 1;
	}
	upo_bst_range_count_imp(n->right, low, high, comp, cont);
	return;  
}
 
//25giugn20definizione un albero binario di ricerca (BST) non possono esserci chiavi duplicate.
upo_bst_value_list_node_t* upo_bst_add_list(upo_bst_value_list_node_t* list, void* value);
upo_bst_node_t *upo_bst_insert_impl(upo_bst_node_t *node, void *key, void *value, upo_bst_comparator_t cmp);
upo_bst_node_t *upo_bst_node_create(void *key, void *value) ;
 
void upo_bst_insert(upo_bst_t bst, void *key, void *value) 
{
	if(bst == NULL) 
	{
		return;
	}
	bst->root = upo_bst_insert_impl(bst->root, key, value, bst->key_cmp); 
}

upo_bst_node_t *upo_bst_node_create(void *key, void *value) 
{  
	upo_bst_node_t *node = calloc(1, sizeof(upo_bst_node_t));
   	if(node == NULL) 
	{ 
		perror("Impossibile creare il nodo");
    	abort();	
	}
	node->key = key;
	node->value = NULL;
	node->value = upo_bst_add_list(node->value, value);
	node->left = NULL;
	node->right = NULL;
	return node; 
}

upo_bst_value_list_node_t* upo_bst_add_list(upo_bst_value_list_node_t* list, void* value) 
{  
    upo_bst_value_list_node_t *nodeList = calloc(1, sizeof(upo_bst_value_list_node_t));
	nodeList->value = value;
	nodeList->next = list;
	list = nodeList;
	return nodeList;
}

upo_bst_node_t *upo_bst_insert_impl(upo_bst_node_t *node, void *key, void *value, upo_bst_comparator_t cmp) 
{
	if(node == NULL) 
	{
		return upo_bst_node_create(key, value);
	}
	else if(cmp(key, node->key) < 0)
	{
    	node->left = upo_bst_insert_impl(node->left, key, value, cmp);
	}
	else if (cmp(key, node->key) > 0)
	{
    	node->right = upo_bst_insert_impl(node->right, key, value, cmp);
	}
	else 
	{
		node->value = upo_bst_add_list(node->value, value);
	}
	return node; 
}

//DIMENSIONE sottoalbero
upo_bst_node_t *upo_bst_get_impl(upo_bst_node_t *node, const void *key, upo_bst_comparator_t key_cmp)
{	
	if (node == NULL)	        
	{
		return NULL;
	}	
	if (key_cmp(key, node->key) < 0)
	{
    	return upo_bst_get_impl(node->left, key, key_cmp);	
	}
	else if (key_cmp(key, node->key) > 0)
	{
    	return upo_bst_get_impl(node->right, key, key_cmp);
	}
 	else       
	{
		return node;   
	}
}
 
int upo_bst_contains(const upo_bst_t tree, const void *key)
{     
	if (upo_bst_get_impl(tree->root, key, upo_bst_get_comparator(tree)) != NULL)
    {
		return 1;
	}
	else
	{
    	return 0;  
	}
}
 
size_t upo_bst_subtree_size_impl(upo_bst_node_t *node, const void *key, const upo_bst_t bst)
{      
	if (node == NULL)
    {
		return 0;
	}
	if (upo_bst_contains(bst, key))
    {	
		return 1 + upo_bst_subtree_size_impl(node->left, key, bst) + upo_bst_subtree_size_impl(node->right, key, bst);
	}   
	else
	{
		return 0;  
	}
}
 
size_t upo_bst_subtree_size(const upo_bst_t bst, const void *key)  
{
	if (bst != NULL)
	{
  		return upo_bst_subtree_size_impl(bst->root, key, bst);
	}  
    return 0;  
}


/*odd-even sort: in cui si confrontano
ripetutamente le coppie di posizione pari e quelle dispari fino a quando tutti gli elementi si trovano nell’ordine corretto*/
void swap(void *src, void *dest, size_t size)
{
    unsigned char *srcP = (unsigned char*) src;
    unsigned char *destP = (unsigned char*) dest;
	for (size_t i = 0; i < size; ++i)
	{
        unsigned char tmp = *(srcP + i);
        *(srcP + i) = *(destP + i);
        *(destP + i) = tmp;
	}  
}
 
void upo_odd_even_sort(void *base, size_t n, size_t size, upo_sort_comparator_t cmp)
{
	size_t i, j;
	unsigned char * array = base;
	for (i=0; i<n-1; i++) 
	{
		for (j=0; j<n-1; j++) 
		{
           	if(cmp(array+j*size, array+(j+1)*size) >0)
           	swap(array+j*size, array+(j+1) *size, size);
		}
	}
}

