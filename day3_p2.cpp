#include <stdio.h>
#include <stdlib.h>

#include <vector>
#include <unordered_map>
#include <algorithm>

struct rect {
	int id;
	int x, y;
	int w, h;
};

struct grid {
	std::unordered_map<int, int> m;

	void fill(const rect& r) {
		for (int i = r.x; i < r.x + r.w; i++) {
			for (int j = r.y; j < r.y + r.h; j++) {
				m[(i << 15) | j]++;
			}
		}
	}

	bool test(const rect& r) {
		for (int i = r.x; i < r.x + r.w; i++) {
			for (int j = r.y; j < r.y + r.h; j++) {
				if (m[(i << 15) | j] > 1) {
					return false;
				}
			}
		}
		return true;
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
		rect r;
		sscanf(line, "#%d @ %d,%d: %dx%d", &r.id, &r.x, &r.y, &r.w, &r.h);
		rects.push_back(r);
	}
	fclose(fp);

	// make grid and fill
	grid g;
	for (const auto &r : rects) {
		g.fill(r);
	}

	for (const auto &r : rects) {
		if (g.test(r)) {
			printf("%d\n", r.id);
		}
	}

	return 0;
}
