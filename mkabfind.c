/* abfind.par has the format: 

abfind
standard_out  '/%directory path%/test_std_solar_fe2.out'
summary_out   '/%directory path%/test_sum_solar_fe2.out'
model_in      '/%directory path%/solar.atm'
lines_in      '/%directory path%/abfind_solar.lst'
terminal      'x11'
atmosphere    1
molecules     2
lines         3
flux/int      0
damping       0
units         0
obspectrum    0  
plot          0
strong        0

*/

#include "headers.h"


int mkabfind(float temp, float logg, float p, 
	     float mturb, int num, char path[99], int pos)
{
	FILE *abfile;
	
	switch (num)
	{
		case 0:
			abfile = fopen("star.par", "w");
		break;
		case 1:
			abfile = fopen("sun.par", "w");
		break;
		default:
			fprintf(stderr, "Some how, a value that does not fit was sent to mkabfind. . . \n");
	}
			
	fprintf(abfile, "abfind\n");
	
/* begin printing the abfind.par file %st%4.0f_g%3.2f_p%3.2f_vt%3.2f*/
	switch (num)
	{
		case 0:
		   fprintf(abfile,
		   	"standard_out   '%stest_std_stellar.out'\n", path);
		   fprintf(abfile,
		   	"summary_out    '%stest_sum_stellar.out'\n", path);
		   fprintf(abfile,"model_in       'star1.atm'\n");
		   fprintf(abfile, "lines_in       '%sStellar.lst'\n", path);
		break;
		/*%st%4.0f_g%3.2f_m%3.2f_vt%3.2f*/
		case 1:
		   fprintf(abfile,
		   	"standard_out   '%stest_std_solar.out'\n", path);
		   fprintf(abfile,
		   	"summary_out    '%stest_sum_solar.out'\n", path);
		   fprintf(abfile,
		   	"model_in       '%s'\n",sunmdl);
		   fprintf(abfile,
		   	"lines_in       '%sSolar.lst'\n", path);
		break;
		default:
		   fprintf(stderr,
			"Some how, a value that does not fit was sent to mkabfind. . . \n");
	}	
	fprintf(abfile, "terminal       'x11'\natmosphere       1\nmolecules    2\n");
	fprintf(abfile, "lines          3\nflux/int      0\n");
	fprintf(abfile, "damping                0\nunits                0\nobspectrum   0\n");
	fprintf(abfile, "plot           0\nstrong               0");
/* this should conclude writing the abfind.par file */
	fclose(abfile);
	return 1;
}


/* RUNMOOG SILENTLY!!! */
//int runmoogsilent(char trgt[12])
//{
//	int n;
//	char *trupth = getenv("PATH");

//	system("mv %s batch.par", trgt);

//	system("moogsilent");
//	return SUCCESS;
//}


/* RUNMOOG !!! */
/*int runmoog(char trgt[12])
{
	int n;
	FILE *mgpp;
	char mcmd[256];
	char *trupth;
	
	trupth = getenv("PATH");
	
	setenv("PATH","/opt/astro/",1);
	sprintf(mcmd, "%s >%smoog.out 2>%serr.out",moogpath,logpath,logpath);
*/	
/* Since we have the program working with an input textfile, let's try and use a pipe instead */
/*	mgpp = popen(mcmd, "w");
	fprintf(mgpp,"%s\n", trgt);
	for(n=100; n > 0; n--)
		fprintf(mgpp, "y\n");
	pclose(mgpp);	
	setenv("PATH",trupth,1);
	return 1;
}
*/











