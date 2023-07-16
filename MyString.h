#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct {
	char* data;
	size_t size;
	size_t capacity;
} my_string;

void init_string(my_string* str) {
	str->data = NULL;
	str->size = 0;
	str->capacity = 0;
}

void clear_string(my_string* str) {
	free(str->data);
	str->data = NULL;
	str->size = 0;
	str->capacity = 0;
}

void append_char(my_string* str, char c) {
	if (str->size == str->capacity) {
		str->capacity = (str->capacity == 0) ? 1 : str->capacity * 5;
		str->data = (char*)realloc(str->data, str->capacity * sizeof(char));
	}
	str->data[str->size++] = c;
}

void append_string(my_string* str, const char* s) {
	while (*s) {
		append_char(str, *s++);
	}
}

void read_string(my_string* str) {
	clear_string(str);
	int c;
	while ((c = getchar()) != ' ') {
		append_char(str, c);
	}
	append_char(str, '\0');
}

void reverse_string(my_string* str) {
	for (int i = 0; i < str->size / 2; i++) {
		char temp = str->data[i];
		str->data[i] = str->data[str->size - i - 1];
		str->data[str->size - i - 1] = temp;
	}
}