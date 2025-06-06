/* See LICENSE file for copyright and license details. */
#include <stdarg.h>

#include "util.h"

void
die(const char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	vfprintf(stderr, fmt, ap);
	va_end(ap);

	if (fmt[0] && fmt[strlen(fmt)-1] == ':') {
		fputc(' ', stderr);
		perror(NULL);
	} else {
		fputc('\n', stderr);
	}

	exit(1);
}

void *
ecalloc(size_t nmemb, size_t size)
{
	void *p;

	if (!(p = calloc(nmemb, size)))
		die("calloc:");
	return p;
}

void 
add(struct q *qu, char ch) 
{
	if(qu->size < Q_MAX_SZ) 
	{
		qu->size++;
		strncat(qu->str, &ch, 1);
	}
}

/*char 
poll(struct q *qu) 
{
	char ch = '\0';
	ch = qu->str[0];
	if(qu->size > 0) 
	{
		for(int idx = 0; idx < qu->size - 1; idx++) 
		{
			qu->str[idx] = qu->str[idx + 1];
		}
		qu->str[qu->size - 1] = '\0';
		qu->size--;
	}
	return ch;
}*/

void 
poll_back(struct q *qu) 
{
	if(qu->size == 0) 
	{
		return;
	}
	qu->str[qu->size - 1] = '\0';
	qu->size--;
}

char 
peek(const struct q qu) 
{
    char ch;
    if(qu.size >= 1)
	   ch = qu.str[qu.size - 2];
    else
       ch = '\0';
    return ch;
}

char 
peek2(const struct q qu) 
{
    char ch;
    if(qu.size >= 2)
	   ch = qu.str[qu.size - 3];
    else
       ch = '\0';
    return ch;
}

void 
clear(struct q *qu) 
{
	qu->str[0] = '\0';
	qu->size = 0;
}

void 
printq(const struct q qu)
{
	printf("%s\n", qu.str);
}

void 
posInStr(char *str, char ch, uchar *arr) 
{
	uchar cnt = 0;
	char* sh;
	sh = strchr(str, ch);
	while(sh) 
	{
		arr[cnt++] = sh - str;
		sh = strchr(sh + 1, ch);
	}
}

void 
substr(const char *str, uchar pos, char *sub) 
{
    int len = strlen(str);
    if(pos < len)
	    strncpy(sub, str + pos, len - pos);
    sub[len - pos] = '\0';
}

uchar 
lowerstr(char *str) 
{
	for(int i = 0; str[i]; i++)
	{
		str[i] = tolower(str[i]);
	}
	return 1;
}