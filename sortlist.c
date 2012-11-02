/*
 *	sortlist.c
 *	By: Patrick Tkacik
 */

#include "headers.h"


int copylinedata(linedata *,linedata);
linedata *readspectre(FILE *, int *);
int heapsort(int, linedata *);


/* 
 * A simple Function to test if a list has 
 * already been sorted from smallest wavelength
 * to the largest wavelength, or just small to 
 * large in general.
 */

int is_sorted(int len, linedata *ra)
{
	int i = 0;
	while(i < (len - 1))
	{
		if(ra[i].wavelength > ra[i + 1].wavelength)
		{
			fprintf(stderr, "\tCurrent list failed sorting test at %i\n", i);
			return 1;     /* If the test for sorting fails return 1 */
		}
		i += 1;
	}
	
	fprintf(stderr, "  Current list is sorted!\n");
	return 0;
}

/*
 * Just like the name implies, this function "eats" the
 * one or more lines of coments from the begining of the 
 * lines lists.
 */

int eatcomment(FILE *in, int lines)
{
	char chr[100];

	while (lines > 0)
	{
		fgets(chr,100,in); 
		lines -= 1;
	}

	return 1;
}	

/* Write the data into the properly formatted file for MOOG to read */

int write4moog(linedata *sun, linedata *star, int len, int n)
{
	FILE *solsort, *stelsort;
	int start = 0;
	
	
	
	if (n == 0)
	{
		fprintf(stderr, "\tWritting to files. . . ");
		stelsort = fopen("Stellar.lst", "w");
		solsort = fopen("Solar.lst", "w");
	
		fprintf(solsort, "#Solar Lines from Sorting Program\n");
		fprintf(stelsort, "#Stellar Lines from Sorting Program\n");
	}
	
	else
	{
		stelsort = fopen("Stellar.lst", "a");
		solsort = fopen("Solar.lst", "a");
		fprintf(stderr, "\tAppending to list files. . . ");
	}
	
	
/*
 * Alright, let's go ahead and print the new lines list to the temp files 
 * "Stellar.lst" and "Solar.lst"
 */
	while (start < len)
	{
	
/* Printing information to the Sorted Solar List */

		fprintf(solsort, "  %4.3f", sun[start].wavelength);
		fprintf(solsort, "    %2.3f", sun[start].species);
		fprintf(solsort, "     %1.2f", sun[start].logGF);
		fprintf(solsort, "  %1.2f", sun[start].ep);
		fprintf(solsort, "         %1.1f", sun[start].constant);
		fprintf(solsort, "                %.1f\n", sun[start].eqw);
		
/* Print the information to the Sorted Stellar Lines list */
		
		fprintf(stelsort, "  %4.3f", star[start].wavelength);
		fprintf(stelsort, "    %2.3f", star[start].species);
		fprintf(stelsort, "     %1.2f", star[start].logGF);
		fprintf(stelsort, "  %1.2f", star[start].ep);
		fprintf(stelsort, "         %1.1f", star[start].constant);
		fprintf(stelsort, "                %.1f\n", star[start].eqw);
/*		fprintf(stderr, "\nstar = %i of %i", start,len);*/
		start += 1;
	}
	
	fclose(solsort);
	fclose(stelsort);
	
	fprintf(stderr, "Finished!\n");
	return 0;
}

/*
 * Test the wavelength of the solar lines against
 * the wavelength of the stellar lines, and copy 
 * the lines that have the same wavelengths into 
 * the output array of lines
 */
linedata *trimlist(linedata *sol, linedata *str, int snln, int strln)
{
	int i, ii;
	
	i = 0;
	ii = 0;

	/* out is the trimed solar lines list, since it has the same lenght	*/
	/* as the stellar lines list, we can go ahead and malloc the memory 	*/
	/* it requires right now, based on the length of the stellar list	*/
	linedata *out = malloc(strln * sizeof(linedata));

fprintf(stderr, "COPYING AND TRIMING LINES. . . ");
	
	/* a simple algorithm follows, where an index moves through each
	   list, and is used to copy lines when matches are found. 		*/
	while(i < strln && ii < snln)
	{
/*		fprintf(stderr, "\n%f\t%f\t", sol[ii].wavelength, str[i].wavelength);*/
/*		fprintf(stderr, "ii = %i\ti = %i", ii, i);*/
		if(sol[ii].wavelength == str[i].wavelength)
		{
/*			fprintf(stderr, "\tMATCH DISCOVERED!");*/
			copylinedata(&out[i], sol[ii]);
			i++;
			ii++;
		}
		else
			ii += 1;
			
			
	}
	fprintf(stderr, "COMPLETE!\n");
	return out;
}

/* Sort the lines lists for the sun and the star */
int sortlists(FILE *star, FILE *sun, int num)
{
	linedata *stel;
	linedata *sol;
	linedata *ssol;
	int lin,linr,i;

	fprintf(stderr, "Sorting the lists now!\n");

	/* Remove the comments from the beginning of the files */	
	eatcomment(star, 1);
	eatcomment(sun, 1);

	fprintf(stderr, "Comments eaten!\n");

	/* Read the data from the files into the linedata arrays */
	sol = readspectre(sun, &lin);
	
	stel = readspectre(star,&linr);
	
	
	/* Check for presorted lists, then sort if needed */
	if(is_sorted(lin, sol))
		heapsort(lin, sol-1);
	if(is_sorted(linr, stel))
		heapsort(linr, stel-1);
	
/*	for(i = 0; i < linr; i++)*/
/*	{*/
/*		fprintf(stderr,"i = %i\t", i);*/
/*		lineprint(stel[i]);*/
/*	}*/

	/* Cull solar lines that are not found in the star from the solar list	*/
	ssol = trimlist(sol, stel, lin, linr);
	

	
	/* Write the Stellar and Sollar lines to a unified lines list		*/
	/* to be read by MOOG							*/
	write4moog(ssol, stel, linr, num);
	
	return linr;
}







