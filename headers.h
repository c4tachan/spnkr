/*
 *	astro.c headers file
 *
 *	Written by: Patrick Tkacik
 */

#ifndef ASTRO
#define ASTRO 1

#define SUCCESS  1
#define FAILURE -1

/*
 * Includes for the program
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>
/*
 * Homebrews
 */
#include "structsnvars.h"
/*
 * Function Declarations
 */
int configurator();
int makemodel(float,float,float,float);
int mkabfind(float,float,float,float,int,char *,int);
int runmoog(char *);
int sortlists(FILE *,FILE *,int);
int eatcomment(FILE *,int);

#endif /* ASTRO */
