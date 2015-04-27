#ifndef OUT_ALGORITHM3D_H_
#define OUT_ALGORITHM3D_H_

#include <iostream>
#include <math.h>
/********************************************************/

/* AABB-triangle overlap test code                      */

/* by Tomas Akenine-Möller                              */

/* Function: int triBoxOverlap(float boxcenter[3],      */

/*          float boxhalfsize[3],float triverts[3][3]); */

/* History:                                             */

/*   2001-03-05: released the code in its first version */

/*   2001-06-18: changed the order of the tests, faster */

/*                                                      */

/* Acknowledgement: Many thanks to Pierre Terdiman for  */

/* suggestions and discussions on how to optimize code. */

/* Thanks to David Hunt for finding a ">="-bug!         */

/********************************************************/


#define XXID 0

#define YYID 1

#define ZZID 2



#define CROSS(dest,v1,v2) \
          dest[0]=v1[1]*v2[2]-v1[2]*v2[1]; \
          dest[1]=v1[2]*v2[0]-v1[0]*v2[2]; \
          dest[2]=v1[0]*v2[1]-v1[1]*v2[0];

#define DOT(v1,v2) (v1[0]*v2[0]+v1[1]*v2[1]+v1[2]*v2[2])

#define SUB(dest,v1,v2) \
          dest[0]=v1[0]-v2[0]; \
          dest[1]=v1[1]-v2[1]; \
          dest[2]=v1[2]-v2[2];

#define FINDMINMAX(x0,x1,x2,min,max) \
  min = max = x0;   \
  if(x1<min) min=x1;\
  if(x1>max) max=x1;\
  if(x2<min) min=x2;\
  if(x2>max) max=x2;

int planeBoxOverlap(float normal[3], float vert[3], float maxbox[3]);	// -NJMP-


/*======================== XXID-tests ========================*/

#define AXISTEST_X01(a, b, fa, fb)			   \
	p0 = a*v0[YYID] - b*v0[ZZID];			       	   \
	p2 = a*v2[YYID] - b*v2[ZZID];			       	   \
        if(p0<p2) {min=p0; max=p2;} else {min=p2; max=p0;} \
	rad = fa * boxhalfsize[YYID] + fb * boxhalfsize[ZZID];   \
	if(min>rad || max<-rad) return 0;

#define AXISTEST_X2(a, b, fa, fb)			   \
	p0 = a*v0[YYID] - b*v0[ZZID];			           \
	p1 = a*v1[YYID] - b*v1[ZZID];			       	   \
        if(p0<p1) {min=p0; max=p1;} else {min=p1; max=p0;} \
	rad = fa * boxhalfsize[YYID] + fb * boxhalfsize[ZZID];   \
	if(min>rad || max<-rad) return 0;

/*======================== YYID-tests ========================*/

#define AXISTEST_Y02(a, b, fa, fb)			   \
	p0 = -a*v0[XXID] + b*v0[ZZID];		      	   \
	p2 = -a*v2[XXID] + b*v2[ZZID];	       	       	   \
        if(p0<p2) {min=p0; max=p2;} else {min=p2; max=p0;} \
	rad = fa * boxhalfsize[XXID] + fb * boxhalfsize[ZZID];   \
	if(min>rad || max<-rad) return 0;

#define AXISTEST_Y1(a, b, fa, fb)			   \
	p0 = -a*v0[XXID] + b*v0[ZZID];		      	   \
	p1 = -a*v1[XXID] + b*v1[ZZID];	     	       	   \
        if(p0<p1) {min=p0; max=p1;} else {min=p1; max=p0;} \
	rad = fa * boxhalfsize[XXID] + fb * boxhalfsize[ZZID];   \
	if(min>rad || max<-rad) return 0;

/*======================== ZZID-tests ========================*/

#define AXISTEST_Z12(a, b, fa, fb)			   \
	p1 = a*v1[XXID] - b*v1[YYID];			           \
	p2 = a*v2[XXID] - b*v2[YYID];			       	   \
        if(p2<p1) {min=p2; max=p1;} else {min=p1; max=p2;} \
	rad = fa * boxhalfsize[XXID] + fb * boxhalfsize[YYID];   \
	if(min>rad || max<-rad) return 0;

#define AXISTEST_Z0(a, b, fa, fb)			   \
	p0 = a*v0[XXID] - b*v0[YYID];				   \
	p1 = a*v1[XXID] - b*v1[YYID];			           \
        if(p0<p1) {min=p0; max=p1;} else {min=p1; max=p0;} \
	rad = fa * boxhalfsize[XXID] + fb * boxhalfsize[YYID];   \
	if(min>rad || max<-rad) return 0;

int triBoxOverlap(float boxcenter[3],float boxhalfsize[3],float triverts[3][3]);

#endif
