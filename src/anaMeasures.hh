//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Sat Oct 18 20:46:10 2014 by ROOT version 5.18/00b
//////////////////////////////////////////////////////////

#ifndef anaMeasures_h
#define anaMeasures_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TString.h>
#include <TH1D.h>

class anaMeasures {
public :

  static const Int_t USBWCnChannels = 8;

  TTree          *fChain;   //!pointer to the analyzed TTree or TChain
  Int_t           fCurrent; //!current Tree number in a TChain
  
  // Declaration of leaf types
  Int_t           eventID;
  Double_t        UnixTime;
  Double_t        TDC;
  Float_t         MeasuredBaseline[USBWCnChannels];
  Float_t         Amplitude[USBWCnChannels];
  Float_t         Charge[USBWCnChannels];
  Float_t         LeadingEdgeTime[USBWCnChannels];
  Float_t         TrailingEdgeTime[USBWCnChannels];
  Float_t         RateCounter[USBWCnChannels];
  
  // List of branches
  TBranch        *b_eventID;   //!
  TBranch        *b_UnixTime;   //!
  TBranch        *b_TDC;   //!
  TBranch        *b_MeasuredBaseline;   //!
  TBranch        *b_Amplitude;   //!
  TBranch        *b_Charge;   //!
  TBranch        *b_LeadingEdgeTime;   //!
  TBranch        *b_TrailingEdgeTime;   //!
  TBranch        *b_RateCounter;   //!
  
  anaMeasures(TString fileN);
  ~anaMeasures();
  Int_t Cut(Long64_t entry);
  Int_t GetEntry(Long64_t entry);
  Long64_t LoadTree(Long64_t entry);
  void Init(TTree *tree);
  void Loop0(TString histoOutName);
  void GetFirstLastUnixTime(Double_t &UnixTimeFirst, Double_t &UnixTimeLast);
  Bool_t Notify();
  void Show(Long64_t entry = -1);
  void h1D1Init(TH1D *h1D1[USBWCnChannels],TString h1name, TString h1Title,
		Int_t Nbin, Float_t Vmin, Float_t Vmax);
};

#endif
