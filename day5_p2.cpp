#include <stdio.h>

#include <string>

int main() {
	FILE *fp = fopen("day5.txt", "rb");
	if (!fp) {
		return 1;
	}

	// read the whole thing into a string
	fseek(fp, 0, SEEK_END);
	long length = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	std::string result;
	result.resize(length);
	fread(result.data(), length, 1, fp);


	// try from a to z to see which one is better
	size_t min_length = 0xFFFFFFFFFFFFFFFFULL;
	for (int c = 'a'; c <= 'z'; c++) {
		std::string current = result;

		// remove 'c' chains first
		printf("removing chains of '%c' first\n", c);
		for (size_t i = 0; i < current.size(); i++) {
			if (tolower(current[i]) == c) {
				current.erase(i, 1);
				i = 0;
			}
		}

		// then react the full thing
		int total_reacted = 0;
		for (;;) {
			int reacted = 0;
			for (size_t i = 0; i < current.size(); i++) {
				int a = current[i + 0];
				int b = current[i + 1];

				// react so remove
				if (a != b && tolower(a) == tolower(b)) {
					current.erase(i, 2);
					reacted++;
				}
			}
			if (reacted == 0) {
				break;
			}
			total_reacted += reacted;
		}

		if (current.size() - 1 < min_length) {
			min_length = current.size() - 1;
			printf("minimum %d reactions\n", total_reacted);
		}
	}


	printf("smallest unit is %zu\n", min_length);

	fclose(fp);

	return 0;
}
