#include <bits/stdc++.h>
using namespace std;

bool can_fit_without_rotation(int64_t w, int64_t h, int64_t x, int64_t y, int64_t z) {
	if (x + 2 * min(y, z) <= w and 2 * y + 2 * z <= h) return true;
	if (x + z + min(y, z) <= w and x + y + z + y <= h) return true;
	if (x + z + x + y + x <= w and y + z <= h) return true;
	if (x + y + z <= w and x + y + z + x <= h) return true;
	return false;
}

bool can_fit(int64_t w, int64_t h, int64_t x, int64_t y, int64_t z) {
	array<int64_t, 2> cardboard = {w, h};
	array<int64_t, 3> box = {x, y, z};
	sort(cardboard.begin(), cardboard.end());
	do{
		sort(box.begin(), box.end());
		do{
			if (can_fit_without_rotation(cardboard[0], cardboard[1], box[0], box[1], box[2]))
				return true;
		} while(next_permutation(box.begin(), box.end()));
	} while(next_permutation(cardboard.begin(), cardboard.end()));
	return false;
}

int main() {
	int64_t w, h, x, y, z;
	cout << "Size of box: ";
	cin >> x >> y >> z;
	cout << "Size of cardboard: ";
	cin >> w >> h;
	if (can_fit(w, h, x, y, z))
		cout << "Yes. It is possible to cut a " << 
			"box " << x << " × " << y << " × " << z << " out of a " <<
			"cardboard of size " << w << " × " << h << endl;
	else
		cout << "No. It is impossible to cut a " << 
			"box " << x << " × " << y << " × " << z << " out of a " <<
			"cardboard of size " << w << " × " << h << endl;
}
