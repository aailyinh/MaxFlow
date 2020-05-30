#include<iostream>
#include <stdlib.h>  
#include<ctime>
#include<queue>
#include<vector>
#define NumbOfNodes 10
#define maxvalue 2000
#define NumbOfTests 500

using namespace std;

struct Node
{
	int number;
	int flag[2];
	int PossibleNodes[NumbOfNodes];
};

int C[NumbOfNodes][NumbOfNodes];
int c[NumbOfNodes][NumbOfNodes];
int result = 0;
int point = NumbOfNodes - 1;
vector<int> d(NumbOfNodes), p(NumbOfNodes);
queue<int> q;
vector<bool> used(NumbOfNodes);
Node Network[NumbOfNodes];

void generate() {
	for (int i = 0; i < NumbOfNodes; i++) {
		if (i == NumbOfNodes - 1) {
			break;
		}
		else {
			for (int j = i + 1; j < NumbOfNodes; j++) {
				C[i][j] = 1 + rand() % maxvalue;
				c[i][j] = C[i][j];
			}
		}
	}
}

void MatrixInit() {
	for (int i = 0; i < NumbOfNodes; i++) {
		for (int j = 0; j < NumbOfNodes; j++) {
			C[i][j] = 0;
			c[i][j] = 0;
		}
	}
}

void bfs() {
	int start = 0;
	q.push(start);
	used[start] = true;
	p[start] = -1;
	while (!q.empty()) {
		int v = q.front();
		q.pop();
		for (size_t i = 0; i < NumbOfNodes; ++i) {
			int to = Network[v].PossibleNodes[i];
			if (to == 0) {
				break;
			}
			if (!used[to]) {
				used[to] = true;
				q.push(to);
				d[to] = d[v] + 1;
				p[to] = v;
			}
		}
	}
	for (int v = point; v != -1; v = p[v]) {
		Network[v].flag[1] = p[v];
		Network[v].flag[0] = c[p[v]][v];
	}
	for (int z = 0; z < NumbOfNodes; z++) {
		p[z] = 0;
		d[z] = 0;
		used[z] = 0;
	}
}

void output() {
	int i = 0;
	cout << endl << "Maximal value of flow in system: " << result;
	cout << endl << "Values of flows in each tube:" << endl;
	for (i = 0; i < NumbOfNodes; i++) {
		for (int j = i; j < NumbOfNodes; j++) {
			if (C[i][j]) {
				cout << "Tube (" << (i + 1) << "," << (j + 1) << "): ";
				cout << "(" << (C[i][j] - c[i][j]) << "," << (C[j][i] - c[j][i]) << ") ";
				cout << "Flow: " << abs(C[i][j] - c[i][j]) << " ";
				if (C[i][j] - c[i][j] != 0) {
					cout << "Route: ";
					if (C[i][j] - c[i][j] > 0) {
						cout << (i + 1) << "->" << (j + 1);
					}
					else {
						cout << (j + 1) << "->" << (i + 1);
					}
				}
				cout << endl;
			}
		}
	}
	result = 0;
}

void MaxFlow() {
	for (int i = 0; i < NumbOfNodes; i++) {
		Network[i].number = i;
		Network[i].flag[0] = 0;
		Network[i].flag[1] = -1;
	}
	int i = 0, j = 0, min = 0, counter = 0;
	while (1) {
		for (i = 0; i < NumbOfNodes; i++) {
			Network[i].flag[0] = 0;
			Network[i].flag[1] = -1;
			for (j = 0; j < NumbOfNodes; j++) {
				Network[i].PossibleNodes[j] = 0;
			}
		}
		Network[0].flag[0] = INT16_MAX;
		Network[0].flag[1] = -2;
		for (i = 0; i < NumbOfNodes; i++) {
			counter = 0;
			for (j = 0; j < NumbOfNodes; j++) {
				if (c[i][j] > 0) {
					Network[i].PossibleNodes[counter] = j;
					counter++;
				}
			}
		}
		bfs();
		min = c[Network[point].flag[1]][point];
		if (min == 0) {
			break;
		}
		for (i = 0; i < NumbOfNodes; i++) {
			if ((Network[i].flag[0] != 0) && (Network[i].flag[0] < min)) {
				min = Network[i].flag[0];
			}
		}
		result += min;
		for (i = 0; i < NumbOfNodes; i++) {
			if ((Network[i].flag[1] != -1) && (Network[i].flag[1] != -2)) {
				c[Network[i].flag[1]][i] -= min;
				c[i][Network[i].flag[1]] += min;
			}
		}
	}
}

int main() {
	srand(time(NULL));
	clock_t resTest = 0;
	for (int i = 0; i < NumbOfTests; i++) {
		MatrixInit();
		generate();
		clock_t start_time = clock();
		MaxFlow();
		//output();
		clock_t end_time = clock();
		resTest += end_time - start_time;
	}
	resTest /= NumbOfTests;
	cout << resTest;
	return 0;
}