#include <bits/stdc++.h>
#pragma GCC optimize("-Ofast")
using namespace std;

const bool DEBUG = false;
typedef vector<map<int, pair<int, set<int>>>> Graph;
typedef tuple<int, int, vector<int>, vector<int>> Query;

int number_of_edges;
Graph G;
vector<Query> queris;

inline vector<int> simple_bfs(int s, int t) {
	int n = G.size() - 1;
	vector<int> visited(n + 1);
	queue<int> Q;
	Q.push(s);
	visited[s] = -1;
	while (!visited[t]) {
		int u = Q.front(); Q.pop();
		if (DEBUG) cout << u << endl;
		for (auto &[v, edge] : G[u]) {
			if (DEBUG) cout << u << " -> " << v << endl;
			if (visited[v]) continue;
			visited[v] = u;
			Q.push(v);
		}
	}
	vector<int> path;
	int u = t; path.push_back(u);
	while (visited[u] != -1) path.push_back(u = visited[u]);
	return path;
}

inline vector<int> simple_dfs(int s, int t) {
	int n = G.size() - 1;
	vector<int> visited(n + 1);
	stack<int> Q;
	Q.push(s);
	visited[s] = -1;
	while (!visited[t]) {
		int u = Q.top(); Q.pop();
		if (DEBUG) cout << u << endl;
		for (auto &[v, edge] : G[u]) {
			if (DEBUG) cout << u << " -> " << v << endl;
			if (visited[v]) continue;
			visited[v] = u;
			Q.push(v);
		}
	}
	vector<int> path;
	int u = t; path.push_back(u);
	while (visited[u] != -1) path.push_back(u = visited[u]);
	return path;
}


inline void print_path(vector<int> &path) {
	int n = path.size();
	cout << n - 1;
	for (int i = n - 1; i >= 0; i--) cout << " " << path[i];
	cout << endl;
}

int main() {
	int n, m, q;
	cin >> n >> m >> q;
	number_of_edges = m;
	G.resize(n + 1);
	queris.resize(q);
	
	for (int i = 0; i < m; i++) {
		int u, v, w, k;
		cin >> u >> v >> w >> k;
		set<int> C;
		for (int i = 0; i < k; i++) {
			int c;
			cin >> c;
			C.insert(c);
		}
		G[u][v] = {w, C};
		G[v][u] = {w, C};
	}

	while (q--) {
		int u, v; cin >> u >> v;
		vector<int> path1, path2;
		path1 = simple_bfs(u, v);
		path2 = simple_dfs(u, v);
		print_path(path1);
		print_path(path2);
	}
}
