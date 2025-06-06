#include "mnpl.h"
#include <unistd.h>

static uchar lenofword = 0;
static char lastcontrol = '\0';
static int best_idx = -1;
static uchar numofvowel = 0;
static uchar doubleletter = 0;

static struct latin_map keymap[] = { 
									{ "aws", "U1EAF", 0, 0, 0, 5 }, //ắ
									{ "awj", "U1EB7", 0, 0, 0, 5 }, //ặ								
									{ "awr", "U1EB3", 0, 0, 0, 5 }, //ẳ 
									{ "awf", "U1EB1", 0, 0, 0, 5 }, //ằ
									{ "aas", "U1EA5", 0, 0, 0, 5 }, //ấ
									{ "aaf", "U1EA7", 0, 0, 0, 5 }, //ầ
									{ "aax", "U1EAB", 0, 0, 0, 5 }, //ẫ								
									{ "aar", "U1EA9", 0, 0, 0, 5 }, //ẩ 
									{ "aaj", "U1EAD", 0, 0, 0, 5 }, //ậ
									{ "awx", "U1EB5", 0, 0, 0, 5 }, //ẵ
									{ "aa", "U00E2", 0, 0, 0, 5 }, //â
									{ "aw", "U0103", 0, 0, 0, 5 }, //ă 
									{ "as", "U00E1", 0, 0, 0, 5 }, //á 
									{ "ax", "U00E3", 0, 0, 0, 5 }, //ã 
									{ "af", "U00E0", 0, 0, 0, 5 }, //à 
									{ "aj", "U1EA1", 0, 0, 0, 5 }, //ạ
									{ "ar", "U1EA3", 0, 0, 0, 5 }, //ả
									{ "ees", "U1EBF", 0, 0, 0, 4 }, //ế								
									{ "eef", "U1EC1", 0, 0, 0, 4 }, //ề 
									{ "eex", "U1EC5", 0, 0, 0, 4 }, //ễ
									{ "eer", "U1EC3", 0, 0, 0, 4 }, //ể
									{ "eej", "U1EC7", 0, 0, 0, 4 }, //ệ
									{ "ee", "U00EA", 0, 0, 0, 4 }, //ê
									{ "ef", "U00E8", 0, 0, 0, 4 }, //è								
									{ "es", "U00E9", 0, 0, 0, 4 }, //é 
									{ "er", "U1EBB", 0, 0, 0, 4 }, //ẻ
									{ "ej", "U1EB9", 0, 0, 0, 4 }, //ẹ
									{ "ex", "U1EBD", 0, 0, 0, 4 }, //ẽ
									{ "if", "U00EC", 0, 0, 0, 3 }, //ì								
									{ "is", "U00ED", 0, 0, 0, 3 }, //í 
									{ "ir", "U1EC9", 0, 0, 0, 3 }, //ỉ
									{ "ij", "U1ECB", 0, 0, 0, 3 }, //ị
									{ "ix", "U0129", 0, 0, 0, 3 }, //ĩ								
									{ "yf", "U1EF3", 0, 0, 0, 3 }, //ỳ								
									{ "ys", "U00FD", 0, 0, 0, 3 }, //ý 
									{ "yr", "U1EF7", 0, 0, 0, 3 }, //ỷ
									{ "yj", "U1EF5", 0, 0, 0, 3 }, //ỵ
									{ "yx", "U1EF9", 0, 0, 0, 3 }, //ỹ								
									{ "oof", "U1ED3", 0, 0, 0, 2 }, //ồ
									{ "oor", "U1ED5", 0, 0, 0, 2 }, //ổ
									{ "oos", "U1ED1", 0, 0, 0, 2 }, //ố
									{ "oox", "U1ED7", 0, 0, 0, 2}, //ỗ
									{ "ooj", "U1ED9", 0, 0, 0, 2}, //ộ
									{ "owf", "U1EDD", 0, 0, 0, 2}, //ờ
									{ "owr", "U1EDF", 0, 0, 0, 2}, //ở
									{ "ows", "U1EDB", 0, 0, 0, 2}, //ớ
									{ "owx", "U1EE1", 0, 0, 0, 2}, //ỡ
									{ "owj", "U1EE3", 0, 0, 0, 2}, //ợ
									{ "oo", "U00F4", 0, 0, 0, 2 }, //ô
									{ "of", "U00F2", 0, 0, 0, 2}, //ò
									{ "os", "U00F3", 0, 0, 0, 2}, //ó
									{ "or", "U1ECF", 0, 0, 0, 2}, //ỏ
									{ "oj", "U1ECD", 0, 0, 0, 2}, //ọ
									{ "ox", "U00F5", 0, 0, 0, 2}, //õ
									{ "ow", "U01A1", 0, 0, 0, 2}, //ơ
									{ "uws", "U1EE9", 0, 0, 0, 1}, //ứ
									{ "uwr", "U1EED", 0, 0, 0, 1}, //ử
									{ "uwf", "U1EEB", 0, 0, 0, 1}, //ừ
									{ "uwx", "U1EEF", 0, 0, 0, 1}, //ữ
									{ "uwj", "U1EF1", 0, 0, 0, 1}, //ự
									{ "uw", "U01B0", 0, 0, 0, 1}, //ư
									{ "uj", "U1EE5", 0, 0, 0, 1}, //ụ
									{ "us", "U00FA", 0, 0, 0, 1}, //ú
									{ "ur", "U1EE7", 0, 0, 0, 1}, //ủ
									{ "uf", "U00F9", 0, 0, 0, 1}, //ù
									{ "ux", "U0169", 0, 0, 0, 1}, //ũ
									{ "dd", "U0111", 0, 0, 0, 0}, //đ
								   };
