/* paramsR.c
 *
 * Michael Patrick Tkacik
 * March 23, 2010
 *
 * The params function contained within this file
 * is a rewrite of the origional params function
 * in an attempt to stream line the code that I
 * have written so that it may be more readable
 * both to me and to any one else who might read it.
 *
 */

#include "headers.h"

float prnsts(float *prm, double *sts, int n, FILE *rec)
{
	system("clear");
	fprintf(stderr,"temp\tlogg\t m\tmtvel\t\t");
	fprintf(stderr,"EPcorr\tFeI - FeII\tRWCorr\t\tFeI stddev\tFeII stddev\t\t%i\n", n);
	
	fprintf(stderr, "%.0f\t%.2f\t%.2f\t%.2f", prm[0], prm[1], prm[2], prm[3]);
					
	fprintf(stderr, "\t\t% .3lf\t% .3lf\t\t% .3lf\t\t% .3lf (%i)\t% .3lf (%i)\n",
		sts[0], (sts[2] - sts[3]), sts[1], sts[4], (int) prm[4], sts[5], (int) prm[5]);
	
	
	fprintf(rec, "\t%.0f\t%.2f\t%.2f\t%.2f", prm[0], prm[1], prm[2], prm[3]);
	fprintf(rec, "\t\t% .3lf\t% .3lf\t\t% .3lf\t\t% .3lf (%i)\t% .3lf (%i)\n",
		sts[0], (sts[2] - sts[3]), sts[1], sts[4], (int) prm[4], sts[5], (int) prm[5]);
	
	fflush(rec);
	
	prm[2] = sts[2];
	
	return SUCCESS;
}

FILE *plotdata(char *mgbnds, FILE *plts)
{
//	char pltcmd[100];
/* Use SM to work with the data from MOOG */	
	fprintf(stderr, "opening ploting window\n");

	fflush(plts);
	
	fprintf(plts, " macro read \"/usr/bin/moogplots.sm\"");
//	fprintf(stderr, "\n%s", mgbnds);
//	fprintf(plts, "\necho %s", mgbnds);
	fprintf(plts, "\n%s", mgbnds);
	fprintf(plts, "\nmoogplots");
	fprintf(plts, "\necho Hi, at least this worked!\n");
	fflush(plts);

	return plts;
}

float abv(float i)
{
	if (i > 0)
		return i;
	else
		return (i * -1);
}

float partest(float *t, float *g, float *m, float *v)
{
	if (*t < 3500)
		*t = 3500;
	else if (*t > 7750)
		*t = 7750;
	if (*g < 0)
		*g = 2.5;
	else if (*g > 5)
		*g = 2.5;
	if (*m < -3.5)
		*m = 0;
	else if (*m > 0.5)
		*m = 0;
	if (*v < 0.0)
		*v = 0;
	return SUCCESS;
}


int statsdata(char *mgbnds, double *sts)
{
	int n = 0;
	FILE *stats;
	char sttscmd[200], inpt[100];
	sprintf(sttscmd, "sm %s macro read stelstats.sm stelstats quit",
			mgbnds);
	stats = popen(sttscmd, "r");	
	
	if(stats == NULL)
	{
		fprintf(stderr, "\n The statistics pipe failed to open, EXITING\n");
		exit(EXIT_SUCCESS);
	}
	
	while (6 != n)
	{
		/* Here we read the pipe, and find our statistics */
		fgets(inpt, 100, stats);
		
		n = sscanf(inpt, "%lf %lf %lf %lf %lf %lf", &sts[0], 
			&sts[1], &sts[2], &sts[3], &sts[4], &sts[5]);
		
		//fprintf(stderr, "Reading statistics output. . .%i \n", n);
	}
	pclose(stats);

	return 0;
}

int sigfigs(float x)
{
	if(x >= 0.5)
		return 50;
	else if(x >= 0.25)
		return 10;
	else if(x >= 0.15)
		return 5;
	else if(x >= 0.1)
		return 2;
	else
		return 1;
}

