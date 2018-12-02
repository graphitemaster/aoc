#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

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

	size_t m2s = 0;
	size_t m3s = 0;
	FILE *fp = fopen(*argv, "rb");
	if (!fp) {
		fprintf(stderr, "failed to open input %s", *argv);
		return 1;
	}
	char line[1024];
	while (fgets(line, sizeof line, fp)) {
		int count[256] = { 0 };
		for (const char *ch = line; *ch; ch++) {
			count[*ch]++;
		}

		// count matches
		size_t m2 = 0;
		size_t m3 = 0;
		for (size_t i = 0; i < sizeof count / sizeof *count; i++) {
			if (count[i] == 2) m2++;
			if (count[i] == 3) m3++;
		}

		if (m2 && m3) {
			m2s += m2;
			m3s += m3;
		} else if (m2) {
			m2s++;
		} else if (m3) {
			m3s++;
		}
	}
	fclose(fp);

	printf("%zu * %zu = %zu\n", m2s, m3s, m2s * m3s);

	return 0;
}
