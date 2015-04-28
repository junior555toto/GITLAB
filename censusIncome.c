/*   censusIncome.c 
 *
 *  Reads a census data file and summarize the income information
 *  for men and for women in the file: minimum income, 
 *  maximum income and average income. Writes this information
 *  to an output file.
 */
#include <string.h>
#include <stdio.h>
#include <stdlib.h> /* for exit */
#include <ctype.h> 

#define FALSE 0
#define TRUE 1

 int YOLO1;
 int YOLO2;

/* This structure type is NOT used to store individual
 * records. Instead, it groups together the various
 * variables we need to calculate the min,max and mean
 * for men and women. This makes it easier to pass
 * all this information around to functions 
 */


 //dspfjsdpofopdsjfsdjfopdsjfopjsdfopdsjfopsdjfopdsjfopdsjfopjsdopfjdsfo
 //TEMP COMMENT NA SUD
typedef struct
{
    int countM;      /* number of male records */
    int sumIncomeM;  /* sum of income for all males */
    int minIncomeM;  /* minimum income for all males */ 
    int maxIncomeM;  /* maximum income for all males */
    int countF;      /* number of female records */
    int sumIncomeF;  /* sum of income for all females */
    int minIncomeF;  /* minimum income for all females */ 
    int maxIncomeF;  /* maximum income for all females */
    char district[64]; /* district represented by the file*/
} CENSUS_T;

/* declare subfunctions */
int processInputFile(FILE* pFile, CENSUS_T * pCensus);
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
    CENSUS_T fileData;    /* used to collect multiple stats */
    /* check command line arguments and if okay, 
     * get file name 
     */
    if (argc != 2)
       {
       fprintf(stderr,"Incorrect command line arguments\n");
       fprintf(stderr,"  Usage:\n");
       fprintf(stderr,"    ./censusIncome [inputfile]\n\n");
       exit(1);
       }
    strcpy(inFilename,argv[1]);
    /* create output file name - input file plus ".out" */
    strcpy(outFilename,inFilename);
    strcat(outFilename,".out");
    /* open and check input file */
    pIn = fopen(inFilename,"r");
    if (pIn == NULL)
       {
       fprintf(stderr,"Error - cannot open file '%s'\n",
		   inFilename);
       exit(2);
       }
    /* Call a function to read the file and do the 
     * calculations. Note that we do not actually need to
     * save the full set of data at all. 
     */
    goodLineCount = processInputFile(pIn,&fileData);    
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
    fprintf(pOut,"%s\n",fileData.district);
    fprintf(pOut,"Women: %d\n", fileData.countF);
    if (fileData.countF > 0)
       {
       fprintf(pOut,"Min Income: %d\n", fileData.minIncomeF);
       fprintf(pOut,"Max Income: %d\n", fileData.maxIncomeF);
       fprintf(pOut,"Average Income: %d\n", 
           fileData.sumIncomeF/fileData.countF);
       } 
    else
       {
       fprintf(pOut,"Cannot calculate min, max or average.\n");
       }
    fprintf(pOut,"Men: %d\n", fileData.countM);
    if (fileData.countM > 0)
       {
       fprintf(pOut,"Min Income: %d\n", fileData.minIncomeM);
       fprintf(pOut,"Max Income: %d\n", fileData.maxIncomeM);
       fprintf(pOut,"Average Income: %d\n", 
           fileData.sumIncomeM/fileData.countM);
       }
    else
       {
       fprintf(pOut,"Cannot calculate min, max or average.\n");
       }
    fclose(pOut);
    fprintf(stdout,"Wrote summary to file '%s'\n",
	    outFilename);
     

}


/* Read all the lines from a census file and
 * keep track of counts, totals, min and max in
 * the elements of the passed structure. Calls validation
 * functions to check ID and gender, and skips
 * processing a line if either field is bad.
 *   pFile    -   File pointer to opened file (already checked)
 *   pCensus  -   Pointer to CENSUS_T structure for collecting
 *                  desired information
 * Returns number of good lines processed. Will return 0
 * if it can't read the first line. 
 */
int processInputFile(FILE* pFile, CENSUS_T * pCensus)
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

    /* initialize fields of the structure for computation */
    pCensus->countM = pCensus->countF = 0;
    pCensus->sumIncomeM = pCensus->sumIncomeF = 0;
    pCensus->minIncomeM = pCensus->minIncomeF = 100000000;    
    pCensus->maxIncomeM = pCensus->maxIncomeF = 0;    

    /* read first line - district and line count */
    if (fgets(input,sizeof(input),pFile))
       {
       sscanf(input,"%s %d\n",pCensus->district,&fileCount);
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
	      if (strcmp(gender,"M") == 0)
	         {
		 pCensus->countM++;
                 pCensus->sumIncomeM += income;
                 if (income > pCensus->maxIncomeM)
		     pCensus->maxIncomeM = income;
                 if (income < pCensus->minIncomeM)
		     pCensus->minIncomeM = income;
	         }
	      else
	         {
		 pCensus->countF++;
                 pCensus->sumIncomeF += income;
                 if (income > pCensus->maxIncomeF)
		     pCensus->maxIncomeF = income;
                 if (income < pCensus->minIncomeF)
		     pCensus->minIncomeF = income;
	         }
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

