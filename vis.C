#define vis_cxx
#include "vis.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TGraph.h>
#include "./wfSimulation/src/wfSim.hh";
#include "./wfSimulation/src/waveform.hh";
#include "./wfSimulation/src/waveformSimple.hh";

void vis::DrawOneChannel_waveformSimple(Int_t evID, Int_t chID, TString key_lineOn, TString key_printInfoTrue){
   if (fChain == 0) return;
   Long64_t nentries = fChain->GetEntriesFast();
   Long64_t nbytes = 0, nb = 0;
   Long64_t jentry = evID;
   Long64_t ientry = LoadTree(jentry);
   nb = fChain->GetEntry(jentry);   nbytes += nb;
   Int_t i = 0;
   const Int_t np = 1024;
   const Int_t nCh = 8;
   Int_t npBaseLine = 20;
   Double_t tLeft  = 30;
   Double_t tRight = 150;
   Double_t vVal = 100;
   Double_t* wfT = new Double_t[np];
   Double_t* wfA = new Double_t[np];
   for(i = 0;i<np;i++){
     wfA[i] = ch[chID][i];
     wfT[i] = i*SamplingPeriod/1000.0;
   }
   waveformSimple *wf = new waveformSimple( wfT, wfA, np, npBaseLine);
   wf->findPar(tLeft,tRight,vVal);
   wf->Draw(key_lineOn);
   if(key_printInfoTrue == "printInfoTrue"){
     wf->printPar();
   }
}

void vis::DrawOneChannel_Mamy(Int_t evID, Int_t chID){
  const Int_t nn = 2;
  TGraph *gr;
  Int_t j = 0;
  //for(Int_t j = 0; j<nn; j++){
  if (fChain == 0) return;
  Long64_t nentries = fChain->GetEntriesFast();
  Long64_t nbytes = 0, nb = 0;
  Long64_t jentry = evID;
  Long64_t ientry = LoadTree(jentry);
  nb = fChain->GetEntry(jentry);   nbytes += nb;
  Int_t i = 0;
  const Int_t np = 1024;
  const Int_t nCh = 8;
  Float_t t[np];
  Float_t A[np];
  for(i = 0;i<np;i++){
    A[i] = ch[chID][i];
    t[i] = i*SamplingPeriod/1000.0;
  }
  gr = new TGraph(np,t,A); 
  //}
  TCanvas *c1 = new TCanvas("c1","canva",10,10,1200,800);
  //c1->Divide(1,5);
  //for(i = 0;i<nn;j++){
  //c1->cd(i+1);
  gr->Draw("APL");
  //}
}

void vis::DrawOneChannel_waveform(Int_t evID, Int_t chID)
{
   if (fChain == 0) return;
   Long64_t nentries = fChain->GetEntriesFast();
   Long64_t nbytes = 0, nb = 0;
   Long64_t jentry = evID;
   Long64_t ientry = LoadTree(jentry);
   nb = fChain->GetEntry(jentry);   nbytes += nb;
   Int_t i = 0;
   const Int_t np = 1024;
   const Int_t nCh = 8;
   Double_t* wfT = new Double_t[np];
   Double_t* wfA = new Double_t[np];
   for(i = 0;i<np;i++){
     wfA[i] = ch[chID][i];
     wfT[i] = i*SamplingPeriod/1000.0;
   }
   waveform *wf = new waveform( wfT, wfA, np, "yes");
   wf->findParametersOftheWaveform();
   wf->Draw(0.5);
}

void vis::DrawOneChannel(Int_t evID, Int_t chID)
{
   if (fChain == 0) return;
   Long64_t nentries = fChain->GetEntriesFast();
   Long64_t nbytes = 0, nb = 0;
   Long64_t jentry = evID;
   Long64_t ientry = LoadTree(jentry);
   nb = fChain->GetEntry(jentry);   nbytes += nb;
   Int_t i = 0;
   const Int_t np = 1024;
   const Int_t nCh = 8;
   Float_t t[np];
   Float_t A[np];
   for(i = 0;i<np;i++){
     A[i] = ch[chID][i];
     t[i] = i*SamplingPeriod/1000.0;
   }
   TString grtitle;
   grtitle  = " EventID = "; grtitle += evID;
   grtitle += " chID = "; grtitle += chID;
   TGraph *gr = new TGraph(np,t,A); 
   gr->SetTitle(grtitle.Data());
   gr->GetXaxis()->SetTitle("Time, ns");
   gr->GetYaxis()->SetTitle("Amplitude, V");
   TCanvas *c1 = new TCanvas("c1","canva",10,10,1200,800);
   gr->Draw("APL");
}

void vis::DrawAllChannels(Int_t evID)
{
   if (fChain == 0) return;
   Long64_t nentries = fChain->GetEntriesFast();
   Long64_t nbytes = 0, nb = 0;
   Long64_t jentry = evID;
   Long64_t ientry = LoadTree(jentry);
   nb = fChain->GetEntry(jentry);   nbytes += nb;

   Int_t i = 0;
   const Int_t np = 1024;
   const Int_t nCh = 8;

   Float_t t[np];
   Float_t A0[np];
   Float_t A1[np];
   Float_t A2[np];
   Float_t A3[np];
   Float_t A4[np];
   Float_t A5[np];
   Float_t A6[np];
   Float_t A7[np];

   for(i = 0;i<np;i++){
     A0[i] = ch[0][i];
     A1[i] = ch[1][i];
     A2[i] = ch[2][i];
     A3[i] = ch[3][i];
     A4[i] = ch[4][i];
     A5[i] = ch[5][i];
     A6[i] = ch[6][i];
     A7[i] = ch[7][i];
     t[i] = i*SamplingPeriod/1000.0;
   }

   TGraph *gr[nCh];
   gr[0] = new TGraph(np,t,A0);
   gr[1] = new TGraph(np,t,A1);
   gr[2] = new TGraph(np,t,A2);
   gr[3] = new TGraph(np,t,A3);
   gr[4] = new TGraph(np,t,A4);
   gr[5] = new TGraph(np,t,A5);
   gr[6] = new TGraph(np,t,A6);
   gr[7] = new TGraph(np,t,A7);
 
   TCanvas *c1 = new TCanvas("c1","canva",10,10,1200,800);
   c1->Divide(2,4);
   for(i=0;i<nCh;i++){
     c1->cd(i+1);
     gr[i]->Draw("APL");
   }

}


void vis::Loop()
{
//   In a ROOT session, you can do:
//      Root > .L vis.C
//      Root > vis t
//      Root > t.GetEntry(12); // Fill t data members with entry number 12
//      Root > t.Show();       // Show values of entry 12
//      Root > t.Show(16);     // Read and show values of entry 16
//      Root > t.Loop();       // Loop on all entries
//

//     This is the loop skeleton where:
//    jentry is the global entry number in the chain
//    ientry is the entry number in the current Tree
//  Note that the argument to GetEntry must be:
//    jentry for TChain::GetEntry
//    ientry for TTree::GetEntry and TBranch::GetEntry
//
//       To read only selected branches, Insert statements like:
// METHOD1:
//    fChain->SetBranchStatus("*",0);  // disable all branches
//    fChain->SetBranchStatus("branchname",1);  // activate branchname
// METHOD2: replace line
//    fChain->GetEntry(jentry);       //read all branches
//by  b_branchname->GetEntry(ientry); //read only this branch
   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntriesFast();
   cout<<"nentries = "<<nentries<<endl;
   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;
   }
}
