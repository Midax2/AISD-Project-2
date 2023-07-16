#define _CRT_SECURE_NO_WARNINGS
#include "MyString.h"
#include "MyVector.h"
#include "Dijkstra.h"
#include "Hashmap.h"
#include <ctype.h>

typedef struct {
	char** map_visual;
	int height;
	int width;
}Map;

typedef struct {
	int X, Y;
	double cost;
}Neighbor;

typedef struct {
	bool** checked;
	int time;
	int min_time;
}Road;

typedef struct {
	int x, y, time;
}Node;

void trim_whitespace(char* str) {
	int len = strlen(str);
	int start = 0;
	while (len > 0 && isspace(str[len - 1])) {
		str[--len] = '\0';
	}
	while (isspace(str[start])) {
		start++;
	}
	memmove(str, str + start, len - start + 1);
}

void sort_dist(Neighbor* dist, int n) {
	int i, j;
	Neighbor key;
	for (i = 1; i < n; i++) {
		key = dist[i];
		j = i - 1;
		while (j >= 0 && dist[j].cost > key.cost) {
			dist[j + 1] = dist[j];
			j = j - 1;
		}
		dist[j + 1] = key;
	}
}

void road_find(Road* road, Map map, int curr_x, int curr_y, int goal_x, int goal_y) {
	my_vector queue;
	init_vector(&queue);

	int front = 0, rear = -1;
	rear++;
	Node start_node = { curr_x, curr_y, 0 };
	push_back(&queue, &start_node);

	road->checked[curr_y][curr_x] = true;

	while (front <= rear) {
		Node current = *(Node*)at(&queue, front);
		front++;

		Neighbor neighbors[4];
		int num_neighbors = 0;
		int dx[4] = { -1, 0, 1, 0 };
		int dy[4] = { 0, -1, 0, 1 };
		for (int i = 0; i < 4; i++) {
			int x = current.x + dx[i];
			int y = current.y + dy[i];

			if (x < 0 || y < 0 || x >= map.width || y >= map.height) {
				continue;
			}

			if (road->checked[y][x]) {
				continue;
			}

			if (x == goal_x && y == goal_y) {
				road->min_time = current.time + 1;
				clear_vector(&queue);
				return;
			}

			if (map.map_visual[y][x] == '#') {
				neighbors[num_neighbors].X = x;
				neighbors[num_neighbors].Y = y;
				double dist_sq = (x - goal_x) * (x - goal_x) + (y - goal_y) * (y - goal_y);
				neighbors[num_neighbors].cost = dist_sq;
				num_neighbors++;
			}
		}

		sort_dist(neighbors, num_neighbors);

		for (int i = 0; i < num_neighbors; i++) {
			rear++;
			Node* neighbor = (Node*)malloc(sizeof(Node));
			neighbor->x = neighbors[i].X;
			neighbor->y = neighbors[i].Y;
			neighbor->time = current.time + 1;
			push_back(&queue, neighbor);
			road->checked[neighbors[i].Y][neighbors[i].X] = true;
		}
	}
	clear_vector(&queue);
}

void read_map_cities(Map* map, my_vector* cities, hashmap_t* cities_map, my_string* city_name, bool** checked) {
	my_string line;
	init_string(&line);
	for (int i = 0; i < map->height + 2; i++) {
		if (i < map->height) {
			map->map_visual[i] = (char*)malloc((map->width + 1) * sizeof(char));
			checked[i] = (bool*)malloc(map->width * sizeof(bool));
			for (int j = 0; j < map->width; j++) {
				char test = getchar();
				if (test == '\n') {
					j--;
					continue;
				}
				append_char(&line, test);
			}
			append_char(&line, '\0');
			trim_whitespace(line.data);
			strcpy(map->map_visual[i], line.data);
			clear_string(&line);
		}
		if (i > 1) {
			int test_i = i - 2;
			for (int test_j = 0; test_j < map->width; test_j++) {
				checked[test_i][test_j] = false;
				if (map->map_visual[test_i][test_j] == '*') {
					bool skip = false;
					bool access = false;
					for (int y = test_i - 1; y <= test_i + 1; y++) {
						for (int x = test_j - 1; x <= test_j + 1; x++) {
							if (test_i == y && test_j == x)
								continue;
							if (y < 0 || y >= map->height || x < 0 || x >= map->width)
								continue;
							if (isalnum(map->map_visual[y][x]) && !skip) {
								int start_x = x;
								bool left = false;
								if (start_x >= 1 && start_x < map->width - 1) {
									if (isalnum(map->map_visual[y][start_x - 1]) && isalnum(map->map_visual[y][start_x + 1])) continue;
									if (isalnum(map->map_visual[y][start_x - 1])) left = true;
								}
								if (start_x == map->width - 1) left = true;
								while (isalnum(map->map_visual[y][start_x])) {
									append_char(city_name, map->map_visual[y][start_x]);
									if (left) start_x--;
									else start_x++;
								}
								if (left) reverse_string(city_name);
								append_char(city_name, '\0');
								skip = true;
							}
							else if (map->map_visual[y][x] == '*' || map->map_visual[y][x] == '#') {
								access = true;
							}
						}
					}
					City* city = newCity(city_name->data, test_j, test_i, cities->size);
					city->haveRoads = access;
					push_back(cities, city);
					hashmap_put(cities_map, city->name.data, city);
					clear_string(city_name);
				}
			}
			clear_string(city_name);
		}
	}
}

