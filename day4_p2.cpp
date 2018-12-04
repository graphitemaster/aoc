#include <algorithm>
#include <vector>
#include <string>
#include <optional>
#include <unordered_map>

#include <stdio.h>
#include <string.h>

enum entry_type {
	sleep,
	wake,
	begin
};

struct entry {
	int year;
	int day;
	int month;

	int hour;
	int minute;

	int id;

	entry_type type;
};

int main() {
	FILE *fp = fopen("day4.txt", "rb");
	if (!fp) {
		return 1;
	}

	// process the data from the file into entries
	std::vector<entry> entries;
	char line[1024];
	while (fgets(line, sizeof line, fp)) {
		entry e;
		char message[1024];
		sscanf(line, "[%d-%d-%d %d:%d] %[^\t\n]",
			&e.year, &e.month, &e.day, &e.hour, &e.minute, message);
		if (sscanf(message, "Guard #%d", &e.id) == 1) {
			e.type = entry_type::begin;
		} else if (!strcmp(message, "wakes up")) {
			e.type = entry_type::wake;
		} else if (!strcmp(message, "falls asleep")) {
			e.type = entry_type::sleep;
		} else {
			abort();
		}
		entries.push_back(e);
	}
	fclose(fp);

	// sort those entries by timestamp
	std::sort(entries.begin(), entries.end(),
		[](const entry &a, const entry &b) {
			if (a.year < b.year)
				return true;
			if (a.year == b.year && a.month < b.month)
				return true;
			if (a.year == b.year && a.month == b.month && a.day < b.day)
				return true;
			if (a.year == b.year && a.month == b.month && a.day == b.day && a.hour < b.hour)
				return true;
			if (a.year == b.year && a.month == b.month && a.day == b.day && a.hour == b.hour && a.minute < b.minute)
				return true;
			return false;
		});

	// copy ids forward for all entries and build a table per guard
	std::unordered_map<int, std::vector<entry>> guard_states;
	const size_t count = entries.size();
	for (size_t i = 0; i < count; i++) {
		const entry &a = entries[i];
		if (a.type != entry_type::begin) {
			continue;
		}

		// add guard state
		guard_states[a.id].push_back(a);

		// this is a guard..
		for (size_t j = i + 1; j < count; j++) {
			entry &b = entries[j];
			if (b.type == entry_type::begin) {
				break; // encountered a new guard, stop
			}

			// give it this id
			b.id = a.id;

			// add the guard state
			guard_states[a.id].push_back(b);
		}
	}

	int most_sleep_time = 0;
	int most_sleep_guard = 0;

	// heat map of how many times we were asleep that minute
	std::unordered_map<int, std::array<int, 60>> most_slept_minute;

	// calculate the time spent
	for (const auto &[id, states] : guard_states) {
		const size_t count = states.size();
		int total_sleep_duration = 0;
		std::array<int, 60> slept_minute = {0};
		for (size_t i = 0; i < count; i++) {
			const auto &a = states[i];
			if (a.type != entry_type::sleep) {
				continue;
			}
			int sleep_start_time = a.hour * 60 + a.minute;
			int sleep_duration = 0;
			// accumulate time slept until awake
			for (size_t j = i + 1; j < count; j++) {
				const auto &b = states[j];
				if (b.type == entry_type::wake) {
					// mark all all the minutes as common sleep minutes for the guard
					for (int m = a.minute; m < b.minute; m++) {
						slept_minute[m]++;
					}
					int sleep_end_time = b.hour * 60 + b.minute;
					sleep_duration = sleep_end_time - sleep_start_time;
					break;
				}
			}
			total_sleep_duration += sleep_duration;
			most_slept_minute[id] = slept_minute;
		}

		if (total_sleep_duration > most_sleep_time) {
			most_sleep_guard = id;
			most_sleep_time = total_sleep_duration;
		}
	}

	// find the guard that has the most amount of time slept on a specific minute
	int best_value = 0;
	int best_id = 0;
	int best_max = 0;

	for (const auto &[id, minutes] : most_slept_minute) {
		const int *value = std::max_element(minutes.begin(), minutes.end());
		const int minute = std::distance(minutes.begin(), value);
		if (*value > best_value) {
			best_value = *value;
			best_id = id;
			best_max = minute;
		}
	}

	printf("guard %d spent the most time on minute %d\n", best_id, best_max);

	return 0;
}