static char shortcuts[Q_MAX_SZ][UNI_LEN] = {""};

void 
addshortcut(void)
{
    genshortcut();
    char ch = queue.str[0];
    int i;
    for(i = 0; i < SHORTCUT_MAX; i++)
    {
        if(scmap[i].key == '\0')
        {
            scmap[i].key = ch;
            for(int ix = 0; ix < Q_MAX_SZ; ix++)
            {
                strcpy(scmap[i].shortcut[ix], shortcuts[ix]);
            }
            return;
        }
        if(scmap[i].key && scmap[i].key == ch)
        {
            for(int ix = 0; ix < Q_MAX_SZ; ix++)
                strcpy(scmap[i].shortcut[ix], shortcuts[ix]);
            return;
        }
    }
}

XEvent
makeevent(KeyCode kc)
{
	XEvent ev;
	memset(&ev, 0, sizeof(XEvent));
    
    /**/ev.xkey.display = dpy;
    ev.xkey.window = curFocus;
    ev.xkey.root = DefaultRootWindow(dpy);
    ev.xkey.subwindow = None;
    ev.xkey.time = CurrentTime;
    ev.xkey.x = 1;
    ev.xkey.y = 1;
    ev.xkey.x_root = 1;
    ev.xkey.y_root = 1;
    ev.xkey.same_screen = True;


	ev.xkey.keycode = kc;
	ev.xkey.type = KeyPress;
	ev.xkey.send_event = True;
    return ev;
}

void
genword(char str[Q_MAX_SZ][UNI_LEN])
{
    uchar unicode_cnt = 0;
    usleep(100000);
    for(int i = 0; i < Q_MAX_SZ; i++)
    {
        if(strcmp(str[i], "") != 0)
        {
            if(strlen(str[i]) > 1)
            {
                if(unicode_cnt > 0)
                    usleep(100000);
                genkeypress(str[i]);
                unicode_cnt++;
            }else
            {
            	KeySym ks = XStringToKeysym(str[i]);
            	fakekeypress(XKeysymToKeycode(dpy, ks));
            }
        }
    }
}

void
loadshortcut(void)
{
    char qstr[Q_MAX_SZ];
    strcpy(qstr, queue.str);
	for(int i = 0; i < 1; i++)
    	fakekeypress(XKeysymToKeycode(dpy, XK_BackSpace));

	char *p = qstr;
	while(*p)
	{
		char sc = *p;
		if(!isprint(sc))
			continue;
		for(int i = 0; i < SHORTCUT_MAX; i++)
		{
			if(sc == scmap[i].key)
			{
				genword(scmap[i].shortcut);
            	fakekeypress(XKeysymToKeycode(dpy, XK_space));
				break;
			}
		}
		p++;
	}
    reset();
}

void 
delvowel(uchar *larrow, int del_current) 
{
    if(del_current)
	{
    	fakekeypress(XKeysymToKeycode(dpy, XK_BackSpace));
        poll_back(&queue2);
    }else
	{
    	fakekeypress(XKeysymToKeycode(dpy, XK_Left));
    }
	*larrow = lenofword - keymap[best_idx].vowel_pos;
	KeyCode c = XKeysymToKeycode(dpy, XK_Left);
	for(int ix = 0; ix < *larrow; ix++)
		fakekeypress(c);
	fakekeypress(XKeysymToKeycode(dpy, XK_BackSpace));
}

