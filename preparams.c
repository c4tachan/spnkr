/* preparams.c
 *
 * Michael Patrick Tkacik
 * March 23, 2010
 *
 * This file contains functions that run before the params function, 
 * to prepare the solar lines, and to create the sm command strings.
 */

#include "headers.h"

int makeCMDS(char *mgbnds, int a, int b)
{
	sprintf(mgbnds, "define start1 %i define end1 %i define start2 %i define end2 %i",
		7, (6 + a), (14 + a), (13 + a + b));
	return 0;
}


int prelim(int nfe1, int nfe2)
{
	float prm[6];
	char mgcmds[64], yn[1];
	FILE *plts = popen("sm >sm.out","w");
	FILE *record = fopen("record.txt", "w");
	
	prm[4] = (float) nfe1;
	prm[5] = (float) nfe2;
	
	makeCMDS(mgcmds, nfe1, nfe2);
	
	fprintf(stderr, "MOOGIN' the Sun. . . ");
	
	system("cp sun.par batch.par");
	system("moogsilent >moog.out");

	system("cat test_sum_solar.out /etc/newline.txt >test_sum_sun.out");
	
	fprintf(stderr, "COMPLETE!\n");
	
	system("cp star.par batch.par");

	fprintf(record, "#\ttemp\tlogg\t m\tmtvel\t\tEPcorr\tFeI - FeII\tRWCorr\t\tFeI stddev\tFeII stddev\n");
	fflush(record);
	
	prm[0] = 5000;
	prm[1] = 4.5;
	prm[2] = 0;
	prm[3] = 1.5;
	
	do
	{
		params(plts, record, mgcmds, prm, 50);
	
		fprintf(stdout, "Have you finished testing parameters? [y/n] ");
		fscanf(stdin, " %c", yn);
	}while('y' != *yn);
	
	fclose(record);
	
	fprintf(stderr, "Final Parameters Listing\n");
	fprintf(stderr, "Temperature		= %f\n", prm[0]);
	fprintf(stderr, "Surface Gravity		= %f\n", prm[1]);
	fprintf(stderr, "Metalicity		= %f\n", prm[2]);
	fprintf(stderr, "Micro-Turbulence	= %f\n", prm[3]);
	
	return SUCCESS;
}
