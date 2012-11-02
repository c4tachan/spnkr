/*
 * Configurator.c contains code for parsing the configuration
 * file written by the user and stored in the program directory.
 * use of a configuration file will allow the user to modify 
 * crucial file paths, parameter limits, and correlation objectives.
 *
 * Written by: M. Patrick Tkacik
 * May 27, 2009
 */


/*
 * Universal variables
 */

/* logfile location path strings */
char *logpath;

/* external program paths */
char *moogpath, *smpath, *mspwnpath, *mgpltspath;

/* solar file paths */
char *sunlst1, *sunlst2, *sunmdl;

/* limits from config.txt */
float mlht, mllt, mlhg, mllg, mlhm, mllm, mlhu, mllu;

#include "headers.h"

int isblank(int);

/* zwitchers() manages the switching calls */

int zwitchers(int done, int tagnum, char line[100])
{

/*	fprintf(stderr, "In Zwitchers!\n");*/
/*"sun", "mspawn", "moog", "sm", "path",
  "Fe1", "Fe2", "limit", "list", "high",
  "low", "temp", "grav", "metalicity",
  "mturb", "prog", "macro";
 */	
	switch (tagnum + 1)
	{
		case 1:	/* sun */
			done = 2000;
			break;
		case 2: /* mspawn */
			done = 3000;
			break;
		case 3: /* moog */
			done = 4000;
			break;
		case 4: /* sm */
			done = 5000;
			break;
		case 5: /* path */
			done = done + 100;
			break;
		case 6: /* Fe1 */
			done = done + 200;
			break;
		case 7: /* Fe2 */
			done = done + 300;
			break;
		case 8: /* limit */
			done = done + 400;
			break;
		case 9: /* list */
			done = done + 10;
			break;
		case 10:/* high */
			done = done + 20;
			break;
		case 11:/* low */
			done = done + 30;
			break;
		case 12:/* temp */
			done = done + 1;
			break;
		case 13:/* grav */
			done = done + 2;
			break;
		case 14:/* metalicity */
			done = done + 3;
			break;
		case 15:/* mturb */
			done = done + 4;
			break;
		case 16:/* prog */
			done = done + 40;
			break;
		case 17:/* macro */
			done = done + 50;
			break;
		case 18:/* model */
			done = done + 60;
			break;
		default:
/*			fprintf(stderr, "Ready to set globals?\n");*/
			done = done;
	}
/*/* external program paths */
/*char *moogpath, *smpath, *mspwnpath, *mgpltspath;*/

/*/* solar file paths */
/*char *sunlst1, *sunlst2, *sunmdl;*/

/*/* limits from config.txt */
/*float mlht, mllt, mlhg, mllg, mlhm, mllm, mlhu, mllu;*/

	switch (done)
	{
		case    1:	/* finished case */
			done = -1;
			break;		
		/* Solar parameters */
		case 2210:	/* solar Fe1 lines list */
			sunlst1 = malloc(sizeof(char) * strlen(line));
			while(isblank((int) line[0]) != 0)
			{
				sprintf(line, "%s", &line[1]);
			}
			strncpy(sunlst1, line, (strlen(line)-1));
			done = -1;
			break;
		case 2310:	/* solar Fe2 lines list */
			sunlst2 = malloc(sizeof(char) * strlen(line));
			while(isblank((int) line[0]) != 0)
			{
				sprintf(line, "%s", &line[1]);
			}
			strncpy(sunlst2, line, (strlen(line)-1));
			done = -1;
			break;
		case 2160:	/* solar atmospheric model */
			sunmdl = malloc(sizeof(char) * strlen(line));
			while(isblank((int) line[0]) != 0)
			{
				sprintf(line, "%s", &line[1]);
			}
			strncpy(sunmdl, line, (strlen(line)-1));
			done = -1;
			break;
		
		/* mspawn parameters */
		case 3100:	/* mspawn program path */
			mspwnpath = malloc(sizeof(char) * strlen(line));
			while(isblank((int) line[0]) != 0)
			{
				sprintf(line, "%s", &line[1]);
			}
			strncpy(mspwnpath, line, (strlen(line)-1));
			done = -1;
			break;
		case 3421:	/* high temp limit */
			sscanf(line, "%f", &mlht);
			done = -1;
			break;
		case 3422:	/* high grav limit */
			sscanf(line, "%f", &mlhg);
			done = -1;
			break;
		case 3423:	/* high metalicity limit */
			sscanf(line, "%f", &mlhm);
			done = -1;
			break;
		case 3424:	/* high mturb limit */
			sscanf(line, "%f", &mlhu);
			done = -1;
			break;
		case 3431:	/* low temp limit */
			sscanf(line, "%f", &mllt);
			done = -1;
			break;
		case 3432:	/* low grav limit */
			sscanf(line, "%f", &mllg);
			done = -1;
			break;
		case 3433:	/* low metalicity limit */
			sscanf(line, "%f", &mllm);
			done = -1;
			break;
		case 3434:	/* low mturb limit */
			sscanf(line, "%f", &mllu);
			done = -1;
			break;
		
		/* moog parameters */
		case 4100:	/* moog program path */
			moogpath = malloc(sizeof(char) * strlen(line));
			while(isblank((int) line[0]) != 0)
			{
				sprintf(line, "%s", &line[1]);
			}
			strncpy(moogpath, line, (strlen(line)-1));
			done = -1;
			break;
			
		/* sm parameters */
		case 5140:	/* sm program path */
			smpath = malloc(sizeof(char) * strlen(line));
			while(isblank((int) line[0]) != 0)
			{
				sprintf(line, "%s", &line[1]);
			}
			strncpy(smpath, line, (strlen(line)-1));
			done = -1;
			break;
		case 5150:	/* sm macro path */
			mgpltspath = malloc(sizeof(char) * strlen(line));
			while(isblank((int) line[0]) != 0)
			{
				sprintf(line, "%s", &line[1]);
			}
			strncpy(mgpltspath, line, (strlen(line)-1));
			done = -1;
			break;
			
		/* default option */
		default:
/*			fprintf(stderr, "DEFAULT DONE CASE!\n");*/
			done = done;
	}
	
	if(done > -1 && tagnum == -1)
		done = -1;
	
	return done;
}

