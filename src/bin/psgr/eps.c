/*
  ----------------------------------------------------------------
	Speech Signal Processing Toolkit (SPTK): version 3.0
			 SPTK Working Group

		   Department of Computer Science
		   Nagoya Institute of Technology
				and
    Interdisciplinary Graduate School of Science and Engineering
		   Tokyo Institute of Technology
		      Copyright (c) 1984-2000
			All Rights Reserved.

  Permission is hereby granted, free of charge, to use and
  distribute this software and its documentation without
  restriction, including without limitation the rights to use,
  copy, modify, merge, publish, distribute, sublicense, and/or
  sell copies of this work, and to permit persons to whom this
  work is furnished to do so, subject to the following conditions:

    1. The code must retain the above copyright notice, this list
       of conditions and the following disclaimer.

    2. Any modifications must be clearly marked as such.

  NAGOYA INSTITUTE OF TECHNOLOGY, TOKYO INSITITUTE OF TECHNOLOGY,
  SPTK WORKING GROUP, AND THE CONTRIBUTORS TO THIS WORK DISCLAIM
  ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING ALL
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT
  SHALL NAGOYA INSTITUTE OF TECHNOLOGY, TOKYO INSITITUTE OF
  TECHNOLOGY, SPTK WORKING GROUP, NOR THE CONTRIBUTORS BE LIABLE
  FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY
  DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
  WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS
  ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
  PERFORMANCE OF THIS SOFTWARE.
 ----------------------------------------------------------------
*/

/********************************************************
*	Setup Commands for EPSF				*
********************************************************/
#include	<stdio.h>
#include        <stdlib.h>
#include	<time.h>
#include	<ctype.h>
#include	"psgr.h"

#if	HAS_STDLIB
#include	<strings.h>
#include	<stdlib.h>
#else
#include	<string.h>
#endif

#define	plot_min(x, min, pmin)	((x<min) ? ((x>=pmin)?(x):(pmin)) : (min))
#define	plot_max(x, max, pmax)	((x>max) ? ((x<=pmax)?(x):(pmax)) : (max))
								     

float		loffset;

void epsf_setup(FILE *fp, float shrink, int xoffset, int yoffset, struct bbmargin bbm, int ncopy )
{
	int	xmin, ymin, xmax, ymax;

	if ( !psmode )
		bbox(fp, &xmin, &ymin, &xmax, &ymax,
						shrink, xoffset, yoffset, bbm);
	epsf_init(&xmin, &ymin, &xmax, &ymax, ncopy);
	epsf_scale(shrink, xoffset, yoffset);
}	

void epsf_init(int *xmin, int *ymin, int *xmax, int *ymax, int ncopy )
{
	char	*user_name;
	char	*creation_date[32];
	long	t;

	user_name = (char *)getenv("USER");
	t = time(NULL);
	strcpy((char *)creation_date, asctime(localtime(&t)));

	if ( psmode )
		printf("%%!PS-Adobe-2.0\n");
	else
		printf("%%!PS-Adobe-2.0 EPSF-1.2\n");

	if ( title != NULL )
		printf("%%%%Title: %s\n", title);
	else if ( filename != NULL )
		printf("%%%%Title: %s\n", filename);
	else
		printf("%%%%Title: Figure(s) by %s(fig/fdrw).\n", progname);

/*	printf("%%%%Creator: %s (Version %s, %s)\n", progname, Version, Date);
*/
	printf("%%%%For: %s\n", user_name);
	printf("%%%%CreationDate: %s", creation_date);

	if ( !psmode )
		printf("%%%%BoundingBox: %d %d %d %d\n",
						 *xmin, *ymin, *xmax, *ymax);
	printf("%%%%EndComments\n\n");

	printf("%%%%BeginProlog\n");
	dict();
	printf("%%%%EndProlog\n\n");

	printf("%%%%BeginSetup\n");
	printf("%%%%PaperSize: %s\n", media);
	printf("%%%%PageOrientation: %s\n", orientation);
	printf("%%%%Feature: *ManualFeed False\n");
	printf("%%%%Feature: *Resolution %d\n", resolution);
	if ( ncopy > 1 )
		printf("/#copies %d def\n", ncopy);
	printf("%%%%EndSetup\n\n");
	if ( psmode )
		printf("%%%%Page: 1\n");
}

void epsf_scale(float shrink,int xoffset,int yoffset )
{
	float	unit_length;

	unit_length = shrink * PU_PT;

	if ( !landscape )  {
		printf("%d %d translate\n", norm(xoffset*PU_PT+SHIFT),
					    norm(yoffset*PU_PT+SHIFT));
		printf("%f %f scale\n", unit_length, unit_length);
	}
	else  {
		printf("%d %d translate\n", norm(xoffset*PU_PT+SHIFT),
					    norm(yoffset*PU_PT+SHIFT));
		printf("%f %f scale\n", unit_length, unit_length);
		loffset = 4 * SHIFT / unit_length;
		printf("%d 0 translate\n", norm(yleng*PU_PT+loffset));
/*		printf("%d 0 translate\n", norm((yleng+LAND_OFFSET)*PU_PT));
*/		printf("90 rotate\n");
	}		
}

