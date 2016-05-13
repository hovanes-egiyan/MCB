#ifndef _TUserMCBOut_h
#define _TUserMCBOut_h

#include "TObject.h"

class TUserMCBOut : public TObject {

 public:
  char bremsprocs[5];		// amo,inC,inR,ihb,sum,dif
  int MaxH;			// max lattice index for H
  int MaxKL;			// max lattice index for K,L
  double IntensityMin;		// min intensity
  double IntensityMax;		// max intensity
  double IntensityTot;		// max value set by input
  double NormVolume;		// random volume for normalization
  double no_Electrons;		// no of electrons used in run
  double no_Photons;		// no of photons, preset
  double no_OutOfBounds;	// no of photons out of range of code
  double no_CollPhotons;	// no of photons within collimator
  int RandomVectors[25];	// 25 random vectors
  
  TUserMCBOut();
  virtual ~TUserMCBOut();
   
  ClassDef(TUserMCBOut, 1);
};

#endif
