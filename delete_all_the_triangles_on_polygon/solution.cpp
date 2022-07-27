#include <bits/stdc++.h>
using namespace std;
#pragma GCC optimize("O3,unroll-loops")

const int inf = 1e6;

vector<vector<int>> G;
map<pair<int, int>, set<int>> edges;

tuple<int, int, int> sorted(int u, int v, int w) {
	if (u > v) swap(u, v); 
	if (u > w) swap(u, w);
	if (v > w) swap(v, w); 
	return {u, v, w};
}


void get_triangles_and_edges() {
	int n; cin >> n;
	vector<set<int>> P(n);
	for (int u = 0; u < n; u++) {
		int v = (u + 1) % n;
		P[u].insert(v); P[v].insert(u);
	}
	for (int i = 0; i < n - 3; i++) {
		int u, v;
		cin >> u >> v; u--; v--;
		P[u].insert(v); P[v].insert(u);
	}
	vector<tuple<int, int, int>> triangles;
	set<tuple<int, int, int>> triangles_set;
	for (int u = 0; u < n; u++) {
		vector<int> sorted_Pu;
		for (int v : P[u]) sorted_Pu.push_back(v);
		sort(sorted_Pu.begin(), sorted_Pu.end());
		for (int i = 0; i < sorted_Pu.size(); i++) {
			int v = sorted_Pu[i], w = sorted_Pu[(i + 1) % sorted_Pu.size()];
			if (P[v].find(w) == P[v].end()) continue;
			triangles_set.insert(sorted(u, v, w));
		}
	}
	for (auto t: triangles_set) triangles.push_back(t);
	G.resize(triangles.size());
	for (int i = 0; i < triangles.size(); i++) {
		auto [u, v, w] = triangles[i];
		if (edges.find({u, v}) == edges.end()) { set<int> empty_set; edges[{u, v}] = empty_set; }
		edges[{u, v}].insert(i);
		if (edges.find({v, w}) == edges.end()) { set<int> empty_set; edges[{v, w}] = empty_set; }
		edges[{v, w}].insert(i);
		if (edges.find({u, w}) == edges.end()) { set<int> empty_set; edges[{u, w}] = empty_set; }
		edges[{u, w}].insert(i);
	}
}

map<tuple<int, int, bool>, int> dfs_cache;
int dfs(int u, int prev, bool edge_to_prev_is_deleted) {
	if (dfs_cache.find({u, prev, edge_to_prev_is_deleted}) != dfs_cache.end())
		return dfs_cache[{u, prev, edge_to_prev_is_deleted}];
	int result = 0;
	if (edge_to_prev_is_deleted) {
		for (int v : G[u]) {
			if (v == prev) continue;
			result += min(dfs(v, u, false), dfs(v, u, true) + 1);
		}
		return dfs_cache[{u, prev, edge_to_prev_is_deleted}] = result;
	}
	auto &vs = G[u];
	int count = 0;
	for (int v : vs) if (v != prev) count++;
	if (count == 0)
		return dfs_cache[{u, prev, edge_to_prev_is_deleted}] = inf;
	if (count == 1) {
		int v = (vs[0] != prev ? vs[0] : vs[1]);
		return dfs_cache[{u, prev, edge_to_prev_is_deleted}] = dfs(v, u, true) + 1;
	}

	vector<int> vs2;
	for (int v : vs) if (v != prev) vs2.push_back(v);
	int v = vs2[0], w = vs2[1];
	return dfs_cache[{u, prev, edge_to_prev_is_deleted}] = min(min(
		dfs(v, u, false) + 1 + dfs(w, u, true),
		dfs(v, u, true) + 1 + dfs(w, u, false)),
		dfs(v, u, true) + 2 + dfs(w, u, true)
	);
}

int main() {
	ios::sync_with_stdio(false); cin.tie(0);
        get_triangles_and_edges();
	for (auto &edge_r : edges) {
		auto &edge = edge_r.second;
		if (edge.size() != 2) continue;
		int u = *edge.begin(), v = *edge.rbegin();
		G[u].push_back(v);
		G[v].push_back(u);
	}
	for (int u = 0; u < G.size(); u++) {
		vector<int> &Gu = G[u];
		if (Gu.size() == 1) {
			cout << dfs(u, -1, false) << endl;
			break;
		}
	}
}
