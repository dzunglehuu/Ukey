#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <X11/keysym.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/XTest.h>
#include <X11/extensions/XInput2.h>
#include "type.h"
#include "util.h"
#include <pthread.h>
#include <wchar.h>
#include <wctype.h>

/* macros */
#define CLEANMASK(mask)         (mask & ~(numlockmask|LockMask) & (ShiftMask|ControlMask|Mod1Mask|Mod2Mask|Mod3Mask|Mod4Mask|Mod5Mask))
#define LENGTH(X)               (sizeof X / sizeof X[0])
#define MODKEY Mod1Mask

#define INPUTMASK               KeyPressMask|KeyReleaseMask|FocusChangeMask/*|ButtonPressMask*/

extern Display *dpy;
extern uchar lettershift;
extern int scratch_keycode;
extern struct q queue;
extern struct q queue2;
extern Window curFocus;
extern int xi_opcode;
extern uchar fakekey;
extern struct shortcut_map scmap[SHORTCUT_MAX];

void* updatemap(void* arg);
uchar iscombination(char ch1, char ch2, char ch3);
void delvowel(uchar *num_of_larrow, int del_current) ;
uchar iscontrolkey(char chr);
uchar isdupvowel(const char *str);
void reset(void);
void backspace(void);
void leftarrow(void);
void rightarrow(void);
void fakekeypress(KeyCode code);
void genkeypress(const char *unicode);
void manipulate(int idx);
void process_dup_control(char ch);
void addshortcut(void);
void loadshortcut(void);
void genshortcut();
XEvent makeevent(KeyCode kc);

