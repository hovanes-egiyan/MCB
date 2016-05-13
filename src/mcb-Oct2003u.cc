/******************************************************************************
*
*	MCB	Monte Carlo Bremsstrahlung Simulation	/AN/4/10/00
*		Version 4.0
*
*	use make to compile
*
*	13/11/00/AN     new Wilson (RHF-calc) form factor implemented
*	19/04/02/pg	smooth
*	29/12/04/pg	ROOT, TTree + TH1F + GetUserInfo() for Inp & Out
*	29/03/05/pg	adapt input to/with ANB
*
******************************************************************************/
#include <stdio.h>    
#include <math.h>
#include <string.h>
#include <stdlib.h>
#ifndef __USE_GNU
#define __USE_GNU
#endif
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/resource.h>
#include <fcntl.h>
#include <unistd.h>

#include <TObject.h>
#include <TList.h>
#include <TFile.h>
#include <TTree.h>
#include <TH1.h>

#include "TUserMCBInp.h"
#include "TUserMCBOut.h"

#include "randoms.h"
#include "vec.h"

using namespace std;
 
/******************************************************************************
		defines, physical constants....
******************************************************************************/
#define FALSE 		0
#define TRUE 		1
#define M_2PI 		2.*M_PI
#define THETARANGE	16.73		/* MCB: max polar photon angle:
					   0.01 rad * E0
					   MCB -- E0=855MeV/0.511MeV */
#define SCREEN_DEBOF	110.185
#define MASS_ELEC	0.511003373282		/* electronmass in MeV*/
#define ALATTICE	923.7			/* lattice spacing in
						   nat. units */
#define DHKL		(M_2PI/ALATTICE)	/* 2*Pi/lattice const. in
						   nat. units. */
#define NCELL		8			/* number of atoms in
						   elementary lattice cell */
#define F1		(8. *M_PI/(ALATTICE*ALATTICE*ALATTICE*NCELL))
						/* used for Phi1,2,4 */
#define F2		(96.*M_PI/(ALATTICE*ALATTICE*ALATTICE*NCELL))
#define F4		F1
#define HMAX		0			/* in this version: miller
						   index h = 0 */
#define KLMAX		24			/* maximal values for miller
						   indices k,l (see KLMax) */
#define T_DEBYE_DIAMOND	2230.0			/* debye temp of diamond */
#define ENDPOINT	0.002			/* exclude the endpoints in
						   the cross sections ->
						   unreliable results*/
#define TNULL		273.15			/* absolute zero temperature */

#define Z_Diam      6.0
#define Z_Diam3     1.8171206             /*   Z^1/3  */
#define A_Diam     12.0
#define alpha_Diam (Z_Diam/137.0)
#define rho_Diam    3.513
#define C_Euler     0.577
/******************************************************************************
		global variables, mostly from input-file
******************************************************************************/
char	CODE[5] = "mcb:";		/* selection in input */
double	E0MeV, sigE0MeV;		/* electron beam energy and spread */
double	energy, sigE0;			/* electron beam energy and spread */
double	Theta, Alpha, Phi, Kappa;	/* orientation of the diamond lattice*/
double	sigx, sigy;			/*beam spot size in x-und y-direction*/
double	sigpx,sigpy;			/* transverse beam divergence in x-
					   and y-direction */
double	thickness;			/* thickness of radiator */
double	thickn_dia;			/* thickness of the diamond */
double	thickn_amo;			/* thickness of amorphous foil */
double	coldist, collength;		/* distance radiator--collimator,
					   length of collimator  (all in [m])*/
double	rcol, rcol2, wcol, hcol;	/* radius of the collimator,
					   or in case of rectangular colli:
					   width,height ([m]) */
VEC	coloff;				/* possible colli-offset in [m] */
double	Z, Z3, Zamo, Zcry;		/* Z of radiator, Z3=Z^1/3 */
double	NMax;				/* No of required photons */
double	Nphoton=0, Ncollimator=0;	/* No calculated photons,within colli*/
double	Nbounds=0;			/* No photons out of calc region */
double	Nincident=0;			/*number of incident electrons needed*/
double	IntMax=-1.E100, IntMin=+1.E100;	/* IntMax/Min: test of IntTot */
double	IntTot, RandomVol;		/* Monte Carlo upper limit of random
					   volume (in physical units)
					   in which the intensities are
					   calculated */
int	KLMax;				/* KLMax = Max lattice vector index in
					   [HKL] */
FILE	*file_log, *file_out;		/* file handles and names */
char	fname_in[50], fname_log[50], fname_out[50];
char	comment[100];			/* comment line for *.log file  */
char	brprocs[10];			/* type	of brems calc, file */
char	mode[10];			/* type	of brems calc, argv[2]*/
int	amo_mode;			/* true, if amorphous target used */
int	dif_mode;			/* true, if dif calculated */
long  	time_start, time_stop, time_now;/* vars for timing */
double	temperature;			/* temperature of crystal */
double 	Adebye, CDeb, MDeb, RDeb;	/*temp. dependent Debye-Waller factor*/
double 	reduction, screening;		/*Hubbell-xsec screening, see debye.c*/
double	DebT[100], DebA[100], DebScreen[100], DebMR[100];
					/* see debye.c/.log/.dat */
double	PDeb[7] = {	0.664203, 4.09235E-06, 11.1074,
			0.932192, 0.237098, -0.0564868};
					/* see heitler.kumac	*/
char	processes[] = "ibh, amo, inC, inM, inR, sum, dif";
					/* types of bremsstrahlung processes */
/******************************************************************************
		Variables for MC simulation, observables per event and
		 random generators
******************************************************************************/
VEC	v_null = { 0.0, 0.0, 0.0 };
VEC	re, ms, pb;			/* position of e- on/in the diamond,
					   and beamdiv, multscat transverse
					   vector */
VEC 	p0, pd, pe; 			/* transverse vectors to
					   incident/divergent electron in
					   x,y,b1 basis */
double	E0, x0, delta, Int;		/* energy of scattered e-, incident e-
					   (after multsc and energy spread)
					   and photon */
double	thetag, psig, rho, thg, phg;	/* angle of photon in
					   electron/lab-system  */
VEC	tg;				/* transverse vector of photon in
					   lab-sys */
VEC	g;				/* lattice vec g */
double	g2, gl, gl2;		 	/* long/transv/norm of lattice vec g,
					   struct function */
double	S2, FF2, fdeb;			/* structure- and atomformfactor
					   squared, debyefactor	*/
double	Th, Al;				/* as Theta, Alpha, but for each
					   'divergent' electron */
int 	hm, km, lm;			/* miller indices vec [h,k,l] */
		/* random number generators for the appropriate MC variables */
struct  Rdatg     RE0, Rxe, Rye, Rthm, Rxbd, Rybd;
struct  Rdat3     Rze, Ry0, Rx0, Rps, RU, Rphm;
struct  Rdati     Rh, Rk, Rl;
/******************************************************************************
		function prototypes
******************************************************************************/
void    mcb_event_loop (void);
void	(*Intensity)( void ); 		/* coh or inc MC main loop
					    (toss the dice here) */
void	IntensityCoh( void );
void	IntensityInc( void );
void	(*int_coh)( void ); 		/* calc the coherent intensity, either
					   sum or dif */
void	intcoh_sum( void );
void	intcoh_dif( void );
double	(*int_inc)( double ); 		/* either intinc_hub or intinc_bh */
double	(*int_el) ( double ); 		/* electron contribution */
double	intinc_hub_amo( double U );	/* 2fold (d2I/tdtdx) incoh intensity
					   [Hubbell] */
double	intinc_hub_CDeb( double U );
double	intinc_hub_MDeb( double U );
double	intinc_hub_RDeb( double U );
double	intinc_hub_ALL( double U );
double	intinc_bh ( double );		/* 2fold (d2I/tdtdx) incoh intensity
					   [BH] */
double	intinc_el ( double );		/* 2fold (d2I/tdtdx) incoh intensity
					   [MOw] electron contrib */
double	dummy(double);
int	(*collimator)( void );		/* use either box (wcol,hcol) or
					   zyl (rcol), both with collength */
