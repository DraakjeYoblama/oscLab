#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include "sbuffer.h"

/**
 * basic node for the buffer, these nodes are linked together to create the buffer
 */
typedef struct sbuffer_node {
    struct sbuffer_node *next;  /**< a pointer to the next node*/
    sensor_data_t data;         /**< a structure containing the data */
} sbuffer_node_t;

/**
 * a structure to keep track of the buffer
 */
struct sbuffer {
    sbuffer_node_t *head;       /**< a pointer to the first node in the buffer */
    sbuffer_node_t *tail;       /**< a pointer to the last node in the buffer */
};

pthread_mutex_t buffermutex;

int sbuffer_init(sbuffer_t **buffer) {
    pthread_mutex_init(&buffermutex, NULL);
    *buffer = malloc(sizeof(sbuffer_t));
    if (*buffer == NULL) return SBUFFER_FAILURE;
    (*buffer)->head = NULL;
    (*buffer)->tail = NULL;
    return SBUFFER_SUCCESS;
}

int sbuffer_free(sbuffer_t **buffer) {
    pthread_mutex_lock(&buffermutex);
    sbuffer_node_t *dummy;
    if ((buffer == NULL) || (*buffer == NULL)) {
        pthread_mutex_unlock(&buffermutex);
        return SBUFFER_FAILURE;
    }
    (*buffer)->head = (*buffer)->tail = NULL; // remove end marker (0)
    while ((*buffer)->head) {
        dummy = (*buffer)->head;
        (*buffer)->head = (*buffer)->head->next;
        free(dummy);
    }
    free(*buffer);
    *buffer = NULL;
    pthread_mutex_unlock(&buffermutex);
    pthread_mutex_destroy(&buffermutex);
    return SBUFFER_SUCCESS;
}

int sbuffer_remove(sbuffer_t *buffer, sensor_data_t *data) { //TODO: make this blocking if buffer is empty using pthreads condition variables
    pthread_mutex_lock(&buffermutex);
    sbuffer_node_t *dummy;
    if (buffer == NULL) {
        pthread_mutex_unlock(&buffermutex);
        return SBUFFER_FAILURE;
    }
    if (buffer->head == NULL) {
        pthread_mutex_unlock(&buffermutex);
        return SBUFFER_NO_DATA;
    }
    *data = buffer->head->data;
    dummy = buffer->head;
    if (buffer->head->data.id == 0) //end marker
    {
        // don't remove end marker to make sure every thread sees it
    } else if (buffer->head == buffer->tail) // buffer has only one node
    {
        buffer->head = buffer->tail = NULL;
    } else  // buffer has many nodes empty
    {
        buffer->head = buffer->head->next;
    }
    free(dummy);
    pthread_mutex_unlock(&buffermutex);
    return SBUFFER_SUCCESS;
}

int sbuffer_insert(sbuffer_t *buffer, sensor_data_t *data) {
    pthread_mutex_lock(&buffermutex);
    sbuffer_node_t *dummy;
    if (buffer == NULL) {
        pthread_mutex_unlock(&buffermutex);
        return SBUFFER_FAILURE;
    }
    dummy = malloc(sizeof(sbuffer_node_t));
    if (dummy == NULL) {
        pthread_mutex_unlock(&buffermutex);
        return SBUFFER_FAILURE;
    }
    dummy->data = *data;
    dummy->next = NULL;
    if (buffer->tail == NULL) // buffer empty (buffer->head should also be NULL
    {
        buffer->head = buffer->tail = dummy;
    } else // buffer not empty
    {
        buffer->tail->next = dummy;
        buffer->tail = buffer->tail->next;
    }
    pthread_mutex_unlock(&buffermutex);
    return SBUFFER_SUCCESS;
}