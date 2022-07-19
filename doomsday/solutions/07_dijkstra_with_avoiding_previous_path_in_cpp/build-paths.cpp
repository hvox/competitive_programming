#include <bits/stdc++.h>
#pragma GCC optimize("-Ofast")
using namespace std;

const bool DEBUG = false;
typedef vector<map<int, pair<int, set<int>>>> Graph;

vector<bool> get_doomsdays(Graph &G, vector<int> &path) {
	vector<bool> risks(50000);
	int path_length = path.size();
	for (int i = 1; i < path_length; i++)
		for (int risk : G[path[i - 1]][path[i]].second)
			risks[risk] = true;
	return risks;
}

vector<int> dijkstra(Graph &G, int s, int t, vector<bool> &risks) {
	bool avoid_all_risks = (risks.size() == 0);
	int n = G.size() - 1;
	vector<int> visited(n + 1); for (int i = 0; i < n; i++) if (visited[i] != 0) *((int *) NULL) = 0;
	priority_queue<tuple<int, int, int>> Q;
	Q.push({-0, s, -1});
	while ((not Q.empty()) and !visited[t]) {
		auto [dist, u, prev] = Q.top(); Q.pop();
		if (DEBUG) cout << -dist << " - " << u << " prev=" << prev << endl;
		if (visited[u]) continue;
		if (DEBUG) cout << "not skipped" << endl;
		visited[u] = prev;
		dist = -dist;
		for (auto &[v, edge] : G[u]) {
			if (DEBUG) cout << u << " -> " << v << endl;
			if (visited[v]) continue;
			auto &[edge_dist, edge_risks] = edge;
			int dist2 = 0;
			if (avoid_all_risks)
				dist2 = edge_risks.size();
			else
				for (int risk : edge_risks)
					dist2 += risks[risk] ? 1 : 0;
			Q.push({-(dist + dist2), v, u});
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

int main() {
	int n, m, q;
	cin >> n >> m >> q;
	vector<map<int, pair<int, set<int>>>> G(n + 1);
	
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
		vector<bool> empty_doomsdays_list(0);
		vector<int> path1 = dijkstra(G, u, v, empty_doomsdays_list);
		vector<bool> doomsdays = get_doomsdays(G, path1);
		vector<int> path2 = dijkstra(G, u, v, doomsdays);
		print_path(path1);
		print_path(path2);
	}
}
