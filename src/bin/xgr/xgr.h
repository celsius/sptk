/*
 *	Headder for xgr
 */


/*  for SUN  */

#define	DEFAULT_SHRINK	(254.0/75.0)
#define X0	558		/*  max -> 530	*/
#define Y0	1		/*  max ->  23	*/
#define	X0_LAND	300

#if	NeXT
#define	DEFAULT_SHRINK	3.6
#define X0	561
#define Y0	1

#elif	DGAV
#define	DEFAULT_SHRINK	3.0	/*  max -> 2.9  */
#define X0	610		/*  max -> 590  */
#define Y0	1
#endif

#define XLENG	1995	/*  A4 Width  [x0.1 mm](1950,max=2095)  */
#define YLENG	2870	/*  A4 Height [x0.1 mm](2850,max=2970)  */
#define XLENG_LAND	2870
#define YLENG_LAND	1995
