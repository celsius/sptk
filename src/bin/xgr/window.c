/****************************************************************
*	Setup Window for xgr.					*
****************************************************************/
#include	<X11/Xlib.h>
#include	<X11/Xutil.h>
#include	<X11/cursorfont.h>
#include	<stdio.h>
#include	"gcdata.h"
#include	"xgr.h"

Display		*display;
Window		window;
int		screen;
GC		gc, pixmapGC;
unsigned long	forepix, backpix, highpix, brdrpix, mouspix;

static XSizeHints		xsh;
static Atom			property;
static Pixmap			pixmap;
static Cursor			watch_cur;
static XGCValues		gcval;
static Font			font;
static XEvent			ev;
static XSetWindowAttributes	winatt;
static XWindowAttributes	wa;

long	pixmap_w, pixmap_h;
long	origin_x = 0, origin_y = 0;
long	window_x, window_y, window_w, window_h;
int	bwidth = 2;

extern float	shrink;
extern int	landscape;
extern int	reverse, c_flg;
extern int	fno;
extern char	*fore_color, *back_color;
extern char	*high_color, *brdr_color, *mous_color;
extern char	*geometry;
extern char	*dpy;
extern char	*cmnd, *windowtitle;

open_display( display_name )
	char	*display_name;
{
	if ((display = XOpenDisplay(display_name)) == NULL)  {
		fprintf(stderr, "%s: Can't open display '%s'\n",
			cmnd, XDisplayName(display_name));
		exit(2);
	}
}

init_window( argc, argv )
	int	argc;
	char	*argv[];
{
	unsigned long	get_color_pix();

	screen = DefaultScreen(display);

#if BSD
	bzero(&xsh, sizeof(xsh));
#else
	memset(&xsh, 0, sizeof(xsh));
#endif
	if (geometry)  {
		int	bitmask;

		bitmask = XGeometry(display, screen, geometry, NULL,
				bwidth, 1, 1, 1, 1,
				&(xsh.x), &(xsh.y),
				&(xsh.width), &(xsh.height));
		if (bitmask & (XValue|YValue))  {
			xsh.flags |= USPosition;
		}
		if (bitmask & (WidthValue|HeightValue))  {
			xsh.flags |= USSize;
		}
	}
	else  {
		xsh.flags = USPosition | PSize;
		if (!landscape)  {
			xsh.width  = XLENG / shrink;
			xsh.height = YLENG / shrink;
			xsh.x = X0;
			xsh.y = Y0;
		}
		else  {
			xsh.width  = YLENG / shrink;
			xsh.height = XLENG / shrink;
			xsh.x = X0_LAND;
			xsh.y = Y0;
		}
	}

/**  Color  **/
#ifdef COLOR_BUG
	reverse = 1;
#endif
	if (DisplayPlanes(display, screen) >=3 )  {
		c_flg = 1;
		if (!reverse)  {
			forepix = get_color_pix(fore_color);
			backpix = get_color_pix(back_color);
			highpix = get_color_pix(high_color);
			brdrpix = get_color_pix(brdr_color);
			mouspix = get_color_pix(mous_color);
		}
		else  {
			forepix = get_color_pix(back_color);
			backpix = get_color_pix(fore_color);
			highpix = get_color_pix(high_color);
			brdrpix = get_color_pix(brdr_color);
			mouspix = get_color_pix(mous_color);
		}
	}
	else  {
		if (!reverse)  {
			forepix = BlackPixel(display, screen);
			highpix = BlackPixel(display, screen);
			backpix = WhitePixel(display, screen);
			brdrpix = BlackPixel(display, screen);
			mouspix = BlackPixel(display, screen);
		}
		else  {
			forepix = WhitePixel(display, screen);
			highpix = WhitePixel(display, screen);
			backpix = BlackPixel(display, screen);
			brdrpix = WhitePixel(display, screen);
			mouspix = WhitePixel(display, screen);
		}
	}
	

/**  Generate Window  **/
	window = XCreateSimpleWindow(display, DefaultRootWindow(display),
			xsh.x, xsh.y, xsh.width, xsh.height,
			bwidth,	brdrpix, backpix);

	XSetStandardProperties(display, window, windowtitle, windowtitle,
		None, argv, argc, &xsh);

/*	winatt.bit_gravity = SouthWestGravity;	*/
	XChangeWindowAttributes(display, window, CWBitGravity, &winatt);

/**  Map Window  **/
	XSelectInput(display, window, StructureNotifyMask);
	XMapWindow(display, window);
	for (;;)  {
		XNextEvent(display, &ev);
		if (ev.type == MapNotify)
			break;
	}
	XSelectInput(display, window,
	ButtonPressMask | PointerMotionMask | KeyPressMask | ExposureMask);
/*		KeyReleaseMask|ExposureMask|StructureNotifyMask);
*/
/**  Cursor  **/
	watch_cur = XCreateFontCursor(display, XC_watch);
	XDefineCursor(display, window, watch_cur);

/**  GC  **/
	gcval.line_width = 1;
	gc = XCreateGC(display, window, GCLineWidth, &gcval);

	XSetFunction(display, gc, GXcopy);
	XSetGraphicsExposures(display, gc, False);
	XSetForeground(display, gc, forepix);
	XSetBackground(display, gc, backpix);

	font = XLoadFont(display, f_name[fno]);
	XSetFont(display, gc, font);
}

