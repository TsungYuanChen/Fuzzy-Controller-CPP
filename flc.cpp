#include <dos.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include "utilwin32.h"

#define MAX(i,j)    (i>j)?i:j
#define MIN(i,j)    (i<j)?i:j

/*fuzzy controller*/
float flc(float e,float de)
{
	int n,condition,set;
	float ptt,grade_reg[2],grade[25],a,b;
	float uflc;

	/*menbership function*/
	float
	m1[3][3]={{-1,-1,-0.0f},{-1,0.0f,1},{0.0f,1,1}},/* e */
	m2[3][3]={{-1,-1,-0.0f},{-1,0.0f,1},{0.0f,1,1}},/* de */
	m3[3][3]={{-1,-1,-0.0f},{-1,0.0f,1},{0.0f,1,1}};/* u */

	/*fuzzy rule table--->N:1 Z:2 P:3*/
    
	/*rule table of myself*/
	int rule[9][3]={{1,1,2},{1,2,3},{1,3,2},
	                {2,1,1},{2,2,2},{2,3,1},
				    {3,1,2},{3,2,3},{3,3,2}};

	/*initial value for iterative of COG*/
	a=0;b=0;
	
	/*fuzzy algorithms*/
	for(n=0;n<=8;n++)
	{
		/*fuzzifier of e*/
		condition=0;
        ptt=e;
        set=rule[n][0]-1;
        
		if((ptt<=m1[set][0])||(ptt>=m1[set][2]))
		{			
			if(set==0 && ptt<=m1[set][0])
				grade_reg[condition]=1;
			else if(set==2 && ptt>=m1[set][2])
				grade_reg[condition]=1;
			else
				grade_reg[condition]=0;
		}
		else
		{
			if((ptt>m1[set][1]))
			{
				grade_reg[condition]=(m1[set][2]-ptt)/(m1[set][2]-m1[set][1]);
			}
			else
			{
				grade_reg[condition]=(ptt-m1[set][0])/(m1[set][1]-m1[set][0]);
			}
		}
		
		/*fuzzifier of de*/
        condition=1;
        ptt=de;
        set=rule[n][1]-1;
        
		if((ptt<=m2[set][0])||(ptt>=m2[set][2]))
		{
			if(set==0 && ptt<=m2[set][0])
				grade_reg[condition]=1;
			else if(set==2 && ptt>=m2[set][2])
				grade_reg[condition]=1;
			else
				grade_reg[condition]=0;
		}
		else
		{
			if((ptt>m2[set][1]))
			{
				grade_reg[condition]=(m2[set][2]-ptt)/(m2[set][2]-m2[set][1]);
			}
			else
			{
				grade_reg[condition]=(ptt-m2[set][0])/(m2[set][1]-m2[set][0]);
			}
		}
		
		/*fuzzy inference---Mamdani*/
        grade[n]=MIN(grade_reg[0],grade_reg[1]);
        a=a+(grade[n]*m3[rule[n][2]-1][1]);
        b=b+grade[n];
    }

	/*defuzzifier by COG*/
	uflc=a/b;

	return uflc;
}