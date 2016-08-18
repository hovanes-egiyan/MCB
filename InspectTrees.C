#include <iostream>

#include "TROOT.h"
#include "TSystem.h"
#include "TH2D.h"
#include "TH2I.h"
#include "TTree.h"
#include "TChain.h"
#include "TF1.h"

#include "src/TUserMCBInp.h"
#include "src/TUserMCBOut.h"

using namespace std;
//Declare objects with global scope
//Files and Trees
TFile* fdif;
TChain *tdif;
TTree *ttdif;
TFile* fsum;
TChain *tsum;
TTree *ttsum;
TFile* famo;
TChain *tamo;
TTree *ttamo;
TFile* finc;
TChain *tinc;
TTree *ttinc;

//Histogram weights
Double_t Weight_dif;
Double_t Weight_sum;
Double_t Weight_amo;
Double_t Weight_inc;

//1D histograms
TH1F* hpara1D;
TH1F* hperp1D;
TH1F* hsum1D;
TH1F* hdif1D;
TH1F* hinc1D;  
TH1F* hamo1D;  
TH1F* hCrystI1D;
TH1F* hCrystR1D;
TH1F* hpol1D;

//2D histograms
TH2F* hpara2D;
TH2F* hperp2D;
TH2F* hsum2D;
TH2F* hdif2D;
TH2F* hinc2D;
TH2F* hamo2D;  
TH2F* hCrystI2D;
TH2F* hCrystR2D;
TH2F* hpol2D;


