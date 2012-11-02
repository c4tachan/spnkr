/*
 * Each included algorithm taken nearly verbatim
 * from the book "Numerical Recipes in C" by
 * William H. Press, Brian P. Flannery,
 * Saul A. Teukolsky, and
 * William T. Vetterlin
 */


#define ITMAX 100
#define EPS 3.0e-7

#include"headers.h"

/* Numerical Recipes Error handling function */
int nrerror(char *text)
{
	void exit();
	
	fprintf(stderr, "Numerical Recipes run-time error. . .\n");
	fprintf(stderr, "%s\n", text);
	fprintf(stderr, ". . . now exiting to system. . .\n");
	
	exit(1);
}

/* Frees a vector pointed to by v */
int free_vector(float *v, int nl, int nh)
{
/*	fprintf(stderr, "free_vector()\n");*/
	free((char *) (v+nl));
	return 0;
}


/* allocates a float vector from nl to nh */
float *vector(int nl, int nh)
{
/*	fprintf(stderr, "*vector()\n");*/
	float *v;
	v = (float *) malloc((unsigned) (nh - nl +1)*sizeof(float));
	if(!v)
		nrerror("allocation failure in vector()");
	return v-nl;
}

/* The logarithm of the gamma function */
float gammln(float xx)
{
	double x, tmp, ser;
	static double cof[6] = {76.18009173, -86.50532033, 24.01409822,
				-1.231739516, 0.120858003e-2, -0.536382e-5};
	int j;
	
/*	fprintf(stderr, "gammln()\n");*/
	
	x = xx -1.0;
	tmp = x + 5.5;
	tmp -= (x+0.5)*log(tmp);
	ser = 1.0;
	for(j=0;j<=5;j++)
	{
		x += 1.0;
		ser += cof[j]/x;
	}
	return -tmp+log(2.50662827465*ser);
}



/* continued fraction evaluation for incomplete beta function */
float betacf(float a, float b, float x)
{
	float qap, qam, qab, em, tem, d;
	float bz, bp, bpp;
	float ap, app, aold;
	int m;
	
	float bm = 1.0;
	float az = 1.0;
	float am = 1.0;
	
/*	fprintf(stderr, "betacf()\n");*/
	
	qab = a+b;								/* these q's will be used in factors		*/
	qap = a + 1.0;								/* which occur in the coefficients (6.3.6)	*/
	qam = a - 1.0;
	
	bz = 1.0 -qab*x/qap;
	
	for(m = 1;m<=ITMAX;m++)							/* Continued faction eval. by recurance method	*/
	{
		em = (float) m;
		tem = em + em;
		d = em * (b-em)*x/((qam+tem)*(a+tem));
		ap = az+d*am;							/* one step (the even one) of the recur. meth	*/
		bp = bz+d*bm;
		
		d = -(a+em)*(qab+em)*x/((qap+tem)*(a+tem));
		app = ap+d*az;							/* Next step of the recurrence method		*/
		bpp = bp+d*bz;
		
		aold = az;							/* save the old answer				*/
		
		am = ap/bpp;							/* renormalize to prevent overflows		*/
		bm = bp/bpp;
		az = app/bpp;
		bz = 1.0;
		
		if(fabs(az-aold) < (EPS*fabs(az)))				/* are we done yet?				*/
			return az;
	}
	nrerror("a or b too big, or ITMAX too small in BETACF");
}

/* returns the incomplete beta function */
float betai(float a, float b, float x)
{
	float bt;

/*	fprintf(stderr, "betai()\n");*/

	if(x < 0.0 || x > 1.0)
		nrerror("Bad x in routine BETAI()");
	if(x == 0.0 || x == 1.0)
		bt = 0.0;
	else									/* Factors in front of the continued fraction	*/
		bt = exp(gammln(a+b)-gammln(a)-gammln(b)+a*log(x)+b*log(1.0-x));

	if(x < (a+1.0)/(a+b+2.0))						/* use continued fraction directly		*/
		return bt*betacf(a,b,x)/a;
	else									/* use continued fraction after making the 	*/
		return 1.0-bt*betacf(b,a,1.0-x)/b;				/* symmetry transformation			*/
}

/* complimentary error function	*/
float erfcc(float x)
{
	float t,z,ans;
/*	fprintf(stderr, "erfcc()\n");*/
	z = fabs(x);
	t = 1.0/(1.0 + 0.5 * z);
	
	ans = t * exp(-z*z-1.26551223+t*(1.00002368+t*(0.37409196+t*(0.09678418+t*(-0.18628806+t*(0.27886807+t*(-1.13520398+t*(1.48851587+t*(-0.82215223+t*0.17087277)))))))));
	return x >= 0.0 ? ans : 2.0-ans;
}















	
	
	
	
