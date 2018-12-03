#include <stdio.h>
#include <stdlib.h>

#include <vector>
#include <unordered_map>
#include <algorithm>

struct rect {
	int x, y;
	int w, h;
};

struct grid {
	std::unordered_map<int, int> m;

	void fill(const rect &r) {
		for (int i = r.x; i < r.x + r.w; i++) {
			for (int j = r.y; j < r.y + r.h; j++) {
				m[(i << 15) | j]++;
			}
		}
	}

	int overlapping_area() const {
		return std::count_if(m.begin(),
		                     m.end(),
		                     [](auto const &c) { return c.second > 1; });
	}
};

static int usage(const char *app) {
	printf("%s [input file]\n", app);
	return 1;
}

int main(int argc, char **argv) {
	argc--;
	argv++;

	if (!argc) {
		return usage(argv[-1]);
	}

	// read rects
	FILE *fp = fopen(*argv, "rb");
	if (!fp) {
		fprintf(stderr, "failed to open input %s", *argv);
		return 1;
	}

	std::vector<rect> rects;
	char line[1024];
	while (fgets(line, sizeof line, fp)) {
		int id;
		rect r;
		sscanf(line, "#%d @ %d,%d: %dx%d", &id, &r.x, &r.y, &r.w, &r.h);
		rects.push_back(r);
	}
	fclose(fp);

	// make grid and fill
	grid g;
	for (const auto &r : rects) {
		g.fill(r);
	}

	printf("%d\n", g.overlapping_area());

	return 0;
}
