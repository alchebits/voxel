#include "../include/out_algorithm3d.h"

int planeBoxOverlap(float normal[3], float vert[3], float maxbox[3])	// -NJMP-
{
  int q;
  float vmin[3],vmax[3],v;
  for(q=XXID;q<=ZZID;q++)
  {
    v=vert[q];					// -NJMP-
    if(normal[q]>0.0f)
    {
      vmin[q]=-maxbox[q] - v;	// -NJMP-
      vmax[q]= maxbox[q] - v;	// -NJMP-
    }
    else
    {
      vmin[q]= maxbox[q] - v;	// -NJMP-
      vmax[q]=-maxbox[q] - v;	// -NJMP-
    }
  }

  if(DOT(normal,vmin)>0.0f) return 0;	// -NJMP-
  if(DOT(normal,vmax)>=0.0f) return 1;	// -NJMP-

  return 0;
}


int triBoxOverlap(float boxcenter[3],float boxhalfsize[3],float triverts[3][3])
{

  /*    use separating axis theorem to test overlap between triangle and box */
  /*    need to test for overlap in these directions: */
  /*    1) the {x,y,z}-directions (actually, since we use the AABB of the triangle */
  /*       we do not even need to test these) */
  /*    2) normal of the triangle */
  /*    3) crossproduct(edge from tri, {x,y,z}-directin) */
  /*       this gives 3x3=9 more tests */
   float v0[3],v1[3],v2[3];
//   float axis[3];
   float min,max,p0,p1,p2,rad,fex,fey,fez;		// -NJMP- "d" local variable removed
   float normal[3],e0[3],e1[3],e2[3];

   /* This is the fastest branch on Sun */
   /* move everything so that the boxcenter is in (0,0,0) */
   SUB(v0,triverts[0],boxcenter);
   SUB(v1,triverts[1],boxcenter);
   SUB(v2,triverts[2],boxcenter);

   /* compute triangle edges */
   SUB(e0,v1,v0);      /* tri edge 0 */
   SUB(e1,v2,v1);      /* tri edge 1 */
   SUB(e2,v0,v2);      /* tri edge 2 */

   /* Bullet 3:  */
   /*  test the 9 tests first (this was faster) */
   fex = fabs(e0[XXID]);
   fey = fabs(e0[YYID]);
   fez = fabs(e0[ZZID]);

   AXISTEST_X01(e0[ZZID], e0[YYID], fez, fey);
   AXISTEST_Y02(e0[ZZID], e0[XXID], fez, fex);
   AXISTEST_Z12(e0[YYID], e0[XXID], fey, fex);

   fex = fabs(e1[XXID]);
   fey = fabs(e1[YYID]);
   fez = fabs(e1[ZZID]);

   AXISTEST_X01(e1[ZZID], e1[YYID], fez, fey);
   AXISTEST_Y02(e1[ZZID], e1[XXID], fez, fex);
   AXISTEST_Z0(e1[YYID], e1[XXID], fey, fex);

   fex = fabs(e2[XXID]);
   fey = fabs(e2[YYID]);
   fez = fabs(e2[ZZID]);

   AXISTEST_X2(e2[ZZID], e2[YYID], fez, fey);
   AXISTEST_Y1(e2[ZZID], e2[XXID], fez, fex);
   AXISTEST_Z12(e2[YYID], e2[XXID], fey, fex);

   /* Bullet 1: */
   /*  first test overlap in the {x,y,z}-directions */
   /*  find min, max of the triangle each direction, and test for overlap in */
   /*  that direction -- this is equivalent to testing a minimal AABB around */
   /*  the triangle against the AABB */


   /* test in XXID-direction */
   FINDMINMAX(v0[XXID],v1[XXID],v2[XXID],min,max);
   if(min>boxhalfsize[XXID] || max<-boxhalfsize[XXID]) return 0;

   /* test in YYID-direction */
   FINDMINMAX(v0[YYID],v1[YYID],v2[YYID],min,max);
   if(min>boxhalfsize[YYID] || max<-boxhalfsize[YYID]) return 0;

   /* test in ZZID-direction */
   FINDMINMAX(v0[ZZID],v1[ZZID],v2[ZZID],min,max);
   if(min>boxhalfsize[ZZID] || max<-boxhalfsize[ZZID]) return 0;

   /* Bullet 2: */
   /*  test if the box intersects the plane of the triangle */
   /*  compute plane equation of triangle: normal*x+d=0 */
   CROSS(normal,e0,e1);

   // -NJMP- (line removed here)
   if(!planeBoxOverlap(normal,v0,boxhalfsize)) return 0;	// -NJMP-


   return 1;   /* box and triangle overlaps */
}