/* determine what the word we just read was */

int parsetag(char tag[100])
{
	int i = 0;
	char tags[18][11] =
		{"sun", "mspawn", "moog", "sm", "path",
  		 "Fe1", "Fe2", "limit", "list", "high",
  		 "low", "temp", "grav", "metalicity",
  		 "mturb", "prog", "macro", "model"};
	
	while((strcmp(tag, tags[i]) != 0) && i < 18)
		i += 1;

	if (i > 17)
		return -1;
	return i;
}

/* find the configuration text file */
	
FILE  *findcfgfile(FILE *cfg)
{
	
	char pth[100];
	/* try and open the config file from the default location */
	cfg = fopen("/etc/astro.conf", "r");

	/* if the default location fails, ask the user for the location */
	/* repeat untill the configuration file is found. */
	while(cfg == NULL)
	{
		fprintf(stderr, "I'm sorry, but your configuration file");
		fprintf(stderr, " doesn't seem to exist.");
		fprintf(stderr, " Please locate it for me: ");
		fscanf(stdin, "%s", pth);
		cfg = fopen(pth, "r");
	}
	return cfg;
}

/* parseline() figures out what universal variable to set from the line. */
int parseline(char line[100], int done)
{
	int tagnum = 0;
	char tag[20];
	
	
	while (done > -1)
	{
		sscanf(line, "%s", &tag[0]);

		while(isblank((int) line[0]) != 0)
		{
			sprintf(line, "%s", &line[1]);
		}
		
/* Check for new line and comments that start after some whitespace */		
		if(line[0] == '\n')
			return 1;
		if(line[0] == '#')
			return 1;
			
		tagnum = parsetag(tag);
		
		if (tagnum >= 0)
			sprintf(line, "%s", &line[strlen(tag)+1]);
		
		done = zwitchers(done, tagnum, &line[0]);
	}
	return 1;
}
/* external program paths */
//char *moogpath, *smpath, *mspwnpath, *mgpltspath;

/* solar file paths */
//char *sunlstsunlst1 = malloc(sizeof(char) * strlen(line));
			
/* main driver function for the configuration part of the program */
int configurator()
{
	int fin = 0;
	char line[100];
	FILE *cfg;
	
	system("cp /usr/bin/stelstats.sm ./stelstats.sm");
	system("mkdir -p /tmp/astro");
	
	cfg = findcfgfile(cfg);
	
	logpath = malloc(sizeof(char *) * strlen("/tmp/astro/"));
	
	sprintf(logpath, "/tmp/astro/");

	while(fin == 0)
	{	
		fgets(line, 100, cfg);
		
		if(line[0] == '\n')
			fgets(line, 100, cfg);
		
		parseline(line, 0);

		fin = feof(cfg);		
	}

	return 0;
}
