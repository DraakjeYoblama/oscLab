#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "dplist.h"

/*
 * The real definition of struct list / struct node
 */

struct dplist_node {
    dplist_node_t *prev, *next;
    void *element;
};

struct dplist {
    dplist_node_t *head;

    void *(*element_copy)(void *src_element);

    void (*element_free)(void **element);

    int (*element_compare)(void *x, void *y);
};


dplist_t *dpl_create(// callback functions
        void *(*element_copy)(void *src_element),
        void (*element_free)(void **element),
        int (*element_compare)(void *x, void *y)
) {
    dplist_t *list;
    list = malloc(sizeof(struct dplist));
    list->head = NULL;
    list->element_copy = element_copy;
    list->element_free = element_free;
    list->element_compare = element_compare;
    return list;
}

void dpl_free(dplist_t **list, bool free_element) {
    int size = dpl_size(*list);
    if (size > 0) {
        for (int index = size-1; index>=0; index--) {
            dpl_remove_at_index(*list, index, free_element);
        }
    }
    free(*list);
    *list = NULL;

}

dplist_t *dpl_insert_at_index(dplist_t *list, void *element, int index, bool insert_copy) {
    dplist_node_t *ref_at_index, *list_node;
    if (list == NULL) return NULL;

    list_node = malloc(sizeof(dplist_node_t));


    if (insert_copy) { // TODO: element_copy
        list_node->element = list->element_copy(element);
    } else {
        list_node->element = element;
    }


    // pointer drawing breakpoint
    if (list->head == NULL) { // covers case 1
        list_node->prev = NULL;
        list_node->next = NULL;
        list->head = list_node;
        // pointer drawing breakpoint
    } else if (index <= 0) { // covers case 2
        list_node->prev = NULL;
        list_node->next = list->head;
        list->head->prev = list_node;
        list->head = list_node;
        // pointer drawing breakpoint
    } else {
        ref_at_index = dpl_get_reference_at_index(list, index);
        assert(ref_at_index != NULL);
        // pointer drawing breakpoint
        if (index < dpl_size(list)) { // covers case 4
            list_node->prev = ref_at_index->prev;
            list_node->next = ref_at_index;
            ref_at_index->prev->next = list_node;
            ref_at_index->prev = list_node;
            // pointer drawing breakpoint
        } else { // covers case 3
            assert(ref_at_index->next == NULL);
            list_node->next = NULL;
            list_node->prev = ref_at_index;
            ref_at_index->next = list_node;
            // pointer drawing breakpoint
        }
    }
    return list;

}

dplist_t *dpl_remove_at_index(dplist_t *list, int index, bool free_element) {
    dplist_node_t *ref_at_index;
    if (list == NULL) return NULL; // no list
    if (list->head == NULL) return list; //empty list

    if (index <= 0) { // remove first node
        ref_at_index = list->head;
        list->head->prev = NULL;
        list->head = ref_at_index->next; // order here is important, in case there is only one node
    } else {
        ref_at_index = dpl_get_reference_at_index(list, index);
        assert(ref_at_index != NULL);
        if (index < dpl_size(list)-1) { // remove node in the middle
            ref_at_index->prev->next = ref_at_index->next;
            ref_at_index->next->prev = ref_at_index->prev;
        } else { // remove last node
            assert(ref_at_index->next == NULL);
            ref_at_index->prev->next = NULL;
        }
    }

    //void* test = ref_at_index->element;
    if (free_element) { // TODO: element_free
        list->element_free(&ref_at_index->element);
    }

    free(ref_at_index); // free memory after completion
    return list;
}

int dpl_size(dplist_t *list) {
    if (list == NULL) return -1; // no list

    int count = 0;
    dplist_node_t *dummy = list->head;

    while (dummy != NULL) {
        dummy = dummy->next;
        count++;
    }
    return count;

}

void *dpl_get_element_at_index(dplist_t *list, int index) {
    dplist_node_t *ref_at_index;
    ref_at_index = dpl_get_reference_at_index(list, index);

    return dpl_get_element_at_reference(list, ref_at_index);

}

int dpl_get_index_of_element(dplist_t *list, void *element) {
    if (list == NULL || list->head == NULL) return -1; // no list or empty list

    dplist_node_t *dummy = list->head;

    if (/* dummy->element == NULL */ list->element_compare(NULL, dummy->element)) {
        return -1;
    }
    for (int count = 0; true; count++) {
        if (/* dummy->element == element */ list->element_compare(element, dummy->element)) { //TODO: element_compare
            return count;
        }
        dummy = dummy->next;
    }
    return -1;
}

dplist_node_t *dpl_get_reference_at_index(dplist_t *list, int index) {
    if (list == NULL || list->head == NULL) return NULL; // no list or empty list

    int count;
    dplist_node_t *dummy = list->head;

    if (index <= 0) {
        return dummy;
    }

    for (count = 0; count < index; count++) {
        if (dummy->next == NULL) {
            break;
        } else {
            dummy = dummy->next;
        }
    }
    return dummy;

}

void *dpl_get_element_at_reference(dplist_t *list, dplist_node_t *reference) {
    if (reference == NULL) {
        return 0;
    }
    return reference->element;
}


