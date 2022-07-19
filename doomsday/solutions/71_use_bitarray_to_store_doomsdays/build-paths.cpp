#include <bits/stdc++.h>
using namespace std;
using namespace std::chrono;

float approximate_task_complexity;
int number_of_edges;
int doomsday_cost;
typedef vector<map<int16_t, pair<uint16_t, vector<uint16_t>>>> Graph;
Graph G;
typedef vector<int16_t> Path;
typedef tuple<uint16_t, uint16_t, pair<Path, Path>> Query;
vector<Query> queris;

Path bfs(int s, int t) {
	vector<int16_t> previous(G.size());
	queue<int16_t> Q;
	Q.push(s);
	previous[s] = -1;
	while (!previous[t]) {
		int16_t u = Q.front(); Q.pop();
		for (auto &[v, edge] : G[u]) {
			if (previous[v]) continue;
			previous[v] = u;
			Q.push(v);
		}
	}
	Path path;
	int u = t; path.push_back(u);
	while (previous[u] != -1) path.push_back(u = previous[u]);
	return path;
}

Path dfs(int s, int t) {
	vector<int16_t> previous(G.size());
	stack<int16_t> Q;
	Q.push(s);
	previous[s] = -1;
	while (!previous[t]) {
		int16_t u = Q.top(); Q.pop();
		for (auto &[v, edge] : G[u]) {
			if (previous[v]) continue;
			previous[v] = u;
			Q.push(v);
		}
	}
	Path path;
	int u = t; path.push_back(u);
	while (previous[u] != -1) path.push_back(u = previous[u]);
	return path;
}

Path rfs3(int s, int t) {
	vector<int16_t> previous(G.size());
	queue<int16_t> Q;
	Q.push(s);
	previous[s] = -1;
	while (!previous[t]) {
		int16_t u = Q.front(); Q.pop();
		if (rand() % 3 == 0) {
			Q.push(u);
			continue;
		}
		for (auto &[v, edge] : G[u]) {
			if (previous[v]) continue;
			previous[v] = u;
			Q.push(v);
		}
	}
	Path path;
	int u = t; path.push_back(u);
	while (previous[u] != -1) path.push_back(u = previous[u]);
	return path;
}

Path rfs4(int s, int t) {
	vector<int16_t> previous(G.size());
	queue<int16_t> Q;
	Q.push(s);
	previous[s] = -1;
	while (!previous[t]) {
		int16_t u = Q.front(); Q.pop();
		if (rand() % 4 == 0) {
			Q.push(u);
			continue;
		}
		for (auto &[v, edge] : G[u]) {
			if (previous[v]) continue;
			previous[v] = u;
			Q.push(v);
		}
	}
	Path path;
	int u = t; path.push_back(u);
	while (previous[u] != -1) path.push_back(u = previous[u]);
	return path;
}

vector<bool> get_doomsdays_and_sum_of_edge_costs(Path &path, int *edge_costs) {
	vector<bool> risks(number_of_edges + 1);
	int path_length = path.size();
	for (int i = 1; i < path_length; i++) {
		auto &[edge_cost, edge_risks] = G[path[i - 1]][path[i]];
		for (int risk : edge_risks)
			risks[risk] = true;
		*edge_costs += edge_cost;
	}
	return risks;
}

vector<bool> get_doomsdays(Path &path) {
	vector<bool> risks(number_of_edges + 1);
	int path_length = path.size();
	for (int i = 1; i < path_length; i++)
		for (int risk : G[path[i - 1]][path[i]].second)
			risks[risk] = true;
	return risks;
}

int get_path_cost(pair<Path, Path> &paths) {
	int cost = 0;
	auto risks1 = get_doomsdays_and_sum_of_edge_costs(paths.first, &cost);
	auto risks2 = get_doomsdays_and_sum_of_edge_costs(paths.second, &cost);
	for (int i = 0; i <= number_of_edges; i++)
		if (risks1[i] && risks2[i])
			cost += doomsday_cost;
	return cost;
}


typedef array<int, 4> SetNode;
vector<SetNode> set_nodes;
vector<uint16_t> bits;

bool set_contains(int set_index, int elem) {
	int lvl0 = set_nodes[set_index][elem >> 12];
	int lvl1 = set_nodes[lvl0][(elem >> 10) & 3];
	int lvl2 = set_nodes[lvl1][(elem >> 8) & 3];
	int lvl3 = set_nodes[lvl2][(elem >> 6) & 3];
	int lvl4 = set_nodes[lvl3][(elem >> 4) & 3];
	return (bits[lvl4] >> (elem & 0xf)) & 1;
}

