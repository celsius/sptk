/************************************************************************

  $Id: _reverse.c,v 1.1 2000/03/01 13:58:49 yossie Exp $

  Reverse the order of data in sequence
      reverse(real* p, int l)

          double* p	: data seqence (Overwrite)
          int   l	: length of sequence

************************************************************************/

void reverse(x, l)
double* x;
int l;
{
	int i;
	double d;
	i=0;
	while (i < l-i-1) {
		d = x[i]; x[i]=x[l-i-1]; x[l-i-1]=d;
		i++;
	}
}
