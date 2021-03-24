#include <iostream>
#include <cassert>
#include <cmath>
#include <chrono>
#include <algorithm>
#include <numeric>
#include <vector>

using namespace std;

static uint32_t _x = 2463534242;
static void xorshift32_seed(uint32_t seed) { if (seed) _x = seed; }
static inline uint32_t xorshift32(void) { _x ^= (_x << 13); _x ^= (_x >> 17); _x ^= (_x << 15); return _x; }
static inline uint32_t _rand() { return xorshift32(); }

auto start_time = chrono::high_resolution_clock::now();
int get_elapsed_ms() { return chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - start_time).count(); }

static const int SIZE = 10000;

struct Point {
	int x;
	int y;
};

struct Rectangle {
	Point a;		// top left
	Point b;		// bottom right
	int width() { return b.x - a.x; }
	int height() { return b.y - a.y; }
	int getRect() { return (b.x - a.x) * (b.y - a.y); }
	bool is_cross(const Rectangle &r) {
		if (r.b.x <= a.x || b.x <= r.a.x) return false;
		if (r.b.y <= a.y || b.y <= r.a.y) return false;
		return true;
	}
};

class Solver {
	int N;
	int prev_reported;
	int average_distance;
	int reset_trial_count;
	int reset_trial_success_count;
	vector<Point> p;
	vector<int> r;
	vector<Rectangle> ad;
	vector<vector<int>> neighbor;
	vector<vector<int>> grow_speed_x;
	vector<vector<int>> grow_speed_y;

	void input() {
		cin >> N;
		p.resize(N);
		r.resize(N);
		for (int i = 0; i < N; ++i) {
			cin >> p[i].x >> p[i].y >> r[i];
		}
	}

	void reset() {
		average_distance = int(SIZE / sqrt(N)) / 2;
		ad.resize(N);
		for (int i = 0; i < N; ++i) {
			reset_rectangle(i);
		}
		neighbor = vector<vector<int>>(N);
		grow_speed_x = vector<vector<int>>(N);
		grow_speed_y = vector<vector<int>>(N);
		reset_trial_count = 0;
		reset_trial_success_count = 0;
	}

	void reset_rectangle(int i) {
		ad[i].a = p[i];
		ad[i].b = { p[i].x + 1, p[i].y + 1 };
	}

	void output_points(FILE *fp) {
		for (auto r : ad) {
			fprintf(fp, "%d %d %d %d\n", r.a.x, r.a.y, r.b.x, r.b.y);
		}
	}

	double calculate_total_score() {
		double score = 0;
		for (int i = 0; i < N; ++i) {
			score += calculate_rectangle_score(i);
		}
		return score / N;
	}

	double calculate_rectangle_score(int i) {
		int s = ad[i].getRect();
		double d = 1.0 - (double)min(s, r[i]) / max(s, r[i]);
		return 1.0 - d * d;
	}

	void report_if_needed() {
		int elapsed_ms = get_elapsed_ms();
		if (elapsed_ms - prev_reported >= 250) {
			cerr << elapsed_ms << ", SCORE: " << calculate_total_score() << ", TRIAL: " << reset_trial_success_count << "/" << reset_trial_count << endl;
			prev_reported = elapsed_ms;
			string sequence = to_string(prev_reported);
			string filename = string(max(0, 4 - (int)sequence.length()), '0') + sequence + ".txt";
			FILE *fp = fopen(filename.c_str(), "w");
			if (fp) {
				output_points(fp);
				fclose(fp);
			}
		}
	}

	bool is_overlapped(int i) {
		for (int j = 0; j < N; ++j) {
			if (i != j && ad[i].is_cross(ad[j])) {
				return true;
			}
		}
		return false;
	}

	void calculate_neighbors(int i, int margin) {
		neighbor[i].clear();
		Rectangle e = ad[i];
		e.a.x -= margin, e.a.y -= margin, e.b.x += margin, e.b.y += margin;
		for (int j = 0; j < N; ++j) {
			if (i != j && e.is_cross(ad[j])) neighbor[i].emplace_back(j);
		}
	}

	void calculate_all_neighbors(int margin) {
		for (int i = 0; i < N; ++i) calculate_neighbors(i, margin);
	}

	void set_grow_speed(int i) {
		const int RANGE = average_distance / 4;
		grow_speed_x[i] = { 1 + int(_rand() % RANGE), 1 + int(_rand() % RANGE) };
		grow_speed_x[i][_rand() % 2] *= -1;
		grow_speed_y[i] = { 1 + int(_rand() % RANGE), 1 + int(_rand() % RANGE) };
		grow_speed_y[i][_rand() % 2] *= -1;
	}

