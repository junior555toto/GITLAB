/*   censusAGe.c 
 *
 *  Reads a census data file and create a histogram, counting the
 *  number of people in a number of different age brackets. 
 *  Writes this information to an output file, along with the overall
 *  average age of people in the file.
 */
#include <string.h>
#include <stdio.h>
#include <stdlib.h> /* for exit */
#include <ctype.h> 

#define FALSE 0
#define TRUE 1

/* declare subfunctions */
int processInputFile(FILE* pFile, char district[], int ageHistogram[],
                     int* pAgeTotal);
int validId(char* idstring);
int validGender(char* genderstring);


/* Main function handles file reading and
 * calculations 
 */
int main(int argc, char* argv[])
{
    char inFilename[128]; /* name of input file - from cmd line*/
    char outFilename[128];/* name of output file - constructed */
    FILE* pIn = NULL;     /* input file pointer */
    FILE* pOut = NULL;    /* output file point */
    int goodLineCount = 0;/* number of "good" lines of data */
                          /* this is also the number of people */
    int ageHistogram[5] = {0,0,0,0,0};
    /* number of people in each of several categories */
    /* [0] -> between 0 and 10 */ 
    /* [1] -> between 11 and 20 */     
    /* [2] -> between 21 and 40 */ 
    /* [3] -> between 41 and 60 */ 
    /* [4] -> 60 or higher */ 
    char district[64];    /* each file represents one district */
    int ageTotal = 0;     /* total of all ages */

    /* check command line arguments and if okay, 
     * get file name 
     */
    if (argc != 2)
       {
       fprintf(stderr,"Incorrect command line arguments\n");
       fprintf(stderr,"  Usage:\n");
       fprintf(stderr,"    ./censusAge [inputfile]\n\n");
       exit(1);
       }
    strcpy(inFilename,argv[1]);
    /* construct output file name */
    strcpy(outFilename,"out");
    strcat(outFilename,inFilename);
    /* open and check input file */
    pIn = fopen(inFilename,"r");
    if (pIn == NULL)
       {
       fprintf(stderr,"Error - cannot open file '%s'\n",
		   inFilename);
       exit(2);
       }
    /* Call a function to read the file and do the 
     * calculations. Note that we do not actually actually need to
     * save the full set of data at all. 
     */
    goodLineCount = processInputFile(pIn,district,ageHistogram,&ageTotal);    
    fprintf(stdout,"Read %d good data records from file %s\n",
	    goodLineCount,inFilename);
    fclose(pIn);
    /* Now write the output file using the accumulated information*/
    pOut = fopen(outFilename,"w");
    if (pOut == NULL)
       {
       fprintf(stderr,"Error opening output file '%s'\n",
		   outFilename);
       exit(3);
       }
    fprintf(pOut,"%s: %d people\n",district,goodLineCount);
    fprintf(pOut,"Age 0-10: %d\n", ageHistogram[0]);
    fprintf(pOut,"Age 11-20: %d\n", ageHistogram[1]);
    fprintf(pOut,"Age 21-40: %d\n", ageHistogram[2]);
    fprintf(pOut,"Age 41-60: %d\n", ageHistogram[3]);
    fprintf(pOut,"Age 61 or higher: %d\n", ageHistogram[4]);
    if (goodLineCount > 0)
       fprintf(pOut,"AVERAGE AGE: %4.1f\n", ((float) ageTotal)/goodLineCount);
    else
       fprintf(pOut, "Cannot calculate average age\n");
    fclose(pOut);
    fprintf(stdout,"Wrote summary to file '%s'\n",
	    outFilename);
}


/* Read all the lines from a census file and
 * keep track of number of people in different 
 * age brackets, also the age total. Calls validation
 * functions to check ID and gender, and skips
 * processing a line if either field is bad.
 *   pFile    -   File pointer to opened file (already checked)
 *   district -   For returning district name
 *   ageHistogram - For returning counts of people in each range
 *   pAgeTotal -  For returning total of all ages (for overall average)
 * Returns number of good lines processed. Will return 0
 * if it can't read the first line. 
 * Note we assume the total and histogram have been initialized to 0
 * by the caller.
 */
int processInputFile(FILE* pFile, char district[], int ageHistogram[],
                     int* pAgeTotal)
{
    int fileCount;    /* number of records in the file */
    int goodCount = 0;/* number of validated records in file */
    int i;            /* index variable */
    char input[128];  /* buffer for reading from file */
    /* variables to hold fields in each record */
    char id[16];      /* ID number */
    char first[32];   /* first name */
    char last[32];    /* last name */
    char gender[16];  /* gender */
    int age;          /* age */
    int income;       /* income */
    int bin;          /* which bin to increment? */

    /* read first line - district and line count */
    if (fgets(input,sizeof(input),pFile))
       {
       sscanf(input,"%s %d\n",district,&fileCount);
       /* read each line of data  in the file and process */
       for (i = 0; i < fileCount; i++)
          {
	  if (fgets(input,sizeof(input),pFile) == NULL)
              break; /* this shouldn't happen 
                      * if fgets returns NULL this means that
                      * the first line in the file is wrong
                      * and there are fewer lines than we
                      * expect 
                      */
          sscanf(input,"%s %s %s %s %d %d",id,first,last,gender,
		 &age,&income);
          if ((validId(id)) && (validGender(gender)))
              {
	      goodCount++;
              *pAgeTotal += age;
              if (age > 60)
		 bin = 4;
              else if (age > 40)
		 bin = 3;
              else if (age > 20)
		 bin = 2;
              else if (age > 10)
		 bin = 1;
              else
 		 bin = 0;
              ageHistogram[bin] += 1; /* add 1 to appropriate bin */
              } 
	  else
	      {
	      fprintf(stderr,
                      "Found data error in line %d - skipping\n",
		      i+1);
	      }

          }
       }
    return goodCount;
} 

/* check if ID is valid. 
 *   idstring   -  ID number as string
 * Returns 1 if valid, 0 if wrong format 
 */
int validId(char* idstring)
{
    int bOk = TRUE;
    int i; 
    if (strlen(idstring) != 13)
       {
       bOk = FALSE;
       }
    else
       {
       for (i = 0; i < strlen(idstring); i++)
	  {
	  if (!isdigit(idstring[i]))
	     bOk = FALSE;
	  }
       }  
    return bOk;
}

/* check if gender is valid. 
 *   genderstring   -  gender as string
 * Returns 1 if valid, 0 if wrong format 
 */
int validGender(char* genderstring)
{
    int bOk = TRUE;
    if (!(strcmp(genderstring,"M") == 0) &&
        !(strcmp(genderstring,"F") == 0))
      bOk = FALSE;   
    return bOk;
}

