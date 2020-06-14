#ifndef anaL1Bin_h
#define anaL1Bin_h

//root
#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

class anaL1Bin {
public :

  anaL1Bin(TString name);
  anaL1Bin(TString name, Int_t executeModeID);
  ~anaL1Bin();
  void anaWFfillRootFile(TString fileAnaSetups,TString outRootFileF);
  
private :
  
  static const Int_t USBWCnChannels = 8;
  static const Int_t USBWCnSamplesPoint = 1024;
  
  TTree          *fChain;
  Int_t           fCurrent;
  
  // Declaration of leaf types
  Int_t           EventNumber_usbwc;
  Float_t         SamplingPeriod_usbwc;
  Double_t        EpochTime_usbwc;
  Int_t           Year_usbwc;
  Int_t           Month_usbwc;
  Int_t           Day_usbwc;
  Int_t           Hour_usbwc;
  Int_t           Minute_usbwc;
  Int_t           Second_usbwc;
  Int_t           Millisecond_usbwc;
  Int_t           channel_usbwc;
  Int_t           EventIDsamIndex_usbwc;
  Int_t           FirstCellToPlotsamIndex_usbwc;
  Float_t         MeasuredBaseline_usbwc[USBWCnChannels];
  Float_t         AmplitudeValue_usbwc[USBWCnChannels];
  Float_t         ComputedCharge_usbwc[USBWCnChannels];
  Float_t         RiseTimeInstant_usbwc[USBWCnChannels];
  Float_t         FallTimeInstant_usbwc[USBWCnChannels];
  Float_t         RawTriggerRate_usbwc[USBWCnChannels];
  Short_t         amplValues_usbwc[USBWCnChannels][USBWCnSamplesPoint];

  Int_t _nPointBaseLine[USBWCnChannels];
  Double_t _sigConstTh[USBWCnChannels];
  Double_t _cfdVal[USBWCnChannels];
  Double_t _chargeWinL[USBWCnChannels];
  Double_t _chargeWinR[USBWCnChannels];
  Int_t _nSplinePoints[USBWCnChannels];
  Int_t _sigPolarity[USBWCnChannels];
   
  void readAnaSetup(TString fileAnaSetups);
  
  // List of branches
  TBranch        *b_EventNumber_usbwc;   //!
  TBranch        *b_SamplingPeriod_usbwc;   //!
  TBranch        *b_EpochTime_usbwc;   //!
  TBranch        *b_Year_usbwc;   //!
  TBranch        *b_Month_usbwc;   //!
  TBranch        *b_Day_usbwc;   //!
  TBranch        *b_Hour_usbwc;   //!
  TBranch        *b_Minute_usbwc;   //!
  TBranch        *b_Second_usbwc;   //!
  TBranch        *b_Millisecond_usbwc;   //!
  TBranch        *b_channel_usbwc;   //!
  TBranch        *b_EventIDsamIndex_usbwc;   //!
  TBranch        *b_FirstCellToPlotsamIndex_usbwc;   //!
  TBranch        *b_MeasuredBaseline_usbwc;   //!
  TBranch        *b_AmplitudeValue_usbwc;   //!
  TBranch        *b_ComputedCharge_usbwc;   //!
  TBranch        *b_RiseTimeInstant_usbwc;   //!
  TBranch        *b_RawTriggerRate_usbwc;   //!
  TBranch        *b_amplValues_usbwc;   //!

  Int_t    GetEntry(Long64_t entry);
  Long64_t LoadTree(Long64_t entry);
  void     Init(TTree *tree);
  void     Loop();
};

#endif
