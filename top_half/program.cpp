#include <bits/stdc++.h>
using namespace std;

vector<int> get_top_half(int n) {
	vector<int> A(n);
	function<bool(int, int)> order = [&](int i, int j){
		cout << "? " << A[i] << " " << A[j] << endl;
		string answer;
		cin >> answer;
		if (answer != "<") {
			swap(A[i], A[j]);
			return true;
		}
		return false;
	};
	for(int i = 0; i < n; i++)
		A[i] = i + 1;

	if(n == 6) {
		for (int i = 0; i < 6; i+=2)
			order(i, i+1);
		if (order(0, 2)) swap(A[1], A[3]);
		if (order(0, 4)) swap(A[1], A[5]);
		if (order(2, 4)) swap(A[3], A[5]);
		order(1, 3);
		order(1, 4);
		vector<int> top_half(3);
		for (int i = 0; i < 3; i++)
			top_half[i] = A[i + 3];
		return top_half;
	}

	vector<int> left(n / 2);
	vector<int> right(n / 2);
	for (int i = 0; i < n / 2; i++)
		left[i] = A[i];
	for (int i = 0; i < n / 2; i++)
		right[i] = A[n / 2 + i];

	auto order_elements = [&](vector<int> &A, int i, int j){
		cout << "? " << A[i] << " " << A[j] << endl;
		string answer;
		cin >> answer;
		if (answer != "<") {
			swap(A[i], A[j]);
			return true;
		}
		return false;
	};

	auto ask = [&](int i, int j){
		cout << "? " << i << " " << j << endl;
		string answer;
		cin >> answer;
		return (answer == "<");
	};

	auto get_minimum = [&](vector<int> &A) -> int {
		for(int i = 0; i < A.size() / 2; i++)
			order_elements(A, i + (A.size() + 1) / 2, i);
		for(int i = A.size() / 2; i < A.size() - 1; i++)
			order_elements(A, A.size() - 1, i);
		return A[A.size() - 1];
	};

	auto interactive_sort = [&](vector<int> &A) {
		for(int i = 0; i < A.size(); i++)
			for(int j = i + 1; j < A.size(); j++)
				order_elements(A, j, i);
	};

	bool left_is_sorted = false;
	bool right_is_sorted = false;
	get_minimum(left);
	get_minimum(right);

	for (int t = 0; t < n / 2; t++) {
		int l_top = left[left.size() - 1];
		int r_top = right[right.size() - 1];
		if (ask(l_top, r_top)) {
			left.pop_back();
			if (not left_is_sorted) {
				interactive_sort(left);
				left_is_sorted = true;
			}
		} else {
			right.pop_back();
			if (not right_is_sorted) {
				interactive_sort(right);
				right_is_sorted = true;
			}
		}
	}

	vector<int> top;
	for (int a : left) top.push_back(a);
	for (int a : right) top.push_back(a);
	return top;
}

int main() {
	cout << "Number of comparable things: ";
	int n;
	cin >> n;
	vector<int> top = get_top_half(n);
	for (int el : top)
		cout << el << " ";
	cout << endl;
}
