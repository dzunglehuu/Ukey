#ifndef TYPE_H_
#define TYPE_H_

#define KEY_MAX 4
#define Q_MAX_SZ 15
#define SHORTCUT_MAX 30
#define UNI_LEN 9

typedef unsigned char uchar;

struct latin_map {
	const char keys[KEY_MAX];
	const char unicode[UNI_LEN];
	uchar tracker;
	uchar vowel_pos;
    uchar iscapital;
	const uchar priority;
};

struct shortcut_map {
    char key;
    char shortcut[Q_MAX_SZ][UNI_LEN];
};

#endif