void 
process_dup_control(char ch) 
{
    lastcontrol = '\0';
	uchar larrow = 0;
    delvowel(&larrow, 0);
    char vowel[KEY_MAX];
    vowel[0] = keymap[best_idx].keys[0];
    vowel[1] = '\0';
    KeySym vowel_sym = XStringToKeysym(vowel);
    KeyCode kc = XKeysymToKeycode(dpy, vowel_sym);
	fakekeypress(kc);
	KeyCode c = XKeysymToKeycode(dpy, XK_Right);
	for(int ix = 0; ix < larrow + 1; ix++) 
	{
		fakekeypress(c);
	}
    strcpy(shortcuts[keymap[best_idx].vowel_pos - 1], vowel);
	for (int latin_idx = 0; latin_idx < LENGTH(keymap); latin_idx++)
	{
		keymap[latin_idx].tracker = 0;
		keymap[latin_idx].vowel_pos = 0;
		keymap[latin_idx].iscapital = 0;
	}
}

void* 
updatemap(void* arg) 
{
    char ch = *(char *)arg;
    uchar vowfound = 0;
    if(lastcontrol != '\0' && ch == lastcontrol && 
		!iscombination(peek2(queue), peek(queue), ch)) 
	{
        process_dup_control(ch);
        pthread_exit(NULL);
    }
	if (isspace(ch)) 
	{
        reset();
        pthread_exit(NULL);
	}
	lenofword++;
	for (int latin_idx = 0; latin_idx < LENGTH(keymap); latin_idx++)
	{
		uchar pos = keymap[latin_idx].tracker;
		char remain_keys[KEY_MAX];
		substr(keymap[latin_idx].keys, pos, remain_keys);
		if((keymap[latin_idx].tracker == 0 && remain_keys == strchr(remain_keys, ch)) 
			|| (keymap[latin_idx].tracker > 0 && lowerstr(remain_keys) 
				&& remain_keys == strchr(remain_keys, tolower(ch))))
			keymap[latin_idx].tracker++;
		if(keymap[latin_idx].tracker == 1 && keymap[latin_idx].vowel_pos == 0)
		{
			keymap[latin_idx].vowel_pos = queue.size;
            if(doubleletter)
                keymap[latin_idx].vowel_pos = queue.size - doubleletter;
            vowfound = 1;
            if(lettershift)
                keymap[latin_idx].iscapital = 1;
        }
	}
    if(vowfound)
	{
        numofvowel++;
        vowfound = 0;
    }
	if(iscontrolkey(ch)) 
	{
        if(ch == 'd' || ch == 'w')
		{
            doubleletter += 1;
        }
		lenofword--;
		manipulate(best_idx);
	}
    pthread_exit(NULL);
}

uchar 
iscombination(char ch1, char ch2, char ch3) 
{
    char str[KEY_MAX];
    int i = 0;
    if(ch2){
        str[i++] = ch2;
    }
    str[i++] = ch3;
    str[i] = '\0';
	for (int latin_idx = 0; latin_idx < LENGTH(keymap); latin_idx++)
	{
		if(strcmp(keymap[latin_idx].keys, str) == 0) 
        {
            return 1;
        }
	}
	i = 0;
	if(ch1)
	{
		str[i++] = ch1;
	}
	if(ch2)
	{
		str[i++] = ch2;
	}
	str[i++] = ch3;
	str[i] = '\0';
	for (int latin_idx = 0; latin_idx < LENGTH(keymap); latin_idx++)
	{
		if(strcmp(keymap[latin_idx].keys, str) == 0) 
		{
			return 1;
		}
	}
    return 0;
}

uchar 
isdupvowel(const char *str) 
{
    return strlen(str) == 2 && str[0] == str[1];
}

