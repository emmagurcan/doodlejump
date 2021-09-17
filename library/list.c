

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include "list.h"

const size_t INITIAL_CAPACITY = 10;

typedef struct list {
    void **list;
    size_t capacity;
    size_t size;
    free_func_t freer;
} list_t;

list_t *list_init(size_t initial_size, free_func_t freer) {
    list_t *list = malloc(sizeof(list_t));
    assert(list != NULL);
    list->list = malloc(sizeof(void *) * initial_size);
    assert(list->list != NULL);
    list->capacity = initial_size;
    list->size = 0;
    list->freer = freer;
    return list;
}

void list_free(list_t *list) {
    assert(list != NULL);

    for (size_t i = 0; i < list->size; i++) {
        assert(list->freer != NULL);
        (list->freer)(list_get(list, i));
    }
    free(list->list);

    free(list);
}

void *list_get(list_t *list, size_t index) {
    assert(list != NULL);
    assert(index < list_size(list));
    assert(index >= 0);
    return list->list[index];
}

size_t list_size(list_t *list) {
    assert(list != NULL);
    return list->size;
}

void list_resize(list_t *list) {
    assert(list != NULL);
    if (list->capacity == 0) {
        list->list = realloc(list->list, sizeof(void *));
        list->capacity = 1;
    }
    else {
        list->list = realloc(list->list, 2 * list->capacity * sizeof(void *));
        list->capacity *= 2;  
    }
    assert(list->list != NULL);
}

void list_add(list_t *list, void *elem) {
    assert(list != NULL);
    assert(elem != NULL);

    if (list->capacity <= list->size + 1) {
        list_resize(list);
    }

    list->list[list->size] = elem;
    list->size ++;
}

void *list_remove(list_t *list, size_t index) {
    assert(list != NULL);
    assert(list->size > 0);
    assert(index < list->size);
    assert(index >= 0);
    void *elem = list_get(list, index);
    list->size--;

    for (size_t i = index; i < list->size; i++) {
        list->list[i] = list->list[i+1];
    }

    return elem;
}