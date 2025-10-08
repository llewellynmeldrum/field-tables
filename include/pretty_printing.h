#ifndef PRETTY_PRINTING_H
#define PRETTY_PRINTING_H

#include <stdarg.h>
#include <string.h>
#include <sys/types.h>
#include <sys/sysctl.h>
#include <sys/ioctl.h>

#include <stdio.h>
#include <unistd.h>
static inline void putc_ntimes(const char c, ssize_t n) {
	for (; n; --n) printf("%c", c);
}
static inline ssize_t get_termcols() {
	struct winsize w;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	return w.ws_col;
}
#define BANNER_PAT "-"
static inline void puts_ntimes(const char* s, ssize_t n) {
	for (; n; --n) printf("%s", s);
}

static inline void repeatstr_ncols(const char* s, ssize_t ncols) {
	int n = 0;
	int i = 0;
	int len = strlen(s);
	do {
		i = n % len;
		printf("%c", s[i]);
	} while(++n < ncols);
}

static inline void printf_center(const char* prefix, const char* fmt, ...) {
	ssize_t term_cols = get_termcols();

	char buf[256];
	va_list args;
	va_start(args, fmt);
	int msg_len = vsnprintf(buf, 256, fmt, args);
	va_end(args);
	ssize_t left_pad = (term_cols - msg_len) / 2;
	ssize_t right_pad = term_cols - left_pad - msg_len;
	printf("%s", prefix);
	repeatstr_ncols(" ", left_pad);
	printf("%s", buf);
	repeatstr_ncols(" ", right_pad);
	printf("\e[0m");
}

static inline void printf_banner(const char* prefix, const char* fmt, ...) {
	ssize_t term_cols = get_termcols();

	char buf[256];
	va_list args;
	va_start(args, fmt);
	int msg_len = vsnprintf(buf, 256, fmt, args);
	va_end(args);
	ssize_t left_pad = (term_cols - msg_len) / 2;
	ssize_t right_pad = term_cols - left_pad - msg_len;
	printf("%s", prefix);
	repeatstr_ncols(BANNER_PAT, left_pad);
	printf("%s", buf);
	repeatstr_ncols(BANNER_PAT, right_pad);
	printf("\e[0m");
}

#define FMT_RESET 		"\e[0m"

#define HEADING_FMT 		"\e[1m"
#define SUBHEADING_FMT 		"\e[1m"
#define BODY_FMT 		"\e[0m"
#define SUBBODY_FMT 		"\e[0m"

#define HEADING_INDENT 		""
#define SUBHEADING_INDENT 	"\t"
#define BODY_INDENT 		"\t"
#define SUBBODY_INDENT 		"\t\t"

#define printf_heading(fmt, ...) do{		\
	printf(HEADING_FMT HEADING_INDENT);	\
	printf(fmt "\n", ##__VA_ARGS__);		\
	printf(FMT_RESET);			\
	}while(0)\

#define printf_subheading(fmt, ...) do{\
	printf(SUBHEADING_FMT SUBHEADING_INDENT);\
	printf(fmt "\n", ##__VA_ARGS__);		\
	printf(FMT_RESET);\
	}while(0)\

#define printf_body(fmt, ...) do{\
	printf(BODY_FMT BODY_INDENT);\
	printf(fmt "\n", ##__VA_ARGS__);		\
	printf(FMT_RESET);\
	}while(0)\

#define printf_subbody(fmt, ...) do{\
	printf(SUBBODY_FMT SUBBODY_INDENT);\
	printf(fmt "\n", ##__VA_ARGS__);		\
	printf(FMT_RESET);\
	}while(0)\

#endif //PRETTY_PRINTING_H
