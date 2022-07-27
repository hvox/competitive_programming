#include <bits/stdc++.h>
#pragma GCC optimize("-Ofast")
using namespace std;
using namespace std::chrono;

const bool DEBUG = false;
int n, m;
vector<vector<int>> solutions;
vector<set<int>> G;
set<pair<int, int>> degrees; // sorted by degree set
//            ^-- vertex
//       ^-- degree

void add_edge(int u, int v) {
	int u_degree = G[u].size();
	int v_degree = G[v].size();
	degrees.erase({u_degree, u});
	degrees.insert({u_degree + 1, u});
	degrees.erase({v_degree, v});
	degrees.insert({v_degree + 1, v});
	G[u].insert(v);
	G[v].insert(u);
}

void remove_edge(int u, int v) {
	if (DEBUG) cout << "removing edge " << u << "-" << v << endl;
	int u_degree = G[u].size();
	int v_degree = G[v].size();
	degrees.erase({u_degree, u});
	degrees.insert({u_degree - 1, u});
	degrees.erase({v_degree, v});
	degrees.insert({v_degree - 1, v});
	G[u].erase(v);
	G[v].erase(u);
}

void remove_vertix(int u) {
	if (DEBUG) cout << "removing vertix " << u << endl;
	set<int> neighbords = G[u];
	for (int v : neighbords)
		remove_edge(u, v);
	degrees.erase({0, u});
}

bool remove_leaf() {
	if (DEBUG) cout << "trying to remove a leaf" << endl;
	auto [degree, u] = *degrees.begin();
	if (degree > 1) return false;
	if (DEBUG) cout << "leaf = " << u << endl;
	int v = *G[u].begin();
	int leafs_count = 0;
	for (int w : G[v])
		leafs_count += (G[w].size() == 1) ? 1 : 0;
	if (leafs_count == 1) {
		if (DEBUG) cout << "there are no other leafs. Deleting..." << endl;
		solutions.push_back({u, v});
		remove_vertix(u);
		remove_vertix(v);
		return true;
	}
	if (DEBUG) cout << "there are " << leafs_count - 1 << " other leafs" << endl;
	vector<int> solution = {v};
	for (int u : G[v]) if (G[u].size() == 1) solution.push_back(u);
	for (int u : solution) remove_vertix(u);
	solutions.push_back(solution);
	return true;
}

void do_the_thing() {
	while (degrees.size()) {
		if (DEBUG) cout << " -------------------- " << endl;
		if (remove_leaf()) continue;
		auto [degree, u] = *degrees.begin();
		remove_edge(u, *G[u].begin());
	}
}

int main() {
	cin >> n >> m;
	G.resize(n + 1);
	for (int i = 1; i <= n; i++) {
		degrees.insert({0, i});
	}
	for (int i = 1; i <= m; i++) {
		int u, v;
		cin >> u >> v;
		add_edge(u, v);
	}
	if (n == 1) {
		cout << -1 << endl;
		return 0;
	}
	do_the_thing();

	cout << solutions.size() << endl;
	for (auto solution : solutions) {
		cout << solution.size() << endl;
		for (auto u: solution) cout << u << " ";
		cout << endl;
	}
	return 0;
}
