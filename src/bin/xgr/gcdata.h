/*
 *	Hedder for setup plotting parts
 */

#include	"config.h"

#ifndef	X11R3
#define	SIZE	512		/*  buffer size		*/
#else
#define	SIZE	2		/*  buffer size for X11R3's BUG  */
#endif
#define	FCW	22
#define	FCH	25
#define	LFCH	50		/*  Large font height  */

/*
 *	Color name data
 */
static char	*c_name[] = { "black", "black",
			      "blue",   "red", "medium sea green",
			      "violet", "orange", "turquoise"  };
/*
 *	Fonts name
 */
#define	FSymbol	9
static char	*f_name[] = {
	"-adobe-courier-medium-r-normal-*-*-100-*-*-m-*-iso8859-1",
	"-adobe-courier-medium-r-normal-*-*-120-*-*-m-*-iso8859-1",
	"-adobe-courier-medium-r-normal-*-*-140-*-*-m-*-iso8859-1",
	"-adobe-courier-medium-r-normal-*-*-160-*-*-m-*-iso8859-1",
	"-adobe-courier-bold-r-normal-*-*-100-*-*-m-*-iso8859-1",
	"-adobe-courier-bold-r-normal-*-*-120-*-*-m-*-iso8859-1",
	"-adobe-courier-bold-r-normal-*-*-140-*-*-m-*-iso8859-1",
	"-adobe-courier-bold-r-normal-*-*-160-*-*-m-*-iso8859-1",
	"-adobe-symbol-medium-r-normal-*-*-80-*-*-p-*-adobe-fontspecific",
	"-adobe-symbol-medium-r-normal-*-*-100-*-*-p-*-adobe-fontspecific",
	"-adobe-symbol-medium-r-normal-*-*-120-*-*-p-*-adobe-fontspecific",
	"-adobe-symbol-medium-r-normal-*-*-140-*-*-p-*-adobe-fontspecific",
	"-adobe-symbol-medium-r-normal-*-*-160-*-*-p-*-adobe-fontspecific"
};

/*
 *	Line width data
 */
static int	l_width[] = {0, 1, 1, 2, 3, 4, 1, 2, 3, 4};

/*
 *	Line style data
 */
static struct	line_style_info  {
	char	list[8];
	int	no;
}  l_style[] = {
/**/		{ { 1, 1},		2 },	/** Dummy for Solid    **/
/*L6*/		{ { 1, 5},		2 },	/*  dotted2		*/
/*L2*/		{ { 1, 3},		2 },	/*  dotted		*/
/*L4*/		{ { 4, 2},		2 },	/*  dashed		*/
/*L5*/		{ { 7, 3},		2 },	/*  long_dashed		*/
/**/		{ { 1, 1},		2 },	/** Dummy for Solid    **/
/*L3*/		{ { 8, 2, 2, 2},	4 },	/*  dot_dashed		*/
		{ {10, 2, 2, 2},	4 },	/*  dot_dashed2		*/
		{ { 8, 2, 1, 2, 1, 2},	6 },	/*  double_dot_dashed	*/
		{ { 6, 2, 2, 2, 2, 2},	6 }	/*  double_dot_dashed2	*/
	};

/*
 *	Mark bitmap data
 */
#define	mark_width	9
#define	mark_height	9

#include	"marks/mark0.dat"
#include	"marks/mark1.dat"
#include	"marks/mark2.dat"
#include	"marks/mark3.dat"
#include	"marks/mark4.dat"
#include	"marks/mark5.dat"
#include	"marks/mark6.dat"
#include	"marks/mark7.dat"
#include	"marks/mark8.dat"
#include	"marks/mark9.dat"
#include	"marks/mark10.dat"
#include	"marks/mark11.dat"
#include	"marks/mark12.dat"
#include	"marks/mark13.dat"
#include	"marks/mark14.dat"
#include	"marks/mark15.dat"

static char	*mark_bits[] = {
			mark0_bits,  mark1_bits,  mark2_bits,  mark3_bits,
			mark4_bits,  mark5_bits,  mark6_bits,  mark7_bits,
			mark8_bits,  mark9_bits,  mark10_bits, mark11_bits,
			mark12_bits, mark13_bits, mark14_bits, mark15_bits
		};

/*
 *	Till Pattern
 */
#define	till_width	4
#define	till_height	4

#include	"tills/till0.dat"
#include	"tills/till1.dat"
#include	"tills/till2.dat"
#include	"tills/till3.dat"
#include	"tills/till4.dat"
#include	"tills/till5.dat"
#include	"tills/till6.dat"
#include	"tills/till7.dat"
#include	"tills/till8.dat"
#include	"tills/till9.dat"
#include	"tills/till10.dat"
#include	"tills/till11.dat"
#include	"tills/till12.dat"
#include	"tills/till13.dat"
#include	"tills/till14.dat"
#include	"tills/till15.dat"
#include	"tills/till16.dat"

static char	*till_bits[] = {
			till0_bits,  till1_bits,  till2_bits,  till3_bits,
			till4_bits,  till5_bits,  till6_bits,  till7_bits,
			till8_bits,  till9_bits,  till10_bits, till11_bits,
			till12_bits, till13_bits, till14_bits, till15_bits,
			till16_bits
		};