int	box_colli( void );
int	zyl_colli( void ); 
double	structurefactor( void );	/* calculates square of
					   structurefactor per lattice vec */
void	get_debye( double );
void	thetag_psi1( void );
double	Gam( void );
double	atomformfactor( double );	/* only shell, nucleus needed and
					   normalisation, see FF2 */
double	sig_moliere( double, double );
int	read_parameter( void );		/* read parameters for calculation
					   from input file */
void	infoprint ( void );
double	solveeq (double);

/******************************************************************************
		interface to ROOT (TTree+TH1F)
******************************************************************************/
  TFile*  root_file;
  TTree*  t1;
  int chan_k = 870;
  int k_max = 870;
// xscale redone after input read to set proper length
 TH1F *heg = new TH1F("heg","E_{#gamma} col;  k / MeV; int",chan_k,0,chan_k);

extern int open_rootfile(char fname[], char mode[], int );
  char   variable_b_name[20][50];
  char   variable_i_name[20][50];
  char   variable_f_name[20][50];
  char   variable_d_name[20][50];
  int    b_value        [20];
  int    i_value        [20];
  float  f_value        [20];
  double d_value        [20];


extern "C" {
float unirndhp_(float *tmp);}
extern "C" {
float unirdbinit_(int *lux, int *in, int *k1, int *k2);}
extern "C" {
float unirdnsave_(int *lux, int *in, int *k1, int *k2);}
extern "C" {
void rluxat_(int *lux, int *in, int *k1, int *k2);}
extern "C" {
float unirdncont_(int *lux, int *in, int *k1, int *k2);}
extern "C" {
void ranlux_(float a[], int *in);}
extern "C" {
void rluxin_(int *IRvec);}
extern "C" {
void rluxut_(int *IRvec);}

  void Rinit3( struct Rdat3 *, char *, double , double );
  double Rran3(struct Rdat3 *);
  void Rinitg( struct Rdatg *, char *, double , double );
  double Rgauss( struct Rdatg *);
  void Riniti( struct Rdati *, char *, int , int);
  int Rrani( struct Rdati *);

/******************************************************************************
		Read params and prepare simulation
******************************************************************************/

