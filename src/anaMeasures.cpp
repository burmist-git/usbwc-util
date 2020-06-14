#include "anaMeasures.hh"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TString.h>

//C, C++
#include <iostream>
#include <stdlib.h>
#include <assert.h>
#include <fstream>
#include <iomanip>
#include <time.h>

using namespace std;

anaMeasures::anaMeasures(TString fileN){
  TTree *tree = 0;
  ifstream ff(fileN.Data());
  if (ff.good()) {
    ff.close();
  }
  else {
    cout<<"  ERROR ---> file "<<fileN.Data()<<" does not exist"<<endl;
    assert(0);    
  }
  if (tree == 0) {
    TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject(fileN.Data());
    if (!f) {
      f = new TFile(fileN.Data());
    }
    tree = (TTree*)gDirectory->Get("T");   
  }
  Init(tree);
}

anaMeasures::~anaMeasures(){
  if (!fChain) return;
  delete fChain->GetCurrentFile();
}

void anaMeasures::GetFirstLastUnixTime(Double_t &UnixTimeFirst, Double_t &UnixTimeLast){
  if (fChain == 0) return;
  Long64_t nentries = fChain->GetEntriesFast();
  //cout<<" nentries = "<<nentries<<endl;
  Long64_t nbytes = 0, nb = 0;
  Long64_t jentry=0;
  Long64_t ientry = LoadTree(jentry);
  nb = fChain->GetEntry(jentry);   nbytes += nb;
  UnixTimeFirst = UnixTime;
  jentry=nentries-1;
  ientry = LoadTree(jentry);
  nb = fChain->GetEntry(jentry);   nbytes += nb;
  UnixTimeLast = UnixTime;
}

void anaMeasures::h1D1Init(TH1D *h1D1[USBWCnChannels],TString h1name, TString h1Title,
			   Int_t Nbin, Float_t Vmin, Float_t Vmax){
  Int_t i;
  TString h1nameh;
  TString h1Titleh;
  for(i = 0;i<USBWCnChannels;i++){    
    h1nameh = h1name; h1nameh += "_"; h1nameh += "ch_"; h1nameh += i;
    h1Titleh = h1Title; h1nameh += " "; h1Titleh += "ch "; h1Titleh += i;
    h1D1[i] = new TH1D(h1nameh.Data(), h1Titleh.Data(),
                       Nbin, Vmin, Vmax);
  }
}

void anaMeasures::Loop0(TString histoOutName){
  const Int_t nChannels = USBWCnChannels;
  Int_t i = 0;
  cout<<"Loop0"<<endl;
  Double_t UnixTimeFirst; 
  Double_t UnixTimeLast;
  Double_t UnixTimeBinStart;
  Double_t UnixTimeBinStop;
  GetFirstLastUnixTime(UnixTimeFirst, UnixTimeLast);
  UnixTimeBinStart = UnixTimeFirst - 100;
  UnixTimeBinStop  = UnixTimeLast  + 100;
  Int_t nUTbins = (UnixTimeLast - UnixTimeFirst); 
  //printf ("UnixTimeFirst : %14.3f \n", UnixTimeFirst);
  //printf ("UnixTimeLast  : %14.3f \n", UnixTimeLast);
  TH1D *h1_UT = new TH1D("h1_UT","UT",nUTbins,UnixTimeBinStart,UnixTimeBinStop);
  TH1D *h1_Amplitude[nChannels];
  TH1D *h1_Charge[nChannels];
  TH1D *h1_LeadingEdgeTime[nChannels];
  TH1D *h1_deltaLeadingEdgeTime[nChannels];
  h1D1Init(h1_Amplitude,"h1_Amplitude", "Amplitude", 50000, -1.4, 1.4);
  h1D1Init(h1_Charge,"h1_Charge", "Charge", 1000, -10.0, 10.0);
  h1D1Init(h1_LeadingEdgeTime,"h1_LeadingEdgeTime", "LeadingEdgeTime", 1000, 0.0, 380);
  h1D1Init(h1_deltaLeadingEdgeTime,"h1_deltaLeadingEdgeTime", "deltaLeadingEdgeTime", 1000, -1.0, 1.0);
  if (fChain == 0) return;
  Long64_t nentries = fChain->GetEntriesFast();
  cout<<" nentries = "<<nentries<<endl;
  Long64_t nbytes = 0, nb = 0;
  for (Long64_t jentry=0; jentry<nentries;jentry++) {
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);   nbytes += nb;
    h1_UT->Fill(UnixTime);
    for(i = 0;i<nChannels;i++){
      h1_Amplitude[i]->Fill(Amplitude[i]);
      h1_Charge[i]->Fill(Charge[i]);
      h1_LeadingEdgeTime[i]->Fill(LeadingEdgeTime[i]);
      h1_deltaLeadingEdgeTime[i]->Fill(LeadingEdgeTime[0] - LeadingEdgeTime[i]);
    }
  }
  TFile* rootFile = new TFile(histoOutName.Data(), "RECREATE", " Histograms", 1);
  rootFile->cd();
  if (rootFile->IsZombie()){
    cout<<"  ERROR ---> file "<<histoOutName.Data()<<" is zombi"<<endl;
    assert(0);
  }
  else
    cout<<"  Output Histos file ---> "<<histoOutName.Data()<<endl;
  h1_UT->Write();
  for(i = 0;i<nChannels;i++){
    h1_Amplitude[i]->Write();
    h1_Charge[i]->Write();
    h1_LeadingEdgeTime[i]->Write();
    h1_deltaLeadingEdgeTime[i]->Write();
  }
}

Int_t anaMeasures::GetEntry(Long64_t entry){
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}

Long64_t anaMeasures::LoadTree(Long64_t entry){
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (!fChain->InheritsFrom(TChain::Class()))  return centry;
   TChain *chain = (TChain*)fChain;
   if (chain->GetTreeNumber() != fCurrent) {
      fCurrent = chain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void anaMeasures::Init(TTree *tree){
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);
   fChain->SetBranchAddress("eventID", &eventID, &b_eventID);
   fChain->SetBranchAddress("UnixTime", &UnixTime, &b_UnixTime);
   fChain->SetBranchAddress("TDC", &TDC, &b_TDC);
   fChain->SetBranchAddress("MeasuredBaseline", MeasuredBaseline, &b_MeasuredBaseline);
   fChain->SetBranchAddress("Amplitude", Amplitude, &b_Amplitude);
   fChain->SetBranchAddress("Charge", Charge, &b_Charge);
   fChain->SetBranchAddress("LeadingEdgeTime", LeadingEdgeTime, &b_LeadingEdgeTime);
   fChain->SetBranchAddress("TrailingEdgeTime", TrailingEdgeTime, &b_TrailingEdgeTime);
   fChain->SetBranchAddress("RateCounter", RateCounter, &b_RateCounter);
   Notify();
}

Bool_t anaMeasures::Notify(){
   return kTRUE;
}

void anaMeasures::Show(Long64_t entry){
   if (!fChain) return;
   fChain->Show(entry);
}

Int_t anaMeasures::Cut(Long64_t entry){
   return 1;
}
