#include <bits/stdc++.h>
#pragma GCC optimize("-Ofast")
using namespace std;

const bool DEBUG = false;

typedef vector<map<int, pair<int, set<int>>>> Graph;
typedef tuple<int, int, vector<int>, vector<int>> Query;

int number_of_edges;
Graph G;
vector<Query> queris;

vector<bool> get_doomsdays(vector<int> &path) {
	vector<bool> risks(number_of_edges + 1);
	int path_length = path.size();
	for (int i = 1; i < path_length; i++)
		for (int risk : G[path[i - 1]][path[i]].second)
			risks[risk] = true;
	return risks;
}

vector<int> dijkstra_nonheuristical(int s, int t, vector<bool> &risks) {
	if (DEBUG) cout << "dijkstra nonheuristical" << endl;
	bool avoid_all_risks = (risks.size() == 0);
	int n = G.size() - 1;
	vector<int> visited(n + 1); for (int i = 0; i < n; i++) if (visited[i] != 0) *((int *) NULL) = 0;
	priority_queue<tuple<int, vector<bool>, int, int>> Q;
	vector<bool> start_risks(number_of_edges + 1);
	Q.push({-0, start_risks, s, -1});
	if (DEBUG) cout << "dijkstra nonheuristical queue" << endl;
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
			int dist2 = dist;
			vector<bool> next_risks = current_risks;
			if (avoid_all_risks) {
				for (int risk : edge_risks) {
					if (DEBUG) cout << risk << " -> ";
					if (!next_risks[risk])
						dist2 += (next_risks[risk] = true) ? 1 : 0;
					if (DEBUG) cout << "ok" << endl;
				}
			} else {
				for (int risk : edge_risks)
					if (!next_risks[risk] and risks[risk])
						dist2 += (next_risks[risk] = true) ? 1 : 0;
			}
			Q.push({-dist2, next_risks, v, u});
		}
	}
	if (!visited[t]) {
		vector<int> empty_path(0);
		return empty_path;
	}
	vector<int> path;
	int u = t;
	path.push_back(u);
	while (visited[u] != -1) {
		u = visited[u];
		path.push_back(u);
	}
	reverse(path.begin(), path.end());
	return path;
}

void print_path(vector<int> &path) {
	int n = path.size();
	cout << n - 1;
	for (int i = 0; i < n; i++) cout << " " << path[i];
	cout << endl;
}

void *process_query(void *query_as_void) {
	Query *query = (Query *) query_as_void;
	int u = get<0>(*query);
	int v = get<1>(*query);
	vector<bool> empty_doomsdays_list(0);
	vector<int> path1 = dijkstra_nonheuristical(u, v, empty_doomsdays_list);
	vector<bool> doomsdays = get_doomsdays(path1);
	vector<int> path2 = dijkstra_nonheuristical(u, v, doomsdays);
	get<2>(*query) = path1;
	get<3>(*query) = path2;
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

	/*
	vector<pthread_t> threads(q);
	for (int i = 0; i < q; i++)
		if (pthread_create(&threads[i], NULL, process_query, &queris[i]))
			return 42;
	for (int i = 0; i < q; i++) {
   		void *status;
		pthread_join(threads[i], &status);
		print_path(get<2>(queris[i]));
		print_path(get<3>(queris[i]));
	}
	*/
	for (int i = 0; i < q; i++) {
		process_query(&queris[i]);
		print_path(get<2>(queris[i]));
		print_path(get<3>(queris[i]));
	}
}
