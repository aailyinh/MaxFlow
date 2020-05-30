#include <iostream>
#include <stdlib.h>  
#include <ctime>
#define NumbOfNodes 100
#define maxvalue 2000
#define NumbOfTests 1

using namespace std;

struct Node
{
	int number;
	int flag[2];
};

bool IsEnd(int ends[], int numbofend, int k) {
	if (numbofend == -1) {
		return false;
	}
	for (int l = 0; l <= numbofend; l++) {
		if (ends[l] == k) {
			return true;
		}
	}
	return false;
}

int C[NumbOfNodes][NumbOfNodes] = { 0 };
int c[NumbOfNodes][NumbOfNodes] = { 0 };
int result = 0;

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
	Node PossibleNodes[NumbOfNodes];
	Node Network[NumbOfNodes];
	for (int i = 0; i < NumbOfNodes; i++) {
		Network[i].number = i;
		Network[i].flag[0] = 0;
		Network[i].flag[1] = -1;
	}
	int i = 0, j = 0, k = 0, node = 0, max = 0, min = 0;
	int ends[NumbOfNodes] = { 0 };
	while (1) {
		if (j == -1) {
			break;
		}
		for (i = 0; i < NumbOfNodes; i++) {
			Network[i].flag[0] = 0;
			Network[i].flag[1] = -1;
		}
		i = 0;
		Network[i].flag[0] = INT16_MAX;
		Network[i].flag[1] = -2;
		node = 0;
		int NumbOfEnds = -1;
		while (node != NumbOfNodes - 1) {
			j = -1;
			for (k = 0; k < NumbOfNodes; k++) {
				if (c[i][k] != 0) {
					if (i > 0) {
						if ((Network[k].flag[1]==-1) && !IsEnd(&ends[0], NumbOfEnds, k)) {
							j++;
							PossibleNodes[j].number = k;
						}
					}
					else {
						if (!IsEnd(&ends[0], NumbOfEnds, k)) {
							j++;
							PossibleNodes[j].number = k;
						}
					}
				}
			}
			if (j != -1) {
				max = c[i][PossibleNodes[0].number];
				node = PossibleNodes[0].number;
				for (k = 0; k <= j; k++) {
					if (c[i][PossibleNodes[k].number] > max) {
						node = PossibleNodes[k].number;
						max = c[i][node];
					}
				}
				Network[node].flag[0] = max;
				Network[node].flag[1] = i;
				i = node;
			}
			else {
				if (i == 0) {
					node = NumbOfNodes - 1;
				}
				else {
					NumbOfEnds++;
					ends[NumbOfEnds] = i;
					node = Network[i].flag[1];
					Network[i].flag[0] = 0;
					Network[i].flag[1] = -1;
					i = node;
				}
			}
		}
		if (j != -1) {
			min = c[Network[node].flag[1]][node];
			for (k = 0; k < NumbOfNodes; k++) {
				if ((Network[k].flag[0] != 0) && (Network[k].flag[0] < min)) {
					min = Network[k].flag[0];
				}
			}
			result += min;
			for (k = 0; k < NumbOfNodes; k++) {
				if ((Network[k].flag[1] != -1) && (Network[k].flag[1] != -2)) {
					c[Network[k].flag[1]][k] -= min;
					c[k][Network[k].flag[1]] += min;
				}
			}
			j = 0;
		}
	}
}

int main() {
	srand(time(NULL));
	double resTest = 0;
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