void OpenFiles(){
  //Load TUserMCBOut library to allow calculation of weights
  // gSystem->Load("libTUserMCBOut.so");
  //gSystem->Load("libTUserMCBInp.so");
  gSystem->Load("/home/hovanes/GlueX/MCB/linux/obj/libTUserMCBIO.so");
  TUserMCBOut* InfoOut_amo;
  TUserMCBOut* InfoOut_inc;
  TUserMCBOut* InfoOut_sum;
  TUserMCBOut* InfoOut_dif;

  Double_t renorm=1000;
  //Open files, get trees, get OutInfo, calc. weights
  fdif=new TFile("/home/hovanes/GlueX/MCB/vgl010.dif.root");
  ttdif=(TTree*)fdif->Get("mcb_tree");
  ttdif->SetNameTitle("tdif","tdif");
  TList* treelist=ttdif->GetUserInfo();
  InfoOut_dif=(TUserMCBOut*)treelist->At(1);
  Weight_dif=InfoOut_dif->NormVolume *  renorm/InfoOut_dif->no_Electrons;
  
  fsum=new TFile("/home/hovanes/GlueX/MCB/vgl010.sum.root");
  ttsum=(TTree*)fsum->Get("mcb_tree");
  ttsum->SetNameTitle("tsum","tsum");
  treelist=ttsum->GetUserInfo();
  InfoOut_sum=(TUserMCBOut*)treelist->At(1);
  Weight_sum=InfoOut_sum->NormVolume *  renorm/InfoOut_sum->no_Electrons;

  famo=new TFile("/home/hovanes/GlueX/MCB/vgl010.amo.root");
  ttamo=(TTree*)famo->Get("mcb_tree");
  ttamo->SetNameTitle("tamo","tamo");
  treelist=ttamo->GetUserInfo();
  InfoOut_amo=(TUserMCBOut*)treelist->At(1);
  Weight_amo=InfoOut_amo->NormVolume *  renorm/InfoOut_amo->no_Electrons;

  finc=new TFile("/home/hovanes/GlueX/MCB/vgl010.inc.root");
  ttinc=(TTree*)finc->Get("mcb_tree");
  ttinc->SetNameTitle("tinc","tinc");
  treelist=ttinc->GetUserInfo();
  InfoOut_inc=(TUserMCBOut*)treelist->At(1);
  Weight_inc=InfoOut_inc->NormVolume *  renorm/InfoOut_inc->no_Electrons;

}
void ChainFiles(Int_t NFiles,TString vgl){
  //Chains together input files
  //Nfile=nu,ber of files
  //seeds=seeds of files to be analysed eg. {"1","2","3"...}
  //vgl=vgl input file e.g. vgl001

  //Load TUserMCBOut library to allow calculation of weights
  //gSystem->Load("libTUserMCBOut.so");
  //gSystem->Load("libTUserMCBInp.so");
  gSystem->Load("/home/hovanes/GlueX/MCB/linux/obj/libTUserMCBIO.so");
  TUserMCBOut* InfoOut_amo;
  TUserMCBOut* InfoOut_inc;
  TUserMCBOut* InfoOut_sum;
  TUserMCBOut* InfoOut_dif;

  Double_t renorm=1000;
  //Open files, get trees, get OutInfo, calc. weights
  tdif=new TChain("mcb_tree");
  tsum=new TChain("mcb_tree");
  tamo=new TChain("mcb_tree");
  tinc=new TChain("mcb_tree");
    
  for(Int_t i=0;i<NFiles;i++){
//    TString Seed(TString("seed")+seeds[i]);
//     tdif->Add(TString("/home/hovanes/GlueX/MCB/")+vgl+TString(".dif.")+Seed+TString(".root"));
//     tsum->Add(TString("/home/hovanes/GlueX/MCB/")+vgl+TString(".sum.")+Seed+TString(".root"));
//     tamo->Add(TString("/home/hovanes/GlueX/MCB/")+vgl+TString(".amo.")+Seed+TString(".root"));
//     tinc->Add(TString("/home/hovanes/GlueX/MCB/")+vgl+TString(".inc.")+Seed+TString(".root"));
   //  TFile* ftest;
//     if(!(ftest=TFile::Open(TString("/home/hovanes/GlueX/MCB/")+vgl+TString(".dif.")+Seed+TString(".root")))){cout<<"no "<<vgl+TString(".dif.")+Seed<<endl; ftest->Close();delete ftest;continue;}
//     ftest->Close();delete ftest;
//     if(!(ftest=TFile::Open(TString("/home/hovanes/GlueX/MCB/")+vgl+TString(".sum.")+Seed+TString(".root")))){cout<<"no "<<vgl+TString(".dif.")+Seed<<endl; ftest->Close();delete ftest;continue;}
//     ftest->Close();delete ftest;
//     if(!(ftest=TFile::Open(TString("/home/hovanes/GlueX/MCB/")+vgl+TString(".amo.")+Seed+TString(".root")))){cout<<"no "<<vgl+TString(".dif.")+Seed<<endl; ftest->Close();delete ftest;continue;}
//     ftest->Close();delete ftest;
//     if(!(ftest=TFile::Open(TString("/home/hovanes/GlueX/MCB/")+vgl+TString(".inc.")+Seed+TString(".root")))){cout<<"no "<<vgl+TString(".dif.")+Seed<<endl; ftest->Close();delete ftest;continue;}
//     ftest->Close();delete ftest;

    tdif->Add(TString("/home/hovanes/GlueX/MCB/")+vgl+TString(".dif")+TString(".root"));
    tsum->Add(TString("/home/hovanes/GlueX/MCB/")+vgl+TString(".sum")+TString(".root"));
    tamo->Add(TString("/home/hovanes/GlueX/MCB/")+vgl+TString(".amo")+TString(".root"));
    tinc->Add(TString("/home/hovanes/GlueX/MCB/")+vgl+TString(".inc")+TString(".root"));


    cout<<"ADDED "<<TString("/home/hovanes/GlueX/MCB/")+vgl+TString(".dif")+TString(".root")<<endl;
    //Add the TUserMCBOut
    TFile *fi;
    TTree *tr;
    TUserMCBOut* InfoOut;
    //    fi=new TFile(TString("/home/hovanes/GlueX/MCB/")+vgl+TString(".dif.")+Seed+TString(".root"));
    fi=new TFile(TString("/home/hovanes/GlueX/MCB/")+vgl+TString(".dif")+TString(".root"));
    tr=(TTree*)fi->Get("mcb_tree");
    TList* treelist=tr->GetUserInfo();
    InfoOut=(TUserMCBOut*)treelist->At(1);
//    cout<<"InfoOut "<<InfoOut->bremsprocs<<" "<<InfoOut->IntensityMin<<endl;
    if(i==0)InfoOut_dif=new TUserMCBOut(*(TUserMCBOut*)treelist->At(1));
    cout<<"InfoOut_dif "<<InfoOut_dif->bremsprocs<<" "<<InfoOut_dif->IntensityMin<<endl;
//    else InfoOut_dif->Add(*(TUserMCBOut*)treelist->At(1));
    fi->Close();

    //    fi=new TFile(TString("/home/hovanes/GlueX/MCB/")+vgl+TString(".sum.")+Seed+TString(".root"));
    fi=new TFile(TString("/home/hovanes/GlueX/MCB/")+vgl+TString(".sum")+TString(".root"));
    tr=(TTree*)fi->Get("mcb_tree");
    treelist=tr->GetUserInfo();
    if(i==0)InfoOut_sum=new TUserMCBOut(*(TUserMCBOut*)treelist->At(1));
    cout<<"InfoOut_sum "<<InfoOut_sum->bremsprocs<<" "<<InfoOut_sum->IntensityMin<<endl;
//    else InfoOut_sum->Add(*(TUserMCBOut*)treelist->At(1));
    fi->Close();

    //    fi=new TFile(TString("/home/hovanes/GlueX/MCB/")+vgl+TString(".amo.")+Seed+TString(".root"));
    fi=new TFile(TString("/home/hovanes/GlueX/MCB/")+vgl+TString(".amo")+TString(".root"));
    tr=(TTree*)fi->Get("mcb_tree");
    treelist=tr->GetUserInfo();
    if(i==0)InfoOut_amo=new TUserMCBOut(*(TUserMCBOut*)treelist->At(1));
    cout<<"InfoOut_amo "<<InfoOut_amo->bremsprocs<<" "<<InfoOut_amo->IntensityMin<<endl;
//    else InfoOut_amo->Add(*(TUserMCBOut*)treelist->At(1));
    fi->Close();

    //    fi=new TFile(TString("/home/hovanes/GlueX/MCB/")+vgl+TString(".inc.")+Seed+TString(".root"));
    fi=new TFile(TString("/home/hovanes/GlueX/MCB/")+vgl+TString(".inc")+TString(".root"));
    tr=(TTree*)fi->Get("mcb_tree");
    treelist=tr->GetUserInfo();
    if(i==0)InfoOut_inc=new TUserMCBOut(*(TUserMCBOut*)treelist->At(1));
    cout<<"InfoOut_inc "<<InfoOut_inc->bremsprocs<<" "<<InfoOut_inc->IntensityMin<<endl;
//    else InfoOut_inc->Add(*(TUserMCBOut*)treelist->At(1));
    fi->Close();
  }
  tdif->SetNameTitle("tdif","tdif");
  tsum->SetNameTitle("tsum","tsum");
  tamo->SetNameTitle("tamo","tamo");
  tinc->SetNameTitle("tinc","tinc");

  Weight_dif=InfoOut_dif->NormVolume *  renorm/InfoOut_dif->no_Electrons;
  Weight_sum=InfoOut_sum->NormVolume *  renorm/InfoOut_sum->no_Electrons;
  Weight_amo=InfoOut_amo->NormVolume *  renorm/InfoOut_amo->no_Electrons;
  Weight_inc=InfoOut_inc->NormVolume *  renorm/InfoOut_inc->no_Electrons;


}
void AddFileToChain(){
}

