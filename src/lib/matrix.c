/*****************************************************************
	compute matrix functions

	multiple matrix
		multim(x, xx, xy, y, yx, yy, a)
		
		double	x[]	: matrix1
		int	xx	: row number of matrix1
		int	xy	: line number of matrix1
		double  y[]	: matrix2
		int	yx	: row number of matrix2
		int	yy	: line number of matrix2
		double  a[]	: answer
		
		if xx=1 and xy=1 ,then x[0] is regarded as scalar
		
	add matrix
		addm(x, y, xx, yy, a)
		double	x[]	: matrix1
		double  y[]	: matrix2
		int	xx	: row number 
		int	xy	: line number
		double  a[]	: answer

			Naohiro Isshiki Feb.1996
*******************************************************************/
#include<stdio.h>
#include<SPTK.h>

static double	*tmp;
static int	tmpsize = 0;

void mm(x,xx,xy,y,yx,yy,a)
double   x[],y[],a[];
int     xx,xy,yx,yy;
{	
        register int i,j,k;
	register double *wx,*wy;
	if(xx == 1 && xy == 1 ){
		for(i = yx * yy - 1; i >=0; i-- )
			a[i] = x[0]*y[i];
		return;
	}

        if (xx != yy){
                fprintf(stderr,"bad matrix size x= %d*%d,y= %d*%d\n"
		,xx,xy,yx,yy);
                exit(1);
        }

	wx = x;
        for(i=0;i<xy;i++){
                for(j=0;j<yx;j++){
			wy = &y[j];
                        *a=0;
                        for(k=0;k<xx;k++){
                                *a += *wx * *wy ;
				wx++;
				wy += yx;
			}
			wx -= xx; 
			
			a++;
                }
		wx += xx;
	}

}

void multim(x,xx,xy,y,yx,yy,a)
double   x[],y[],a[];
int     xx,xy,yx,yy;
{
        register int i;

	if (x == a){
		if ((xy > yy ? xy : yy) * yx > tmpsize){
			if(tmp != NULL)
				free(tmp);
			tmpsize = (xy > yy ? xy : yy) * yx;
			tmp = (double *)getmem(tmpsize, sizeof(*tmp));
		
		}
		mm(x,xx,xy,y,yx,yy,tmp);
		if(xx == xy)
			for(i = yx *yy -1; i >= 0 ; i-- )
				a[i] = tmp[i]; 
		else
			for(i = xy *yx -1; i >= 0 ; i-- )
				a[i] = tmp[i]; 
	}
	else{
		mm(x,xx,xy,y,yx,yy,a);
	}
				

}

void am(x,y,xx,yy,a)
double   x[],y[],a[];
int      xx,yy;
{
	register int i,j,k;


	for(i=0;i<yy;i++)
		for(j=0;j<xx;j++)
			a[j+i*xx] = x[j+i*xx] + y[j+i*xx];
}

void addm(x,y,xx,yy,a)
double   x[],y[],a[];
int      xx,yy;
{
        register int i;
	
	if (x == a){
		if (xx * yy > tmpsize){
			if(tmp != NULL)
				free(tmp);
			tmpsize = xx * yy;
			tmp = (double *)getmem(tmpsize, sizeof(*tmp));
		}
		am(x,y,xx,yy,tmp);
		for(i = xx *yy -1; i >= 0 ; i-- )
			a[i] = tmp[i]; 
	}
	else{
		am(x,y,xx,yy,a);
	}
}