init_pixmap()
{
	if (!landscape)  {
		pixmap_w = XLENG / shrink;
		pixmap_h = YLENG / shrink;
	}
	else  {
		pixmap_w = YLENG / shrink;
		pixmap_h = XLENG / shrink;
	}
	pixmap = XCreatePixmap(display, window, pixmap_w, pixmap_h,
			       DefaultDepth(display, screen));
	pixmapGC = XCreateGC(display, pixmap, 0L, &gcval);
	XFillRectangle(display, pixmap, pixmapGC, 0, 0, pixmap_w, pixmap_h);
	XSetFunction(display, pixmapGC, GXcopy);
	XSetFillStyle(display, pixmapGC, FillSolid);
	XSetForeground(display, pixmapGC, forepix);
	XSetBackground(display, pixmapGC, backpix);
}
	
close_window()
{
	XFreeCursor(display, watch_cur);
	XFreeGC(display, gc);
	XFreeGC(display, pixmapGC);
	XFreePixmap(display, pixmap);
	XDestroyWindow(display, window);
/**/	exit(0);
/**/	XCloseDisplay(display);		/* this function case an error !! */
}

main_loop()
{
	char	key;
	int	paper_w, paper_h;
	float	shr_w, shr_h;
	long	diff;

	if (!landscape)  {
		paper_w = XLENG / shrink;
		paper_h = YLENG / shrink;
	}
	else  {
		paper_w = YLENG / shrink;
		paper_h = XLENG / shrink;
	}
	set_all();
	XUndefineCursor(display, window);
	for (;;)  {
	    XNextEvent(display, &ev);
	    switch(ev.type)  {
		case Expose:
		    if (ev.xexpose.count == 0)
			get_window_size();
		    realize_part(ev.xexpose.x, ev.xexpose.y, 
				 ev.xexpose.width, ev.xexpose.height, 
				 ev.xexpose.x, ev.xexpose.y);
		    break;
		case MappingNotify:
/*		    XRefreshKeyboardMapping(&ev);
*/		    XRefreshKeyboardMapping((XMappingEvent *)&ev);
		    break;
/*		case ConfigureNotify:
		    get_window_size();
		    shr_w = paper_w / window_w;
		    shr_h = paper_h / window_h;
		    shrink = (shr_w >= shr_h) ? shr_w :shr_h;
		    rewind(stdin);
		    plot();
		    main_loop();

		    origin_x += window_x;
		    origin_y += window_y;
		    realize();
		    realize_part(origin_x, origin_y, window_w, window_h,
				 origin_x, origin_y);
		    origin_x = paper_w - xsh.width;
		    origin_y = paper_h - xsh.height;
		    origin_x += xsh.x;
		    origin_y += xsh.y;
		    break;
*/		case MotionNotify:
		    break;
		case ButtonPress:
		    break;
		case KeyPress:
		    get_window_size();
		    XLookupString(&ev.xkey, &key, 1, NULL, NULL);
		    switch (key)  {
			case 'j':
			    diff = paper_h - window_h;
			    if (origin_y >= diff)  {
				    beep();			break;
			    }
			    origin_y += window_h / 4;
			    if (origin_y > diff)
				    origin_y = diff;
			    if (origin_y < 0)
				    origin_y = 0;
			    realize();
			    continue;				break;
			case 'k':
			    if (origin_y <= 0)  {
				    beep();			break;
			    }
			    origin_y -= window_h / 4;
			    if (origin_y < 0)
				    origin_y = 0;
			    realize();
			    continue;				break;
			case 'l':
			    diff = paper_w - window_w;
			    if (origin_x >= diff)  {
				    beep();			break;
			    }
			    origin_x += window_w / 4;
			    if (origin_x > diff)
				    origin_x = diff;
			    if (origin_x < 0)
				    origin_x = 0;
			    realize();
			    continue;				break;
			case 'h':
			    if (origin_x <= 0)  {
				    beep();			break;
			    }
			    origin_x -= window_w / 4;
			    if (origin_x < 0)
				    origin_x = 0;
			    realize();
			    continue;				break;
			case 'q':
			case '\003':		/*  control-C  */
			case '\004':		/*  control-D  */
						close_window();	break;
			default:		beep();		break;
		    }
		    break;
		default:
		    break;
	    }
	}
}

realize()
{
	realize_part(origin_x, origin_y, xsh.width, xsh.height, 0, 0);
}

realize_part( src_x, src_y, width, height, dest_x, dest_y )
	int	src_x, src_y, width, height, dest_x, dest_y;
{
	XDefineCursor(display, window, watch_cur);
	XCopyArea(display, pixmap, window, gc,
		  src_x, src_y, width, height, dest_x, dest_y);
	XUndefineCursor(display, window);
}

set_all()
{
	XCopyArea(display, window, pixmap, pixmapGC,
		  0, 0, xsh.width, xsh.height, 0, 0);
}	
	
beep()
{
	XBell(display, 100);
}

get_window_size()
{
	XGetWindowAttributes(display, window, &wa);
	window_x = wa.x;
	window_y = wa.y;
	window_w = wa.width;
	window_h = wa.height;
}

unsigned long	get_color_pix( color_name )
	char	*color_name;
{
	Colormap        cmap;
	XColor          color, exact;
	
	cmap = DefaultColormap(display, screen);
	XAllocNamedColor(display, cmap, color_name, &color, &exact);
	
	return (color.pixel);
}
