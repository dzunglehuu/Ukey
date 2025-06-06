#include <errno.h>
#include <locale.h>
#include <signal.h>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <X11/XKBlib.h>
#include "mnpl.h"

typedef struct {
	unsigned int mod;
	KeySym keysym;
	void (*func)();
} Key;

/* function declarations */
static void cleanup(void);
static void keypress_xi2(XIRawEvent *e);
static void key_process(char ch, KeySym keysym, uchar state);
static void run(void);
static void setup(void);
static void setup_xinput2(void);
static int has_xi2(void);
static void select_events();
static void sigchld(int unused);
static void switch_input(void);
static void stop(void);
static void switch_letter(void);


Display *dpy;
Window curFocus;
uchar lettershift = 0;
int scratch_keycode = 0;
struct q queue;
struct q queue2;
int xi_opcode = 0;
struct shortcut_map scmap[SHORTCUT_MAX];
static Window root;
static int control = 1;
static int running = 1;

static const Key keys[] = {
	/* modifier                    	key        		function 	  */
	{ ShiftMask,             		NoSymbol, 		switch_letter },
	{ ControlMask,             		XK_1, 			switch_input  },
	{ Mod1Mask,             		XK_u, 			stop,  		  },
};
static const Key incontrol_keys[] = {
	/* modifier                    	key        		function	*/
	{ 0,             				XK_BackSpace, 	backspace	},
	{ 0,             				XK_Delete, 		reset		},
	{ 0,             				XK_Up, 			reset		},
	{ 0,             				XK_Down, 		reset		},
	{ 0,             				XK_Escape, 		reset		},
	{ 0,             				XK_Left, 		leftarrow	},
	{ 0,             				XK_Right, 		rightarrow	},
	{ ControlMask,             		XK_period, 	    addshortcut	},
	{ ControlMask,             		XK_space, 		loadshortcut},
};

void
cleanup(void)
{
	XUngrabKey(dpy, AnyKey, AnyModifier, curFocus);
	KeySym keysym_list[2] = { 0, 0 };
	XChangeKeyboardMapping(dpy, scratch_keycode, 2, keysym_list, 1);
}

void
keypress_xi2(XIRawEvent *e)
{
    XkbStateRec state;
    XkbGetState(dpy, XkbUseCoreKbd, &state);
    //int group = state.group;
    //KeySym s = XkbKeycodeToKeysym(dpy, e->detail, group, 0 /*shift level*/);
    lettershift = 0;

	XComposeStatus status;
    KeySym keysym = NoSymbol;
    char text[32];
    XEvent ev = makeevent(e->detail);
	XKeyEvent *ek;
    ek = &ev.xkey;
	XLookupString(ek, text, sizeof(text) - 1, &keysym, &status);
    if(e->detail != scratch_keycode)
        key_process(text[0], keysym, state.mods);
}

void
key_process(char c, KeySym keysym, uchar state)
{
	char ch = tolower(c);
	for (int i = 0; i < LENGTH(keys); i++)
	{
		if (((keysym == keys[i].keysym) || (NoSymbol == keys[i].keysym)) &&
			(state & keys[i].mod) == keys[i].mod &&
			keys[i].func)
		{
			keys[i].func();
		}
	}
    if(control) {
		printf ("Key is: %c\n", ch);
		for (int i = 0; i < LENGTH(incontrol_keys); i++)
		{
			if (keysym == incontrol_keys[i].keysym &&
				(state & incontrol_keys[i].mod) == incontrol_keys[i].mod &&
				incontrol_keys[i].func)
			{
				incontrol_keys[i].func();
                return;
			}
		}
        if(isprint(ch))
        {
        	add(&queue, ch);
        	add(&queue2, ch);
    		pthread_t ptid;
    		pthread_create(&ptid, NULL, &updatemap, &ch);
    		int *ret;
    		pthread_join(ptid, (void **)&ret); 
        }
    }
}

void
run(void)
{
	XEvent ev;
	XGenericEventCookie *cookie = &ev.xcookie;
	while (running && !XNextEvent(dpy, &ev)){
        if(XFilterEvent(&ev, None) == True) continue;
		if (cookie->type == GenericEvent &&
			cookie->extension == xi_opcode &&
			XGetEventData(dpy, cookie))
		{
            if(cookie && cookie->evtype == XI_RawKeyPress)
            {
                keypress_xi2(cookie->data);
            }
		}
    }
    XFreeEventData(dpy, cookie);
}

void 
switch_input(void)
{
	printf("Switch!!!!!!!!\n");
	control = control ^ 1;
}

void 
stop(void)
{
	running = 0;
}

void 
switch_letter(void)
{
	lettershift = 1;
}

void
setup(void)
{
	/* clean up any zombies immediately */
	sigchld(0);
    int revert;
	root = DefaultRootWindow(dpy);
    XGetInputFocus (dpy, &curFocus, &revert);
    XSelectInput(dpy, curFocus, INPUTMASK);
	setup_xinput2();
    for(int i = 0; i < SHORTCUT_MAX; i++)
    {
        scmap[i].key = '\0';
        for(int ix = 0; ix < Q_MAX_SZ; ix++)
            strcpy(scmap[i].shortcut[ix], "");
    }
}

int 
has_xi2(void)
{
    int major, minor;
    int rc;

    /* We support XI 2.2 */
    major = 2;
    minor = 2;

    rc = XIQueryVersion(dpy, &major, &minor);
    if (rc == BadRequest) {
		printf("No XI2 support. Server supports version %d.%d only.\n", major, minor);
		return 0;
    } else if (rc != Success) {
		fprintf(stderr, "Internal Error! This is a bug in Xlib.\n");
    }
    printf("XI2 supported. Server provides version %d.%d.\n", major, minor);

    return 1;
}

void
setup_xinput2(void)
{
    int e1, error;
	if (!XQueryExtension(dpy, "XInputExtension", &xi_opcode, &e1, &error)) {
		printf("X Input extension not available.\n");
		return;
	}
	if(!has_xi2())
		return;
	select_events();
    XSelectInput(dpy, curFocus, 0);
}

void 
select_events()
{
    XIEventMask evmasks[1];
    unsigned char mask1[(XI_LASTEVENT + 7)/8];

    memset(mask1, 0, sizeof(mask1));

    /* select for button and key events from all master devices */
    XISetMask(mask1, XI_RawKeyPress);
    XISetMask(mask1, XI_RawKeyRelease);
    XISetMask(mask1, XI_FocusOut);
    XISetMask(mask1, XI_Leave);
    XISetMask(mask1, XI_Enter);

    evmasks[0].deviceid = XIAllMasterDevices;
    evmasks[0].mask_len = sizeof(mask1);
    evmasks[0].mask = mask1;

    XISelectEvents(dpy, root, evmasks, 1);
    XFlush(dpy);
}

void
sigchld(int unused)
{
	if (signal(SIGCHLD, sigchld) == SIG_ERR)
		die("can't install SIGCHLD handler:");
	while (0 < waitpid(-1, NULL, WNOHANG));
}

int
main(int argc, char *argv[])
{
	if (!setlocale(LC_CTYPE, "") || !XSupportsLocale())
		fputs("warning: no locale support\n", stderr);
	if (!(dpy = XOpenDisplay(0)))
		die("ukey: cannot open display");
	setup();
	run();
	cleanup();
    XCloseDisplay(dpy);
	return EXIT_SUCCESS;
}
