#include <bits/stdc++.h>
using namespace std;

int64_t max_int = 127;

map<int64_t, int64_t> factorial_cache;
int64_t factorial(int64_t n) {
	if (n < 2) return 1;
	if (factorial_cache.find(n) != factorial_cache.end()) {
		return factorial_cache[n];
	}
	int64_t result = (factorial(n - 1) * n) % max_int;
	factorial_cache[n] = result;
	return result;
}

map<int64_t, int64_t> nonprefixed_cache;
int64_t nonprefixed(int64_t n) {
	if (nonprefixed_cache.find(n) != nonprefixed_cache.end()) {
		return nonprefixed_cache[n];
	}
	int64_t result = 0;
	for (int k = 1; k < n; k++) {
		result += factorial(n - k) * nonprefixed(k);
		result %= max_int;
	}
	result = (factorial(n) - result) % max_int;
	nonprefixed_cache[n] = result;
	return result;
}

map<tuple<int64_t, int64_t>, int64_t> intervals_cache;
int64_t intervals(int64_t k, int64_t n) {
	if (k > n) return 0;
	if (k == n) return 1;
	if (intervals_cache.find({k, n}) != intervals_cache.end()) {
		return intervals_cache[{k, n}];
	}
	int64_t result = 0;
	for (int size = 1; size <= n; size++) {
		result += intervals(k - 1, n - size) * factorial(size);
		result %= max_int;
	}
	intervals_cache[{k, n}] = result;
	return result;
}

map<int64_t, int64_t> nonsp_perms_cache;
int64_t nonsp_perms(int64_t n) {
	if (nonsp_perms_cache.find(n) != nonsp_perms_cache.end()) {
		return nonsp_perms_cache[n];
	}
	int64_t result = 0;
	if (n > 2) {
		for (int k = 1; k < n; k++) {
			result += factorial(n - k) * nonprefixed(k);
			result %= max_int;
		}
		result *= 2;
		result %= max_int;
	}
	for (int k = 3; k < n; k++) {
		result += intervals(k, n) * nonsp_perms(k);
		result %= max_int;
	}
	result = (factorial(n) - result) % max_int;
	nonsp_perms_cache[n] = result;
	return result;
}

int main() {
	cout << "Number of sizes: ";
	int t;
	cin >> t;
	cout << "Max int: ";
	cin >> max_int;
	while (t--) {
		cout << "Input size of permutation: ";
		int64_t n;
		cin >> n;
		cout << "Number of permutation without subpermutations: ";
		cout << nonsp_perms(n) << endl;
	}
}