void read_planes(hashmap_t* cities_map, Graph* graph) {
	int type;
	scanf("%d", &type);
	for (int i = 0; i < type; i++) {
		my_string city_A;
		init_string(&city_A);
		my_string city_B;
		init_string(&city_B);
		int time;
		read_string(&city_A);
		read_string(&city_B);
		scanf("%d", &time);
		trim_whitespace(city_A.data);
		trim_whitespace(city_B.data);
		City* A = (City*)hashmap_get(cities_map, city_A.data);
		City* B = (City*)hashmap_get(cities_map, city_B.data);
		Distance* distance = newDistance(A, B, time);
		push_back(&graph->distances[A->index], distance);
	}
}

void find_all_paths(Map* map, my_vector* cities, bool** checked, Graph* graph) {
	for (int i = 0; i < cities->size; i++) {
		City* A = (City*)at(cities, i);
		if (!A->haveRoads) continue;
		for (int j = i; j < cities->size; j++) {
			City* B = (City*)at(cities, j);
			if (!B->haveRoads) continue;
			Road* road = (Road*)malloc(sizeof(Road));
			for (int i = 0; i < map->height; i++) {
				for (int j = 0; j < map->width; j++) {
					checked[i][j] = false;
				}
			}
			road->time = -1;
			road->min_time = -1;
			road->checked = checked;
			if (A != B) {
				road_find(road, *map, A->X, A->Y, B->X, B->Y);
			}
			else {
				road->min_time = 0;
			}
			if (road->min_time != -1) {
				Distance* distance = newDistance(A, B, road->min_time);
				push_back(&graph->distances[A->index], distance);
				Distance* distanceReverse = newDistance(B, A, road->min_time);
				push_back(&graph->distances[B->index], distanceReverse);
			}
			free(road);
		}
	}
}

void do_commands(my_vector* cities, hashmap_t* cities_map, Graph* graph) {
	int type;
	scanf("%d", &type);
	for (int i = 0; i < type; i++) {
		my_string city_A;
		init_string(&city_A);
		my_string city_B;
		init_string(&city_B);
		int parammetr;
		read_string(&city_A);
		read_string(&city_B);
		scanf("%d", &parammetr);
		trim_whitespace(city_A.data);
		trim_whitespace(city_B.data);
		City* A = (City*)hashmap_get(cities_map, city_A.data);
		City* B = (City*)hashmap_get(cities_map, city_B.data);
		Dijkstra(graph, *cities, A->index, B->index, parammetr);
	}
}

int main() {
	Map map;
	my_vector cities;
	init_vector(&cities);
	my_string city_name;
	init_string(&city_name);
	scanf("%d %d", &(map.width), &(map.height));
	int hash_size = map.height * map.width / 5;
	if (hash_size == 0) hash_size = 5;
	hashmap_t* cities_map = hashmap_create(hash_size, city_hash, city_cmp);
	map.map_visual = (char**)malloc(map.height * sizeof(char*));
	bool** checked = (bool**)malloc(map.height * sizeof(bool*));
	read_map_cities(&map, &cities, cities_map, &city_name, checked);
	Graph* graph = createGraph(cities.size);
	read_planes(cities_map, graph);
	find_all_paths(&map, &cities, checked, graph);
	do_commands(&cities, cities_map, graph);
	for (int i = 0; i < map.height; i++) {
		free(map.map_visual[i]);
		free(checked[i]);
	}
	for (int i = 0; i < cities.size; i++) {
		City* city = (City*)at(&cities, i);
		free(city);
	}
	clear_string(&city_name);
	clear_vector(&cities);
	free(checked);
	free(map.map_visual);
	deleteGraph(graph);
	hashmap_destroy(cities_map);
	return 0;
}