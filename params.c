/*	
 *	Params.c
 *	
 *	The methods included in this file attempt to itterate through the 
 *	values using a bisector method algorthm in the hope that it will be
 *	possible to find the solution to the star's parameters.
 *
 */

#include "headers.h"

time_t starttime;


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
	return 1;
}

/*	
 *	** FUTURE DESCRIPTION **
 *	Params attempts to automatically locate the correct parameters for 
 *	the star's surface Gravity, Effective Temperature, Micro Turbulence
 *	and Metallicity.
 */

/* 
 * params currently handles the looping of the user trying to find the 
 * proper stellar parameters
 */
		
int params(int nfe1, int nfe2)
{	
	starttime = time(NULL);
	
	/* I know, I know, I have way to many freaking variables.
	 * I am sure there is a better way to do this, but I just don't
	 * feel like trying to figure it out right now. As a result, this
	 * method is a PRIME candidate for third party patches.
	 */
	float temp, mtvel, logg, m;
	float otemp,omtvel,ologg;
	float inep, inrw, fe1, fe2;
	float abinep, abinrw, stdfe1, stdfe2;
	int auto1 = 0;
	int auto2 = 0;
	int n = 2;
	int done1 = 0;
	int done2 = 0;
	int plt, itts, last;
	char inpt[100], yn[1];
	FILE *stats;
	FILE *plts = popen("sm","w");
	abinep = 1;
	abinrw = 1;

	m = 0.0;
	itts = 0;
 	fprintf(stderr, "MOOGIN' the Sun. . . ");
	
	runmoog("./sun.par");
	fprintf(stderr, "COMPLETE!\n");
	
	char mgbnds[64];
	char pltcmd[100];
	char sttscmd[100];
	
	sprintf(mgbnds, "define start1 %i define end1 %i define start2 %i define end2 %i",
		7, (6+nfe1), (14+nfe1), (13+nfe1+nfe2));
	
// find out if the user wants to do this automagically
	
	fprintf(stderr, "Do you want to automate this process? [Y/N] ");
	fscanf(stdin, " %c", &yn[0]);

	if ('Y' == *yn || 'y' == *yn)
	{
		auto1	=  1;
		auto2	=  0;
		plt	= -2;
	}
	
	else
	{
		auto1 = 0;
		plt = 1;
	}
	
	do
	{
	
		done2 = 0;
	
		while (done2 == 0)
		{
	/* The following bit of code is for my use only. I am trying to understand how changes are made by the user to the various values
	 * during the parameter search
	 */
	 		
/*	 		otemp = temp;*/
/*	 		omtvel = mtvel;*/
/*	 		ologg = logg;*/
	 		
	 /* END OF PARAMETER COLLECTING CODE BLOCK. WE NOW RETURN YOU TO YOUR REGULARLY SCHEDULED CODE. */		
			
			itts++;
						
			if (auto2 == 0)
			{			
				fprintf(stderr,"Type in an Effective Temperature:\n");
				scanf("%f", &temp);
				fprintf(stderr,"Type in a Surface Gravity:\n");
				scanf("%f", &logg);
				//fprintf(stderr,"Type in a Metallicity:\n");
				//scanf("%f", &m);
				fprintf(stderr,"Type in a Microturbulence:\n");
				scanf("%f", &mtvel);
				
				if (auto1 == 1)
					auto2 = 1;
			}
/*			fprintf(chngs, "%f\t%f\t%f\t%f\t%f\t%f\n",(otemp-temp),(omtvel-mtvel),(ologg-logg),inep,inrw,(fe1-fe2));*/
	/* make the model based on the input values */
			partest(&temp, &logg, &m, &mtvel);
			
			makemodel(temp, logg, m, mtvel);
	
			fprintf(stderr, "Running Moog. . . ");
			
			runmoog("./star.par");
			
			fprintf(stderr, "COMPLETE!\n");
	
			
			n = 0;
	/* Let's set the statistical values to zero */
			inep = 0;
			inrw = 0;
			fe1 = 0;
			fe2 = 0;
					
			if(plt == 1)
			{
				fprintf(stderr, "opening ploting window\n");
				sprintf(pltcmd, "sm >sm.out");
				fflush(plts);
				plts = popen(pltcmd, "w");
				fflush(plts);
				plt = 0;
			}
			else if(plt == -1)
			{				
				fprintf(plts, "QUIT\n");
				pclose(plts);
			}
			
	/* Okay, I want to display a plot of the data, so I will use this	*/
			if (plt == 0)
			{
				fprintf(plts, "macro read \"/usr/bin/moogplots.sm\"\n");
				fprintf(plts, "%s", mgbnds);
				fprintf(plts, "\nmoogplots");
				fprintf(plts, "\necho Hi, at least this worked!\n");
				fflush(plts);
			}
/*			fprintf(stderr, pltcmd);*/
			

	/* Since I have now learned how to open a process with a pipe
	 * between two programs, we open a pipe to sm below.
	 */
			
			
			sprintf(sttscmd, "sm %s macro read stelstats.sm stelstats quit",
					mgbnds);
			stats = popen(sttscmd, "r");	
			while (n != 6)
			{
				/* Here we read the pipe, and find our statistics */
				fgets(inpt, 100, stats);
				
				n = sscanf(inpt, "%f %f %f %f %f %f", &inep, 
					&inrw, &fe1, &fe2, &stdfe1, &stdfe2);
			}
			pclose(stats);
			

			
						
			abinep = abv(inep);
			system("clear");
			fprintf(stderr,"temp\tlogg\t m\tmtvel\t EPcorr\tRWCorr\tFeI - FeII\tFeI stddev\tFeII stddev\n");
			fprintf(stderr, "%.0f\t%.2f\t%.2f\t%.2f", temp, logg, m, mtvel);
			
							
			fprintf(stderr, "\t%.3f\t%.3f\t   %.3f\t%.3f (%i)\t%.3f (%i)\n",
				inep, inrw, (fe1 - fe2), stdfe1, nfe1, stdfe2, nfe2);
	
			m = fe1;
			abinep = abv(inep);
			abinrw = abv(inrw);




/*
 * Begin the block of code that will automate the process of finding the parameters
 */

			if (auto1 == 1)
			{
				if(0.005 > abinep && 0.005 > abinrw && 0.005 > (abv(fe1 - fe2)))
					done2 = 1;
				else
				{
					if(abinep > (fe1 - fe2) && last == 2)
					{
						last = 1;
						if 	(inep > 0)
							temp = temp + 1;
						else if	(inep < 0)
							temp = temp - 1;
					}
				
					else if(abinep > abinrw && last == 3)
					{
						last = 1;
						if 	(inep > 0)
							temp = temp + 1;
						else if	(inep < 0)
							temp = temp - 1;
					}
			
					else if (abinrw > (fe1 - fe2) && last == 1)
					{
						last = 2;
						if 	(inrw > 0)
							logg = logg + 0.01;
						else if (inrw < 0)
							logg = logg - 0.01;
					}
				
					else if (abinrw > abinep && last == 3)
					{
						last = 2;
						if 	(inrw > 0)
							logg = logg + 0.01;
						else if (inrw < 0)
							logg = logg - 0.01;
					}
			
					else
					{
						last = 3;
						if	((fe1 - fe2) > 0)
							mtvel = mtvel + 0.01;
						else if	((fe1 - fe2) > 0)
							mtvel = mtvel - 0.01;
					}
				}
				
				fprintf(stderr, "%f seconds elapsed\n%i itterations\n", difftime(time(NULL), starttime), itts);
			}

/* End the block of code that determines the parameters automatically */

			if(0.005 > abinep && 0.005 > abinrw && 0.005 > (abv(fe1 - fe2)))
				done2 = 1;	
		}
		
		fprintf(stdout, "Have you finished testing parameters? [y/n] ");
		fscanf(stdin, " %c", yn);
			
		if('y' == yn[0])
		{
			plt = -1;
			done1 = 1;
		}
			
	}while (done1 == 0);
/*	fclose(chngs);*/
	
	fprintf(stderr, "Final Parameters Listing\n");
	fprintf(stderr, "Temperature		= %f\n", temp);
	fprintf(stderr, "Surface Gravity		= %f\n", logg);
	fprintf(stderr, "Metalicity		= %f\n", m);
	fprintf(stderr, "Micro-Turbulence	= %f\n", mtvel);

	return 1;		
}