int main( int argc, char **argv )
{
  TUserMCBOut* InfoOut;
  TUserMCBOut* InfoOutR;
  TUserMCBInp* InfoInp;

//    double	the, phe, dmy, Ka, sig_ms, al, th;
  char  	*s;
  char	fname_base[50];
  char	fname_spec[50];
  char	fname_path[50]="";
  int n_entr,ikx,i_chain;
  int i,l,IRvec[30];
  double integral;

  if (argc == 3){
      strcpy (fname_in, argv[1]);
      strcpy (mode, argv[2]);
  }
  else if (argc == 4){
      strcpy (fname_in, argv[1]);
      strcpy (mode, argv[2]);
      strcpy (fname_path, argv[3]);
  }
  else {
	printf(
	   "\nPlease enter the name of the input file (without ending 'inp')");
	printf(", mode and pathfor storage\n");
	scanf("%s%s%s", fname_in,mode,fname_path);	}

  printf(" name, mode and path %s %s, %s\n",fname_in,mode,fname_path);
  if( (s=strstr(fname_in,".inp")) != NULL ) *s=0;	/* no file-ending */
  if( strcmp(mode,"amo")!=0 &&
      strcmp(mode,"inc")!=0 &&			/* possible start */
      strcmp(mode,"ibh")!=0 &&
      strcmp(mode,"inC")!=0 &&
      strcmp(mode,"inM")!=0 &&
      strcmp(mode,"inR")!=0 &&
      strcmp(mode,"sum")!=0 &&
      strcmp(mode,"dif")!=0 ){  printf(
	"\nvalid modes: amo, ibh, inC, inM, inR, sum or dif\n");
	exit( -1 ); }

  strcpy(fname_base,fname_in);
  strcat( fname_in,  ".inp" );
  if( (file_log = fopen( fname_in, "r" )) == NULL ){	/* test input file */
	fprintf( stderr, "\n\nFatal Error: couldn't open input file\n\n" );
	exit( -1 ); }
  fclose(file_log);
					/* set path and filenames */
  if( (l=strlen(fname_path)) >= 0 &&
      (l-(int)((s=strrchr(fname_path,'/'))-fname_path+1)) != 0){
      if(strlen(fname_path)>1) strcat(fname_path,"/");
      }
  sprintf(fname_log,"%s%s.%s.log",fname_path,fname_base,mode);
  sprintf(fname_out,"%s%s.%s.root",fname_path,fname_base,mode);
  sprintf(fname_spec,"%s%s.%s.root",fname_path,fname_base,mode);
  i_chain = 0;
  while(  fopen(fname_out,"r") != NULL){
      fprintf(stderr," file exists, get index for next chain\n");
      i_chain++;
  sprintf(fname_out,"%s%s.%s%2.2d.root",fname_path,fname_base,mode,i_chain);
  sprintf(fname_log,"%s%s.%s%2.2d.log",fname_path,fname_base,mode,i_chain);
  if(i_chain >1){
      sprintf(fname_spec,"%s%s.%s%2.2d.root",
	      fname_path,fname_base,mode,(i_chain-1));
      }
  }  // loop for proper max index within chain
  fprintf(stderr," finally the name is: %s\n",fname_out);
// ===========================================================================
  if( (file_log = fopen( fname_log, "w" ) ) == NULL ){	/* open log file */
	fprintf( stderr, "\n\nFatal Error: couldn't open log file\n\n" );
	exit( -1 ); }

  read_parameter();					/* read user input */

// ===========================================================================
  k_max = chan_k = (int)(E0MeV * 1.02);
  while ( chan_k > 1024 ) { chan_k /= 2;}
  fprintf(stderr," photon spectral range: 0 - %d, no.channels %d\n",
	  k_max,chan_k);
  heg->SetBins(chan_k,0,k_max);
// ===========================================================================
  TH1F *hegstore = 0;
  TTree *fmcb =  0;
  InfoOutR = new TUserMCBOut;
  if(i_chain >> 0){			/* open previous root for histo */
      TFile *fheg = new TFile( fname_spec, "READ");
      hegstore =  (TH1F *)fheg->Get("heg");
      heg->Add(hegstore,1);
      integral=heg->Integral(0,chan_k);
      i=heg->GetNbinsX();
      fprintf(stdout,
	  " col. Eg spectrum with %d bins, already accumulated events: %f\n\n",
	      i,integral);
				/* open random number file from prev.run */
      fmcb = (TTree *)fheg->Get("mcb_tree");
      fmcb->ls();
      TList* uio = fmcb->GetUserInfo();
      uio->ls();
      uio->Dump();
      InfoOutR =  (TUserMCBOut *)uio->Last();
      for(i=0;i<25;i++){
 	  IRvec[i] = InfoOutR->RandomVectors[i];
 	  }
      fheg->Close();
      rluxin_(IRvec);
  }

  InfoInp = new TUserMCBInp;
  strcpy(InfoInp->bremsprocs,mode);
  InfoInp->Zcry = Zcry;
  InfoInp->Zamo = Zamo;
  InfoInp->thickn_dia = thickn_dia;
  InfoInp->thickn_amo = thickn_amo;
  InfoInp->Theta = Theta;
  InfoInp->Alpha = Alpha;
  InfoInp->Phi = Phi;
  InfoInp->E0 = E0MeV;
  InfoInp->sigE0 = sigE0MeV;
  InfoInp->sigx = sigx;
  InfoInp->sigy = sigy;
  InfoInp->sigpx = sigpx;
  InfoInp->sigpy = sigpy;
  InfoInp->coldist = coldist;
  InfoInp->collength = collength;
  if (hcol == 0){
      InfoInp->col1 = rcol;
      InfoInp->col2 = 0;
      }
  else{
      InfoInp->col1 = wcol;
      InfoInp->col2 = hcol;
      }
  InfoInp->coloff_x = coloff.x;
  InfoInp->coloff_y = coloff.y;
  InfoInp->temperature = temperature;
  InfoInp->IntTot = IntTot;
  InfoInp->KLMax = KLMax;

// ***************************************************************************
  Rinit3( &Ry0, "y0", 0., IntTot );		/*initalize random generators*/
  Rinit3( &Rx0, "x0", ENDPOINT, 1.-ENDPOINT );	/* range minus fringe */
  Rinit3( &Rps, "ps", 0., M_2PI );
  Rinit3( &RU , "U", 0., THETARANGE );
  Rinitg( &RE0, "E0", energy, sigE0 );
  Rinitg( &Rxe, "xe", 0., sigx );
  Rinitg( &Rye, "ye", 0., sigy ); 
  Rinitg( &Rthm,"thm", 0., 1. );		/* range depends on dy via
						   sig_ms (plane) */
  Rinit3( &Rphm,"phm", 0., M_2PI );
  Rinitg( &Rxbd,"xbd", 0., sigpx );
  Rinitg( &Rybd,"ybd", 0., sigpy );
  Riniti( &Rh, "h", -HMAX,  HMAX );
  Riniti( &Rk, "k", -KLMax, KLMax );
  Riniti( &Rl, "l", -KLMax, KLMax );

  p0 = v_setpol( Theta, M_PI_2, Alpha+Phi );
  thickness = thickn_dia;		/* set first to dia, mod in switch */
  amo_mode = FALSE;
  dif_mode = FALSE;
					/* find brems-type nr: 0,5,10,15,20 */
  switch( (int)(strstr(processes,brprocs)-processes) ) {
	case 0:			/* ibh: incoherent (+electron) from BH */
		Intensity = IntensityInc;
		int_inc = intinc_bh;
		int_el  = dummy;
		Z = Zamo; Z3 = pow(Z,1./3.);
		thickness = thickn_amo;
		amo_mode = TRUE;
		RandomVol = IntTot * energy*MASS_ELEC * THETARANGE;	
		fprintf( file_log,
			 "incoherent Bethe Heitler intensity with IntTot=%f\n",
			 IntTot );
  	break;
  	case 5:		/* amo: amorphous incoherent (+electron)
							from Hubbell/Owens */
		Intensity = IntensityInc;
		Z = Zamo; Z3 = pow(Z,1./3.);
		thickness = thickn_amo;
		amo_mode = TRUE;
		int_inc = intinc_hub_ALL;
		screening = SCREEN_DEBOF;
		reduction = 1.0;
		int_el  = intinc_el;
		RandomVol = IntTot * energy*MASS_ELEC * THETARANGE;
		fprintf( file_log,
		   "amorphous incoherent Hubbel intensity with IntTot=%f\n",
			 IntTot );
  	break;
	case 10:	/* inc: crystal incoherent (+electron)
							from Hubbell/Owens */
		Intensity = IntensityInc;
		Z = Zcry; Z3 = pow(Z,1./3.);
		int_inc = intinc_hub_ALL;		/* using effective
							   screening */
		screening = 27.237 + 0.010836 * temperature;
		reduction = 1.0;
		int_el  = intinc_el;
		RandomVol = IntTot * energy*MASS_ELEC * THETARANGE;
		fprintf( file_log,
			 "crystal incoherent Hubbel intensity (CDeb=eff"
			 " screening) with IntTot=%f\n",
			 IntTot );
  	break;
	case 15:	/* inc: crystal incoherent (+electron)
							from Hubbell/Owens */
		Intensity = IntensityInc;
		Z = Zcry; Z3 = pow(Z,1./3.);
		int_inc = intinc_hub_ALL;
		screening = SCREEN_DEBOF;
		reduction = MDeb;	/* using mean debye reduction factor */
		int_el  = intinc_el;
		RandomVol = IntTot * energy*MASS_ELEC * THETARANGE;
		fprintf( file_log,
			 "crystal incoherent Hubbel intensity"
			 " (MDeb=mean debye reduction) with IntTot=%f\n"
			 ,IntTot );
  	break;
	case 20:    /* inc: crystal incoherent (+electron) from Hubbell/Owens*/
		Intensity = IntensityInc;
		Z = Zcry; Z3 = pow(Z,1./3.);
		int_inc = intinc_hub_ALL;
			/* using 2fold differential debye reduction factor */
		screening = SCREEN_DEBOF;
		reduction = -1.0;
		int_el  = intinc_el;
		RandomVol = IntTot * energy*MASS_ELEC * THETARANGE;
		fprintf( file_log,
			 "crystal incoherent Hubbel intensity "
			 "(RDeb=2fold diff debye reduction)"
			 "with IntTot=%f\n",IntTot );
  	break;
  	case 25:	/* sum: coherent sum (sig_sum=sig_para+sig_perp) */
		Intensity = IntensityCoh;
		Z = Zcry; Z3 = pow(Z,1./3.);
		int_coh = intcoh_sum;
		RandomVol = IntTot * energy*MASS_ELEC * M_2PI
				   * (2*HMAX+1)*(2*KLMax+1)*(2*KLMax+1);
		fprintf( file_log,
		"coherent sum (para+perp) intensity with IntTot=%f\n",
			 IntTot );
	break;
  	case 30:	/* dif: coherent dif (sig_dif=sig_para-sig_perp) */
		Intensity = IntensityCoh;
		Z = Zcry; Z3 = pow(Z,1./3.);
		int_coh = intcoh_dif;
		RandomVol = IntTot * energy*MASS_ELEC * M_2PI
				   * (2*HMAX+1)*(2*KLMax+1)*(2*KLMax+1);
		dif_mode = TRUE;
		fprintf( file_log,
			 "coherent dif (para-perp) intensity (Polarisation) "
			 "with IntTot=%f\n",
			  IntTot );
	break;
  	default:
	  	fprintf (file_log,
			 "\nFATAL ERROR: unknown type: %s\n"
			 "only allowed: %s\nProgram aborted!\n",
			brprocs, processes );
		exit(1);
  }
  Rinit3( &Rze, "ze", 0., thickness );		/*initalize random thickness */

  n_entr=19;
  ikx=open_rootfile( fname_out, mode, n_entr );
  fprintf( file_log, "ROOT-Tree file %s booked\n", fname_out );
  fflush( file_log );
  time_start = time (NULL);
/******************************************************************************
		EVENT - loop
******************************************************************************/
  mcb_event_loop ();	  	  /*  mcb events */

  time_stop = time( NULL );

  InfoOut = new TUserMCBOut;
  strcpy(InfoOut->bremsprocs,mode);
  InfoOut->MaxH = HMAX;
  InfoOut->MaxKL = KLMax;
  InfoOut->IntensityMin = IntMin;
  InfoOut->IntensityMax = IntMax;
  InfoOut->IntensityTot = IntTot;
  InfoOut->NormVolume = RandomVol;
  InfoOut->no_Electrons = Nincident;
  InfoOut->no_Photons = Nphoton;
  InfoOut->no_OutOfBounds = Nbounds;
  InfoOut->no_CollPhotons = Ncollimator;
  rluxut_(IRvec);
  for(i=0;i<25;i++){
	  InfoOut->RandomVectors[i] = IRvec[i];
	  }

  fprintf( file_log,
	   "\nInfo for analysis:\n"
	   "%f\t\tNphoton   (photons produced)\n"
	   "%f\t\tNcollimator (photons passed colli) from\n"
	   "%f\t\tNincident (incident electrons) in\n"
	   "%f\t\tRandomVol (Volume of Monte Carlo variables)\n\n",
  	Nphoton, Ncollimator, Nincident, RandomVol );
  fprintf(file_log, " no of rejects by bounds:%f\n",Nbounds);
  fprintf( file_log,
 "Total time:\t%8i seconds\n(%g s per photonevent, %g s per electronevent)\n",
	   (int)(time_stop - time_start),
	   (double)(time_stop-time_start)/(double)(Nphoton+1),
	   (double)(time_stop-time_start)/(double)(Nincident+1) );
  fprintf( file_log,
	"Maximal intensity: %g\tMinimal intensity: %g\n"
	"Accepted intensity Range (Total IntTot): 0 ... %f\n",
  	IntMax, IntMin, (float)(IntTot) );
  if( strstr( "sum, dif", brprocs) != NULL )
    fprintf( file_log, "Lattice Vecs used: HMAX=%i\tKLMax=%i\n", HMAX, KLMax );
  if( Nphoton>0 )
  	fprintf( file_log, "Events written in root tree file :%s\n",fname_out);

  t1->GetUserInfo()->Add(InfoInp);
  t1->GetUserInfo()->Add(InfoOut);
  t1->Print();  // print info on screen
  integral=heg->Integral(0,chan_k);
      fprintf(file_log," total accumulated events in col Eg: %f\n",integral);
  root_file->Add(heg);
  root_file = t1->GetCurrentFile();   // may be different from first file
                                      // (split mode!)
  root_file->Write();
  root_file->Close();

  fclose( file_log );
  sprintf( comment,
  "echo Closing down at `date` ... DONE >> %s;cat %s", fname_log, fname_log );
  system( comment );
  return 0;
}

