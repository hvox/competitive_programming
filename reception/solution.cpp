#include <bits/stdc++.h>
using namespace std;

typedef tuple<int, int, int> elemT;

int get_tree_size(int n) {
	int result = 2;
	while (n > 1) {
		result *= 2;
		n = (n + 1) / 2;
	}
	return result;
}

elemT f(elemT u, elemT v) {
	auto [start1, end1, time1] = u;
	auto [start2, end2, time2] = v;
	return {start1, max(end2, end1 + time2), time1 + time2};
}

void set_value(vector<elemT> &tree, int i, elemT value) {
	tree[i] = value;
	i /= 2;
	if (i) set_value(tree, i, f(tree[2 * i], tree[2 * i + 1]));
}

elemT accumulate(vector<elemT> &tree, int L, int R) {
	if (L == R) return tree[L];
	if (L % 2 != 0) return f(tree[L], accumulate(tree, L + 1, R));
	if (R % 2) return f(accumulate(tree, L, R - 1), tree[R]);
	return accumulate(tree, L / 2, R / 2);
}


void build_reception(vector<tuple<int, int>> queries) {
	vector<int> starts;
	map<int, int> t2i;
	for (auto [start, time] : queries) {
		if (time == -1) continue;
		if (t2i.find(start) != t2i.end()) continue;
		t2i[start] = starts.size();
		starts.push_back(start);
	}
	sort(starts.begin(), starts.end());
	for (int i = 0; i < starts.size(); i++) {
		t2i[starts[i]] = i;
	}
	int tree_size = get_tree_size(starts.size());
	int offset = tree_size / 2;
	vector<elemT> tree(tree_size);
	for (int i = 0; i < starts.size(); i++) {
		set_value(tree, offset + i, {starts[i], starts[i], 0});
	}
	for (auto [τ, δ] : queries) {
		if (δ == -1) {
			int i = 0;
			int j = starts.size() - 1;
			while (i != j) {
				int k = (i + j + 1) / 2;
				if (starts[k] <= τ) {
					i = k;
				} else {
					j = k - 1;
				}
			}
			int result = 0;
			if (τ < starts[i]) {
				result = 0;
			} else {
				auto [start, end, _] = accumulate(tree, offset, offset + i);
				result = max(end - τ, 0);
			}
			cout << "wait time: " << result << endl;
			continue;
		}
		int i = t2i[τ];
		set_value(tree, offset + i, {τ, τ + δ, δ});
	}
}

int main() {
	cout << "Number of queries: ";
	int q;
	cin >> q;
	cout << "Format:" << endl;
	cout << " + <start> <delta_time> -- add guy to the queue" << endl;
	cout << " - <index_of_event>     -- delete guy from the queue" << endl;
	cout << " ? <time>               -- request waited time" << endl;
	vector<tuple<int, int>> queries(q);
	for (int i = 0; i < q; i++) {
		cout << "querry #" << i + 1 << endl;
		char command;
		cin >> command;
		if (command == '+') {
			int τ, δ;
			cin >> τ >> δ;
			queries[i] = {τ, δ};
		}
		else {
			if (command == '-') {
				int j;
				cin >> j;
				queries[i] = {get<0>(queries[j - 1]), 0};
			}
			else {
				int τ;
				cin >> τ;
				queries[i] = {τ, -1};
			}
		}
	}
	build_reception(queries);
}
