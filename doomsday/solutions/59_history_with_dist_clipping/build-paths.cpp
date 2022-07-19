#include <bits/stdc++.h>
#pragma GCC optimize("-Ofast")
#define CURRENT_TIME duration_cast<microseconds>(high_resolution_clock::now() - the_start_time).count()
#define HISTORY_SIZE 64
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


typedef tuple<int16_t, int16_t, set<uint16_t>> PathNode;
// { vertix, previous_path_entry, set of doomsdays }
typedef pair<int, int16_t> QueueEntry;
// { current_cost, path node index }

struct queue_entity_comparator {
public:
    inline bool operator() (const QueueEntry &left, const QueueEntry &right) {
	return left.first > right.first;
    }
};

inline Path greedy(int16_t s, int16_t t, vector<bool> risks) {
	int n = G.size() - 1;
	vector<int> min_cost(n + 1, 0x7fffffff);
	vector<PathNode> nodes = {{s, 0, {}}};
	priority_queue<QueueEntry, vector<QueueEntry>, queue_entity_comparator> queue;
	queue.push({0, 0});
	int t_node = -1;
	while (true) {
		auto [current_cost, node_index] = queue.top(); queue.pop();
		auto u = get<0>(nodes[node_index]);
		if (u == t) {
			t_node = node_index;
			break;
		}
		if (min_cost[u] < current_cost) continue;
		for (auto &[v, uv_edge] : G[u]) {
			if (min_cost[v] <= current_cost) continue;
			auto &[edge_cost, edge_risks] = uv_edge;
			int v_cost = current_cost + edge_cost;
			set<uint16_t> next_node_risks;
			for (auto risk : edge_risks) {
				if (!risks[risk]) continue;
				next_node_risks.insert(risk);
				int i = node_index;
				int risk_cost = doomsday_cost;
				for (int _ = 0; _ < HISTORY_SIZE; _++) {
					if (get<2>(nodes[i]).contains(risk))
						risk_cost = 0;
					i = get<1>(nodes[i]);
				}
				v_cost += risk_cost;
			}
			if (min_cost[v] <= v_cost) continue;
			min_cost[v] = v_cost;
			queue.push({v_cost, nodes.size()});
			nodes.push_back({v, node_index, next_node_risks});
		}
	}
	Path path;
	path.push_back(t);
	while (t_node != 0) {
		t_node = get<1>(nodes[t_node]);
		path.push_back(get<0>(nodes[t_node]));
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

	for (int i = 0; i < q || true; i++) {
		if (CURRENT_TIME > 983666) break;
		auto &[u, v, paths] = queris[i % q];
		if ((i % (2*q)) < q) paths.first = greedy(u, v, get_doomsdays(paths.second));
		else paths.second = greedy(u, v, get_doomsdays(paths.first));
	}

	for (int i = 0; i < q; i++) {
		auto &paths = get<2>(queris[i]);
		print_path(paths.first.size() ? paths.first : paths.second);
		print_path(paths.second);
	}
	return 0;
}