void mcb_event_loop (void)
{
  double	the, phe, dmy, Ka, sig_ms, weight;
/******************************************************************************
		EVENT - loop
******************************************************************************/
  while( Nphoton < NMax ) {
	infoprint();			/* nice info for the user */
	
	E0 = Rgauss( &RE0 );		/* beam energy */
				/* position of the electron on radiator */
	re = v_set( Rgauss(&Rxe), Rgauss(&Rye), 0. );
	if( thickness > 0. ) {
		re.z = Rran3( &Rze );		/* choose depth in radiator for
						   bremsstrahlung process */
		sig_ms = sig_moliere(re.z, E0*MASS_ELEC);
						/* multiple scattering from
						   Moliere theory,
						   deviation in plane,see PDB*/
		Rthm.sig2 = 2.* sig_ms*sig_ms;	/* half angle^2 in space */ 
		ms = v_setpol( Rgauss(&Rthm), M_PI_2, Rran3(&Rphm) );
		}
	else {
		ms = v_null;
		sig_ms = 0.;
		}

	pb = v_set( Rgauss(&Rxbd), Rgauss(&Rybd), 0. );
	pe = v_add( pb, ms );		/* add moliere scattering and beamdiv*/
	v_cart2pol( pe, &the, &dmy, &phe );
	pd = v_add( p0, pe );
	v_cart2pol( pd, &Th, &dmy, &Ka );
	Al = Ka - Phi;
	(*Intensity)();
	if(thg > 1.6e-2){
	  fprintf(stdout, " thg out of bounds: %f\n",thg);
	  Nbounds++;
	  continue;
	  }				/* writes the results now in ROOT */
	f_value[0] = (float)(x0*E0*MASS_ELEC);
	f_value[1] = (float)the;
	f_value[2] = (float)phe;
	f_value[3] = (float)thg;
	f_value[4] = (float)phg;
	f_value[5] = (float)re.x;
	f_value[6] = (float)re.y;
	f_value[7] = (float)re.z;
	f_value[8] = (float)Int;
	f_value[9] = (float)sig_ms;
	f_value[10]= (float)Th;
	f_value[11]= (float)Al;

	d_value[0]= (double)sqrt(g2);
	d_value[1]= (double)gl2;
	d_value[2]= (double)g2-gl2;

	b_value[0]   = hm;
	b_value[1]   = km;
	b_value[2]   = lm;
	b_value[3]   = (*collimator)();	
	t1->Fill();
//			    if collimated
	if(b_value[3] == 1){
	    weight = Int/fabs(Int);
	    heg->Fill(x0*E0*MASS_ELEC,weight);
	}
  }
/******************************************************************************
  		end of event loop, log some info...
**************************************************************************** */
}

/*=============================================================================
 * coherent intensity: toss the dice.....	using MC rejection method
	AN/12.97
 *
=============================================================================*/
void IntensityCoh( void )
{
  double	y0, y0Int, dmy;

  do{
	do{  
	     y0   = Rran3( &Ry0 );
	     x0   = Rran3( &Rx0 );
	     psig = Rran3( &Rps );
	     do{
			hm = Rrani(&Rh);
			km = Rrani(&Rk);
			lm = Rrani(&Rl);
			Nincident++;
		} while( (S2=structurefactor()) == 0. );
		delta = x0/(2.0*E0*(1.0-x0));
		g  = v_smul( DHKL, v_set( (double)hm, (double)km,(double)lm ));
		gl = g.x*cos(Th) + sin(Th)*( g.y*cos(Al) + g.z*sin(Al) );
	   } while( gl < delta );
	gl2 = gl*gl;
	g2  = v_sqr( g );
	fdeb= exp( -Adebye*g2 );
	FF2 = atomformfactor(g2) /g2;
	FF2 = FF2 * FF2;

	int_coh();

	if( Int>IntMax )	IntMax = Int;
	if( Int<IntMin )	IntMin = Int;
	if(dif_mode) { y0Int = fabs(Int);}
	else         { y0Int = Int;}
	if( y0Int>IntTot )
		fprintf ( file_log,
	"mcb > intensity out of random volume range : |Int|=%f, IntTot=%f\n", 
			  y0Int, IntTot );
    }  while( y0 > y0Int );

  Nphoton++;
  thetag_psi1();
  if( (rho=psig+Al+Phi) > M_2PI ) rho -= M_2PI;
  if(  rho<0 )	rho += M_2PI;
  tg  = v_add( v_setpol( thetag, M_PI_2, rho ), pe );
  v_cart2pol( tg, &thg, &dmy, &phg );
}
/*=============================================================================
		functions of lattice vec and angles for COHERENT INTENSITY
=============================================================================*/
void thetag_psi1()
{
  double gamma, expr;

  gamma = Gam();
  expr  = gl/delta - 1. + gamma*gamma;
  if (gl < delta || expr < 0.0) {
	printf( "ERROR: not able to calc thetag(psig)\n" );
	exit( -1 );
	}
  thetag = (gamma+sqrt(expr))/E0;
}

double Gam( void )
{
  double	t = psig - Al;
  return	-g.y * cos(t) + g.z * sin(t);
}

double Lambda( void )
{
  double	dmy = delta/gl2;
  return	dmy*dmy * (gl - delta);
}

double structurefactor( void )
{
  register int	n1=abs(hm), n2=abs(km), n3=abs(lm),
				modul=n1%2+n2%2+n3%2;

 if( n1+n2+n3 == 0 )			return  0.;
 if( modul==0 && (hm+km+lm)%4==0 )	return 64.;
 else if( modul==3 )			return 32.;
 else					return  0.;
}
void intcoh_dif( void )
{
  double Ga, La, Y, De, rho, Phi4, psi;

  Ga  = Gam();
  La  = Lambda();
  rho = 2. * ( psig + Phi - Al );
  psi = 2. * ( psig - Al );
  De  = ( g.y*g.y - g.z*g.z ) * sin(psi)    + 2. * g.y * g.z * cos(psi);
  Y   = ( g.y*g.y - g.z*g.z ) * cos(2.*Phi) + 2. * g.y * g.z * sin(2*Phi);

  Phi4= F4 * S2 * fdeb * FF2 * 
	( delta * Y/gl2 - 4.*cos(rho) * Ga*Ga * La
	  - 2.*sin(rho) * De * La * gl/delta );

  Int = 2.*(1.-x0)*Phi4;
}
void intcoh_sum( void )
{
  double	emx = 1.-x0, Ga, Phi1, Phi2, s, t, gt2;

  Ga  = Gam();
  s   = sin(Th);							
  t   = g.y*cos(Al) + g.z*sin(Al); 
  gt2 = s*s * (g.x*g.x-t*t) + g.y*g.y + g.z*g.z;
  Phi1 = F1 * S2 * fdeb * FF2 * delta*gt2/gl2;
  Phi2 = F2 * S2 * fdeb * FF2 * Ga*Ga * Lambda();
  
  Int = (1.+emx*emx) * Phi1 - 2./3. * emx * Phi2;
}
void IntensityInc( void )
{
  double y0, U, dmy;
  do{
    y0   = Rran3( &Ry0 );
    x0   = Rran3( &Rx0 );
    psig = Rran3( &Rps );
    U    = Rran3( &RU );
    Int = int_inc( U ) + int_el( U );
    if(Int>IntTot)
      fprintf(file_log," mcb-inc> ERROR: int<IntTot;  Int=%f,  IntTot=%f\n",
	      Int, IntTot); 
    if( Int>IntMax )  IntMax = Int;
    if( Int<IntMin )  IntMin = Int;
    Nincident++;
    } while( y0 > Int );

  Nphoton++;
  thetag = U/E0;  
  tg  = v_add( v_setpol( thetag, M_PI_2, psig ), pe );
  v_cart2pol( tg, &thg, &dmy, &phg );
}
double intinc_hub_amo( double U )
{
  double psi1, psi2, Mv;
  double v  = 1./(1.+U*U),
         v2 = v*v,
         emx = 1. - x0,
         tmp1 = x0/(2.*E0*emx),
         tmp2 = Z3*v/SCREEN_DEBOF;

  Mv    = - log( tmp1*tmp1 + tmp2*tmp2 );
  psi1  =  4. * v2 * (Mv-1.);
  psi2  = 12. * v2 * ( 1. + (2.*Mv-8.)*(v-v2) );

  return ( (1. + emx*emx) * psi1 - 2./3. * emx * psi2 ) * U;
}

