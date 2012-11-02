/*
 * structsnvars.h
 *
 * This file contains the universal variables for file paths
 * and parameter limits as well as
 * the structs that are used in this program.
 *
 * Written by: M. Patrick Tkacik
 * May 27, 2009
 */
 
/*
 * Universal file Path Strings
 */

/* logfile location path strings */
extern char *logpath;

/* external program paths */
extern char *moogpath, *smpath, *mspwnpath, *mgpltspath;

/* solar file paths */
extern char *sunlst1, *sunlst2, *sunmdl;

/* limits from config.txt */
extern float mlht, mllt, mlhg, mllg, mlhm, mllm, mlhu, mllu;
 
/*
 * below this line are structs used for the program
 */
 
typedef struct linedata
{
	float wavelength;	/* From both handmade and MOOG lists	*/
	float eqw;		/* From both				*/
	float logGF;		/* From both				*/
	float ep;		/* From both				*/
	float constant;		/* Handmade list only			*/
	float species;		/* Handmade list only			*/
	float logRW;		/* MOOG list only			*/
	float abund;		/* MOOG list only			*/
	float delavg;		/* MOOG list only			*/
	struct linedata *next;	/* Deprecated pointers			*/
	struct linedata *last;	/* for use in a linked list		*/
}linedata;
