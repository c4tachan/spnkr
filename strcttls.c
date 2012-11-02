/* strcttls.c
 *
 * Struct tools (strcttls.c) is a file containing a number
 * of useful functions that involve the reading of data into,
 * copying, and printing of the linedata structs.
 *
 * Written by M. Patrick Tkacik
 * June 9, 2009
 */







#include"headers.h"

/*typedef struct linedata*/
/*{*/
/*	float wavelength;	/* From both handmade and MOOG lists */
/*	float eqw;		/* From both */
/*	float logGF;		/* From both */
/*	float ep;		/* From both */
/*	float constant;		/* Handmade list only */
/*	float species;		/* Handmade list only */
/*	float logRW;		/* MOOG list only */
/*	float abund;		/* MOOG list only */
/*	float delavg;		/* MOOG list only */
/*	struct linedata *next;*/
/*	struct linedata *last;*/
/*}linedata;*/

/* function to determine the species currently needed from moog */
float getspecies(char *str)
{
	float spc;
	int dec = 0;
	int num = 1;
	int cmp = 0;
	char *spec[] = {"H ","He","Li","Be","B ","C ","N ","O ","F ","Ne",	/* 95 elements in this list */
			"Na","Mg","Al","Si","P ","S ","Cl","Ar","K ","Ca",
			"Sc","Ti","V ","Cr","Mn","Fe","Co","Ni","Cu","Zn",
			"Ga","Ge","As","Se","Br","Kr","Rb","Sr","Y ","Zr",
			"Nb","Mo","Tc","Ru","Rh","Pd","Ag","Cd","In","Sn",
			"Sb","Te","I ","Xe","Cs","Ba","La","Ce","Pr","Nd",
			"Pm","Sm","Eu","Gd","Tb","Dy","Ho","Er","Tm","Yb",
			"Lu","Hf","Ta","Wl","Re","Os","Ir","Pt","Au","Hg",
			"Tl","Pb","Bi","Po","At","Rn","Fr","Ra","Ac","Th",
			"Pa","U ","Np","Pu","Am"};
	
	fprintf(stderr, "getting the species from \"%s\"\n", str);
	
	while(0 != (cmp = strncmp(str,spec[num],2)) && num < 95)
	{
/*		fprintf(stderr, "num = %i \tspec = %s\t%i\n",num,spec[num],cmp);*/
		num += 1;
	}
		if (num == 95)
		fprintf(stdout,"Attempted to read an invalid species from MOOG's output");
	
	str = str + 3;
	
	fprintf(stderr, "num = %i\n", num);
	dec = -1;
	
	while(0 == strncmp((str+dec+1),"I",1))
		dec += 1;	
	
	spc = num + 1.0 + (dec*0.10);
		
	fprintf(stderr, "spc = %f\n", num + 1.0 + (dec*0.10));
	return spc;
}

/* Copy the contents of one linedata struct into the first struct */
int copylinedata(linedata *a, linedata b)
{
	a->wavelength	= b.wavelength;
	a->eqw 		= b.eqw;
	a->logGF 	= b.logGF;
	a->ep		= b.ep;
	a->constant	= b.constant;
	a->species	= b.species;
	a->logRW	= b.logRW;
	a->abund	= b.abund;
	a->delavg	= b.delavg;
	a->next		= b.next;
	a->last		= b.last;
	
	return 0;
}
/* Fill in the struct list by reading in from the files of star information */
linedata *readspectre(FILE *in, int *len)
{
	int read, num;
	linedata *lines = malloc(sizeof(linedata));
	linedata cur;
	char buf[100];
	
	num = 0;
	
	while(0 == feof(in))
	{
		fgets(buf, 100, in);
		read = sscanf(buf,"%f %f %f %f %f %f",
			&cur.wavelength,&cur.species,&cur.logGF,
			&cur.ep,&cur.constant,&cur.eqw);
			
		if(6 == read)
		{
			lines = realloc(lines,(sizeof(linedata)*(num+1)));
			copylinedata((lines+num), cur);

/*			lineprint(*(lines+num));*/

			num += 1;
		}
	}
	*len = num;
	fprintf(stderr, "We read %i lines from the spectre output file.\n", num);
	return lines;
}

/* read the output that moog generates into an array of structs */
linedata *readmoog(FILE *in, int *len, float spec)
{
	char buf[100];
	int read, num;
	linedata *lines = malloc(sizeof(linedata));
	linedata cur;
	float cspec;
	num = 0;
		
	while(0 == feof(in))
	{
		fgets(buf,100, in);
/*		fprintf(stderr, "reading moog file line by line!\n");*/
		if(0 == strncmp("Abundance Results for Species", buf, 29))
			cspec = getspecies(buf + 30);
		
		if(spec == cspec)
		{
		
			read = 0;
			read = sscanf(buf,"%f %f %f %f %f %f %f",
				&cur.wavelength, &cur.ep, &cur.logGF, &cur.eqw,
				&cur.logRW, &cur.abund, &cur.delavg);
		
			if(read == 7)
			{
				lines = realloc(lines,(sizeof(linedata)*(num+1)));
				copylinedata((lines + num), cur);
				num += 1;
			}
		}
	}
	fprintf(stderr, "We read %i lines from the moog output file.\n", num);
	*len = num;
	return lines;
}


