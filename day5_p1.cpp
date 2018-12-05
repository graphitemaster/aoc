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

	int total_reacted = 0;
	for (;;) {
		int reacted = 0;
		for (size_t i = 0; i < result.size(); i++) {
			int a = result[i + 0];
			int b = result[i + 1];

			// react so remove
			if (a != b && tolower(a) == tolower(b)) {
				result.erase(i, 2);
				reacted++;
			}
		}
		if (reacted == 0) {
			break;
		}
		total_reacted += reacted;
	}

	printf("%d reacted leaving %zu units\n", total_reacted, result.size()-1);

	fclose(fp);

	return 0;
}