double intinc_hub_CDeb( double U )
{
  double psi1, psi2, Mv;
  double v  = 1./(1.+U*U),
         v2 = v*v,
         emx = 1. - x0,
         tmp1 = x0/(2.*E0*emx),
         tmp2 = Z3*v/CDeb;

  Mv    = - log( tmp1*tmp1 + tmp2*tmp2 );
  psi1  =  4. * v2 * (Mv-1.);
  psi2  = 12. * v2 * ( 1. + (2.*Mv-8.)*(v-v2) );
  
  return ( (1. + emx*emx) * psi1 - 2./3. * emx * psi2 ) * U;
}

double intinc_hub_MDeb( double U )
{
  double psi1, psi2, Mv;
  double v  = 1./(1.+U*U),
         v2 = v*v,
         emx = 1. - x0,
         tmp1 = x0/(2.*E0*emx),
         tmp2 = Z3*v/SCREEN_DEBOF;

  Mv    = - log( tmp1*tmp1 + tmp2*tmp2 );
  psi1  =  4. * v2 * (Mv-1.);
  psi2  = 12. * v2 * ( 1. + (2.*Mv-8.)*(v-v2) );
  
  return ( (1. + emx*emx) * psi1 - 2./3. * emx * psi2 ) * U * MDeb;
}

double intinc_hub_RDeb( double U )
{
  double psi1, psi2, Mv;
  double v  = 1./(1.+U*U),
         v2 = v*v,
         emx = 1. - x0,
         tmp1 = x0/(2.*E0*emx),
         tmp2 = Z3*v/SCREEN_DEBOF;

  Mv    = - log( tmp1*tmp1 + tmp2*tmp2 );
  psi1  =  4. * v2 * (Mv-1.);
  psi2  = 12. * v2 * ( 1. + (2.*Mv-8.)*(v-v2) );
  RDeb	= ( PDeb[0] + PDeb[1]*exp(PDeb[2]*x0) )
				* ( PDeb[3] + U/E0*(PDeb[4]+PDeb[5]*U/E0) );

  return ( (1. + emx*emx) * psi1 - 2./3. * emx * psi2 ) * U * RDeb;
}

double intinc_hub_ALL( double U )
{
  double		v  = 1./(1.+U*U);
  register double	emx = 1. - x0,
			v2 = v*v,
			red, Mv, 
			tmp1 = x0/(2.*E0*emx),
			tmp2 = Z3*v/screening;

  if( reduction==-1 )
		red = ( PDeb[0] + PDeb[1]*exp(PDeb[2]*x0) )
				  * ( PDeb[3] + U/E0*(PDeb[4]+PDeb[5]*U/E0) );
  else	red = reduction;
  Mv    = - log( tmp1*tmp1 + tmp2*tmp2 );

  return ( ((1. + emx*emx) * 4. * v2 * (Mv-1.)
	     - 2./3. * emx * 12.* v2 * ( 1. + (2.*Mv-8.)*(v-v2) ) )  
								 * U * red);
}
double Iincoh( double x, double E0)
{
 const double uc = pow( rcol/(coldist+0.15) * E0, 2 ),
              v  = 1./(1.+uc),
              v2 = v*v;
 double tmp = pow(x/(2.*E0*(1-x)),2),
        Mv, M1, b, c, psi1, psi2,
        emx = 1. - x;

 Mv = -log( tmp + pow(v*Z3/SCREEN_DEBOF,2.) );
 M1 = -log( tmp + pow(  Z3/SCREEN_DEBOF,2.) );
 b  = Z3*2.*E0 * (1-x)/x;
 c  = (2./b) * atan( b*(1.-v)/(1.+v*b*b) );
 psi1 = 2. * ( 1.+M1 - (1.+Mv)*v - c );
 psi2 = -(40./3.)*v2*v + 18.*v2 - (8./(b*b)+6.)*v + 8./(b*b) + 4./3.
        + (4.*v2*v-6.*v2)*Mv + 2.*M1 - (6./(b*b))*(Mv-M1+2./3.*c);

 return (1. + emx*emx) * psi1 - 2./3. * emx * psi2;
}
double intinc_bh( double U )
{
  double psi1 = 17.35,  psi2 = 16.54,  emx = 1. - x0;

  return ( (1. + emx*emx) * psi1 - 2./3. * emx * psi2 ) * 2.*U/pow((1+U*U),2.);
}
double intinc_el( double U )
{
  double psi1, psi2, eps, psieps,  emx = 1. - x0;
  
  eps = 100./(E0*Z3*Z3) * x0/emx;
  if( eps<=0.88 ){
	eps     = 0.88 - eps;
	psieps  = 19.70 + eps * ( 4.177 + eps * ( -3.806
			+ eps * ( 31.84 + eps * ( -58.63 + eps*40.77 ))));
	}
  else	psieps  = 19.19 - 4.*log(eps);
  psi1    = psieps - 4. - 8./3. * log(Z);
  psi2    = psi1 + 2./3.;
  return( (1. + emx*emx) * psi1 - 2./3. * emx * psi2 )*2.*U/pow((1.+U*U),2.)/Z;
}
int zyl_colli( void )
{
  VEC		xe;
  double	a2, b2;
  int		thru;

  xe = v_sub( re, coloff );		/* possible colli-offset (off-axis) */
  xe.z=0;
  a2 = v_sqr( v_add( xe, v_smul(coldist,tg) ) );
  b2 = v_sqr( v_add( xe, v_smul(coldist+collength,tg) ) );
  Ncollimator += ( thru = a2<rcol2 && b2<rcol2 );

  return thru;
}

int box_colli( void )
{
  VEC	x1, x2, xe;
  int		thru;

  xe = v_sub( re, coloff );		/* possible colli-offset (off-axis) */
  xe.z=0;
  x1 = v_add( xe, v_smul(coldist,tg) );
  x2 = v_add( xe, v_smul(coldist+collength,tg) );
  Ncollimator += ( thru = fabs(x1.x)<wcol && fabs(x2.x)<wcol &&
		          fabs(x1.y)<hcol && fabs(x2.y)<hcol );

  return thru;
}

/******************************************************************************
	atomic formfactor of C12
		    Carbon Formfactor by Wilson (valence)
		        Three parametrizations:
		        	low q: gauss,
		    	mid q:  polynomial and
		    	high q: dipole for 
    note: total ff = 1-f_carbon/6
 *****************************************************************************/
