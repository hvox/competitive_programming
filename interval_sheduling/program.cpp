#include <bits/stdc++.h>
using namespace std;

typedef tuple<int, int, int> workplace;
const int inf = 1e8;

int get_maximum_number_of_jobs_done(vector<workplace> places) {
	// future work time intervals (start, dt, end)
	priority_queue<workplace, vector<workplace>, greater<workplace>> future;
	future.push({inf * 3, 1, inf * 3});
	for (auto [start, end, dt] : places)
		future.push({start, dt, end});
	// current work time intrevals (dt, start, end)
	priority_queue<workplace, vector<workplace>, greater<workplace>> current;
	current.push({inf * 4, -inf * 2, inf * 2});
	int time = -inf;
	int jobs_done = 0;
	while (true) {
		auto [start2, dt2, end2] = future.top();
		while (start2 <= time) {
			future.pop();
			current.push({dt2, start2, end2});
			auto [start, dt, end] = future.top();
			start2 = start;
			end2 = end;
			dt2 = dt;
		}
		auto [dt1, start1, end1] = current.top();
		while (end1 - dt1 < time) {
			if (current.size() == 1) break;
			current.pop();
			auto [dt, start, end] = current.top();
			start1 = start;
			end1 = end;
			dt1 = dt;
		}
		int next_event = min((int)end1, (int)start2);
		if (time + dt1 <= next_event) {
			int delta_jobs = (next_event - time) / dt1;
			jobs_done += delta_jobs;
			time += delta_jobs * dt1;
			continue;
		}
		int best_end = time + dt1;
		while (true) {
			auto [start, dt, end] = future.top();
			if (start >= best_end or future.size() == 1) break;
			best_end = min(best_end, start + dt);
			future.pop();
			current.push({dt, start, end});
		}
		if (best_end >= inf) break;
		time = best_end;
		jobs_done += 1;
	}
	return jobs_done;
}

int main() {
	int n;
	while (cin >> n) {
		vector<workplace> places(n);
		for (int i = 0; i < n; i++) {
			int start, end, job_time;
			cin >> start >> end >> job_time;
			places[i] = {start, end, job_time};
		}
		cout << get_maximum_number_of_jobs_done(places) << endl;
	}
}
