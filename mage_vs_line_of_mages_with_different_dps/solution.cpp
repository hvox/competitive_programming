#include <bits/stdc++.h>
using namespace std;
#pragma GCC optimize("O3,unroll-loops")
#define infinity 0x7fffffffffffffff
int n, m;

char scales[4] = {1, 3, 9, 27};
int64_t dp[300001][81];
int main() {
	ios::sync_with_stdio(false); cin.tie(0);
	cin >> n >> m;
	int64_t powers[n];
	for (int i = 0; i < n; i++)
		cin >> powers[i];
	int l_limit[m], r_limit[m];
	for (int i = 0; i < m; i++) {
		cin >> l_limit[i] >> r_limit[i];
		l_limit[i]--;
		r_limit[i]--;
	}
	int mask_count = pow(3, m);
	for (int i = 0; i <= n; i++)
		for (int j = 0; j < mask_count; j++)
			dp[i][j] = infinity;
	dp[0][0] = 0;
	for (int i = 0; i < n; i++) {
		for (char mask = 0; mask < mask_count; mask++) {
			if (dp[i][mask] == infinity) continue;
			int active_rounds = m;
			bool can_save_state = true;
			char msk = mask;
			for (int round = 0; round < m; round++, msk/=3) {
				switch (msk % 3) {
				case 0:
					if (l_limit[round] <= i && i <= r_limit[round]) {
						dp[i][mask + scales[round]] = min(dp[i][mask + scales[round]], dp[i][mask]);
					}
					break;
				case 1:
					can_save_state &= l_limit[round] <= i && i <= r_limit[round];
					active_rounds = min(active_rounds, round);
					dp[i][mask + scales[round]] = min(dp[i][mask + scales[round]], dp[i][mask]);
					break;
				}
			}
			if (can_save_state)
				dp[i + 1][mask] = min(dp[i + 1][mask], dp[i][mask] + active_rounds * powers[i]);
		}
	}
	int64_t best_solution = infinity;
	for (int mask = 0; mask < mask_count; mask++)
		best_solution = min(best_solution, dp[n][mask]);
	cout << best_solution << endl;
}
