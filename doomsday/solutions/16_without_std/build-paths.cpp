#include <bits/stdc++.h>
#pragma GCC optimize("-Ofast")
using namespace std;

const bool DEBUG = false;
typedef vector<map<int, pair<int, set<int>>>> Graph;
typedef tuple<int, int, vector<int>, vector<int>> Query;

int number_of_edges;
Graph G;
vector<Query> queris;

inline vector<bool> get_doomsdays(vector<int> &path) {
	vector<bool> risks(number_of_edges + 1);
	int path_length = path.size();
	for (int i = 1; i < path_length; i++)
		for (int risk : G[path[i - 1]][path[i]].second)
			risks[risk] = true;
	return risks;
}

inline vector<int> dijkstra1(int s, int t) {
	int n = G.size() - 1;
	vector<int> visited(n + 1);
	vector<vector<pair<int, int>>> Q(50000 * 16 + 1);
	auto *queue_top = &Q[0];
	(*queue_top).push_back({s, -1});
	while (!visited[t]) {
		while (queue_top->empty()) queue_top++;
		auto [u, prev] = (*queue_top).back(); (*queue_top).pop_back();
		if (visited[u]) continue;
		visited[u] = prev;
		for (auto &[v, edge] : G[u]) {
			if (DEBUG) cout << u << " -> " << v << endl;
			if (visited[v]) continue;
			auto &[edge_dist, edge_risks] = edge;
			int dist2 = edge_risks.size();
			(queue_top + dist2)->push_back({v, u});
		}
	}
	if (DEBUG) cout << "t found in " << queue_top - &Q[0] << " lvls" << endl;
	vector<int> path;
	int u = t; path.push_back(u);
	while (visited[u] != -1) path.push_back(u = visited[u]);
	reverse(path.begin(), path.end());
	return path;
}

inline vector<int> dijkstra2(int s, int t, vector<bool> risks) {
	int n = G.size() - 1;
	vector<int> visited(n + 1);
	vector<vector<pair<int, int>>> Q(50000 * 16 + 1);
	auto *queue_top = &Q[0];
	(*queue_top).push_back({s, -1});
	while (!visited[t]) {
		while (queue_top->empty()) queue_top++;
		auto [u, prev] = (*queue_top).back(); (*queue_top).pop_back();
		if (visited[u]) continue;
		visited[u] = prev;
		for (auto &[v, edge] : G[u]) {
			if (DEBUG) cout << u << " -> " << v << endl;
			if (visited[v]) continue;
			auto &[edge_dist, edge_risks] = edge;
			int dist2 = 0; for (int risk : edge_risks) dist2 += risks[risk] ? 1 : 0;
			(queue_top + dist2)->push_back({v, u});
		}
	}
	vector<int> path;
	int u = t; path.push_back(u);
	while (visited[u] != -1) path.push_back(u = visited[u]);
	reverse(path.begin(), path.end());
	return path;
}

inline void print_path(vector<int> &path) {
	int n = path.size();
	cout << n - 1;
	for (int i = 0; i < n; i++) cout << " " << path[i];
	cout << endl;
}

inline void *process_query(void *query_as_void) {
	Query *query = (Query *) query_as_void;
	int u = get<0>(*query);
	int v = get<1>(*query);
	vector<int> path1 = dijkstra1(u, v);
	get<2>(*query) = path1;
	get<3>(*query) = dijkstra2(u, v, get_doomsdays(path1));
	return NULL;
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

	for (int i = 0; i < q; i++) {
		int u, v;
		cin >> u >> v;
		vector<int> empty_list(0);
		queris[i] = {u, v, empty_list, empty_list};
	}

	for (int i = 0; i < q; i++) {
		process_query(&queris[i]);
		print_path(get<2>(queris[i]));
		print_path(get<3>(queris[i]));
	}
}
