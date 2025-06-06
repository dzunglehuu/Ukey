#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "type.h"

struct q {
	char str[Q_MAX_SZ];
	int size;
};

#define MAX(A, B)               ((A) > (B) ? (A) : (B))
#define MIN(A, B)               ((A) < (B) ? (A) : (B))
#define BETWEEN(X, A, B)        ((A) <= (X) && (X) <= (B))

void die(const char *fmt, ...);
void *ecalloc(size_t nmemb, size_t size);
void add(struct q *qu, char ch);
char poll(struct q *qu);
void poll_back(struct q *qu);
char peek(const struct q qu);
char peek2(const struct q qu);
void clear(struct q *qu);
void printq(const struct q qu);
void posInStr(char *str, char ch, uchar *arr);
void substr(const char *str, uchar pos, char *sub);
uchar lowerstr(char *str);
