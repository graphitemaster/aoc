#include <stdio.h>
#include <stdlib.h>

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

	long frequency = 0;
	char line[128];
	while (fgets(line, sizeof line, fp)) {
		char *end;
		const long value = strtol(line, &end, 10);
		frequency += value;
	}

	printf("%d\n", frequency);

	fclose(fp);

	return 0;
}
