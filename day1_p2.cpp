#include <stdio.h>
#include <stdlib.h>

#include <vector>
#include <unordered_set>

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

	// read lines into vector of frequency changes
	std::vector<long> changes;
	char line[1024];
	while (fgets(line, sizeof line, fp)) {
		char *end = nullptr;
		const long change = strtol(line, &end, 10);
		changes.emplace_back(change);
	}
	fclose(fp);

	// find the result
	std::unordered_set<long> seen;
	long frequency = 0;
	for (;;) {
		for (const auto change : changes) {
			const long result = frequency + change;
			const auto find = seen.find(result);
			if (find != seen.end()) {
				printf("%d\n", result);
				return 0;
			}
			seen.emplace(result);
			frequency = result;
		}
	}

	return 0;
}