double atomformfactor( double q2 )
{
 double q=sqrt(q2), ff;

 if( Z != 6.0)  fprintf( file_log,
       "******** Warning: wrong usage of fct atomformfactor, Z!=6 *******\n" );

  if( q <= 0.1 )		/* wilson valence gauss parametrization */
    ff = 1. - ( 0.286977+2.26099*exp(-9635.97*q2)+1.56165*exp(-278.863*q2)
       + 1.05075*exp(-4143.12*q2)+0.839259*exp(-23609.3*q2) )/6.;
  else				/* wilson logarithmic polynomial param.*/
    if( q <= 0.3 )
      ff = 1. - exp( 1.7056 -32.3042*q + 50.5055*q2 -37.3811*q*q2 )/6.;
    else		/* normalized dipole ff (C=111) from there on */
					/* 3731 = C^2 * Z^(-2/3), Z=6,C=111 */
      ff = 1. - (3.94323/(1.+q2*3731.46))/6.;

 return ff;
}
/******************************************************************************
		read_parameter: reads parameter from input file 
******************************************************************************/
int read_parameter(void)
{
  FILE	*con, *file_debye;
  char	*s;
  int	n_parm,i_line,j_line;
  int	j=0;
  char	f_line[132];
  char	fnam_debye[100];
  char	err_mess[30] = "input ERROR line %3d:\n%s\n";
  double a,b,c,d;

  if( (con = fopen(fname_in,"r")) == NULL)  {
	fprintf( stderr, "\n\nFatal Error: input file %s not found!\n\n",
		 fname_in);
  	exit(-1);
	}
  if(strncmp(CODE,"mcb:",4)==0){
      fprintf( file_log,
	   "\n******************   MCB ROOT - log file  "
	   " ******************   29/03/05/pg\n\nreading file %s\n\n",
		 fname_in );
  }
  for(i_line=0,j_line=0; fgets(f_line,132,con)!=NULL; i_line++){
		if(f_line[0] == '#' ||	/* check if comment line */
		   f_line[0] == '*'){	/* check if comment line */
			continue;}	/* next loop if comment */
	if(j_line > 16 && strncmp(f_line,CODE,4) != 0){
			continue;}	/* not relevant for mcb */
	else if(j_line > 16){
	    s = (s=(char *)strchr(f_line,':')) != NULL ? s+1 : f_line;
	    sprintf(f_line,"%s",s);
	   }
     switch(j_line){
/* ****************************************************************************
   write comment line 							     */
	 case 0:
	     fprintf (file_log, "\nComment:\n%s\n", f_line);
	     break;
/* ****************************************************************************
   electron energy in [MeV]  ==> m_e					     */
	 case 1:
	     if((n_parm = sscanf(f_line,"%lf",&E0MeV))  < 1){
					/* check for format errors */
			fprintf(stderr,err_mess,i_line,f_line);
			exit(-1);	/* bad return after format error */
			}
	     energy = E0MeV/MASS_ELEC;	/* energy of the electron beam*/
	     fprintf (file_log, "energy = %f MeV\n",E0MeV);
	     break;
/* ****************************************************************************
   variance electron energy in [MeV]  ==> m_e				     */
	 case 2:
	     if((n_parm = sscanf(f_line,"%lf",&sigE0MeV))  < 1){
					/* check for format errors */
			fprintf(stderr,err_mess,i_line,f_line);
			exit(-1);	/* bad return after format error */
			}
	     sigE0 =sigE0MeV/MASS_ELEC;	/* energy spread of primary beam*/
	     fprintf (file_log, "energy spread (sigma) = %f MeV\n",sigE0MeV);
	     break;
/* ****************************************************************************
   lattice orientation Theta in [rad]  	       				     */
	 case 3:
	     if((n_parm = sscanf(f_line,"%lf",&Theta))  < 1){
					/* check for format errors */
			fprintf(stderr,err_mess,i_line,f_line);
			exit(-1);	/* bad return after format error */
			}
	     fprintf(file_log,"Theta = %f rad\n",Theta);
	     break;
/* ****************************************************************************
   lattice orientation Alpha in [rad]  	       				     */
	 case 4:
	     if((n_parm = sscanf(f_line,"%lf",&Alpha))  < 1){
					/* check for format errors */
			fprintf(stderr,err_mess,i_line,f_line);
			exit(-1);	/* bad return after format error */
			}
	     fprintf(file_log,"Alpha = %f rad\n",Alpha);
	     break;
/* ****************************************************************************
   lattice orientation Phi in [rad]  	       				     */
	 case 5:
	     if((n_parm = sscanf(f_line,"%lf",&Phi))  < 1){
					/* check for format errors */
			fprintf(stderr,err_mess,i_line,f_line);
			exit(-1);	/* bad return after format error */
			}
	     Kappa = Phi + Alpha;			/* 'own' definition */
	     fprintf(file_log,"Phi = %f rad\n",Phi);
	     break;				/* normally Phi=M_PI/4.0 */
/* ****************************************************************************
   horizontal beam spot size in [mm] ==> [m]  	   			     */
	 case 6:
	     if((n_parm = sscanf(f_line,"%lf",&sigx))  < 1){
					/* check for format errors */
			fprintf(stderr,err_mess,i_line,f_line);
			exit(-1);	/* bad return after format error */
			}
	     sigx *= 1.E-3;
	     fprintf(file_log,"sigx = %f m\n",sigx);
	     break;
/* ****************************************************************************
   vertical beam spot size in [mm] ==> [m]  				     */
	 case 7:
	     if((n_parm = sscanf(f_line,"%lf",&sigy))  < 1){
					/* check for format errors */
			fprintf(stderr,err_mess,i_line,f_line);
			exit(-1);	/* bad return after format error */
			}
	     sigy *= 1.E-3;
	     fprintf(file_log,"sigy = %f m\n",sigy);
	     break;
/* ****************************************************************************
   horizontal beam divergence in [mrad] ==> [rad]  			     */
	 case 8:
	     if((n_parm = sscanf(f_line,"%lf",&sigpx))  < 1){
					/* check for format errors */
			fprintf(stderr,err_mess,i_line,f_line);
			exit(-1);	/* bad return after format error */
			}
	     sigpx *= 1.E-3;
	     fprintf(file_log,"sigpx = %f rad\n",sigpx);
	     break;
/* ****************************************************************************
   vertical beam divergence in [mrad] ==> [rad]  			     */
	 case 9:
	     if((n_parm = sscanf(f_line,"%lf",&sigpy))  < 1){
					/* check for format errors */
			fprintf(stderr,err_mess,i_line,f_line);
			exit(-1);	/* bad return after format error */
			}
	     sigpy *= 1.E-3;
	     fprintf(file_log,"sigpy = %f rad\n",sigpy);
	     break;
/* ****************************************************************************
   thickness of diamond in [mm]	and of amorphous radiator foil [mm]	     */
	 case 10:
	     if((n_parm = 
		    sscanf(f_line,"%lf%lf",&thickn_dia,&thickn_amo)) < 2){
					/* check for format errors */
			fprintf(stderr,err_mess,i_line,f_line);
			exit(-1);	/* bad return after format error */
			}
	     fprintf(file_log,"radiator thickness diamond: %f mm",thickn_dia);
	     fprintf(file_log,"; amorphous foil: %f mm\n",thickn_amo);
	     break;
/* ****************************************************************************
   distance radiator-collimator in [m]					     */
	 case 11:
	     if((n_parm = sscanf(f_line,"%lf",&coldist))  < 1){
					/* check for format errors */
			fprintf(stderr,err_mess,i_line,f_line);
			exit(-1);	/* bad return after format error */
			}
	     if( coldist==0 ){
		 fprintf(file_log,
			 "input > ERROR: distance radiator to colli == 0 !!" );
		 exit(-1);	/* bad return after wrong input */
	     }
 	     fprintf(file_log,"distance radiator-collimator entrance = %f m\n",
		     coldist);
	     break;
/* ****************************************************************************
   length of collimator in [m]						     */
	 case 12:
	     if((n_parm = sscanf(f_line,"%lf",&collength))  < 1){
					/* check for format errors */
			fprintf(stderr,err_mess,i_line,f_line);
			exit(-1);	/* bad return after format error */
			}
	     fprintf(file_log,"length of collimator = %f m\n",collength);
	     if(strncmp(CODE,"anb:",4)==0){
		 coldist += collength/2.;
		 fprintf(file_log,
			 "effective distance radiator-collimator = %f m\n",
			 coldist);
	     }
	     break;
/* ****************************************************************************
   cylindrical or rectangular collimator
   sizes in [mm] ==> [m]						     */
	 case 13:
	     if((n_parm = sscanf(f_line,"%lf%lf",&wcol,&hcol))  < 2){
					/* check for format errors */
			fprintf(stderr,err_mess,i_line,f_line);
			exit(-1);	/* bad return after format error */
			}
	     if(strncmp(CODE,"mcb:",4)==0){
		 if( hcol == 0 ) {
		     rcol = wcol * 1.E-3;		/* [mm] -> [m] */
		     rcol2= rcol*rcol;
		     collimator = zyl_colli;
		     fprintf( file_log, "radius of cylindrical colli = %f m\n",
			      rcol );		}
		 else {
		     wcol *= 0.5E-3;	/* [mm] -> [m], use half widht/height*/
		     hcol *= 0.5E-3;	/* just as radius instead of diameter*/
		     collimator = box_colli;
		     fprintf(file_log,
			     "half widht/height of box colli = %f, %f m\n"
			     , wcol, hcol );
		 }
	     }
	     else{			/* for anb */
		 fprintf(file_log,"radius of collimator: %f mm,\n", wcol );
		 rcol = wcol * 1.E-3;		/* [mm] -> [m] */
	     }
	     break;
/* ****************************************************************************
   offset of collimator in x or y directions, z=0
   sizes in [mm] ==> [m]						     */
	 case 14:
	     if((n_parm = sscanf(f_line,"%lf%lf",&coloff.x,&coloff.y)) < 2){
					/* check for format errors */
			fprintf(stderr,err_mess,i_line,f_line);
			exit(-1);	/* bad return after format error */
			}
	     coloff.x *=  1.E-3;	/* possible colli-offset ==>[m]*/
	     coloff.y *=  1.E-3;	/* (off-axis) */
	     coloff.z=0;
	     fprintf(file_log,
			"(off-axis) collimator offset(x,y) = (%f,%f) m\n",
			coloff.x, coloff.y );
	     break;
/* ****************************************************************************
   crystal temperature in [K]						     */
	 case 15:
	     if((n_parm = sscanf(f_line,"%lf%s",
				    &temperature,fnam_debye))  < 2){
					/* check for format errors */
			fprintf(stderr,err_mess,i_line,f_line);
			exit(-1);	/* bad return after format error */
			}
	     fprintf(file_log,"temperature of diamond = %f Kelvin\n",
			temperature);
	     break;
/* ****************************************************************************
   Z of crystal and amorphous radiator					     */
	 case 16:
	     if((n_parm = sscanf(f_line,"%lf%lf",&Zcry,&Zamo))  < 2){
					/* check for format errors */
			fprintf(stderr,err_mess,i_line,f_line);
			exit(-1);	/* bad return after format error */
			}
	     fprintf( file_log,"using Z=%3.0f crystal radiator\n", Zcry );
	     fprintf( file_log,"using Z=%3.0f amorphous radiator\n", Zamo );
	     break;
/* ****************************************************************************
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
                 MCB  relevant 
   process/method of simulation						     */
	 case 17:
	     if((n_parm = sscanf(f_line,"%s",brprocs))  < 1){
					/* check for format errors */
			fprintf(stderr,err_mess,i_line,f_line);
			exit(-1);	/* bad return after format error */
			}
	     if(strcmp(mode,"inc")!=0) strcpy(brprocs,mode);
	     fprintf (file_log, "type of Bremsstrahlung: <%s>\n", brprocs );
	     break;
/* ****************************************************************************
   number of lattice vectors						     */
	 case 18:
	     if((n_parm = sscanf(f_line,"%d",&KLMax))  < 1){
					/* check for format errors */
			fprintf(stderr,err_mess,i_line,f_line);
			exit(-1);	/* bad return after format error */
			}
	     if( KLMax > KLMAX ) {
		 fprintf( file_log,
 "\nFATAL ERROR: Maximal index (k,l) of lattice vector may not exeed %i\n"
 "otherwise compile mcb with higher KLMAX\n", KLMAX );
		 exit(1);
		}
	     fprintf (file_log,
		 "Max Lattice vector index in [HKL]: -%d<=(k,l)<=%d\n",
		      KLMax, KLMax );
	     break;
/* ****************************************************************************
   number of photons to be simulated					     */
	 case 19:
	     if((n_parm = sscanf(f_line,"%lf",&NMax))  < 1){
					/* check for format errors */
			fprintf(stderr,err_mess,i_line,f_line);
			exit(-1);	/* bad return after format error */
			}
	     fprintf (file_log, "%f photon - events will be simulated\n",
			 NMax);
	     break;
/* ****************************************************************************
   choose maximal intensity,
   usually between 5 und 25, check log file				     */
	 case 20:
	     if((n_parm = sscanf(f_line,"%lf%lf%lf%lf",&a,&b,&c,&d))  < 1){
					/* check for format errors */
			fprintf(stderr,err_mess,i_line,f_line);
			exit(-1);	/* bad return after format error */
			}
	     if(n_parm == 1 ) {IntTot = a;}
	     else{
		 if(strcmp(mode,"amo")==0) IntTot = a;
		 else if(strcmp(mode,"inc")==0) IntTot = b;
		 else if(strcmp(mode,"sum")==0) IntTot = c;
		 else if(strcmp(mode,"dif")==0) IntTot = d;
		 else IntTot = a;
	     }
	     fprintf (file_log, "maximal intensity for MC integral %f\n",
			 IntTot);
	     break;
/* ****************************************************************************
    	     */
     }	 	 	 	 /* end switch */
     j_line++;
  }	 	 	 /* end for loop */
  fclose (con);

  if( (file_debye=fopen(fnam_debye,"r")) == NULL) {
	fprintf( file_log ,
	  "Fatal Error > unable to open data - file: <%s>\n", fnam_debye );
	exit( -1 );
	}
  fgets(comment,100,file_debye);
  fprintf( file_log, "Debye - paramter file comment:\n%s\n", comment );
  do {
      fscanf( file_debye, "%lf%lf%lf%lf",&a,&b,&c,&d);
      DebT[j]=a; DebScreen[j]=b;DebA[j]=c; DebMR[j]=d;
	j++;
	}  while( !feof( file_debye ) );
  fclose( file_debye );
  fprintf( file_log, "input > Debye data file <%s> read\n", fnam_debye );

  get_debye( temperature ); 
  fflush( file_log );
  return(0);
}