	int get_grow_speed_x(int i) {
		if (!grow_speed_x[i].empty()) {
			return grow_speed_x[i].back();
		}
		return 0;
	}

	int get_grow_speed_y(int i) {
		if (!grow_speed_y[i].empty()) {
			return grow_speed_y[i].back();
		}
		return 0;
	}

	void simple_grow_rectangle(int i, int unit) {
		int rx = abs(r[i] - ad[i].getRect()) / ad[i].height();
		if (rx > 0) {
			int next_x = max(0, ad[i].a.x - min(rx, unit));
			swap(next_x, ad[i].a.x);
			if (is_overlapped(i)) {
				swap(next_x, ad[i].a.x);
			} else {
				rx = (r[i] - ad[i].getRect()) / ad[i].height();
			}
		}
		if (rx > 0) {
			int next_x = min(SIZE, ad[i].b.x + min(rx, unit));
			swap(next_x, ad[i].b.x);
			if (is_overlapped(i)) {
				swap(next_x, ad[i].b.x);
			}
		}
		int ry = abs(r[i] - ad[i].getRect()) / ad[i].width();
		if (ry > 0) {
			int next_y = max(0, ad[i].a.y - min(ry, unit));
			swap(next_y, ad[i].a.y);
			if (is_overlapped(i)) {
				swap(next_y, ad[i].a.y);
			} else {
				ry = (r[i] - ad[i].getRect()) / ad[i].width();
			}
		}
		if (ry > 0) {
			int next_y = min(SIZE, ad[i].b.y + min(ry, unit));
			swap(next_y, ad[i].b.y);
			if (is_overlapped(i)) {
				swap(next_y, ad[i].b.y);
			}
		}
	}

	void grow_rectangle_by_speed(int i) {
		int dx = get_grow_speed_x(i);
		int dy = get_grow_speed_y(i);
		if (dx == 0 && dy == 0) return;
		int rx = abs(r[i] - ad[i].getRect()) / ad[i].height();
		if (dx < 0) {
			int next_x = max(0, ad[i].a.x - min(rx, -dx));
			swap(next_x, ad[i].a.x);
			if (is_overlapped(i)) {
				swap(next_x, ad[i].a.x);
				grow_speed_x[i].pop_back();
			}
		} else if (dx > 0) {
			int next_x = min(SIZE, ad[i].b.x + min(rx, dx));
			swap(next_x, ad[i].b.x);
			if (is_overlapped(i)) {
				swap(next_x, ad[i].b.x);
				grow_speed_x[i].pop_back();
			}
		}
		int ry = abs(r[i] - ad[i].getRect()) / ad[i].width();
		if (dy < 0) {
			int next_y = max(0, ad[i].a.y - min(ry, -dy));
			swap(next_y, ad[i].a.y);
			if (is_overlapped(i)) {
				swap(next_y, ad[i].a.y);
				grow_speed_y[i].pop_back();
			}
		} else if (dy > 0) {
			int next_y = min(SIZE, ad[i].b.y + min(ry, dy));
			swap(next_y, ad[i].b.y);
			if (is_overlapped(i)) {
				swap(next_y, ad[i].b.y);
				grow_speed_y[i].pop_back();
			}
		}
	}

	void try_reset_and_grow() {
		vector<Rectangle> previous_ad = ad;
		double previous_score = calculate_total_score();

		int i = _rand() % N;
		calculate_neighbors(i, 100);
		if (neighbor[i].empty()) return;
		int j =  neighbor[i][_rand() % neighbor[i].size()];

		reset_rectangle(i);
		reset_rectangle(j);
		set_grow_speed(i);
		set_grow_speed(j);
		for (int t = 0; t < 100; ++t) {
			grow_rectangle_by_speed(i);
			grow_rectangle_by_speed(j);
		}
		for (int t = 0; t < 100; ++t) {
			simple_grow_rectangle(i, 2);
			simple_grow_rectangle(j, 2);
		}

		reset_trial_count += 1;
		if (calculate_total_score() > previous_score) {
			reset_trial_success_count += 1;
		} else {
			ad = previous_ad;
		}
	}

	void iterate() {
		for (int t = 0; t < 100; ++t) {
			for (int i = 0; i < N; ++i) {
				simple_grow_rectangle(i, 5);
			}
		}
		try_reset_and_grow();
	}

public:
	Solver() : prev_reported(0) { }

	void init() {
		input();
		reset();
	}

	void run() {
		while (get_elapsed_ms() <= (5000 - 250)) {
			report_if_needed();
			iterate();
		}
		output_points(stdout);
	}
};

int main(int argc, char* argv[]) {
	cout.precision(10);
	Solver solver;
	solver.init();
	solver.run();
}
