#include <bits/stdc++.h>
using namespace std;

tuple<int, int> get_perfect_containers (map<int, int> categories) {
	int max_container_size = 1;
	for (auto [_, category_size] : categories)
		max_container_size = max(max_container_size, category_size + 1);
	for (int size = max_container_size;; size--) {
		int total_containers = 0;
		bool failed = false;
		for (auto [_, category_size] : categories) {
			int containers = (category_size + size - 1) / size;
			if (containers * size - category_size > containers) {
				failed = true;
				break;
			}
			total_containers += containers;
		}
		if (failed) continue;
		return {total_containers, size};
	}
}

int main() {
	int number_of_tests;
	cout << "number of tests: ";
	cin >> number_of_tests;
	for (; number_of_tests != 0; number_of_tests--) {
		int number_of_elements;
		cin >> number_of_elements;
		map<int, int> categories;
		for (int i = 0; i < number_of_elements; i++) {
			int category;
			cin >> category;
			if (categories.find(category) == categories.end())
				categories.insert({category, 0});
			categories[category] += 1;
		}
		auto [count, size] = get_perfect_containers(categories);
		cout << "number of containers: " << count << endl;
		cout << "size of containers: " << size << endl;
	}
}