void DisplayInfoIn(TTree* tree){
  TUserMCBInp* InfoIn;
  TList* treelist=tree->GetUserInfo();
  InfoIn=(TUserMCBInp*)treelist->At(0);

  cout<<"Zcry "<< InfoIn->Zcry<<endl;
  cout<<"Zamo "<< InfoIn->Zamo<<endl;
  cout<<"thickn_dia "<< InfoIn->thickn_dia<<endl;
  cout<<"thickn_amo "<< InfoIn->thickn_amo<<endl;
  cout<<"Theta "<< InfoIn->Theta<<endl;
  cout<<"Alpha "<< InfoIn->Alpha<<endl;
  cout<<"Phi "<< InfoIn->Phi<<endl;
  cout<<"E0 "<< InfoIn->E0<<endl;
  cout<<"sigE0 "<< InfoIn->sigE0<<endl;
  cout<<"sigx "<< InfoIn->sigx<<endl;
  cout<<"sigy "<< InfoIn->sigy<<endl;
  cout<<"sigpx "<< InfoIn->sigpx<<endl;
  cout<<"sigpy "<< InfoIn->sigpy<<endl;
  cout<<"coldist "<< InfoIn->coldist<<endl;
  cout<<"collength "<< InfoIn->collength<<endl;
  cout<<"col1 "<< InfoIn->col1<<endl;
  cout<<"col2 "<< InfoIn->col2<<endl;
  cout<<"coloff_x "<< InfoIn->coloff_x<<endl;
  cout<<"coloff_y "<< InfoIn->coloff_y<<endl;
  cout<<"KLMax "<< InfoIn->KLMax<<endl;
  cout<<"temperature "<< InfoIn->temperature<<endl;
  cout<<"IntTot "<< InfoIn->IntTot<<endl;


}
void CloseFiles(){
  fdif->Close();
  fsum->Close();
  famo->Close();
  finc->Close();
}

