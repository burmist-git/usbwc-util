//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Wed Nov 12 12:31:49 2014 by ROOT version 5.32/04
// from TTree T/USBWC Data Tree
// found on file: ../root_data/Run_11_Data_10_15_2014_Ascii/Run_11_Data_10_15_2014_Ascii.root
//////////////////////////////////////////////////////////

#ifndef vis_h
#define vis_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.

// Fixed size dimensions of array or collections stored in the TTree if any.

class vis {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   Int_t           eventID;
   Float_t         SamplingPeriod;
   Double_t        UnixTime;
   Double_t        TDC;
   Float_t         ch[8][1024];
   Float_t         Baseline[8];
   Float_t         Amplitude[8];
   Float_t         Charge[8];
   Float_t         LeadingEdgeTime[8];
   Float_t         TrailingEdgeTime[8];
   Float_t         RateCounter[8];

   // List of branches
   TBranch        *b_eventID;   //!
   TBranch        *b_SamplingPeriod;   //!
   TBranch        *b_UnixTime;   //!
   TBranch        *b_TDC;   //!
   TBranch        *b_ch;   //!
   TBranch        *b_Baseline;   //!
   TBranch        *b_Amplitude;   //!
   TBranch        *b_Charge;   //!
   TBranch        *b_LeadingEdgeTime;   //!
   TBranch        *b_TrailingEdgeTime;   //!
   TBranch        *b_RateCounter;   //!

   //vis(TTree *tree=0);
   vis(TString fileN);
   virtual ~vis();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
   void DrawAllChannels(Int_t evID);
   void DrawOneChannel(Int_t evID, Int_t chID;);
   void DrawOneChannel_waveform(Int_t evID, Int_t chID);
   void DrawOneChannel_waveformSimple(Int_t evID, Int_t chID, TString key_lineOn);
   void DrawOneChannel_Mamy(Int_t evID, Int_t chID);
};

#endif

#ifdef vis_cxx
vis::vis(TString fileN) : fChain(0) 
{
  TTree *tree=0;
  // if parameter tree is not specified (or zero), connect the file
  // used to generate this class and read the Tree.
  if (tree == 0) {
    TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject(fileN.Data());
    if (!f || !f->IsOpen()) {
      f = new TFile(fileN.Data());
    }
    f->GetObject("T",tree);   
  }
  Init(tree);
}

vis::~vis()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t vis::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t vis::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void vis::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("eventID", &eventID, &b_eventID);
   fChain->SetBranchAddress("SamplingPeriod", &SamplingPeriod, &b_SamplingPeriod);
   fChain->SetBranchAddress("UnixTime", &UnixTime, &b_UnixTime);
   fChain->SetBranchAddress("TDC", &TDC, &b_TDC);
   fChain->SetBranchAddress("ch", ch, &b_ch);
   fChain->SetBranchAddress("Baseline", Baseline, &b_Baseline);
   fChain->SetBranchAddress("Amplitude", Amplitude, &b_Amplitude);
   fChain->SetBranchAddress("Charge", Charge, &b_Charge);
   fChain->SetBranchAddress("LeadingEdgeTime", LeadingEdgeTime, &b_LeadingEdgeTime);
   fChain->SetBranchAddress("TrailingEdgeTime", TrailingEdgeTime, &b_TrailingEdgeTime);
   fChain->SetBranchAddress("RateCounter", RateCounter, &b_RateCounter);
   Notify();
}

Bool_t vis::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void vis::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t vis::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef vis_cxx
