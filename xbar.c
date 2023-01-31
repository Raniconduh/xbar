#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#include <X11/Xlib.h>

#include "config.h"


void sighandler(int);
void redraw_bar(void);
void write_bar(void);


Display * dpy;
Window root;

struct {
	int s;
	int h;
	int w;
} screen;

struct {
	Window w;
	GC gc;
	int fh;
	int wh;

	char * txt;
	int tlen;
} bar;


void redraw_bar(void) {
	XClearWindow(dpy, bar.w);
	XDrawString(dpy, bar.w, bar.gc, (screen.w / 2) - (bar.tlen * 5 / 2),
	            (bar.fh + bar.wh) / 2 - 2, bar.txt, bar.tlen);
}


void write_bar(void) {
	char * txt;
	if (!XFetchName(dpy, root, &txt)) return;
	XFree(bar.txt);
	int tlen = strlen(txt);
	bar.txt = txt;
	bar.tlen = tlen;

	redraw_bar();
}


int main() {
	if (!(dpy = XOpenDisplay(0))) {
		fputs("Could not connect to display\n", stderr);
		exit(1);
	}

	signal(SIGINT, sighandler);
	signal(SIGTERM, sighandler);

	screen.s = DefaultScreen(dpy);
	root     = RootWindow(dpy, screen.s);
	screen.w = XDisplayWidth(dpy, screen.s);
	screen.h = XDisplayHeight(dpy, screen.s);

	XSelectInput(dpy, root, PropertyChangeMask);

	bar.txt = NULL;
	bar.tlen = 0;
	bar.wh = 12;
	bar.w = XCreateSimpleWindow(dpy, root, 0, 0, screen.w,
	                            bar.wh, BORDER_WIDTH, BORDER_COL, BARBG);
	XMapWindow(dpy, bar.w);
	XMoveWindow(dpy, bar.w, 0, 0);
	XSync(dpy, False);

	XSelectInput(dpy, bar.w, StructureNotifyMask
	                         | VisibilityChangeMask
	                         | ResizeRedirectMask
	);

	XFontStruct * finfo = XLoadQueryFont(dpy, "fixed");
	bar.fh = finfo->ascent + finfo->descent;
	bar.gc = XCreateGC(dpy, bar.w, 0, 0);

	XSetFont(dpy, bar.gc, finfo->fid);
	XSetForeground(dpy, bar.gc, BARFG);

	write_bar();

	XEvent ev;
	while (!XNextEvent(dpy, &ev)) {
		switch (ev.type) {
			case PropertyNotify:
				write_bar();
				break;
			case UnmapNotify:
				XMapWindow(dpy, bar.w);
				XMoveWindow(dpy, bar.w, 0, 0);
				break;
			case VisibilityNotify:
				redraw_bar();
				break;
			case ResizeRequest:
				XResizeWindow(dpy, bar.w, screen.w, bar.wh);
				redraw_bar();
				break;
			case ConfigureNotify:
				XMoveWindow(dpy, bar.w, 0, 0);
				break;
			default: break;
		}
	}

	XCloseDisplay(dpy);
	return 0;
}

void sighandler() {
	XCloseDisplay(dpy);
}
