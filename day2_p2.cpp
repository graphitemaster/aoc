#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <vector>
#include <string>

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

	FILE *fp = fopen(*argv, "rb");
	if (!fp) {
		fprintf(stderr, "failed to open input %s", *argv);
		return 1;
	}

	// read lines
	std::vector<std::string> lines;
	char line[1024];
	while (fgets(line, sizeof line, fp)) {
		if (size_t nl = strcspn(line, "\r\n")) {
			line[nl] = '\0';
		}
		lines.emplace_back(line);
	}
	fclose(fp);

	for (const auto &x : lines) {
		const size_t length = x.size();
		for (const auto &y : lines) {
			if (x == y) {
				continue;
			}

			std::string build;
			for (size_t i = 0; i < length; i++) {
				const int cx = x[i];
				const int cy = y[i];
				if (cx == cy) {
					build += cx;
				}
			}

			if (build.empty()) {
				continue;
			}

			if (build.size() == length - 1) {
				printf("%s\n", build.c_str());
				return 0;
			}
		}
	}

	return 0;
}