//Function to Draw 1D histogram given a tree, histogram name, variable, cut, binning
TH1F* Draw1DHist(TTree *t,TString hisname,TString var,TCut Cut,Int_t Nbins,Double_t Min,Double_t Max){
  hisname=t->GetName()+TString("_")+hisname;
  TH1F* his=new TH1F(hisname,hisname,Nbins,Min,Max);
  his->Sumw2();
  t->Draw(var+TString(">>")+hisname,Cut,"goff");
  return his;
}

//Bad Name!
//Get the construct the basic spectra, then derive the rest
//e.g.ConstructParaPerp1D("Eg","Eg","(col==1)*(-1*(intensity<0)+(intensity>0))",200,0,900)
//note need the intesity into give right weight
TH1F* ConstructParaPerp1D(TString hisname,TString var,TCut Cut,Int_t Nbins,Double_t Min,Double_t Max,Double_t IntMin,Double_t IntMax){
  TDirectory * dir=gDirectory;
  gROOT->cd();
  hsum1D=Draw1DHist(tsum,hisname,var,Cut,Nbins,Min,Max);
  hdif1D=Draw1DHist(tdif,hisname,var,Cut,Nbins,Min,Max);
  hinc1D=Draw1DHist(tinc,hisname,var,Cut,Nbins,Min,Max);
  hamo1D=Draw1DHist(tamo,hisname,var,Cut,Nbins,Min,Max);
  cout<<"Weight diff "<<Weight_dif<<" sum "<<Weight_sum<<" amo "<<Weight_amo<<" inc "<<Weight_inc<<endl;
  hsum1D->Scale(Weight_sum);
  //hdif1D->Scale(-Weight_dif);
  hdif1D->Scale(Weight_dif);
  hamo1D->Scale(Weight_amo);
  hinc1D->Scale(Weight_inc);
  
  hamo1D->SetNameTitle(hisname+TString("Amo"),hisname+TString("Amo"));
  hinc1D->SetNameTitle(hisname+TString("Inc"),hisname+TString("Inc"));
  hsum1D->SetNameTitle(hisname+TString("Sum"),hisname+TString("Sum"));
  hdif1D->SetNameTitle(hisname+TString("Dif"),hisname+TString("Dif"));
  dir->cd();
  hamo1D->Write();//Write
  hinc1D->Write();//Write
  hsum1D->Write();//Write
  hdif1D->Write();//Write

  gROOT->cd();
  hpara1D=new TH1F(*hsum1D);
  hperp1D=new TH1F(*hsum1D);

  hpara1D->Add(hdif1D,-1);
  hperp1D->Add(hdif1D,1);

  hCrystI1D=new TH1F(*hinc1D);
  hCrystI1D->SetNameTitle(hisname+TString("CrystI"),hisname+TString("CrystI"));
  hCrystI1D->Add(hsum1D,1);
  hCrystR1D=new TH1F(*hCrystI1D);
  hCrystR1D->Scale(1/hCrystR1D->Integral(hCrystR1D->GetXaxis()->FindFixBin(IntMin),hCrystR1D->GetXaxis()->FindFixBin(IntMax)));
  hCrystR1D->SetNameTitle(hisname+TString("CrystR"),hisname+TString("CrystR"));
  hamo1D->Scale(1/hamo1D->Integral(hamo1D->GetXaxis()->FindFixBin(IntMin),hamo1D->GetXaxis()->FindFixBin(IntMax)));
  hCrystR1D->Divide(hamo1D);
  hpol1D=new TH1F(*hdif1D);
  hpol1D->SetNameTitle(hisname+TString("Pol"),hisname+TString("Pol"));
  hpol1D->Divide(hCrystI1D);
  
  dir->cd();
  hpol1D->Write();//Write

  return hCrystR1D;
}
TH2F* Draw2DHist(TTree *t,TString hisname,TString var,TCut Cut,Int_t NbinsX,Double_t MinX,Double_t MaxX,Int_t NbinsY,Double_t MinY,Double_t MaxY){
  hisname=t->GetName()+TString("_")+hisname;
  TH2F* his=new TH2F(hisname,hisname,NbinsX,MinX,MaxX,NbinsY,MinY,MaxY);
  his->Sumw2();
  t->Draw(var+TString(">>")+hisname,Cut,"goff");
  return his;
}
//ConstructParaPerp2D("YvXEg2","17000*sin(thg)*cos(phg):17000*sin(thg)*sin(phg)","(col==1)*(-1*(intensity<0)+(intensity>0))&&(Eg>100&&Eg<160)",40,-20,20,40,-20,20);
 TH2F* ConstructParaPerp2D(TString hisname,TString var,TCut Cut,Int_t NbinsX,Double_t MinX,Double_t MaxX,Int_t NbinsY,Double_t MinY,Double_t MaxY,Double_t IntMinX,Double_t IntMaxX,Double_t IntMinY,Double_t IntMaxY){
  gROOT->cd();
  hsum2D=Draw2DHist(tsum,hisname,var,Cut,NbinsX,MinX,MaxX,NbinsY,MinY,MaxY);
  hdif2D=Draw2DHist(tdif,hisname,var,Cut,NbinsX,MinX,MaxX,NbinsY,MinY,MaxY);
  hinc2D=Draw2DHist(tinc,hisname,var,Cut,NbinsX,MinX,MaxX,NbinsY,MinY,MaxY);
  hamo2D=Draw2DHist(tamo,hisname,var,Cut,NbinsX,MinX,MaxX,NbinsY,MinY,MaxY);
  cout<<"Weight diff "<<Weight_dif<<" sum "<<Weight_sum<<" amo "<<Weight_amo<<" inc "<<Weight_inc<<endl;
  hsum2D->Scale(Weight_sum);
  hdif2D->Scale(-Weight_dif);
  hamo2D->Scale(Weight_amo);
  hinc2D->Scale(Weight_inc);

  hpara2D=new TH2F(*hsum2D);
  hperp2D=new TH2F(*hsum2D);

  hpara2D->Add(hdif2D,-1);
  hperp2D->Add(hdif2D,1);

  hCrystI2D=new TH2F(*hinc2D);
  hCrystI2D->SetNameTitle(hisname+TString("CrystI"),hisname+TString("CrystI"));
  hCrystI2D->Add(hsum2D,1);
  hCrystR2D=new TH2F(*hCrystI2D);
  hCrystR2D->Scale(1/hCrystR2D->Integral(hCrystR2D->GetXaxis()->FindFixBin(IntMinX),hCrystR2D->GetXaxis()->FindFixBin(IntMaxX),hCrystR2D->GetYaxis()->FindFixBin(IntMinY),hCrystR2D->GetYaxis()->FindFixBin(IntMaxY)));
  hCrystR2D->SetNameTitle(hisname+TString("CrystR"),hisname+TString("CrystR"));
  hamo2D->Scale(1/hamo2D->Integral(hamo2D->GetXaxis()->FindFixBin(IntMinX),hamo2D->GetXaxis()->FindFixBin(IntMaxX),hamo2D->GetYaxis()->FindFixBin(IntMinY),hamo2D->GetYaxis()->FindFixBin(IntMaxY)));
  hCrystR2D->Divide(hamo2D);
  hpol2D=new TH2F(*hdif2D);
  hpol2D->SetNameTitle(hisname+TString("Pol"),hisname+TString("Pol"));
  hpol2D->Divide(hCrystI2D);

  return hCrystR2D;
}
TF1* AmorphousFit(TH1F* his){
  TF1* amob=new TF1("amob","((1+(1-x/855)*(1-x/855))*13.79-0.6666667*(1-x/855)*13.12)*[0]",1,800);
  his->Fit("amob");
  return amob;
}
