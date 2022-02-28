#include <bits/stdc++.h>
using namespace std;

vector<tuple<int, int>> restore_tree (int l, int r, vector<int> left, vector<int> right) {
	vector<tuple<int, int>> none_tree;
	int expected_left_code = r;
	int expected_right_code = l;
	int n = l + r;
	if(left.size() > --expected_left_code)
		return none_tree;
	while(left.size() < expected_left_code)
		left.push_back(l);
	if(right.size() > --expected_right_code)
		return none_tree;
	while(right.size() < expected_right_code)
		right.push_back(n);
	multiset<int> coded;
	for (auto vtx : left) coded.insert(vtx);
	for (auto vtx : right) coded.insert(vtx);
	set<int> free;
	for (int vtx = 1; vtx <= n; vtx++)
		if (coded.find(vtx) == coded.end())
			free.insert(vtx);
	int l0 = 0, r0 = 0;
	vector<tuple<int, int>> tree;
	for(int t = 2; t < n; t++) {
		if(free.size() == 0) return none_tree;
		int leaf = (*free.begin());
		free.erase(leaf);
		int node = (leaf <= l) ? right[r0++] : left[l0++];
		coded.erase(coded.find(node));
		if(coded.find(node) == coded.end())
			free.insert(node);
		tree.push_back({node, leaf});
	}
	tree.push_back({*free.begin(), *free.rbegin()});
	return tree;
}

int main() {
	int size_of_left_part;
	cout << "size of left side: ";
	cin >> size_of_left_part;
	int size_of_right_part;
	cout << "size of right side: ";
	cin >> size_of_right_part;
	int left_prufer;
	cout << "left vertices in prufer code: ";
	cin >> left_prufer;
	int right_prufer;
	cout << "right vertices in prufer code: ";
	cin >> right_prufer;
	cout << "code for left: ";
	vector<int> left(left_prufer);
	for(int i = 0; i < left_prufer; i++)
		cin >> left[i];
	cout << "code for right: ";
	vector<int> right(right_prufer);
	for(int i = 0; i < right_prufer; i++)
		cin >> right[i];
	auto tree = restore_tree(size_of_left_part, size_of_right_part, left, right);
	if(tree.size() == 0)
		cout << "Impossible to build a tree" << endl;
	else {
		cout << "The tree have been successfully built!" << endl;
		for (auto [u, v] : tree)
			cout << "edge: " << u << " " << v << endl;
	}
}
