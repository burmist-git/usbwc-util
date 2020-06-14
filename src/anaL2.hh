///////////////////////////////
//                           //
//        08.05.2014         //
//                           //
///////////////////////////////
#ifndef anaL2_hh
#define anaL2_hh

//root
#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TH1D.h>
#include <TGraph.h>

class anaL2 {
public :

  static const Int_t USBWCnChannels = 8;
  static const Int_t USBWCnSamplesPoint = 1024;

  TTree          *fChain;
  Int_t           fCurrent;
  
  // Declaration of leaf types
  Int_t           eventIDL1;
  Double_t        unixTimeL1;
  Double_t        TDCL1;
  Float_t         baselineUSBWC[USBWCnChannels];
  Float_t         amplitudeUSBWC[USBWCnChannels];
  Float_t         chargeUSBWC[USBWCnChannels];
  Float_t         leadingEdgeTimeUSBWC[USBWCnChannels];
  Float_t         trailingEdgeTimeUSBWC[USBWCnChannels];
  Float_t         rateCounterUSBWC[USBWCnChannels];
  Float_t         amplMax[USBWCnChannels];
  Float_t         amplMaxTime[USBWCnChannels];
  Float_t         amplMin[USBWCnChannels];
  Float_t         amplMinTime[USBWCnChannels];
  Float_t         amplBaseline[USBWCnChannels];
  Float_t         timeCFD[USBWCnChannels];
  Float_t         timeConstTh[USBWCnChannels];
  Float_t         sigRiseT[USBWCnChannels];
  Float_t         sigWidth[USBWCnChannels];
  Float_t         sigFallT[USBWCnChannels];
  Float_t         sigChargeInWindowVT[USBWCnChannels];
  
  // List of branches
  TBranch        *b_eventIDL1;   //!
  TBranch        *b_unixTimeL1;   //!
  TBranch        *b_TDCL1;   //!
  TBranch        *b_baselineUSBWC;   //!
  TBranch        *b_amplitudeUSBWC;   //!
  TBranch        *b_chargeUSBWC;   //!
  TBranch        *b_leadingEdgeTimeUSBWC;   //!
  TBranch        *b_trailingEdgeTimeUSBWC;   //!
  TBranch        *b_rateCounterUSBWC;   //!
  TBranch        *b_amplMax;   //!
  TBranch        *b_amplMaxTime;   //!
  TBranch        *b_amplMin;   //!
  TBranch        *b_amplMinTime;   //!
  TBranch        *b_amplBaseline;   //!
  TBranch        *b_timeCFD;   //!
  TBranch        *b_timeConstTh;   //!
  TBranch        *b_sigRiseT;   //!
  TBranch        *b_sigWidth;   //!
  TBranch        *b_sigFallT;   //!
  TBranch        *b_sigChargeInWindowVT;   //!

  anaL2(TString nameF);
  ~anaL2();
  virtual Int_t    GetEntry(Long64_t entry);
  virtual Long64_t LoadTree(Long64_t entry);
  virtual void     Init(TTree *tree);
  virtual void     Loop(TString nameF, Int_t runID);
  // for run 0
  void Loop0(TString nameF);
  void LoopTS(TString nameF, TString nameFTS);
  virtual void     Show(Long64_t entry = -1);
  void h1D1Init(TH1D *h1D1[USBWCnChannels],TString h1name, TString h1Title,
		Int_t Nbin, Float_t Vmin, Float_t Vmax);
  void tGraphInit(TGraph *gr[USBWCnChannels], TString grName, TString grTitle);
  void h2D2Init(TH2D *h1D1[USBWCnChannels],TString h1name, TString h1Title,
		Int_t Nbin1, Float_t Vmin1, Float_t Vmax1,
		Int_t Nbin2, Float_t Vmin2, Float_t Vmax2);

  void tProfInit(TProfile *tprof[USBWCnChannels],TString h1name, TString h1Title,
		 Int_t Nbin1, Float_t Vmin1, Float_t Vmax1);

  Double_t GetUnixTimeStart();
  Double_t GetUnixTimeStop();

  Double_t GetBA1511Gain(Double_t high_v);

};
#endif
