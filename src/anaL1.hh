#ifndef anaL1_h
#define anaL1_h

//root
#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

class anaL1 {
public :

  anaL1(TString name);
  anaL1(TString name, Int_t executeModeID);
  ~anaL1();
  void anaWFfillRootFile(TString fileAnaSetups,TString outRootFileF);

private :

  static const Int_t USBWCnChannels = 8;
  static const Int_t USBWCnSamplesPoint = 1024;

  TTree          *fChain;
  Int_t           fCurrent;
  
  // Declaration of leaf types
  Int_t           eventID;
  Float_t         SamplingPeriod;
  Double_t        UnixTime;
  Double_t        TDC;
  Float_t         ch[USBWCnChannels][USBWCnSamplesPoint];
  Float_t         Baseline[USBWCnChannels];
  Float_t         Amplitude[USBWCnChannels];
  Float_t         Charge[USBWCnChannels];
  Float_t         LeadingEdgeTime[USBWCnChannels];
  Float_t         TrailingEdgeTime[USBWCnChannels];
  Float_t         RateCounter[USBWCnChannels];

  
  // List of branches
  TBranch        *b_eventID;
  TBranch        *b_SamplingPeriod;
  TBranch        *b_UnixTime;
  TBranch        *b_TDC;
  TBranch        *b_ch;
  TBranch        *b_Baseline;
  TBranch        *b_Amplitude;
  TBranch        *b_Charge;
  TBranch        *b_LeadingEdgeTime;
  TBranch        *b_TrailingEdgeTime;
  TBranch        *b_RateCounter;  

  Int_t    GetEntry(Long64_t entry);
  Long64_t LoadTree(Long64_t entry);
  void     Init(TTree *tree);
  void     Loop();
};

#endif
