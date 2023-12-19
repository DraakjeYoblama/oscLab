
// based on milestone3/clab7/plab4

#include "sbuffer.h"

/**
 * basic node for the buffer, these nodes are linked together to create the buffer
 */
typedef struct sbuffer_node {
    struct sbuffer_node *next;  /**< a pointer to the next node*/
    sensor_data_t data;         /**< a structure containing the data */
    int stage;                  /**< indicator of how many times the node has been read = what stage of the process it is on */
} sbuffer_node_t;

/**
 * a structure to keep track of the buffer
 */
struct sbuffer {
    sbuffer_node_t *head;       /**< a pointer to the first node in the buffer */
    sbuffer_node_t *tail;       /**< a pointer to the last node in the buffer */
};

pthread_mutex_t buffermutex;
pthread_cond_t filled, updated;

int sbuffer_init(sbuffer_t **buffer) {
    pthread_mutex_init(&buffermutex, NULL);
    pthread_cond_init(&filled, NULL);
    pthread_cond_init(&updated, NULL);
    *buffer = malloc(sizeof(sbuffer_t));
    if (*buffer == NULL) return SBUFFER_FAILURE;
    (*buffer)->head = NULL;
    (*buffer)->tail = NULL;
    return SBUFFER_SUCCESS;
}

int sbuffer_free(sbuffer_t **buffer) {
    sbuffer_node_t *dummy;
    pthread_mutex_lock(&buffermutex);
    if ((buffer == NULL) || (*buffer == NULL)) {
        pthread_mutex_unlock(&buffermutex);
        return SBUFFER_FAILURE;
    }

    while ((*buffer)->head) {
        dummy = (*buffer)->head;
        if ((*buffer)->head == (*buffer)->tail) // buffer has only one node
        {
            (*buffer)->head = (*buffer)->tail = NULL; // remove end marker (0)
        } else  // buffer has many nodes
        {
            (*buffer)->head = (*buffer)->head->next;
        }
        free(dummy);
    }
    free(*buffer);
    *buffer = NULL;
    pthread_mutex_unlock(&buffermutex);
    pthread_cond_destroy(&filled);
    pthread_mutex_destroy(&buffermutex);
    return SBUFFER_SUCCESS;
}

int sbuffer_remove(sbuffer_t *buffer, sensor_data_t *data, int stage) {
    sbuffer_node_t *dummy;
    pthread_mutex_lock(&buffermutex);
    if (buffer == NULL) {
        pthread_mutex_unlock(&buffermutex);
        return SBUFFER_FAILURE;
    }
    while (buffer->head == NULL || buffer->head->stage != stage-1) {
        // block if head is null or node is at the wrong stage
        pthread_cond_wait(&updated, &buffermutex);
    }
    *data = buffer->head->data;
    dummy = buffer->head;
    if (buffer->head->data.id == 0) //end marker
    {
        // don't remove end marker to make sure every thread sees it
    } else if (buffer->head == buffer->tail) // buffer has only one node
    {
        buffer->head = buffer->tail = NULL;
    } else  // buffer has many nodes
    {
        buffer->head = buffer->head->next;
    }
    free(dummy);
    pthread_mutex_unlock(&buffermutex);
    return SBUFFER_SUCCESS;
}

int sbuffer_read(sbuffer_t *buffer, sensor_data_t *data, int stage) {
    sbuffer_node_t *dummy;
    pthread_mutex_lock(&buffermutex);
    if (buffer == NULL) {
        pthread_mutex_unlock(&buffermutex);
        return SBUFFER_FAILURE;
    }
    while (buffer->head == NULL || buffer->tail->stage != stage-1) {
        // block if head is null or all nodes are at the wrong stage (if final node is at the wrong stage, all of them are)
        pthread_cond_wait(&filled, &buffermutex);
    }
    dummy = buffer->head;
    while (dummy->stage != stage-1) { // read nodes that aren't the first one if the first one already has a high enough stage, to speed things up
        if (dummy->next == NULL) {
            pthread_mutex_unlock(&buffermutex);
            return SBUFFER_FAILURE;
        }
        dummy = dummy->next;
    }
    dummy->stage++; // increment stage count
    *data = dummy->data;
    if (dummy->data.id == 0) //end of buffer marker
    {
        // do nothing
    }
    pthread_mutex_unlock(&buffermutex);
    pthread_cond_signal(&updated);
    return SBUFFER_SUCCESS;
}

int sbuffer_insert(sbuffer_t *buffer, sensor_data_t *data, int stage) {
    sbuffer_node_t *dummy;
    pthread_mutex_lock(&buffermutex);
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
    dummy->stage = stage;
    dummy->next = NULL;
    if (buffer->tail == NULL) // buffer empty (buffer->head should also be NULL)
    {
        buffer->head = buffer->tail = dummy;
    } else // buffer not empty
    {
        buffer->tail->next = dummy;
        buffer->tail = buffer->tail->next;
    }
    pthread_mutex_unlock(&buffermutex);
    pthread_cond_signal(&filled);
    return SBUFFER_SUCCESS;
}

int sbuffer_cond(int amount) {
    for (int i = 0; i<amount; i++) {
        pthread_cond_signal(&filled);
    }
    return SBUFFER_SUCCESS;
}