#include <bits/stdc++.h>
#pragma GCC optimize("-Ofast")
using namespace std;
using namespace std::chrono;

char arr[3001][3001];
int n, m;

void optimize() {
	int R = m;
	for (int i = 0; i < n; i++) {
		int length = 0;
		char last_char = 0;
		for (int j = 0; j < m; j++) {
			if (last_char == arr[i][j]) {
				length++;
			} else {
				R = gcd(length, R);
				length = 1;
				last_char = arr[i][j];
			}
		}
	}
	if (R == 1) return;
	m /= R;
	for (int i = 0; i < n; i++) for (int j = 0; j < m; j++)
		arr[i][j] = arr[i][j * R];
	return;
}

void transpose() {
	int t = n;
	n = m;
	m = t;
	for (int i = 0; i < n; i++) for (int j = 0; j < m; j++) {
		char t = arr[i][j];
		arr[i][j] = arr[j][i];
		arr[j][i] = t;
	}
	int k = n > m ? m : n;
	for (int i = 0; i < k; i++) for (int j = i + 1; j < k; j++) {
		char t = arr[i][j];
		arr[i][j] = arr[j][i];
		arr[j][i] = t;
	}
}

int main() {
	cin >> n >> m;
	for (int i = 0; i < n; i++) {
		string s;
		cin >> s;
		for (int j = 0; j < m; j++)
			arr[i][j] = s[j];
	}
	optimize();
	transpose();
	optimize();
	transpose();
	cout << n << " " << m << endl; for (int i = 0; i < n; i++) { for (int j = 0; j < m; j++) cout << arr[i][j]; cout << endl; }
	return 0;
}
