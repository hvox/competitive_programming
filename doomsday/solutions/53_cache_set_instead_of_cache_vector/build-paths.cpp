#include <bits/stdc++.h>
#pragma GCC optimize("-Ofast")
#define CURRENT_TIME duration_cast<microseconds>(high_resolution_clock::now() - the_start_time).count()
using namespace std;
using namespace std::chrono;

const bool DEBUG = false;
typedef vector<map<int16_t, pair<uint16_t, vector<uint16_t>>>> Graph;
typedef vector<int16_t> Path;
typedef tuple<uint16_t, uint16_t, pair<Path, Path>> Query;

int number_of_edges;
Graph G;
vector<Query> queris;
int doomsday_cost;

inline Path bfs(int s, int t) {
	int n = G.size() - 1;
	vector<int16_t> visited(n + 1);
	queue<int16_t> Q;
	Q.push(s);
	visited[s] = -1;
	while (!visited[t]) {
		int16_t u = Q.front(); Q.pop();
		if (DEBUG) cout << u << endl;
		for (auto &[v, edge] : G[u]) {
			if (DEBUG) cout << u << " -> " << v << endl;
			if (visited[v]) continue;
			visited[v] = u;
			Q.push(v);
		}
	}
	Path path;
	int u = t; path.push_back(u);
	while (visited[u] != -1) path.push_back(u = visited[u]);
	return path;
}

inline vector<bool> get_doomsdays(Path &path) {
	vector<bool> risks(number_of_edges + 1);
	int path_length = path.size();
	for (int i = 1; i < path_length; i++)
		for (int risk : G[path[i - 1]][path[i]].second)
			risks[risk] = true;
	return risks;
}

const size_t cache_size = 257;
typedef pair<pair<uint16_t, uint16_t>, pair<int16_t, array<uint8_t, cache_size>>> QueueEntry;
struct queue_entity_comparator {
public:
    inline bool operator() (const QueueEntry left, const QueueEntry right) {
	auto [d1, w1] = left.first;
	auto [d2, w2] = right.first;
	return d1 * doomsday_cost + w1 > d2 * doomsday_cost + w2;
    }
};

inline Path dijkstra(int16_t s, int16_t t, vector<bool> risks) {
	int n = G.size() - 1;
	vector<bool> visited(n + 1);
	vector<int16_t> prevs(n + 1); prevs[s] = -1;
	vector<int> dists(n + 1); for (int i = 1; i <= n; i++) dists[i] = 0x7fffffff;
	priority_queue<QueueEntry, vector<QueueEntry>, queue_entity_comparator> Q;
	Q.push({{0, 0}, {s, {0, 0, 0}}});
	while (!visited[t]) {
		auto [dooms_and_w, u_and_last_risks] = Q.top(); Q.pop();
		auto [dooms, w] = dooms_and_w;
		auto [u, last_risks] = u_and_last_risks;
		if (visited[u]) continue;
		visited[u] = true;
		for (auto &[v, edge] : G[u]) {
			auto new_last_risks = last_risks;
			if (DEBUG) cout << u << " -> " << v << endl;
			if (visited[v]) continue;
			auto &[edge_w, edge_risks] = edge;
			int w2 = w + edge_w;
			int d2 = dooms;
			for (int risk : edge_risks) {
				if (!risks[risk]) continue;
				if (new_last_risks[risk % cache_size] == risk & 0xff) continue;
				new_last_risks[risk % cache_size] = risk & 0xff;
				d2++;

			}
			if (d2 * doomsday_cost + w2 >= dists[v]) continue;
			dists[v] = d2 * doomsday_cost + w2;
			prevs[v] = u;
			Q.push({{d2, w2}, {v, new_last_risks}});
		}
	}
	Path path;
	int u = t; path.push_back(u);
	while (prevs[u] != -1) path.push_back(u = prevs[u]);
	return path;
}

inline void print_path(Path &path) {
	int n = path.size();
	cout << n - 1;
	for (int i = n - 1; i >= 0; i--) cout << " " << path[i];
	cout << endl;
}

int main() {
	auto the_start_time = high_resolution_clock::now();
	int n, m, q;
	cin >> n >> m >> q;
	doomsday_cost = n * 400;
	number_of_edges = m;
	G.resize(n + 1);
	queris.resize(q);
	
	for (int i = 0; i < m; i++) {
		uint16_t u, v, w, k;
		cin >> u >> v >> w >> k;
		vector<uint16_t> C;
		for (int i = 0; i < k; i++) {
			uint16_t c;
			cin >> c;
			C.push_back(c);
		}
		G[u][v] = {w, C};
		G[v][u] = {w, C};
	}

	for (int i = 0; i < q; i++) {
		uint16_t u, v; cin >> u >> v;
		queris[i] = {u, v, {{}, bfs(u, v)}};
	}

	for (int i = 0;; i = (i + 1) % (2*q)) {
		if (CURRENT_TIME > 983666) break;
		auto &[u, v, paths] = queris[i % q];
		if (i < q) paths.first = dijkstra(u, v, get_doomsdays(paths.second));
		else paths.second = dijkstra(u, v, get_doomsdays(paths.first));
	}

	for (int i = 0; i < q; i++) {
		auto &paths = get<2>(queris[i]);
		print_path(paths.first.size() ? paths.first : paths.second);
		print_path(paths.second);
	}
	return 0;
}
