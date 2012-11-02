/*
 *	Stellar Sorting Program
 *	
 *	Written By: Patrick Tkacik
 *	Includes Code by: Eric Bubar
 *
 *	Sorts a hand written Lines List
 *	and removes all lines not common
 *	to both the stellar and solar 
 *	lines list files.
 *
 *	Follows this by running MOOG to
 *	determine the abundances of matter
 *	in the star for use in determining
 *	the properties of the star.
 */

#include "headers.h"

int main()
{
	char starlist[99];
	char starlist2[99];
	int fe1, fe2;
	configurator();

/* Get the Fe I list from the user and open it and the corresponding solar list */
	fprintf(stderr, "Where is the Fe-I lines list located?\n");
	scanf("%s", starlist);
	FILE *stellar = fopen(starlist, "r");
	FILE *solar = fopen(sunlst1, "r");

	
/* Get the Fe II list from the user and open it and the solar list */	
	fprintf(stderr, "Where is the Fe-II lines list located?\n");
	scanf("%s", starlist2);
	FILE *stellar2 = fopen(starlist2, "r");
	FILE *solar2 = fopen(sunlst2, "r");

	
	fprintf(stderr, "Thank you!\n");
	
	fe1 = sortlists(stellar, solar, 0);
	fclose(stellar);
	fclose(solar);
	
	fprintf(stderr, "Fe1 Lists are sorted. . . ");
	
	fe2 = sortlists(stellar2, solar2, 1);
	fclose(stellar2);
/*	fclose(solar2);*/
	
	fprintf(stderr, "Fe2 Lists are sorted. . . Continuing!\n");
	
	fprintf(stderr, "READY FOR PARAMS FUNCTION!\n");
	
	prelim(fe1, fe2);
	return 0;
}