/******************************************************************************
  determines diamond temperature dependend values,
  e.g. debye waller factor, from table-interpolation	see debye.c    AN/9/98 
 *****************************************************************************/
void get_debye( double temp )			/* temperature in Kelvin */
{
  int		j=0;
  double	d;

  if( temp<TNULL ) temp = TNULL;		/* lowest entry in table:
						   0 degree Celsius */
  while( DebT[j] > temp )	j++;
  d = (temp-DebT[j-1]) / (DebT[j]-DebT[j-1]);
  Adebye = DebA[j-1]*(1.-d) + DebA[j]*d;	/* interpolate from table */
  CDeb  = DebScreen[j-1]*(1.-d) + DebScreen[j]*d;
  MDeb  = DebMR[j-1]*(1.-d) + DebMR[j]*d;

  fprintf( file_log, "get_debye > diamond-temperature dependend values:\n"
		     "get_debye > Adebye : %g\tScreening (CDeb): %g\n"
		     "get_debye > Mean Debye reduction (MDeb): %g\n",
		      Adebye, CDeb, MDeb );
}

/******************************************************************************
	update log-file and screen on number of processed events
******************************************************************************/
void infoprint ( void )
{
  static	int	freq = 10, less = 0;
  double 	elpsd;
  time_t	rest;
  struct tm 	*now;
  struct tm 	*end;
//  struct rusage ruse;

  if ((unsigned long)Nphoton%freq == 0) {
	if( Nphoton > 100 ) {
	  	time_now = time(NULL);
		elpsd= time_now-time_start;
		rest = (time_t)( elpsd * (NMax-Nphoton)/Nphoton ) + time_now;
	  	time_now = time(NULL);
		now  = localtime( &time_now );
		fprintf( stdout,
		 "Event Nr e/ph: %10.0f\t%9.0f %2u:%2u:%2u estimated end at",
		Nincident, Nphoton, now->tm_hour, now->tm_min, now->tm_sec );
		end  = localtime( &rest );
		fprintf( stdout," %2u:%2u:%2u\n",
				    end->tm_hour, end->tm_min, end->tm_sec );
		}
	else {
		fprintf( stdout,   "Event Nr e/ph: %10.0f\t%9.0f\n",
			 Nincident, Nphoton );
		}
	fprintf( file_log, "Event Nr e/ph: %10.0f\t%9.0f\n",
		 Nincident, Nphoton );
	fflush( file_log );	
	fflush( stdout );
	if( ++less >= 10 ) {	freq *=10; less=0;	}
	}
}

