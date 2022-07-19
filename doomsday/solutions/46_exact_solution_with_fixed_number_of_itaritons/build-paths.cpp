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

const size_t cache_size = 33;
typedef pair<pair<uint16_t, uint16_t>, pair<int16_t, array<int16_t, cache_size>>> QueueEntry;
struct queue_entity_comparator {
public:
    inline bool operator() (const QueueEntry left, const QueueEntry right) {
	auto [d1, w1] = left.first;
	auto [d2, w2] = right.first;
	return d1 * doomsday_cost + w1 >= d2 * doomsday_cost + w2;
    }
};

bool array_contains(array<int16_t, cache_size> &array, int16_t needle) {
	for (auto element : array) if (element == needle) return true;
	return false;
}

inline Path dijkstra(int16_t s, int16_t t, vector<bool> risks) {
	int n = G.size() - 1;
	vector<int16_t> visited(n + 1);
	priority_queue<tuple<int, set<uint16_t>, int16_t, int16_t>> Q;
	set<uint16_t> start_risks;
	tuple<int, set<uint16_t>, int16_t, int16_t> start_state = {-0, start_risks, s, -1};
	Q.push(start_state);
	while ((not Q.empty()) and !visited[t]) {
		auto [negative_dist, current_risks, u, prev] = Q.top(); Q.pop();
		if (DEBUG) cout << u << "  dist = " << -negative_dist << endl;
		if (visited[u]) continue;
		visited[u] = prev;
		int dist = -negative_dist;
		for (auto &[v, edge] : G[u]) {
			if (DEBUG) cout << u << " -> " << v << endl;
			if (visited[v]) continue;
			auto &[edge_dist, edge_risks] = edge;
			int dist2 = dist + edge_dist;
			set<uint16_t> next_risks = current_risks;
			for (uint16_t risk : edge_risks) {
				if (risks[risk] and not next_risks.contains(risk)) {
					next_risks.insert(risk);
					dist2 += doomsday_cost;
				}
			}
			Q.push({-dist2, next_risks, v, u});
		}
	}
	Path path;
	auto u = t;
	path.push_back(u);
	while (visited[u] != -1) {
		u = visited[u];
		path.push_back(u);
	}
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

	for (int i = 0; i < 4 * q; i++) {
		if (CURRENT_TIME > 980666) break;
		auto &[u, v, paths] = queris[i % q];
		if (i < (q % (2*q))) paths.first = dijkstra(u, v, get_doomsdays(paths.second));
		else paths.second = dijkstra(u, v, get_doomsdays(paths.first));
	}

	for (int i = 0; i < q; i++) {
		auto &paths = get<2>(queris[i]);
		print_path(paths.first.size() ? paths.first : paths.second);
		print_path(paths.second);
	}
	return 0;
}