uchar 
iscontrolkey(char chr) 
{
	for (int idx = 0; idx < LENGTH(keymap); idx++)
	{
        uchar wordtocompare = lenofword - 1;
		char ch = '\0';
		char len = strlen(keymap[idx].keys);
		if(len > 0)
			ch = keymap[idx].keys[len - 1];
		if((tolower(ch) == tolower(chr)) && (keymap[idx].tracker == len))
        {
            if(doubleletter)
			{
                wordtocompare += doubleletter;
            }
			if(!isdupvowel(keymap[idx].keys) && numofvowel > 1 && lenofword > 2 && (keymap[idx].vowel_pos >= wordtocompare))
				continue;
            best_idx = idx;
            lastcontrol = ch;
			return 1;
        }
	}
	return 0;
}

void 
reset(void) 
{
    printf("calling reset\n");
	lenofword = 0;
    lastcontrol = '\0';
	clear(&queue);
	clear(&queue2);
    numofvowel = 0;
    best_idx = -1;
    lettershift = 0;
    doubleletter = 0;
	for (int latin_idx = 0; latin_idx < LENGTH(keymap); latin_idx++)
	{
		keymap[latin_idx].tracker = 0;
		keymap[latin_idx].vowel_pos = 0;
		keymap[latin_idx].iscapital = 0;
	}
    for(int ix = 0; ix < Q_MAX_SZ; ix++)
    {
        strcpy(shortcuts[ix], "");
    }
}

void 
backspace(void) 
{
    reset();
}

void 
leftarrow(void) 
{
    reset();
}

void 
rightarrow(void) 
{
    //reset();
}

void 
fakekeypress(KeyCode code) 
{
    XEvent ev;
	XTestFakeKeyEvent(dpy, code, True, 0);
    XNextEvent(dpy, &ev);
	XTestFakeKeyEvent(dpy, code, False, 0);
    XNextEvent(dpy, &ev);
	XFlush(dpy);
}

void 
genkeypress(const char *unicode) 
{
	KeySym *keysyms = NULL;
	int keysyms_per_keycode = 0;
	int keycode_low, keycode_high;
	/* get the range of keycodes usually from 8 - 255 */
	XDisplayKeycodes(dpy, &keycode_low, &keycode_high);
	/* get all the mapped keysyms available */
	keysyms = XGetKeyboardMapping(
				  dpy, keycode_low, 
		keycode_high - keycode_low, 
			  &keysyms_per_keycode);
	for (int idx = keycode_low; idx <= keycode_high; idx++)
	{
		int key_is_empty = 1;
		for (int sub_idx = 0; sub_idx < keysyms_per_keycode; sub_idx++)
		{
			int symindex = (idx - keycode_low) * keysyms_per_keycode + sub_idx;
			if(keysyms[symindex] != 0) 
			{
				key_is_empty = 0;
			} else 
			{
				break;
			}
		}
		if(key_is_empty) 
		{
			scratch_keycode = idx;
			KeySym sym = XStringToKeysym(unicode);
			KeySym keysym_list[2] = { sym, sym };
			XChangeKeyboardMapping(dpy, scratch_keycode, 2, keysym_list, 1);
			KeyCode code = scratch_keycode;
			fakekeypress(code);
			break;
		}
	}
	XFree(keysyms);
	XFlush(dpy);
	
}

void
genshortcut()
{
    char pos = -1;
    if(best_idx >= 0)
        pos = keymap[best_idx].vowel_pos;
    char *p = queue2.str;
    uchar idx = 0;
    while(*p)
    {
        if(strlen(shortcuts[idx]) > 1)
        {
            idx++;
            p++;
            continue;
        }
        char str[2];
        str[0] = *p;
        str[1] = '\0';
        strcpy(shortcuts[idx++], str);
        p++;
    }
    if(pos >= 0)
        strcpy(shortcuts[pos - 1], keymap[best_idx].unicode);
}

void 
manipulate(int idx)
{
	if(best_idx < 0)
		return;
	uchar larrow = 0;
    delvowel(&larrow, 1);
    if(keymap[best_idx].iscapital)
	{
        char hexstr[9];
        substr(keymap[best_idx].unicode, 1, hexstr);
        wchar_t wch = strtol(hexstr, NULL, 16);
        wch = towupper(wch);
        char upperstr[9];
        sprintf(upperstr, "U%04x", wch);
    	genkeypress(upperstr);
        genshortcut();
    }else
	{
    	genkeypress(keymap[best_idx].unicode);
        genshortcut();
    }
	XFlush(dpy);
	KeyCode c = XKeysymToKeycode(dpy, XK_Right);
	for(int ix = 0; ix < larrow; ix++)
		fakekeypress(c);
    keymap[best_idx].tracker = 0;
}