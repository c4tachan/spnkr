/*
 * Each included algorithm taken nearly verbatim
 * from the book "Numerical Methods" by
 * William H. Press, Brian P. Flannery,
 * Saul A. Teukolsky, and
 * William T. Vetterlin
 */
#include"headers.h"
static float sqrarg;
#define SQR(a) (sqrarg=(a), sqrarg*sqrarg)

float *vector(int, int);

/*
 * Uses the Heapsort Agorithm to sort ra into ascending order,
 * and makes changes to rb accordingly.
 */

int sort2(int n, float *ra, float *rb)
{
	int l,j,ir,i;
	float rrb,rra;
	
	l = (n >> 1) + 1;
	ir = n;
	fprintf(stderr, "sort2()\n");
	for(;;)
	{
		if(l > 1)
		{
			rra = ra[--l];
			rrb = rb[l];
		}
		else
		{
			rra = ra[ir];
			rrb = rb[ir];
			ra[ir] = ra[1];
			rb[ir] = rb[1];
			
			if(--ir == 1)
			{
				ra[1] = rra;
				rb[1] = rrb;
				return 0;
			}
		}
		i = l;
		j = l << 1;
		
		while(j <= ir)
		{
			if(j < ir && ra[j] < ra[j+1])
				j++;
			if(rra < ra[j])
			{
				ra[i] = ra[j];
				rb[i] = rb[j];
				j += (i = j);
			}
			else j = ir + 1;
		}
		ra[i] = rra;
		rb[i] = rrb;
	}
}


/*
 * Given a sorted array, w[1. . . n], replaces elements with their rank,
 * including midranking of ties, and returns as s, the sum of f^3-f, where
 * f is the number of elements in each tie.
 */
int crank(int n, float *w, float *s)
{
	int j = 1;
	int ji, jt;
	float t, rank;
	
	fprintf(stderr, "crank()\n");
	
	*s = 0.0;
	
	while(j < n)
	{
/*		fprintf(stderr, "%i\t", j);*/
/*		fprintf(stderr, "%f\t%f\n", w[j], w[j+1]);*/
		
		if(w[j+1] != w[j])						/* Not a tie				*/
		{
			w[j] = j;
			j++;
		}
		else
		{								/* A tie				*/
			for(jt=j+1;jt<=n;jt++)					/* How far does it go?			*/
				if(w[jt] != w[j]) break;
			rank = 0.5*(j+jt-1);					/* this is the mean rank of the tie,	*/
			for(ji=j;ji<=(jt-1);ji++)				/* so enter it into all the tied entries*/
			{
/*				fprintf(stderr, "%f\t%f\n", w[ji], w[ji+1]);*/
				w[ji] = rank;
			}
			t = jt - j;
			*s += t*t*t-t;						/* and update s.			*/
			j = jt;
		}
	}
	
		if(j == n)	
			w[n] = n;						/* if the last element is not tied,	*/
										/* than this is its rank.		*/
	return 0;	
}	


/*
 * Given 2 data arrays, a and b, this routine returns their sum squared
 * difference of ranks d, the number of standard deviations by which d 
 * deviates from it's null-hypothesis expected value as zd, and the two 
 * sided significance level of its deviation from zero as probrs. The
 * external routines crank and sort2 are used. A small value of either
 * probd, or probrs indicates a significant correlation (rs positive)
 * or anticorrelation (rs negative)
 */
int spear(float *data1, float *data2, int n, float *d, float *zd, float *probd, 
		float *rs, float *probrs)
{
	/* Note that arrays a and b are offset arrays with range [1. . . n] */
	int j;
	float vard, t, sg, sf, fac, en3n, en, df, aved, *wksp1, *wksp2;
	
	fprintf(stderr, "spear()\n");
	
	wksp1 = vector(1, n);
	wksp2 = vector(1, n);
	
	for(j = 1; j <= n; j++)
	{
		wksp1[j] = data1[j];
		wksp2[j] = data2[j];
	}
	
	sort2(n, wksp1, wksp2);
	crank(n, wksp1, &sf);

	sort2(n, wksp2, wksp1);							/* Sort each of the data arrays		*/
	crank(n, wksp2, &sg);							/* and convert the entries to ranks	*/
	
	*d = 0.0;
	
	for(j=1; j<=n; j++)
		*d += SQR(wksp1[j] - wksp2[j]);
	
	en = n;
	
	en3n = en*en*en-en;
	
	aved = en3n/6.0 - (sf+sg)/12.0;						/* Expectation value of D		*/
	fac = (1.0-sf/en3n) * (1.0-sg/en3n);
	vard = ((en-1.0)*en*en*SQR(en+1.0)/36.0)*fac;				/* and variance of D give		*/
	*zd = (*d-aved)/sqrt(vard);						/* number of standard deviations	*/
	*probd = erfcc(fabs(*zd)/1.4142136);					/* and significance.			*/

	*rs = (1.0-(6.0/en3n)*(*d+0.5*(sf+sg)))/fac;				/* Rank correlation coefficient		*/
	t = (*rs)*sqrt((en-2.0)/((*rs+1.0)*(1.0-(*rs))));			/* and its t value			*/
	
	df = en - 2.0;
	
	*probrs = betai(0.5*df, 0.5, df/(df+t*t));				/* give its significance		*/
	
	free_vector(wksp2, 1, n);
	free_vector(wksp1, 1, n);

	return 0;
}