int set_insert(int set_index, int elem) {
	SetNode lvl0 = set_nodes[set_index];
	SetNode lvl1 = set_nodes[lvl0[elem >> 12]];
	SetNode lvl2 = set_nodes[lvl1[(elem >> 10) & 3]];
	SetNode lvl3 = set_nodes[lvl2[(elem >> 8) & 3]];
	SetNode lvl4 = set_nodes[lvl3[(elem >> 6) & 3]];
	uint16_t lvl5 = bits[lvl4[(elem >> 4) & 3]];
	lvl5 |= 1 << (elem & 0xf); bits.push_back(lvl5);
	lvl4[(elem >> 4) & 3] = bits.size() - 1; set_nodes.push_back(lvl4);
	lvl3[(elem >> 6) & 3] = set_nodes.size() - 1; set_nodes.push_back(lvl3);
	lvl2[(elem >> 8) & 3] = set_nodes.size() - 1; set_nodes.push_back(lvl2);
	lvl1[(elem >> 10) & 3] = set_nodes.size() - 1; set_nodes.push_back(lvl1);
	lvl0[(elem >> 12) & 3] = set_nodes.size() - 1; set_nodes.push_back(lvl0);
	return set_nodes.size() - 1;
}

typedef pair<int, int16_t> QueueEntry;
struct queue_entity_comparator {
    bool operator() (const QueueEntry &left, const QueueEntry &right) {
	return left.first > right.first;
    }
};


Path greedy(int16_t s, int16_t t, vector<bool> risks) {
	bits = {0};
	set_nodes = {{}};
	int n = G.size() - 1;
	vector<int> min_cost(n + 1, 0x7fffffff);
	vector<int16_t> parents(n + 1);
	vector<int> doomsdays_sets(n + 1);
	priority_queue<QueueEntry, vector<QueueEntry>, queue_entity_comparator> queue;
	parents[s] = -1;
	queue.push({0, s});
	while (true) {
		auto [current_cost, u] = queue.top(); queue.pop();
		if (u == t) break;
		if (min_cost[u] < current_cost) continue;
		for (auto &[v, uv_edge] : G[u]) {
			if (min_cost[v] <= current_cost) continue;
			auto &[edge_cost, edge_risks] = uv_edge;
			int v_cost = current_cost + edge_cost;
			auto next_set = doomsdays_sets[u];
			for (auto risk : edge_risks) {
				if (!risks[risk]) continue;
				if (set_contains(next_set, risk)) continue;
				next_set = set_insert(next_set, risk);
				v_cost += doomsday_cost;
			}
			if (min_cost[v] <= v_cost) continue;
			min_cost[v] = v_cost;
			parents[v] = u;
			doomsdays_sets[v] = next_set;
			queue.push({v_cost, v});
		}
	}
	Path path = {t};
	while (t != s) path.push_back(t = parents[t]);
	return path;
}

void print_path(Path &path) {
	int n = path.size();
	cout << n - 1;
	for (int i = n - 1; i >= 0; i--) cout << " " << path[i];
	cout << endl;
}

int main() {
	auto the_start_time = high_resolution_clock::now();
	srand(0x28fc927b);
	int n, m, q;
	cin >> n >> m >> q;
	approximate_task_complexity = q * m / 9216.666;
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

	int available_iterations = 100 / approximate_task_complexity;
	for (int i = 0; i < q; i++) {
		uint16_t u, v;
		cin >> u >> v;
		pair<Path, Path> best_paths;
		best_paths.first = bfs(u, v);
		best_paths.second = greedy(u, v, get_doomsdays(best_paths.first));
		int min_cost = get_path_cost(best_paths);
		for (int i = 0; i < available_iterations; i++) {
			pair<Path, Path> paths;
			paths.first = (i == 0) ? dfs(u, v) : (i % 2 == 1) ? rfs3(u, v) : rfs4(u, v);
			paths.second = greedy(u, v, get_doomsdays(paths.first));
			int cost = get_path_cost(paths);
			if (cost < min_cost) {
				best_paths = paths;
				min_cost = cost;
			}
		}
		queris[i] = {u, v, best_paths};
	}

	for (int i = 0;; i++) {
		auto dt = duration_cast<microseconds>(high_resolution_clock::now() - the_start_time);
		if (dt.count() > 983666) break;
		auto &[u, v, paths] = queris[i % q];
		if ((i % (2*q)) < q) paths.first = greedy(u, v, get_doomsdays(paths.second));
		else paths.second = greedy(u, v, get_doomsdays(paths.first));
	}

	for (int i = 0; i < q; i++) {
		auto &[u, v, paths] = queris[i];
		print_path(paths.first);
		print_path(paths.second);
	}
	return 0;
}