/* Print the information stored in a linedata structure */
int lineprint(linedata line)
{
	fprintf(stdout, "%.2f\t%.2f\t%.3f\t%.1f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\n",
				line.wavelength,line.ep,line.logGF,
				line.eqw,line.logRW,line.abund,line.delavg,
				line.constant,line.species);
	return 0;
}

/* Make an independent array of an abstract member of a linedata struct */
float *splitoff(linedata *r, char *trgt, int len)
{
	float *out = malloc(len * sizeof(linedata));
	
/*	fprintf(stderr, "splitoff() len = %i\n", len);*/
	
	while(0 < len--)
	{
		if(0 == strcmp(trgt, "wavelength\0"))
			out[len] = r[len].wavelength;
		else if(0 == strcmp(trgt, "eqw"))
			out[len] = r[len].eqw;
		else if(0 == strcmp(trgt, "logGF"))
			out[len] = r[len].logGF;
		else if(0 == strcmp(trgt, "ep"))
			out[len] = r[len].ep;
		else if(0 == strcmp(trgt, "species"))
			out[len] = r[len].species;
		else if(0 == strcmp(trgt, "logRW"))
			out[len] = r[len].logRW;
		else if(0 == strcmp(trgt, "abund"))
			out[len] = r[len].abund;
	
/*		fprintf(stderr, "out = %f\n", out[len]);*/
	}
	
	return out;
}



/* Heapsort algorithm taken nearly verbatim
 * from the book "Numerical Methods" by
 * William H. Press, Brian P. Flannery,
 * Saul A. Teukolsky, and
 * William T. Vetterlin
 */

int heapsort(int n, linedata *ra)
{
	int l, j, ir, i;
	linedata rra;
	
	l = (n >> 1) + 1;
	ir = n;
	
	for(;;)
	{
		if(l > 1)							/* Still in the "Hiring phase"		*/	
			copylinedata(&rra, ra[--l]);
		else
		{
			copylinedata(&rra, ra[ir]);				/* Retirement and Promotion phase	*/
			copylinedata(&ra[ir], ra[1]); 				/* Clear a space at array end		*/
										/* Retire Top of heap into it!		*/
			if(--ir == 1)						/* Done with last promotion		*/
			{							/* The least competent worker of all!	*/
				copylinedata(&ra[1], rra);
				fprintf(stderr, "\tSorting finished!\n");
				return 0;
			}
		}
		
		i = l;								/* Whether we are in Hiring or Retiring */
		j = l << 1;							/* phase, set up to shift element rra	*/
										/* to its proper level.			*/
		while(j <= ir)
		{
			if(j < ir && ra[j].wavelength < ra[j + 1].wavelength)
				++j;						/* Compare to better underling		*/

			if(rra.wavelength < ra[j].wavelength) 			/* Demote rra				*/
			{
				copylinedata(&ra[i], ra[j]);
				j += (i = j);
			}
			
			else							/* This is rra's level. Set j to	*/
				j = ir + 1; 					/* terminate sift down.			*/
		}
		copylinedata(&ra[i], rra);					/* Put rra into its slot		*/
	}
}


					
			
			
/*int main()*/
/*{*/
/*	linedata *moog,*spectre;*/
/*	int n = 0;*/
/*	int i = 0;*/
/*	int m = 0;*/
/*	char buff[100];*/
/*	FILE *trgt;*/
/*	FILE *trg;*/
/*	*/
/*	fprintf(stdout, "what file do you want to read spectre from?\n");*/
/*	fscanf(stdin, "%s", buff);*/
/*	trg = fopen(buff,"r");*/
/*	*/
/*	fprintf(stdout, "what file do you want to read moog from?\n");*/
/*	fscanf(stdin, "%s", buff);*/
/*	*/
/*	trgt = fopen(buff,"r");*/
/*	*/
/*	spectre = readlines(trg, &m);*/
/*	*/
/*	*/
/*	fprintf(stdout,"Spectre Array has %i members\n", m);*/
/*	*/
/*	while(i < m)*/
/*	{*/
/*		lineprint(*(spectre+i));*/
/*		i += 1;*/
/*	}*/
/*	*/
/*	i = 0;*/
/*	*/
/*	*/
/*	moog = readmoog(trgt, &n);*/
/*	fprintf(stdout, "MOOG array lines has %i members\n", n);*/
/*	*/

/*	while(i < n)*/
/*	{*/
/*		lineprint(*(moog+i));*/
/*		i += 1;*/
/*	}*/
/*	return 0;		*/
/*}*/
		
		
		
		
		
		
		
		
		
		
		
		
		
