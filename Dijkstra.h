#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct {
	my_string name;
	int X, Y;
	int index;
	bool haveRoads;
}City;

typedef struct {
	City* A;
	City* B;
	int time;
}Distance;

struct Graph {
	int cityCount;
	my_vector* distances;
};

City* newCity(char* name, int X, int Y, int index) {
	City* city = (City*)malloc(sizeof(City));
	init_string(&city->name);
	append_string(&city->name, name);
	append_char(&city->name, '\0');
	city->haveRoads = false;
	city->X = X;
	city->Y = Y;
	city->index = index;
	return city;
}

Distance* newDistance(City* A, City* B, int time) {
	Distance* distance = (Distance*)malloc(sizeof(Distance));
	distance->A = A;
	distance->B = B;
	distance->time = time;
	return distance;
}

Graph* createGraph(int cityCount) {
	Graph* graph = (Graph*)malloc(sizeof(Graph));
	graph->cityCount = cityCount;
	graph->distances = (my_vector*)malloc(cityCount * sizeof(my_vector));
	for (int i = 0; i < cityCount; i++) {
		init_vector(&graph->distances[i]);
	}
	return graph;
}

void deleteGraph(Graph* graph) {
	for (int i = 0; i < graph->cityCount; i++) {
		clear_vector(&graph->distances[i]);
	}
	free(graph->distances);
	free(graph);
}

char* cityNameByIndex(my_vector cities, int index) {
    if (index >= cities.size) return NULL;
    City* current = (City*)at(&cities, index);
    return current->name.data;
}

void Dijkstra(Graph* graph, my_vector cities, int src, int dest, bool parammetr)
{
	int* dist = (int*)malloc(graph->cityCount * sizeof(int));
	int* prev = (int*)malloc(graph->cityCount * sizeof(int));
	bool* vis = (bool*)malloc(graph->cityCount * sizeof(bool));
	int* queue = (int*)malloc((graph->cityCount + 10) * sizeof(int));
	int front = 0, rear = 0;
	my_vector visited;
	init_vector(&visited);

	for (int i = 0; i < graph->cityCount; i++) {
		dist[i] = INT_MAX;
		prev[i] = -1;
		vis[i] = false;
	}
	dist[src] = 0;
	queue[rear++] = src;
	vis[src] = true;

	while (front != rear) {
		int currIndex = queue[front++];
		vis[currIndex] = false;

		for (int j = 0; j < graph->distances[currIndex].size; j++) {
			Distance* curr_distance = (Distance*)at(&graph->distances[currIndex], j);
			int indexA = curr_distance->A->index;
			int indexB = curr_distance->B->index;
			int time = curr_distance->time;
			if (dist[indexA] != INT_MAX && dist[indexA] + time < dist[indexB]) {
				dist[indexB] = dist[indexA] + time;
				prev[indexB] = indexA;
				if (!vis[indexB]) {
					queue[rear++] = indexB;
					vis[indexB] = true;
				}
			}
		}
	}

	if (prev[dest] != -1) {
		int curr = dest;
		while (curr != src) {
			push_back(&visited, cityNameByIndex(cities, curr));
			curr = prev[curr];
		}
	}
	if (parammetr == 1) {
		printf("%d ", dist[dest]);
		for (int i = (visited.size - 1); i > 0; i--) {
			char* curr_city = (char*)at(&visited, i);
			printf("%s ", curr_city);
		}
		printf("\n");
		free(vis);
		free(queue);
		free(dist);
		free(prev);
		return;
	}
	printf("%d\n", dist[dest]);
	free(vis);
	free(dist);
	free(queue);
	free(prev);
	return;
}