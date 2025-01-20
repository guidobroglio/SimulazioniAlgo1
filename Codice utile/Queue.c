#include "queue_private.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

/*Creazione di una coda vuota*/
upo_queue_t upo_queue_create () {    
    upo_queue_t queue = malloc(sizeof(struct upo_queue_s));
    if(queue == NULL) {
        perror("Impossibile creare la coda");
        abort();
    }
    queue->bot = NULL;
    queue->top = NULL;
    queue->size = 0;
    return queue;
}

/*Distruzione di una coda*/
void upo_queue_destroy (upo_queue_t queue, int destroy_data) {
    if(queue == NULL) {
        perror("Coda vuota");
        abort();
    }
    for(int i = 0; i<queue->size; i++) {
        upo_queue_dequeue(queue, destroy_data);
    }
}

/*Rimozione di tutti gli elementi dalla coda*/
void upo_queue_clear (upo_queue_t queue, int destroy_data) {
    if(queue == NULL) {
        perror("Coda vuota");
        abort();
    }
    upo_queue_destroy(queue, destroy_data);
    free(queue);
}

/*Aggiunta di un elemento dalla coda*/
void upo_queue_enqueue(upo_queue_t queue,void *data) {
      if(queue == NULL) {
        perror("Coda vuota");
        abort();
    }
    upo_queue_node_t *n = malloc(sizeof(struct upo_queue_node_s));
    if(n == NULL) {
        perror("Il nodo non e' stato creato");
        abort();
    }
    n->data = data;
    queue->size += 1;
    n->next = NULL;
    queue->bot = n;
}

/*Eliminazione del prossimo elemento dalla coda*/
void upo_queue_dequeue(upo_queue_t queue,int destroy_data) {
       if(queue == NULL) {
        perror("Coda vuota");
        abort();
    }
    upo_queue_node_t *n = queue->top;
      if(n == NULL) {
        perror("Il nodo non e' stato creato");
        abort();
    }
    queue->top = queue->top->next;
    if(destroy_data)
        free(n->data);
    free(n);
    queue->size -= 1;
}

/*Ritorna il prossimo elemento dalla coda*/
void* upo_queue_peek(const upo_queue_t queue) {
    return queue == NULL ? NULL : queue->top;
}

/*Ritorna il primo elemento della coda*/
size_t upo_queue_size(constupo_queue_t queue) {
    return queue == NULL ? 0 : queue->size;
}

/*Ritorna se la coda e' vuota o meno*/
int upo_queue_is_empty(const upo_queue_t queue) {
    return queue == NULL && queue->bot == NULL && queue->top == NULL ? 0 : 1;
}
