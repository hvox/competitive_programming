#include <bits/stdc++.h>
using namespace std;

vector<int64_t> precompute_everything(const int64_t max_n, int64_t max_int) {
	vector<int64_t> factorials(max_n + 1);
	factorials[0] = 1;
	for (int n = 1; n <= max_n; n++) {
		factorials[n] = (factorials[n - 1] * n) % max_int;
	}
	vector<int64_t> noprefixes(max_n + 1);
	for (int n = 0; n <= max_n; n++) {
		int64_t sum = 0;
		for (int k = 1; k < n; k++) {
			sum += (factorials[n - k] * noprefixes[k]) % max_int;
			sum %= max_int;
		}
		noprefixes[n] = (factorials[n] - sum + max_int) % max_int;
	}
	vector<int64_t> intervals((max_n + 1) * (max_n + 1));
	int64_t off = max_n + 1;
	for (int n = 0; n <= max_n; n++) {
		intervals[n * off + 0] = 0;
		for (int k = 1; k < n; k++) {
			int64_t sum = 0;
			for (int size = 1; size <= n; size++) {
				int64_t term = intervals[(n - size) * off + k - 1];
				term = (term * factorials[size]) % max_int;
				sum = (sum + term) % max_int;
			}
			intervals[n * off + k] = sum;
		}
		intervals[n * off + n] = 1;
		for (int k = n + 1; k <= max_n; k++) {
			intervals[n * off + k] = 0;
		}
	}
	vector<int64_t> spfree_perms(max_n + 1);
	for (int n = 0; n <= max_n; n++) {
		int64_t sum = 0;
		if (n > 2) {
			for (int k = 1; k < n; k++) {
				sum += (factorials[n - k] * noprefixes[k]) % max_int;
				sum %= max_int;
			}
			sum = (sum * 2) % max_int;
		}
		for (int k = 3; k < n; k++) {
			sum += (spfree_perms[k] * intervals[n * off + k]) % max_int;
			sum %= max_int;
		}
		spfree_perms[n] = (factorials[n] - sum + max_int) % max_int;
	}
	return spfree_perms;
}


int main() {
	cout << "Number of sizes: ";
	int t;
	cin >> t;
	cout << "Max int: ";
	int64_t max_int;
	cin >> max_int;
	vector<int64_t> spfree_perms = precompute_everything(400, max_int);
	while (t--) {
		cout << "Input size of permutation: ";
		int64_t n;
		cin >> n;
		cout << "Number of permutation without subpermutations: ";
		cout << spfree_perms[n] << endl;
	}
}