double dummy(double x)
{
  return 0;
}

void Rinit3( struct Rdat3 *r, char *text, double lo, double hi )
{
  r->scale = (hi-lo);
  r->offs  = lo;
  strcpy(r->label,text);
  fprintf(stderr, "Rinit3 %20s: scale %f, offset %f\n",
	  r->label,r->scale,r->offs);
}
double Rran3( struct Rdat3 *r )
{
    float rnd, tmp;
    rnd = unirndhp_(&tmp);
    tmp= r->offs + r->scale*rnd;
    return	r->offs + r->scale*rnd;
}


void Rinitg( struct Rdatg *g, char *text, double mean, double sigma )
{
  Rinit3( &g->xr3, text, -1, 1 );
  Rinit3( &g->yr3, text, -1, 1 );
  g->mean = mean;
  g->sig2 = sigma*sigma;
  g->set  = 0;
  strcpy(g->label,text);
  fprintf(stderr, "Rinitg %20s: mean %f, sigma %f\n",g->label,g->mean,sigma);
}

double Rgauss( struct Rdatg *g )
{
  double	fac, rsq, x, y;

  if( g->set == 0 )
  {
       do
       {
       	x = Rran3( &g->xr3 );
       	y = Rran3( &g->yr3 );
       	rsq = x*x + y*y;
       }
       while (rsq >= 1.0 || rsq == 0.0);
       fac  = sqrt( -2.0 * g->sig2 * log(rsq)/rsq );
       g->next = y*fac;
       g->set  = 1;
       return	x*fac + g->mean;
  }
  else
  {
        g->set = 0;
        return	g->next + g->mean;
  }
}

void Riniti( struct Rdati *r, char *text, int lo, int hi ) 
{
  r->scale = hi-lo+1;
  r->offs  = lo;
  strcpy(r->label,text);
  fprintf(stderr, "Riniti %20s: scale %d, offset %d\n",
	  r->label,r->scale,r->offs);
}

int Rrani(struct Rdati *r)
{
   int neu;
   float tmp;
   neu = r->offs + (int)( r->scale*unirndhp_(&tmp) );
   return neu;
}
/*****************************************
 Primitive 3/4 Dim Double Vektorroutinen
 als Library          (C) 90 FAN
 Neuauflage Feb 1994 (Lorents-Vecroutinen
 Schreibweise: 3D: v_.., 4D: V_..
 Version 'Mon Apr  6 13:09:42 MET DST 1998'
*****************************************/
/* ------ Library fuer 3D-Double-Vektor-Routinen --------------*/

VEC v_set(double  x, double y, double z )
{ VEC c; c.x=x; c.y=y; c.z=z; return( c ); }

VEC v_setpol(double  r, double th, double ph )
{VEC c; c=v_smul(r,v_set(sin(th)*cos(ph),sin(th)*sin(ph),cos(th))); return(c);}

VEC v_add(VEC  a, VEC b )
{ a.x+=b.x; a.y+=b.y; a.z+=b.z; return( a ); }

VEC v_sub(VEC  a, VEC b )
{ a.x-=b.x; a.y-=b.y; a.z-=b.z; return( a ); }

VEC v_smul(double s, VEC a )
{ a.x*=s; a.y*=s; a.z*=s; return( a ); }

double v_sqr(VEC  a ) 
{ return( a.x*a.x+a.y*a.y+a.z*a.z ); }

double v_norm( VEC a )
{ return( sqrt(v_sqr(a)) );}

double v_sprd( VEC a, VEC b )
{ return( a.x*b.x+a.y*b.y+a.z*b.z );}

VEC v_parallel( VEC a, VEC v )
{ v = v_normal( v );  return( v_smul( v_sprd(a,v), v ) );}

VEC v_normal( VEC a )
{ return( v_smul( 1/v_norm(a), a ) );}

void v_cart2pol(VEC  a, double *r,double *t,double *p )
{
  double	rt2 = a.x*a.x + a.y*a.y;
  if( (*r=sqrt(rt2+a.z*a.z)) < V_ZERO )
  {	*r = *t = *p = 0.;	return;	}
  *t = acos(a.z/(*r));
  if( rt2==0 ) {	*p = 0.;  return;	}
  if( a.y>=0. )	*p = acos(a.x/sqrt(rt2));
  else		*p = 2.*M_PI - acos(a.x/sqrt(rt2));
}
double sig_moliere (double s, double pe)
{
 double	om, b, B, thsq, thsq_cr;
// double Zcry = 6.0;
 double A_12C = 12.0;
 double A_58Ni = 58.0;
// double ALPHA = (1./137.03268);
 double ALPHA2 = (1./137.03268)*(1./137.03268);
 double DENS_DIAMOND = 3.513;
 double DENS_NICKEL = 8.908;         // g/cm^3
// double M_EulerC = 0.5772156649;

 s /= 10.;						/* change to [cm] */
 /*  this writing is equivalent to the second one !
 om = 7795.6 * (1.+Zcry)*pow(Zcry,1./3.) * DENS_DIAMOND*s
 	  / ( A_12C * (1.+3.32743*(Zcry*Zcry*ALPHA2) ) );
 b    = log(om) + 1. - 2.*M_EulerC;
 --------------------------------------------------------------------------- */
 if(amo_mode){
     om = 6680. * (1.+Zamo)*pow(Zamo,1./3.) * DENS_NICKEL*s
 	  / ( A_58Ni * (1.+3.32743*(Zamo*Zamo*ALPHA2) ) );
     b    = log(om);
     B    = solveeq( b );
     thsq_cr = 0.15692 * Zamo*(1.+Zamo) * DENS_NICKEL*s / (A_58Ni*pe*pe);
     thsq = thsq_cr * (B - 1.2 );
 /* thsq = thsq_cr * (B - 1.2 ); */
/* fprintf(stderr,"  s %f, p %f angle %f\n",s,pe,thsq*(B-1.2) );] */
 return	sqrt( thsq );
 }
 else{
     om = 6680. * (1.+Zcry)*pow(Zcry,1./3.) * DENS_DIAMOND*s
 	  / ( A_12C * (1.+3.32743*(Zcry*Zcry*ALPHA2) ) );
     b    = log(om);
     B    = solveeq( b );
     thsq_cr = 0.15692 * Zcry*(1.+Zcry) * DENS_DIAMOND*s / (A_12C*pe*pe);
     thsq = thsq_cr * (B - 1.2 );
 /* thsq = thsq_cr * (B - 1.2 ); */
/* fprintf(stderr,"  s %f, p %f angle %f\n",s,pe,thsq*(B-1.2) );] */
 return	sqrt( thsq );
 }
}

/******************************************************************************
	solve B - log(B) = b
	using regula falsi method
******************************************************************************/
double solveeq ( double b )
{
 double	f_r, f_l, f_next, re=40.0, li=1.2, next;

 f_r = re - log(re) - b;
 f_l = li - log(li) - b;
 while( re-li > 1.E-9 ) {
   next   = (re+li)/2;
   f_next = next - log(next) - b;
   if(f_next<=0.0) {
     li =next;
     f_l=f_next;
     }
   else {
     re =next;
     f_r=f_next;
     }
   }

 return	next;
}