int shift(float *prm, int sign, int sigs, int max)
{
	float i = 0;
	int nn = 0;
	char s[4];
	switch (max)
	{
		case	1:	/* Changing Temp Values */
			i = 1;
			nn = 0;
			sprintf(s, "Temp");
		break;
		
		case	2:	/* Changing Log(g) Values */
			i = 0.01;
			nn = 1;
			sprintf(s, "LogG");
		break;
		
		case	3:	/* Changing MicroTurbulence Velocity Values */
			i = 0.01;
			nn = 3;
			
			fprintf(stderr, "\n\nnn = %d\n", nn);
			
			sprintf(s, "mtvl");
		break;
		
		default:
			i = 1;
			nn = 0;
			sprintf(s, "err ");
			fprintf(stderr, "nn = %d\n\n", nn);
	}
	
	prm[nn] = prm[nn] + (sign * sigs * i);
	
	fprintf(stderr, "max =\t\t% i\nnn =\t\t% i\nparam =\t\t %s\n", max, nn, s);
	fprintf(stderr, "sign =\t\t% i\nsigs =\t\t% i\ni =\t\t% lf\n", sign, sigs, i);
	fprintf(stderr, "shift =\t\t% .3lf\nnew =\t\t% .3lf\n", (sign * sigs * i), prm[nn]);
	
	
	
	return SUCCESS;
}

int autoshift(float *prm, double *stats)
{
	double absts[3];
	float fediff = (stats[2] - stats[3]);
	int max = 0;
	int sign = 0;
	int n = 0;
	
	absts[0] = abv(stats[0]);	//Excitation Potential Coorelation
	absts[1] = abv(fediff);		//Iron Difference
	absts[2] = abv(stats[1]);	//Reduced Width Coorelation

/* Case where EP is most coorelated */	
	if(absts[0] >= absts[1] && absts[0] >= absts[2])
		max = 1;
	
/* Case where Iron difference is largest */
	else if(absts[1] > absts[0] && absts[1] >= absts[2])
		max = 2;
		
/* Case where RW is most coorelated */	
	else if(absts[2] > absts[0] && absts[2] > absts[1])
		max = 3;
	
	
	if(absts[max] > 0)
		sign = 1;
	else
		sign = -1;	
	
		
	fprintf(stderr, "EPCorr =\t% .3lf\nfediff =\t% .3lf\nabvfe =\t\t% .3lf\nRWCorr =\t% .3lf\n",
		stats[0],fediff, absts[1], stats[1]);

	
	shift(prm, sign, sigfigs(absts[max]), max);
	
	return SUCCESS;
}

int params(FILE *plts, FILE *rec, char *mgbnds, float *prm, int n)
{
	double *stats = malloc(sizeof(double) * 6);
	
	if(n == 0)
	{
		pclose(plts);
		plts = popen("sm >sm.out", "w");
		n = 50;
	}

/* Get the parameters from the user here */
/*	fprintf(stderr,"Type in an Effective Temperature:\n");
	scanf("%f", &prm[0]);
	fprintf(stderr,"Type in a Surface Gravity:\n");
	scanf("%f", &prm[1]);
	//fprintf(stderr,"Type in a Metallicity:\n");
	//scanf("%f", &prm[2]);
	fprintf(stderr,"Type in a Microturbulence:\n");
	scanf("%f", &prm[3]);
*/	
/* Make sure the parameters are within the values mspawn can handle */
	partest(&prm[0], &prm[1], &prm[2], &prm[3]);
	
	makemodel(prm[0], prm[1], prm[2], prm[3]);
	
	fprintf(stderr, "Running Moog. . . ");
	
	system("moogsilent >moog.out");
	
	system("cat test_sum_stellar.out /etc/newline.txt >test_sum_star.out");

	fprintf(stderr, "COMPLETE!\n");

	plts = plotdata(mgbnds, plts);

	statsdata(mgbnds, stats);

//	fprintf(stderr, "\n\nSUCCESSFUL Read of Statistics!\n\n");

	prnsts(prm, stats, n, rec);
	
	if (0.005 > abv(stats[0]) && 0.005 > abv(stats[1]) && 0.005 > (abv(stats[2] - stats[3])))
	{
		return SUCCESS;
	}
	
	else
	{
		autoshift(prm, stats);
		return params(plts, rec, mgbnds, prm, n-1);
	}
}
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
