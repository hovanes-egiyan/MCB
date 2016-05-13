#ifndef _TUserMCBInp_h
#define _TUserMCBInp_h

#include "TObject.h"

class TUserMCBInp : public TObject {

 public:
  char bremsprocs[5];		// amo,inC,inR,ihb,sum,dif
  double Zcry;			// charge number of crystal
  double Zamo;			// charge number of amorphous foil
  double thickn_dia;		// radiator thickness diamond in [mm]
  double thickn_amo;		// 	and of amorphous radiator foil [mm]
  double Theta;			// lattice orientation Theta in [rad]
  double Alpha;			// lattice orientation Alpha in [rad]
  double Phi;			// lattice orientation Phi in [rad]
  double E0;			// electron energy in [MeV]
  double sigE0;			// variance electron energy in [MeV]
  double sigx;			// horizontal beam spot size in  [m]
  double sigy;			// vertical beam spot size in  [m]
  double sigpx;			// horizontal beam divergence in [rad]
  double sigpy;			// vertical beam divergence in [rad]
  double coldist;		// distance radiator-collimator in [m]
  double collength;		// length of collimator in [m]
  double col1;			// hcol==0: zyl_colli :: hcol>1 box_colli
  double col2;			// zyl_colli:  col1=rcol, col2=nan  in [m]
				// box_colli:  col1=wcol, col2=hcol in [m]
  double coloff_x;		// (off-axis) collimator offset: x in [m]
  double coloff_y;		// (off-axis) collimator offset: y in [m]
  int KLMax;			// Max Lattice vector used
  double temperature;		// temperature of diamond in [K]
  double IntTot;		// maximal intensity for MC integral

  TUserMCBInp();
  virtual ~TUserMCBInp();

  ClassDef(TUserMCBInp, 1);
};

#endif