void epsf_end()
{
	if ( clip_mode )
		printf("GR\n");
	printf("%%Trailer\n");
	if ( psmode )
		printf("%%%%Pages: 1\n");
	printf("showpage\n");
	printf("%%%%EOF\n");
}

void bbox(FILE *fp, int *xmin, int *ymin, int *xmax, int *ymax, float shrink, int xoffset, int yoffset, struct bbmargin bbm )
{
	register char	c;
	int		n, x, y;
	int		plot_xmin, plot_ymin, plot_xmax, plot_ymax;
	double		unit_length;
	int		rotate = 0;
	int		ch = 30, cw = 30;
	float		mag = 0.875;		/*  7/8  */

	*xmin = *ymin = 9999;
	*xmax = *ymax = 0;

	plot_xmin = plot_ymin = 0;
	plot_xmax = 4000;
	plot_ymax = 2850;

	while ( (c=getc(fp)) != (char)EOF )  {
		switch (c)  {
		    case 'M':	fscanf(fp, "%d %d", &x, &y);
				*xmin = plot_min(x, *xmin, plot_xmin);
				*xmax = plot_max(x, *xmax, plot_xmax);
				*ymin = plot_min(y, *ymin, plot_ymin);
				*ymax = plot_max(y, *ymax, plot_ymax);	break;
		    case 'D':	while (getd(fp, &x, &y))  {
					*xmin = plot_min(x, *xmin, plot_xmin);
					*xmax = plot_max(x, *xmax, plot_xmax);
					*ymin = plot_min(y, *ymin, plot_ymin);
					*ymax = plot_max(y, *ymax, plot_ymax);
				}
									break;
		    case '%':	fscanf(fp, "%d %d %d", &n, &x, &y);
				while (getd(fp, &x, &y))  {
					*xmin = plot_min(x, *xmin, plot_xmin);
					*xmax = plot_max(x, *xmax, plot_xmax);
					*ymin = plot_min(y, *ymin, plot_ymin);
					*ymax = plot_max(y, *ymax, plot_ymax);
				}
									break;
		    case 'S':	fscanf(fp, "%d", &ch);	ch += 3;	break;
		    case 'Q':	fscanf(fp, "%d", &cw);	cw += 3;	break;
		    case 'R':	fscanf(fp, "%d", &rotate);		break;
		    case 'P':	n = getstrlength(fp);
				if ( !rotate )  {
					x += n * cw * mag;
					*xmax = plot_max(x, *xmax, plot_xmax);
					*ymin = plot_min(y, *ymin, plot_ymin);
				}
				else  {
					y += n * cw * mag;
					*ymax = plot_max(y, *ymax, plot_ymax);
					x -= ch * mag;
					*xmin = plot_min(x, *xmin, plot_xmin);
				}
				rotate = 0;				break;
		    case '\\':	fscanf(fp, "%d %d", &plot_xmin, &plot_ymin);
				if (plot_xmin < 0)	plot_xmin = 0;
				if (plot_ymin < 0)	plot_ymin = 0;
									break;
		    case 'Z':	fscanf(fp, "%d %d", &plot_xmax, &plot_ymax);
				if (plot_xmax > xleng)	plot_xmax = xleng;
				if (plot_ymax > yleng)	plot_ymax = yleng;
									break;
		    default:						break;
		}
	}

	unit_length = shrink * PU_PT;

/*	fprintf(stderr, "%d %d %d %d\n", *xmin, *ymin, *xmax, *ymax);
*/
	if ( !landscape )  {
		*xmin = norm((*xmin+xoffset-bbm.left  )*unit_length
								+ MIN_OFFSET);
		*ymin = norm((*ymin+yoffset-bbm.bottom)*unit_length
								+ MIN_OFFSET);
		*xmax = norm((*xmax+xoffset+bbm.right )*unit_length
								+ MAX_OFFSET);
		*ymax = norm((*ymax+yoffset+bbm.top   )*unit_length
								+ MAX_OFFSET);
	}
	else  {
		x = *xmin;
		y = *xmax;

		loffset += yoffset;

		*xmin = norm((yleng*unit_length
				- *ymax+loffset-bbm.bottom)*unit_length
								+ MIN_OFFSET);
		*xmax = norm((yleng*unit_length
				- *ymin+loffset+bbm.top)*unit_length
								+ MAX_OFFSET);
		loffset = (loffset - yoffset)/4 + xoffset;
		*ymin = norm((x+loffset-bbm.left )*unit_length + MIN_OFFSET);
		*ymax = norm((y+loffset+bbm.right)*unit_length + MAX_OFFSET);
	}
/*	fprintf(stderr, "%d %d %d %d\n", *xmin, *ymin, *xmax, *ymax);
*/
	rewind(fp);
}

int getd( FILE *fp, int *x, int *y )
{
	static int	c;

	while ( (c=getc(fp)) == '\n' || c == ' ' )
		;

	if ( isdigit(c) )  {
		ungetc(c, fp);
		fscanf(fp, "%d %d", x, y);
		return(1);
	}
	else
		return(0);
}

int getstrlength(FILE *fp )
{
	register int	n;
	int		c;

	n = 0;
	while ( isprint(c=getc(fp)) || (c&0xff) > 0 )
		n++;

	return (n);
}
