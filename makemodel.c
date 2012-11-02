/*
 * makemodel.c
 * written by Eric Bubar
 * for his program to make star models using 
 * mspawn.
 * code yanked and integrated into astro by
 * Patrick Tkacik.
 */

#include "headers.h"


int makemodel(float temp, float logg, float m, float mtvel)
{
  float p; 
  char mcmd[256];
  int num = 0;
  char path[99];
  int pos;
  p = fabs(m);
  
 // fprintf(stderr, "%f\t%f\t%f\t%f\n", temp, logg, m, mtvel);
  
  if(m < 0.00)
    { 
/* run the command to spawn an apropriate stellar model with negative metalicity */
      sprintf(mcmd,
      	"%s -wstar1.atm -t%4.0f -g%3.2f -m%3.2f -v%3.2f >modellog.txt\n", 
      		mspwnpath,temp,logg,p,mtvel);
      
      system(mcmd);
      pos = 0;
    }
  else
    {
/* run the command to spawn an apropriate stellar model with positive metalicity */
      sprintf(mcmd,
      	"%s -wstar1.atm -t%4.0f -g%3.2f -p%3.2f -v%3.2f >modellog.txt\n",
      		mspwnpath,temp,logg,p,mtvel);
      
      system(mcmd);
      pos = 1;
    } 
/* Set the directory path to the location of the atmospheric models */ 
    sprintf(path, "./");
    while (num < 2)
    {
    	mkabfind(temp, logg, p, mtvel, num, path, pos);
    	num += 1;
    }
    return 1;
}
