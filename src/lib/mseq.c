/****************************************************************
    $Id: mseq.c,v 1.1 2000/03/01 13:58:56 yossie Exp $

    M-Sequence Generator 
               using X**31 + X**28 + 1

	int	mseq()

	return value : 	M-Sequence

****************************************************************/

#define	B0	0x00000001
#define	B28	0x10000000
#define	B31	0x80000000
#define	B31_	0x7fffffff
#define	Z	0x00000000

int mseq()
{
    static int    x = 0x55555555;
    register int  x0, x28;

    x >>= 1;

    if (x & B0)
	x0 = 1;
    else
	x0 = -1;

    if (x & B28)
	x28 = 1;
    else
	x28 = -1;

    if (x0 + x28)
	x &= B31_;
    else
	x |= B31;

    return(x0);
}
