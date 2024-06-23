#include <iostream>
#include <vector>
#include <queue>
#include <climits>
#include <algorithm>
#include <numeric>

using namespace std;

// Estructura para representar unión-buscar
class UnionFind {
private:
    vector<int> parent;
    vector<int> rank;

public:
    UnionFind(int size) : parent(size), rank(size, 0) {
        iota(parent.begin(), parent.end(), 0); // Inicializar el padre de cada elemento como él mismo
    }

    int find(int x) {
        if (parent[x] != x) {
            parent[x] = find(parent[x]); // Compresión de caminos
        }
        return parent[x];
    }

    void unionSet(int x, int y) {
        int rootX = find(x);
        int rootY = find(y);
        if (rootX != rootY) {
            if (rank[rootX] > rank[rootY]) {
                parent[rootY] = rootX;
            } else if (rank[rootX] < rank[rootY]) {
                parent[rootX] = rootY;
            } else {
                parent[rootY] = rootX;
                rank[rootX] += 1;
            }
        }
    }
};

// Clase principal para el algoritmo de Gomory-Hu
class GomoryHuTree {
private:
    int n; // Número de nodos
    vector<vector<int>> capacity; // Matriz de capacidades
    UnionFind uf;

public:
    GomoryHuTree(int n) : n(n), capacity(n, vector<int>(n, 0)), uf(n) {}

    void addEdge(int from, int to, int cap) {
        capacity[from][to] += cap; // no dirigido y múltiples aristas posibles
        capacity[to][from] += cap;
    }

    // Función para calcular el flujo máximo entre dos nodos usando Edmonds-Karp (implementación simplificada)
    int maxFlow(int source, int sink, vector<vector<int>>& residualCapacity) {
        int totalFlow = 0;
        vector<int> parent(n);
        bool hasPath;

        do {
            fill(parent.begin(), parent.end(), -1);
            queue<pair<int, int>> q;
            q.push({source, INT_MAX}); 

            parent[source] = source;
            hasPath = false;

            while (!q.empty()) {
                int current = q.front().first;
                int flow = q.front().second;
                q.pop();

                for (int next = 0; next < n; ++next) {
                    if (parent[next] == -1 && residualCapacity[current][next] > 0) {
                        parent[next] = current;
                        int newFlow = min(flow, residualCapacity[current][next]);

                        if (next == sink) {
                            totalFlow += newFlow;
                            int cur = sink;

                            while (cur != source) {
                                int prev = parent[cur];
                                residualCapacity[prev][cur] -= newFlow;
                                residualCapacity[cur][prev] += newFlow;
                                cur = prev;
                            }

                            hasPath = true;
                            break;
                        }

                        q.push({next, newFlow});
                    }
                }
            }
        } while (hasPath);

        return totalFlow;
    }

    // Construcción del árbol de Gomory-Hu
    vector<vector<int>> buildTree() {
        vector<vector<int>> tree(n, vector<int>(n, 0));
        vector<vector<int>> residualCapacity = capacity;

        for (int i = 1; i < n; ++i) {
            int j = uf.find(i); // Encuentra un representante (raíz inicial)
            int flow = maxFlow(i, j, residualCapacity);

            for (int k = 0; k < n; ++k) {
                if (k != i && uf.find(k) == j && residualCapacity[i][k] == 0) {
                    uf.unionSet(k, i);
                }
            }

            tree[j][i] = flow;
            tree[i][j] = flow;
        }

        return tree;
    }
};
