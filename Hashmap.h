#pragma once
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
typedef struct {
    void* key;
    void* value;
} hashmap_element_t;

typedef struct {
    unsigned int capacity;
    unsigned int size;
    hashmap_element_t* elements;
    unsigned int (*hash)(void*);
    int (*cmp)(void*, void*);
} hashmap_t;

unsigned int city_hash(void* key) {
	char* name = (char*)key;
	unsigned int hash = 0;
	while (*name) {
		hash = hash * 131 + *name++;
	}
	return hash;
}

int city_cmp(void* key1, void* key2) {
	char* key3 = (char*)key1;
	char* key4 = (char*)key2;
	bool test = !strcmp(key3, key4);
	return test;
}

hashmap_t* hashmap_create(unsigned int capacity, unsigned int (*hash)(void*), int (*cmp)(void*, void*)) {
	hashmap_t* map = (hashmap_t*)malloc(sizeof(hashmap_t));
	map->capacity = capacity;
	map->size = 0;
	map->elements = (hashmap_element_t*)calloc(capacity, sizeof(hashmap_element_t));
	map->hash = hash;
	map->cmp = cmp;
	return map;
}

void hashmap_put(hashmap_t* map, void* key, void* value) {
	if (map->size >= map->capacity) {
		unsigned int new_capacity = map->capacity * 2;
		hashmap_element_t* new_elements = (hashmap_element_t*)calloc(new_capacity, sizeof(hashmap_element_t));
		for (unsigned int i = 0; i < map->capacity; i++) {
			if (map->elements[i].key != NULL) {
				unsigned int new_index = map->hash(map->elements[i].key) % new_capacity;
				while (new_elements[new_index].key != NULL) {
					new_index = (new_index + 1) % new_capacity;
				}
				new_elements[new_index].key = map->elements[i].key;
				new_elements[new_index].value = map->elements[i].value;
			}
		}
		free(map->elements);
		map->elements = new_elements;
		map->capacity = new_capacity;
	}
	unsigned int index = map->hash(key) % map->capacity;
	while (map->elements[index].key != NULL && map->cmp(map->elements[index].key, key) != 1) {
		index = (index + 1) % map->capacity;
	}
	map->elements[index].key = key;
	map->elements[index].value = value;
	map->size++;
}

void* hashmap_get(hashmap_t* map, void* key) {
	unsigned int index = map->hash(key) % map->capacity;
	while (map->elements[index].key != NULL) {
		if (map->cmp(map->elements[index].key, key) == 1) {
			return map->elements[index].value;
		}
		index = (index + 1) % map->capacity;
	}
	return NULL;
}

void hashmap_destroy(hashmap_t* map) {
	free(map->elements);
	free(map);
}