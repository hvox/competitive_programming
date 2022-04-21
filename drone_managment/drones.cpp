/* ----- NERC QUEST 2022-04-12 -----
 * 45 teams participated.
 * My solution won!
 */

#include <bits/stdc++.h>
using namespace std;

const bool DEBUG = true;
const int PRECICION = 100000; // more is better, but slower

typedef tuple<int, int> Point;
typedef vector<Point> Path;
typedef vector<Path> Paths;

int dist(tuple<int, int> p0, tuple<int, int> p1) {
	auto [x0, y0] = p0;
	auto [x1, y1] = p1;
	double dx = x0 - x1;
	double dy = y0 - y1;
	return ceil(sqrt(dx * dx + dy * dy) / 100);
}

Paths solve_rand(int m, vector<tuple<int, int>> &poss) {
	int n = poss.size();
	Paths paths(m);
	for (int i = 0; i < m; i++) {
		vector<tuple<int, int>> path;
		paths[i] = path;
	}
	set<tuple<int, tuple<int, int>>> poses;
	for (int i = 0; i < n; i++)
		poses.insert({rand(), poss[i]});
	for (int i = 0; i < m; i++) {
		int current_length = 0;
		tuple<int, int> cp = {0, 0};
		vector<tuple<int, int>> path;
		while (poses.size()) {
			tuple<int, int> best_np = {0, 0};
			int best_np_i = 0;
			int best_dist = 0x7fffffff;
			int counter = PRECICION;
			for (auto [id, p] : poses) {
				if (counter-- <= 0) break;
				int d0 = dist(p, cp);
				int d1 = dist(p, {0, 0});
				if (current_length + d0 + 30 + d1 > 720) continue;
				//if (d0 + d1 < best_dist)
				if (d0 < best_dist) {
					best_np = p;
					best_np_i = id;
					best_dist = d0;
				}
			}
			if (best_dist == 0x7fffffff) break;
			poses.erase({best_np_i, best_np});
			current_length += 30 + best_dist;
			path.push_back(best_np);
			cp = best_np;
		}
		paths[i] = path;
	}
	return paths;
}


Paths solve(int m, vector<tuple<int, int>> &poss) {
	int n = poss.size();
	Paths paths(m);
	for (int i = 0; i < m; i++) {
		vector<tuple<int, int>> path(0);
		paths[i] = path;
	}
	if (n > PRECICION) return solve_rand(m, poss);
	set<tuple<int, int>> poses;
	for (int i = 0; i < n; i++) {
		poses.insert(poss[i]);
	}
	for (int i = 0; i < m; i++) {
		int current_length = 0;
		tuple<int, int> cp = {0, 0};
		vector<tuple<int, int>> path;
		while (poses.size()) {
			tuple<int, int> best_np = {0, 0};
			int best_dist = 0x7fffffff;
			for (auto p : poses) {
				int d0 = dist(p, cp);
				int d1 = dist(p, {0, 0});
				if (current_length + d0 + 30 + d1 > 720) continue;
				//if (d0 + d1 < best_dist)
				if (d0 < best_dist) {
					best_np = p;
					best_dist = d0;
				}
			}
			if (best_dist == 0x7fffffff) break;
			poses.erase(best_np);
			current_length += 30 + best_dist;
			path.push_back(best_np);
			cp = best_np;
		}
		paths[i] = path;
	}
	return paths;
}

int main() {
	string none, test_name;
	double sum = 0;
	for (int i = 0; i < 10; i++) {
		cin >> none;
		cin >> test_name;
		cin >> none;
		int n, m;
		cin >> m >> n;
		vector<tuple<int, int>> poss(n);
		map<Point, int> indexes;
		for (int i = 0; i < n; i++) {
			int x, y;
			cin >> x >> y;
			poss[i] = {x, y};
			indexes.insert({poss[i], i});
		}
		cout << "===== " << test_name << " =====" << endl;
		Paths paths = solve(m, poss);
		if (paths.size() != m) return 42;
		double points = 0;
		for (Path &path : paths) {
			points += ((double)path.size()) / n * 10;
			if (not DEBUG) {
				cout << path.size();
				for (auto point : path) {
					cout << " " << indexes[point] + 1;
				}
				cout << endl;
			}
		}
		sum += points;
		if (DEBUG) cout << "total points: " << sum << "; δ: " << points << endl;
	}
	return 0;
}
