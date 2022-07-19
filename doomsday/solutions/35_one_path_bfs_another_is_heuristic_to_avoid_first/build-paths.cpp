#include <bits/stdc++.h>
#pragma GCC optimize("-Ofast")
#define CURRENT_TIME duration_cast<microseconds>(high_resolution_clock::now() - the_start_time).count()
using namespace std;
using namespace std::chrono;

const bool DEBUG = false;
typedef vector<map<uint16_t, pair<uint16_t, vector<uint16_t>>>> Graph;
typedef tuple<uint16_t, uint16_t, pair<vector<int>, vector<int>>> Query;

int number_of_edges;
Graph G;
vector<Query> queris;

inline vector<int> bfs(int s, int t) {
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
	vector<int> path;
	int u = t; path.push_back(u);
	while (visited[u] != -1) path.push_back(u = visited[u]);
	return path;
}

inline vector<bool> get_doomsdays(vector<int> &path) {
	vector<bool> risks(number_of_edges + 1);
	int path_length = path.size();
	for (int i = 1; i < path_length; i++)
		for (int risk : G[path[i - 1]][path[i]].second)
			risks[risk] = true;
	return risks;
}

typedef pair<int, int16_t> QueueEntry;
struct queue_entity_comparator {
public:
    inline bool operator() (const QueueEntry left, const QueueEntry right) {
	return left.first >= right.first;
    }
};
inline vector<int> dijkstra(int s, int t, vector<bool> risks) {
	int n = G.size() - 1;
	int doomsday_cost = 200 * n;
	vector<bool> visited(n + 1);
	vector<int16_t> prevs(n + 1); prevs[s] = -1;
	vector<int> dists(n + 1); for (int i = 1; i <= n; i++) dists[i] = 0x7fffffff;
	priority_queue<QueueEntry, vector<QueueEntry>, queue_entity_comparator> Q;
	Q.push({0, s});
	while ((not Q.empty()) and !visited[t]) {
		auto [dist, u] = Q.top(); Q.pop();
		if (visited[u]) continue;
		visited[u] = true;
		for (auto &[v, edge] : G[u]) {
			if (DEBUG) cout << u << " -> " << v << endl;
			if (visited[v]) continue;
			auto &[edge_dist, edge_risks] = edge;
			int dist2 = dist + edge_dist;
			for (int risk : edge_risks)
				dist2 += risks[risk] ? doomsday_cost : 0;
			if (dist2 >= dists[v]) continue;
			dists[v] = dist2;
			prevs[v] = u;
			Q.push({dist2, v});
		}
	}
	vector<int> path;
	int u = t; path.push_back(u);
	while (prevs[u] != -1) path.push_back(u = prevs[u]);
	return path;
}

inline void print_path(vector<int> &path) {
	int n = path.size();
	cout << n - 1;
	for (int i = n - 1; i >= 0; i--) cout << " " << path[i];
	cout << endl;
}

int main() {
	auto the_start_time = high_resolution_clock::now();
	int n, m, q;
	cin >> n >> m >> q;
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

	vector<vector<int>> empty_list(0);
	for (int i = 0; i < q; i++) {
		uint16_t u, v; cin >> u >> v;
		queris[i] = {u, v, {{}, bfs(u, v)}};
	}

	for (int i = 0;; i = (i + 1) % (2*q)) {
		if (CURRENT_TIME > 980666) break;
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
