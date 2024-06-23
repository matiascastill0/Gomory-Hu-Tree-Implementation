#include <iostream>
#include <vector>
#include <queue>
#include <climits>
#include <algorithm>

using namespace std;

const int INF = numeric_limits<int>::max();

struct Edge {
    int from, to, cap, flow;
};

class Graph {
public:
    int n;
    vector<Edge> edges;
    vector<vector<int>> adj;

    Graph(int n) : n(n), adj(n) {}

    void addEdge(int from, int to, int cap) {
        edges.push_back({from, to, cap, 0});
        adj[from].push_back(edges.size() - 2);
        adj[to].push_back(edges.size() - 1);
    }

    int bfs(int s, int t, vector<int>& parent) {
        fill(parent.begin(), parent.end(), -1);
        queue<pair<int, int>> q;
        q.push({s, INF});

        parent[s] = -2;

        while (!q.empty()) {
            int cur = q.front().first;
            int flow = q.front().second;
            q.pop();

            for (int id : adj[cur]) {
                Edge& e = edges[id];
                if (parent[e.to] == -1 && e.cap - e.flow > 0) {
                    parent[e.to] = id;
                    int newFlow = min(flow, e.cap - e.flow);

                    if (e.to == t) {
                        return newFlow;
                    }

                    q.push({e.to, newFlow});
                }
            }
        }

        return 0;
    }

    int maxFlow(int s, int t) {
        int flow = 0;
        vector<int> parent(n);
        int newFlow;

        while (newFlow = bfs(s, t, parent)) {
            flow += newFlow;
            int cur = t;

            while (cur != s) {
                int id = parent[cur];
                edges[id].flow += newFlow;
                edges[id ^ 1].flow -= newFlow;
                cur = edges[id].from;
            }
        }

        return flow;
    }

    void gomoryHu(Graph& g) {
        vector<int> p(g.n);
        for (int i = 1; i < g.n; ++i) {
            p[i] = 0;
            int f = g.maxFlow(i, p[i]);
            cout << "Max flow from node " << i << " to node " << p[i] << ": " << f << endl;
        }
    }
};
