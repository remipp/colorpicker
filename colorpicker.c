#include <stdio.h>
#include <stdint.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/cursorfont.h>

int main(int argc, char* argv[])
{
	Display *d = XOpenDisplay(NULL);
	if (!d) {
		fprintf(stderr, "couldn't open display\n");
		return 1;
	}

	Window root = DefaultRootWindow(d);
	Cursor cross = XCreateFontCursor(d, XC_cross);

	if (XGrabPointer(d, root, False, ButtonPressMask, GrabModeAsync, GrabModeAsync, root, cross, CurrentTime)) {
		fprintf(stderr, "could not grab pointer\n");
		XFreeCursor(d, cross);
		XCloseDisplay(d);
		return 1;
	}
	if (XGrabKeyboard(d, root, False, GrabModeAsync, GrabModeAsync, CurrentTime)) {
		fprintf(stderr, "could not grab keyboard\n");
		XFreeCursor(d, cross);
		XCloseDisplay(d);
		return 1;
	}

	XEvent ev;
	do {
		XNextEvent(d, &ev);
		if (ev.type == KeyPress)
			goto cleanup;
	} while (ev.type != ButtonPress);

	if (ev.xbutton.button != 1)
		goto cleanup;

	XColor c;
	c.pixel = XGetPixel(XGetImage(d, root, ev.xbutton.x_root, ev.xbutton.y_root, 1, 1, AllPlanes, ZPixmap), 0, 0);
	XQueryColor(d, DefaultColormap(d, DefaultScreen(d)), &c);

	printf("#%x", (uint8_t)c.red << 16 | (uint8_t)c.green << 8 | (uint8_t)c.blue);

cleanup:
	XUngrabKeyboard(d, CurrentTime);
	XUngrabPointer(d, CurrentTime);
	XFreeCursor(d, cross);
	XCloseDisplay(d);
}
