// This is solution of another task from NERC QUEST 2022.
// But by unknown reason the solver raises exception:
// > Expected header line for testcase [5] -- []

#include <bits/stdc++.h>
using namespace std;

typedef tuple<int, int> Point;

int dist(int x0, int y0, int x1, int y1) {
	double dx = x1 - x0;
	double dy = y1 - y0;
	double distance = sqrt(dx*dx + dy*dy);
	return ceil(distance / 100.0) * 2 + 10;
}

tuple<int, int> get_center_of_square(int current_k, int k) {
	int x = ((current_k % k) - k / 2) * (10000 / k) + (10000 / k / 2);
	int y = ((current_k / k) - k / 2) * (10000 / k) + (10000 / k / 2);
	return {x, y};
}

vector<int> solve2(int k, int m, vector<tuple<Point, vector<int>>> &regions) {
	int n = regions.size();
	vector<int> result(m);
	for (int i = 0; i < m; i++) result[i] = rand() % (k * k);
	vector<set<int>> bases(k*k);
	for (int i = 0; i < n; i++)
		for (int reg : get<1>(regions[i]))
			bases[reg].insert(i);
	int regions_count = regions.size();
	while (m) {
		int best_square = -1;
		int best_square_regions_count = 0;
		for (int i = 0; i < k*k; i++)
			if (bases[i].size() > best_square_regions_count) {
				best_square_regions_count = bases[i].size();
				best_square = i;
			}
		if (best_square == -1) break;
		auto targets = bases[best_square];
		set<int> processed_targets;
		auto [x0, y0] = get_center_of_square(best_square, k);
		vector<tuple<int, int>> targets_sorted(targets.size());
		for (int target : targets) {
			auto [x1, y1] = get<0>(regions[target]);
			targets_sorted.push_back({dist(x0, y0, x1, y1), target});
		}
		sort(targets_sorted.begin(), targets_sorted.end());
		int spended_time = 0;
		for (auto [time, target] : targets_sorted) {
			if (spended_time + time > 720) break;
			spended_time += time;
			processed_targets.insert(target);
		}

		m--;
		result[m] = best_square;

		for (int deleted_target : processed_targets) {
			for (int square : get<1>(regions[deleted_target])) {
				bases[square].erase(deleted_target);
			}
		}
	}
	return result;
}

vector<int> solve(int k, int m, vector<tuple<Point, vector<int>>> &regions) {
	int n = regions.size();
	vector<int> result(m);
	for (int i = 0; i < m; i++) result[i] = rand() % (k * k);
	if (regions.size() > 500000) return result;
	vector<set<int>> bases(k*k);
	for (int i = 0; i < n; i++)
		for (int reg : get<1>(regions[i]))
			bases[reg].insert(i);
	int regions_count = regions.size();
	while (m) {
		int best_square = -1;
		int best_square_regions_count = 0;
		for (int i = 0; i < k*k; i++)
			if (bases[i].size() > best_square_regions_count) {
				best_square_regions_count = bases[i].size();
				best_square = i;
			}
		if (best_square == -1) break;
		auto targets = bases[best_square];
		set<int> processed_targets;
		auto [x0, y0] = get_center_of_square(best_square, k);
		vector<tuple<int, int>> targets_sorted(targets.size());
		for (int target : targets) {
			auto [x1, y1] = get<0>(regions[target]);
			targets_sorted.push_back({dist(x0, y0, x1, y1), target});
		}
		sort(targets_sorted.begin(), targets_sorted.end());
		int spended_time = 0;
		for (auto [time, target] : targets_sorted) {
			if (spended_time + time > 720) break;
			spended_time += time;
			processed_targets.insert(target);
		}

		m--;
		result[m] = best_square;

		for (int deleted_target : processed_targets) {
			for (int square : get<1>(regions[deleted_target])) {
				bases[square].erase(deleted_target);
			}
		}
	}
	return result;
}

int main() {
	string none;
	for (int i = 0; i < 10; i++) {
		cin >> none;
		cin >> none;
		cin >> none;
		int k, m, n;
		cin >> k >> m >> n;
		vector<tuple<Point, vector<int>>> regions(n);
		for (int i = 0; i < n; i++) {
			int x, y;
			cin >> x >> y;
			int t;
			cin >> t;
			vector<int> regs;
			while (t--) {
				int region;
				cin >> region;
				regs.push_back(region - 1);
			}
			regions[i] = {{x, y}, regs};
		}
		vector<int> regs = solve(k, m, regions);
		cout << "==========" << endl;
		for (int i = 0; i < m; i++) {
			cout << regs[i] + 1 << " ";
		}
		cout << endl;
	}
	return 0;
}
