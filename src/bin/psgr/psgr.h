struct bbmargin  {		/*  Bounding Box Margin  */
	int	top;
	int	bottom;
	int	left;
	int	right;
} bbm;

struct page_media  {
	char	*size;
	int	width;
	int	height;
};

#define	PU_PT		(72.0/254.0)
#define	SHIFT		15
#define	LAND_OFFSET	254
#define	SCALE		10

#define	MIN_OFFSET	12
#define	MAX_OFFSET	17
#define	CHAR_HEIGHT	10

#define	norm(x)		(int)(x)

extern char	*filename;
extern char	*title;
extern char	*progname;

extern struct page_media	paper[];
extern char	*orientations[];

extern char	*media;
extern int	xleng;
extern int	yleng;
extern int	resolution;
extern int	paper_num;
extern char	*orientation;

extern int	psmode;
extern int	landscape;
extern int	font_no;
extern int	clip_mode;
