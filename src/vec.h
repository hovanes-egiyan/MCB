/*****************************************
 Primitive 3/4 Dim Double Vektorroutinen
 als Library          (C) 90 FAN
 Neuauflage Feb 1994 (Lorents-Vecroutinen
 Schreibweise: 3D: v_.., 4D: V_..
*****************************************/

#define V_ZERO     1E-8


typedef struct
{
  double x,y,z;
}
VEC;

/* Bem: die Fkt-Parameter werden der Reihe nach mit a,b,c... */
extern VEC
 v_set(double, double, double),	/*v=(a,b,c)*/
 v_setpol(double,double,double),/*v=(r,th,ph)*/
 v_add(VEC, VEC),		/*v=a+b    */
 v_sub(VEC, VEC),		/*analog   */
 v_smul(double, VEC),		/*v=a*b    */
 v_parallel(VEC, VEC),		/* parallel-Komp.*/
 v_normal(VEC);			/* v=a/|a|	*/
extern double
 v_sqr(VEC),			/*w=a.a	   */
 v_norm(VEC),			/*sqrt(x^2+y^2)*/
 v_sprd(VEC,VEC);		/* w=a.b	*/
extern void v_cart2pol( VEC, double *, double *, double * );  /* cart vec a ->
 r, theta, phi (pol coord) */
