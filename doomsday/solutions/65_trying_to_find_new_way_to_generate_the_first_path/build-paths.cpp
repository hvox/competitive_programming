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
float approximate_task_complexity;

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

inline Path dfs(int s, int t) {
	int n = G.size() - 1;
	vector<int16_t> visited(n + 1);
	stack<int16_t> Q;
	Q.push(s);
	visited[s] = -1;
	while (!visited[t]) {
		int16_t u = Q.top(); Q.pop();
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

inline Path rfs(int s, int t) {
	int n = G.size() - 1;
	vector<int16_t> visited(n + 1);
	queue<int16_t> Q;
	Q.push(s);
	visited[s] = -1;
	while (!visited[t]) {
		int16_t u = Q.front(); Q.pop();
		if (rand() % 4 == 0) {
			Q.push(u);
			continue;
		}
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

inline vector<bool> get_doomsdays_and_sum_of_edge_costs(Path &path, int *edge_costs) {
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

inline int get_current_cost(Query &query) {
	int cost = 0;
	auto &paths = get<2>(query);
	auto risks1 = get_doomsdays_and_sum_of_edge_costs(paths.first, &cost);
	auto risks2 = get_doomsdays_and_sum_of_edge_costs(paths.second, &cost);
	for (int i = 0; i <= number_of_edges; i++)
		if (risks1[i] && risks2[i])
			cost += doomsday_cost;
	return cost;
}

typedef pair<int, int16_t> QueueEntry;
// { current_cost, vertix }

struct queue_entity_comparator {
public:
    inline bool operator() (const QueueEntry &left, const QueueEntry &right) {
	return left.first > right.first;
    }
};

typedef array<int, 4> SetNode;
inline bool contains(vector<SetNode> &set_nodes, int lvl0, int elem) {
	int lvl1 = set_nodes[lvl0][elem >> 12];
	int lvl2 = set_nodes[lvl1][(elem >> 10) & 3];
	int lvl3 = set_nodes[lvl2][(elem >> 8) & 3];
	int lvl4 = set_nodes[lvl3][(elem >> 6) & 3];
	int lvl5 = set_nodes[lvl4][(elem >> 4) & 3];
	int lvl6 = set_nodes[lvl5][(elem >> 2) & 3];
	int lvl7 = set_nodes[lvl6][(elem >> 0) & 3];
	return lvl7;
}

inline int insert(vector<SetNode> &set_nodes, int set_index, int elem) {
	SetNode lvl0 = set_nodes[set_index];
	SetNode lvl1 = set_nodes[lvl0[elem >> 12]];
	SetNode lvl2 = set_nodes[lvl1[(elem >> 10) & 3]];
	SetNode lvl3 = set_nodes[lvl2[(elem >> 8) & 3]];
	SetNode lvl4 = set_nodes[lvl3[(elem >> 6) & 3]];
	SetNode lvl5 = set_nodes[lvl4[(elem >> 4) & 3]];
	SetNode lvl6 = set_nodes[lvl5[(elem >> 2) & 3]];
	lvl6[elem & 3] = 1; set_nodes.push_back(lvl6);
	lvl5[(elem >> 2) & 3] = set_nodes.size() - 1; set_nodes.push_back(lvl5);
	lvl4[(elem >> 4) & 3] = set_nodes.size() - 1; set_nodes.push_back(lvl4);
	lvl3[(elem >> 6) & 3] = set_nodes.size() - 1; set_nodes.push_back(lvl3);
	lvl2[(elem >> 8) & 3] = set_nodes.size() - 1; set_nodes.push_back(lvl2);
	lvl1[(elem >> 10) & 3] = set_nodes.size() - 1; set_nodes.push_back(lvl1);
	lvl0[(elem >> 12) & 3] = set_nodes.size() - 1; set_nodes.push_back(lvl0);
	return set_nodes.size() - 1;
}

inline Path greedy(int16_t s, int16_t t, vector<bool> risks) {
	vector<SetNode> set_nodes = {{0, 0, 0, 0}, {1, 1, 1, 1}};
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
				if (contains(set_nodes, next_set, risk)) continue;
				next_set = insert(set_nodes, next_set, risk);
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

inline void print_path(Path &path) {
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

	for (int i = 0; i < q; i++) {
		uint16_t u, v; cin >> u >> v;
		if (approximate_task_complexity > 100 || true) {
			//queris[i] = {u, v, {{}, bfs(u, v)}}; // BFS
			//queris[i] = {u, v, {{}, dfs(u, v)}}; // DFS
		       	queris[i] = {u, v, {{}, rfs(u, v)}}; // RFS
			continue;
		}
		Query q1 = {u, v, {bfs(u, v), {}}};
		get<2>(q1).second = greedy(u, v, get_doomsdays(get<2>(q1).first));
		Query q2 = {u, v, {dfs(u, v), {}}};
		get<2>(q2).second = greedy(u, v, get_doomsdays(get<2>(q2).first));
		queris[i] = get_current_cost(q1) < get_current_cost(q2) ? q1 : q2;
	}

	int iterations;
	for (iterations = 0; true; iterations++) {
		int i = iterations;
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
