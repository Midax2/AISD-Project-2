#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    void** data;
    size_t size;
    size_t capacity;
} my_vector;

void init_vector(my_vector* vec) {
    vec->data = NULL;
    vec->size = 0;
    vec->capacity = 0;
}

void clear_vector(my_vector* vec) {
    free(vec->data);
    vec->data = NULL;
    vec->size = 0;
    vec->capacity = 0;
}

void push_back(my_vector* vec, void* elem) {
    if (vec->size == vec->capacity) {
        vec->capacity = (vec->capacity == 0) ? 1 : vec->capacity * 5;
        vec->data = (void**)realloc(vec->data, vec->capacity * sizeof(void*));
    }
    vec->data[vec->size++] = elem;
}

void pop_back(my_vector* vec) {
    if (vec->size > 0) {
        vec->size--;
    }
}

void* at(my_vector* vec, size_t index) {
    if (index >= vec->size) {
        return NULL;
    }
    return vec->data[index];
}