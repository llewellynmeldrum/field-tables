#include "pretty_printing.h"
const char LTORS_SEP = ',';
/* long-to-readable-string */
void ltors(ssize_t l, char* s) {
	ssize_t orig = l;
	ssize_t len = 0;
	int dig_count = 0;
	while (l >= 1) {
		++len;
		dig_count++;
		l /= 10;
		if (l >= 1 && dig_count % 3 == 0) {
			len++;
		}
	}

	l = orig;
	int i = 0;
	dig_count = 0;
	while (l >= 1) {
		int pos = len - i - 1;
		s[pos] = (l % 10) + '0';
		i++;
		dig_count++;
		l /= 10;
		if (pos - 2 >= 0 && dig_count % 3 == 0) {
			// 0'000
			s[pos - 1] = LTORS_SEP;
			i++;
		}
	}

}
