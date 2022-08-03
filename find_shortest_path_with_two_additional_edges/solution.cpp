#include <bits/stdc++.h>

using namespace std;
typedef vector<set<int>> Graph;
#define pow2(x) ((x)*(x))

set<set<int>> get_components(Graph &G) {
	int n = G.size();
	vector<bool> visited(n);
	set<set<int>> components;
	for (int u = 0; u < n; u++) {
		if (visited[u]) continue;
		set<int> component;
		stack<int> S;
		visited[u] = true;
		S.push(u);
		while (!S.empty()) {
			int u = S.top();
			S.pop();
			component.insert(u);
			for (int v: G[u]) {
				if (visited[v]) continue;
				visited[v] = true;
				S.push(v);
			}
		}
		components.insert(component);
	}
	return components;
}

int64_t get_distance_between_components(const set<int> &c1, const set<int> &c2) {
	if (c1.size() > c2.size())
		return get_distance_between_components(c2, c1);
	int64_t distance = 0x7fffffffffffffff;
	for (int u: c1) {
		auto p = c2.lower_bound(u);
		if (p != c2.end()) distance = min(distance, pow2((int64_t) (*p - u)));
		if (--p != c2.end()) distance = min(distance, pow2((int64_t) (*p - u)));
	}
	return distance;
}

int64_t get_min_path(Graph &G) {
	int n = G.size();
	set<set<int>> components = get_components(G);
	set<int> start_component;
	for (auto &component: components) {
		if (component.find(0) != component.end()) {
			start_component = component;
			break;
		}
	}
	components.erase(start_component);
	if (start_component.find(n - 1) != start_component.end())
		return 0;
	set<int> finish_component;
	for (auto &component: components) {
		if (component.find(n - 1) != component.end()) {
			finish_component = component;
			break;
		}
	}
	components.erase(finish_component);
	int64_t distance = get_distance_between_components(start_component, finish_component);
	for (auto &middle: components)
		distance = min(distance,
		               get_distance_between_components(start_component, middle) +
		               get_distance_between_components(middle, finish_component));
	return distance;
}

int main() {
	int tests;
	cin >> tests;
	while (tests--) {
		int n, m;
		cin >> n >> m;
		Graph G(n);
		for (int i = 0; i < m; i++) {
			int u, v;
			cin >> u >> v;
			u--;
			v--;
			G[u].insert(v);
			G[v].insert(u);
		}
		int64_t min_path = get_min_path(G);
		cout << min_path << endl;
	}
}
