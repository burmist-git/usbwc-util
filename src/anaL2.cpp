//my
#include "anaL2.hh"

//root
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TFile.h>
#include <TProfile.h>
#include <TGraph.h>

//C, C++
#include <iostream>
#include <stdlib.h>
#include <assert.h>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <time.h>
#include <string>
#include <stdio.h>
#include <map>

using namespace std;

anaL2::anaL2(TString nameF) : fChain(0) 
{
  TTree *tree = 0;

  ifstream ff(nameF.Data());
  if (ff.good()) {
    ff.close();
  }
  else {
    cout<<"  ERROR ---> file "<<nameF.Data()<<" does not exist"<<endl;
    assert(0);    
  }
  TFile *f = new TFile(nameF.Data());
  f->GetObject("T",tree);  
  Init(tree);
}

anaL2::~anaL2(){
  if (!fChain) return;
  delete fChain->GetCurrentFile();
}

void anaL2::h1D1Init(TH1D *h1D1[USBWCnChannels],TString h1name, TString h1Title,
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

Double_t anaL2::GetBA1511Gain(Double_t high_v){
  if(high_v==900.0){
    return 1.87e+6;
  }
  else if(high_v==950.0){
    return 2.76e+6;
  }
  else if(high_v==1000.0){
    return 3.98e+6;
  }
  else if(high_v==1050.0){
    return 5.95e+6;
  }
  else if(high_v==1100.0){
    return 8.06e+6;
  }
  else{
    cout<<" ERROR: no gain data for this H.V."<<endl
	<<"        H.V. = "<<high_v<<endl;
    assert(0);
  }
  return -999;
}

void anaL2::Loop(TString nameF, Int_t runID){
  if(runID == 0){
    Loop0(nameF);
  }
  else{
    assert(0);
  }
}

void anaL2::tGraphInit(TGraph *gr[USBWCnChannels], TString grName, TString grTitle){
  Int_t i;
  TString grNameh;
  TString grTitleh;
  for(i = 0;i<USBWCnChannels;i++){
    grNameh = grName; grNameh += "_"; grNameh += "ch_"; grNameh += i;
    grTitleh = grTitle; grTitleh += " "; grTitleh += "ch "; grTitleh += i;
    gr[i] = new TGraph();
    gr[i]->SetTitle(grTitleh.Data());
    gr[i]->SetName(grNameh.Data());
  }
}

void anaL2::Loop0(TString nameF){
  Double_t utStop  = GetUnixTimeStop();
  Double_t utStart = GetUnixTimeStart();
  Double_t deltaUT = utStop - utStart;
  Double_t dT_trigg_m_CpFM;
  //cout<<"utStop - utStart = "<<utStop - utStart<<endl;  
  Int_t i = 0;  
  TH1D *h1_baselineUSBWC[USBWCnChannels];
  TH1D *h1_amplitudeUSBWC[USBWCnChannels];
  TH1D *h1_chargeUSBWC[USBWCnChannels];
  TH1D *h1_leadingEdgeTimeUSBWC[USBWCnChannels];
  TH1D *h1_deltaLeadingEdgeTimeUSBWC[USBWCnChannels];
  TH1D *h1_trailingEdgeTimeUSBWC[USBWCnChannels];
  TH1D *h1_rateCounterUSBWC[USBWCnChannels];
  h1D1Init(h1_baselineUSBWC,"h1_baselineUSBWC","baselineUSBWC",400,-1.5,1.5);
  h1D1Init(h1_amplitudeUSBWC,"h1_amplitudeUSBWC","amplitudeUSBWC",4000,-1.5,1.5);
  h1D1Init(h1_chargeUSBWC,"h1_chargeUSBWC","chargeUSBWC",40000,-1000.0,1000);
  h1D1Init(h1_leadingEdgeTimeUSBWC,"h1_leadingEdgeTimeUSBWC","leadingEdgeTimeUSBWC",400,0.0,2500);
  h1D1Init(h1_deltaLeadingEdgeTimeUSBWC,"h1_deltaLeadingEdgeTimeUSBWC","deltaLeadingEdgeTimeUSBWC",40000,-2.0,2.0);
  h1D1Init(h1_trailingEdgeTimeUSBWC,"h1_trailingEdgeTimeUSBWC","trailingEdgeTimeUSBWC",400,0.0,2500);
  h1D1Init(h1_rateCounterUSBWC,"h1_rateCounterUSBWC","rateCounterUSBWC",1000,0.0,10000);
  TGraph *gr_rateCounterUSBWC[USBWCnChannels];
  tGraphInit(gr_rateCounterUSBWC, "gr_rateCounterUSBWC", "rateCounterUSBWC");
  TH1D *h1_amplMax[USBWCnChannels];
  TH1D *h1_amplMaxTime[USBWCnChannels];
  TH1D *h1_amplMin[USBWCnChannels];
  TH1D *h1_amplMinTime[USBWCnChannels];
  TH1D *h1_amplBaseline[USBWCnChannels];
  TH1D *h1_timeCFD[USBWCnChannels];
  TH1D *h1_timeConstTh[USBWCnChannels];
  TH1D *h1_sigRiseT[USBWCnChannels];
  TH1D *h1_sigWidth[USBWCnChannels];
  TH1D *h1_sigFallT[USBWCnChannels];
  TH1D *h1_sigChargeInWindowVT[USBWCnChannels];
  TH1D *h1_sigChargeInWindowVT_npe[USBWCnChannels];
  TH1D *h1_deltaTimeCFD[USBWCnChannels];
  h1D1Init(h1_amplMax,"h1_amplMax","amplMax",20000,-0.1,1.5);
  h1D1Init(h1_amplMaxTime,"h1_amplMaxTime","amplMaxTime",800,0.0,2500);
  h1D1Init(h1_amplMin,"h1_amplMin","amplMin",100,-1.5,0.1);
  h1D1Init(h1_amplMinTime,"h1_amplMinTime","amplMinTime",800,0.0,2500);
  h1D1Init(h1_amplBaseline,"h1_amplBaseline","amplBaseline",100,-1.5,1.5);
  h1D1Init(h1_timeCFD,"h1_timeCFD","timeCFD",40000,0.0,2500);
  h1D1Init(h1_deltaTimeCFD,"h1_deltaTimeCFD","delta TimeCFD",40000,-2000.0,2000.0);
  h1D1Init(h1_timeConstTh,"h1_timeConstTh","timeConstTh",800,0.0,2500);
  h1D1Init(h1_sigRiseT,"h1_sigRiseT","sigRiseT",4000,0.0,2000);
  h1D1Init(h1_sigWidth,"h1_sigWidth","sigWidth",4000,0.0,2000);
  h1D1Init(h1_sigFallT,"h1_sigFallT","sigFallT",4000,0.0,2000);
  h1D1Init(h1_sigChargeInWindowVT,"h1_sigChargeInWindowVT","sigChargeInWindowVT",10000,-1,10);
  h1D1Init(h1_sigChargeInWindowVT_npe,"h1_sigChargeInWindowVT_npe","sigChargeInWindowVT npe",200,-1,10);
  TH1D *h1_amplMax_cut[USBWCnChannels];
  TH1D *h1_timeCFD_cut[USBWCnChannels];
  TH1D *h1_timeConstTh_cut[USBWCnChannels];
  TH1D *h1_sigRiseT_cut[USBWCnChannels];
  TH1D *h1_sigWidth_cut[USBWCnChannels];
  TH1D *h1_sigFallT_cut[USBWCnChannels];
  TH1D *h1_deltaTimeCFD_cut[USBWCnChannels];
  h1D1Init(h1_amplMax_cut,"h1_amplMax_cut","amplMax cut",20000,-0.1,1.5);
  h1D1Init(h1_timeCFD_cut,"h1_timeCFD_cut","timeCFD cut",40000,0.0,2500);
  h1D1Init(h1_timeConstTh_cut,"h1_timeConstTh_cut","timeConstTh cut",800,0.0,2500);
  h1D1Init(h1_sigRiseT_cut,"h1_sigRiseT_cut","sigRiseT cut",4000,0.0,2000);
  h1D1Init(h1_sigWidth_cut,"h1_sigWidth_cut","sigWidth cut",4000,0.0,2000);
  h1D1Init(h1_sigFallT_cut,"h1_sigFallT_cut","sigFallT cut",4000,0.0,2000);
  h1D1Init(h1_deltaTimeCFD_cut,"h1_deltaTimeCFD_cut","delta TimeCFD cut",40000,-2000.0,2000.0);
  TH2D *h2_amplMax_ch0_vs_ch1 = new TH2D("h2_amplMax_ch0_vs_ch1","amplMax ch0 vs ch1",400,-0.1,1.5,400,-0.1,1.5);
  TH2D *h2_amplMax_ch0_vs_dUT = new TH2D("h2_amplMax_ch0_vs_dUT","amplMax ch0 vs dUT",deltaUT,0,deltaUT,400,-0.1,1.5);
  TH2D *h2_amplMax_ch1_vs_dUT = new TH2D("h2_amplMax_ch1_vs_dUT","amplMax ch1 vs dUT",deltaUT,0,deltaUT,400,-0.1,1.5);
  Long64_t nentries = fChain->GetEntriesFast();
  cout<<"nentries = "<<nentries<<endl;
  Long64_t nbytes = 0, nb = 0;
  for (Long64_t jentry=0; jentry<nentries;jentry++) {
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);   nbytes += nb;
    // if (Cut(ientry) < 0) continue;
    dT_trigg_m_CpFM = timeCFD[0] - timeCFD[1];
    for(i = 0;i<USBWCnChannels;i++){
      h1_baselineUSBWC[i]->Fill(baselineUSBWC[i]);
      h1_amplitudeUSBWC[i]->Fill(amplitudeUSBWC[i]);
      h1_chargeUSBWC[i]->Fill(chargeUSBWC[i]);
      h1_leadingEdgeTimeUSBWC[i]->Fill(leadingEdgeTimeUSBWC[i]);
      h1_deltaLeadingEdgeTimeUSBWC[i]->Fill(leadingEdgeTimeUSBWC[i] - leadingEdgeTimeUSBWC[4]);
      h1_trailingEdgeTimeUSBWC[i]->Fill(trailingEdgeTimeUSBWC[i]);
      h1_rateCounterUSBWC[i]->Fill(rateCounterUSBWC[i]);
      gr_rateCounterUSBWC[i]->SetPoint(gr_rateCounterUSBWC[i]->GetN(), gr_rateCounterUSBWC[i]->GetN(), rateCounterUSBWC[i]);
      h1_amplMax[i]->Fill(amplMax[i]);
      h1_amplMaxTime[i]->Fill(amplMaxTime[i]);
      h1_amplMin[i]->Fill(amplMin[i]);
      h1_amplMinTime[i]->Fill(amplMinTime[i]);
      h1_amplBaseline[i]->Fill(amplBaseline[i]);
      h1_timeCFD[i]->Fill(timeCFD[i]);
      //h1_deltaTimeCFD[i]->Fill(timeCFD[0] - timeCFD[i]);
      h1_deltaTimeCFD[i]->Fill(timeCFD[i] - timeConstTh[4]);
      h1_timeConstTh[i]->Fill(timeConstTh[i]);
      h1_sigRiseT[i]->Fill(sigRiseT[i]);
      h1_sigWidth[i]->Fill(sigWidth[i]);
      h1_sigFallT[i]->Fill(sigFallT[i]);
      h1_sigChargeInWindowVT[i]->Fill(sigChargeInWindowVT[i]);
      h1_sigChargeInWindowVT_npe[i]->Fill(sigChargeInWindowVT[i]*10e-9/50.0/(1.6*10e-19)/GetBA1511Gain(1000));
      if(amplMax[0]>0.006){
	if(amplMax[1]>0.006){
	  if(sigFallT[0]>350){
	    if(sigFallT[0]<450){
	      if(sigFallT[1]>350){
		if(sigFallT[1]<450){
		  h1_amplMax_cut[i]->Fill(amplMax[i]);
		  h1_timeCFD_cut[i]->Fill(timeCFD[i]);
		  h1_timeConstTh_cut[i]->Fill(timeConstTh[i]);
		  h1_sigRiseT_cut[i]->Fill(sigRiseT[i]);
		  h1_sigWidth_cut[i]->Fill(sigWidth[i]);
		  h1_sigFallT_cut[i]->Fill(sigFallT[i]);
		  h1_deltaTimeCFD_cut[i]->Fill(timeCFD[i] - timeConstTh[4]);
		}
	      }
	    }
	  }
	}
      }
    }
    h2_amplMax_ch0_vs_ch1->Fill(amplMax[1],amplMax[0]);
    h2_amplMax_ch0_vs_dUT->Fill(unixTimeL1 - utStart,amplMax[0]);
    h2_amplMax_ch1_vs_dUT->Fill(unixTimeL1 - utStart,amplMax[1]);
    //if(sigFallT[0]>1800)
    //cout<<"eventIDL1 "<<eventIDL1<<endl;
  }
  TFile* rootFile = new TFile(nameF.Data(), "RECREATE", " Histograms", 1);
  rootFile->cd();
  if (rootFile->IsZombie()){
    cout<<"  ERROR ---> file "<<nameF.Data()<<" is zombi"<<endl;
    assert(0);
  }
  else
    cout<<"  Output Histos file ---> "<<nameF.Data()<<endl;
  for(i = 0;i<USBWCnChannels;i++){
    h1_baselineUSBWC[i]->Write();
    h1_amplitudeUSBWC[i]->Write();
    h1_chargeUSBWC[i]->Write();
    h1_leadingEdgeTimeUSBWC[i]->Write();
    h1_deltaLeadingEdgeTimeUSBWC[i]->Write();
    h1_trailingEdgeTimeUSBWC[i]->Write();
    h1_rateCounterUSBWC[i]->Write();
    h1_amplMax[i]->Write();
    h1_amplMaxTime[i]->Write();
    h1_amplMin[i]->Write();
    h1_amplMinTime[i]->Write();
    h1_amplBaseline[i]->Write();
    h1_timeCFD[i]->Write();
    h1_deltaTimeCFD[i]->Write();
    h1_timeConstTh[i]->Write();
    h1_sigRiseT[i]->Write();
    h1_sigWidth[i]->Write();
    h1_sigFallT[i]->Write();
    h1_sigChargeInWindowVT[i]->Write();
    h1_sigChargeInWindowVT_npe[i]->Write();
    gr_rateCounterUSBWC[i]->Write();
    h1_amplMax_cut[i]->Write();
    h1_timeCFD_cut[i]->Write();
    h1_timeConstTh_cut[i]->Write();
    h1_sigRiseT_cut[i]->Write();
    h1_sigWidth_cut[i]->Write();
    h1_sigFallT_cut[i]->Write();
    h1_deltaTimeCFD_cut[i]->Write();
  }
  h2_amplMax_ch0_vs_ch1->Write();
  h2_amplMax_ch0_vs_dUT->Write();
  h2_amplMax_ch1_vs_dUT->Write();
  cout<<endl<<endl;
}

void anaL2::LoopTS(TString nameF, TString nameFTS){
  //cout<<"anaL2::LoopTS(TString nameF, TString nameFTS)"<<endl;
  //cout<<"nameF   = "<<nameF<<endl
  //  <<"nameFTS = "<<nameFTS<<endl;
  Double_t utStop  = GetUnixTimeStop();
  Double_t utStart = GetUnixTimeStart();
  Double_t deltaUT = utStop - utStart;
  //printf ("utStart %13.0f \n", utStart);
  //printf ("utStop  %13.0f \n", utStop);
  Double_t dT_trigg_m_CpFM;
  //cout<<"utStop - utStart = "<<utStop - utStart<<endl;  
  Int_t i = 0;  
  TH1D *h1_baselineUSBWC[USBWCnChannels];
  TH1D *h1_amplitudeUSBWC[USBWCnChannels];
  TH1D *h1_chargeUSBWC[USBWCnChannels];
  TH1D *h1_leadingEdgeTimeUSBWC[USBWCnChannels];
  TH1D *h1_deltaLeadingEdgeTimeUSBWC[USBWCnChannels];
  TH1D *h1_trailingEdgeTimeUSBWC[USBWCnChannels];
  TH1D *h1_rateCounterUSBWC[USBWCnChannels];
  h1D1Init(h1_baselineUSBWC,"h1_baselineUSBWC","baselineUSBWC",400,-1.5,1.5);
  h1D1Init(h1_amplitudeUSBWC,"h1_amplitudeUSBWC","amplitudeUSBWC",4000,-1.5,1.5);
  h1D1Init(h1_chargeUSBWC,"h1_chargeUSBWC","chargeUSBWC",40000,-1000.0,1000);
  h1D1Init(h1_leadingEdgeTimeUSBWC,"h1_leadingEdgeTimeUSBWC","leadingEdgeTimeUSBWC",400,0.0,2500);
  h1D1Init(h1_deltaLeadingEdgeTimeUSBWC,"h1_deltaLeadingEdgeTimeUSBWC","deltaLeadingEdgeTimeUSBWC",40000,-2.0,2.0);
  h1D1Init(h1_trailingEdgeTimeUSBWC,"h1_trailingEdgeTimeUSBWC","trailingEdgeTimeUSBWC",400,0.0,2500);
  h1D1Init(h1_rateCounterUSBWC,"h1_rateCounterUSBWC","rateCounterUSBWC",1000,0.0,10000);
  TGraph *gr_rateCounterUSBWC[USBWCnChannels];
  tGraphInit(gr_rateCounterUSBWC, "gr_rateCounterUSBWC", "rateCounterUSBWC");
  TGraph *gr_ts_scanRate[USBWCnChannels];
  tGraphInit(gr_ts_scanRate, "gr_ts_scanRate", "scanRate");
  TGraph *gr_ts_scanChrge[USBWCnChannels];
  tGraphInit(gr_ts_scanChrge, "gr_ts_scanChrge", "scanChrge");
  TGraph *gr_ts_rateChrgeAv[USBWCnChannels];
  tGraphInit(gr_ts_rateChrgeAv, "gr_ts_rateChrgeAv", "rateChrgeAv");
  TH1D *h1_amplMax[USBWCnChannels];
  TH1D *h1_amplMaxTime[USBWCnChannels];
  TH1D *h1_amplMin[USBWCnChannels];
  TH1D *h1_amplMinTime[USBWCnChannels];
  TH1D *h1_amplBaseline[USBWCnChannels];
  TH1D *h1_timeCFD[USBWCnChannels];
  TH1D *h1_timeConstTh[USBWCnChannels];
  TH1D *h1_sigRiseT[USBWCnChannels];
  TH1D *h1_sigWidth[USBWCnChannels];
  TH1D *h1_sigFallT[USBWCnChannels];
  TH1D *h1_sigChargeInWindowVT[USBWCnChannels];
  TH1D *h1_sigChargeInWindowVT_npe[USBWCnChannels];
  TH1D *h1_deltaTimeCFD[USBWCnChannels];
  h1D1Init(h1_amplMax,"h1_amplMax","amplMax",20000,-0.1,1.5);
  h1D1Init(h1_amplMaxTime,"h1_amplMaxTime","amplMaxTime",800,0.0,2500);
  h1D1Init(h1_amplMin,"h1_amplMin","amplMin",100,-1.5,0.1);
  h1D1Init(h1_amplMinTime,"h1_amplMinTime","amplMinTime",800,0.0,2500);
  h1D1Init(h1_amplBaseline,"h1_amplBaseline","amplBaseline",100,-1.5,1.5);
  h1D1Init(h1_timeCFD,"h1_timeCFD","timeCFD",40000,0.0,2500);
  h1D1Init(h1_deltaTimeCFD,"h1_deltaTimeCFD","delta TimeCFD",40000,-2000.0,2000.0);
  h1D1Init(h1_timeConstTh,"h1_timeConstTh","timeConstTh",800,0.0,2500);
  h1D1Init(h1_sigRiseT,"h1_sigRiseT","sigRiseT",4000,0.0,2000);
  h1D1Init(h1_sigWidth,"h1_sigWidth","sigWidth",4000,0.0,2000);
  h1D1Init(h1_sigFallT,"h1_sigFallT","sigFallT",4000,0.0,2000);
  h1D1Init(h1_sigChargeInWindowVT,"h1_sigChargeInWindowVT","sigChargeInWindowVT",10000,-1,10);
  h1D1Init(h1_sigChargeInWindowVT_npe,"h1_sigChargeInWindowVT_npe","sigChargeInWindowVT npe",200,-1,10);
  TH1D *h1_amplMax_cut[USBWCnChannels];
  TH1D *h1_timeCFD_cut[USBWCnChannels];
  TH1D *h1_timeConstTh_cut[USBWCnChannels];
  TH1D *h1_sigRiseT_cut[USBWCnChannels];
  TH1D *h1_sigWidth_cut[USBWCnChannels];
  TH1D *h1_sigFallT_cut[USBWCnChannels];
  TH1D *h1_deltaTimeCFD_cut[USBWCnChannels];
  h1D1Init(h1_amplMax_cut,"h1_amplMax_cut","amplMax cut",20000,-0.1,1.5);
  h1D1Init(h1_timeCFD_cut,"h1_timeCFD_cut","timeCFD cut",40000,0.0,2500);
  h1D1Init(h1_timeConstTh_cut,"h1_timeConstTh_cut","timeConstTh cut",800,0.0,2500);
  h1D1Init(h1_sigRiseT_cut,"h1_sigRiseT_cut","sigRiseT cut",4000,0.0,2000);
  h1D1Init(h1_sigWidth_cut,"h1_sigWidth_cut","sigWidth cut",4000,0.0,2000);
  h1D1Init(h1_sigFallT_cut,"h1_sigFallT_cut","sigFallT cut",4000,0.0,2000);
  h1D1Init(h1_deltaTimeCFD_cut,"h1_deltaTimeCFD_cut","delta TimeCFD cut",40000,-2000.0,2000.0);
  TH2D *h2_amplMax_ch0_vs_ch1 = new TH2D("h2_amplMax_ch0_vs_ch1","amplMax ch0 vs ch1",400,-0.1,1.5,400,-0.1,1.5);
  TH2D *h2_amplMax_ch0_vs_dUT = new TH2D("h2_amplMax_ch0_vs_dUT","amplMax ch0 vs dUT",deltaUT,0,deltaUT,400,-0.1,1.5);
  TH2D *h2_amplMax_ch1_vs_dUT = new TH2D("h2_amplMax_ch1_vs_dUT","amplMax ch1 vs dUT",deltaUT,0,deltaUT,400,-0.1,1.5);
  //////////////////READ TS FILE////////////////////
  // string line;
  ifstream myTSfile(nameFTS.Data());
  TString mot;
  Double_t pos;
  Double_t posStartTime;
  Double_t ts_dT;
  const Int_t nTSpointMax = 1000;
  Int_t nTSpoint = 0;
  Double_t tspos[nTSpointMax];
  Double_t posStartTimeArr[nTSpointMax];
  Double_t ts_dTArr[nTSpointMax];
  Int_t nnTSscanMeasR[nTSpointMax][USBWCnChannels];
  Double_t rateAv[nTSpointMax][USBWCnChannels];
  Double_t chrgeAv[nTSpointMax][USBWCnChannels];
  Double_t rateChrgeAv[nTSpointMax][USBWCnChannels];
  Int_t currentTSpoint = 0;
  Int_t jj = 0;
  Int_t ii = 0;
  if (myTSfile.is_open()){
    myTSfile>>mot>>mot>>mot;
    while (myTSfile>>pos>>posStartTime>>ts_dT){
      //cout << pos <<" "<<posStartTime<< '\n';
      tspos[nTSpoint] = pos;
      posStartTimeArr[nTSpoint] = posStartTime;
      ts_dTArr[nTSpoint] = ts_dT;
      //printf (" %6d %10.5f %13.0f %10.3f \n", nTSpoint, pos, posStartTime, ts_dT);
      nTSpoint++;
    }
    myTSfile.close();
  }
  else{
    cout << "Unable to open file : "<<nameFTS<<endl;
  }
  for(jj = 0;jj<nTSpointMax;jj++){
    for(ii = 0;ii<USBWCnChannels;ii++){
      rateAv[jj][ii] = 0.0;
      chrgeAv[jj][ii] = 0.0;
      rateChrgeAv[jj][ii] = 0.0;
      nnTSscanMeasR[jj][ii] = 0;
    }
  }
  //cout<<"posStartTimeArr[0] = "<<posStartTimeArr[0]<<endl;
  //printf ("posStartTimeArr[0] %13.0f \n", posStartTimeArr[0]);
  ////////////////READ TS FILE////////////////////
  Long64_t nentries = fChain->GetEntriesFast();
  cout<<"nentries = "<<nentries<<endl;
  Long64_t nbytes = 0, nb = 0;
  for (Long64_t jentry=0; jentry<nentries;jentry++) {
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);   nbytes += nb;
    // if (Cut(ientry) < 0) continue;
    dT_trigg_m_CpFM = timeCFD[0] - timeCFD[1];
    for(i = 0;i<USBWCnChannels;i++){
      //////////////////TS INFO////////////////////
      for(jj = 0;jj<nTSpoint;jj++){
	if(unixTimeL1>posStartTimeArr[jj]){
	  if( unixTimeL1 < (posStartTimeArr[jj] + ts_dTArr[jj])){
	    rateAv[jj][i] = rateAv[jj][i] + rateCounterUSBWC[i];
	    chrgeAv[jj][i] = chrgeAv[jj][i] + sigChargeInWindowVT[i];
	    rateChrgeAv[jj][i] = rateChrgeAv[jj][i] + rateCounterUSBWC[i]*sigChargeInWindowVT[i];
	    nnTSscanMeasR[jj][i] = nnTSscanMeasR[jj][i] + 1;
	    //cout<<"ee"<<endl;
	  }
	}
      }
      //////////////////TS INFO////////////////////
      h1_baselineUSBWC[i]->Fill(baselineUSBWC[i]);
      h1_amplitudeUSBWC[i]->Fill(amplitudeUSBWC[i]);
      h1_chargeUSBWC[i]->Fill(chargeUSBWC[i]);
      h1_leadingEdgeTimeUSBWC[i]->Fill(leadingEdgeTimeUSBWC[i]);
      h1_deltaLeadingEdgeTimeUSBWC[i]->Fill(leadingEdgeTimeUSBWC[i] - leadingEdgeTimeUSBWC[4]);
      h1_trailingEdgeTimeUSBWC[i]->Fill(trailingEdgeTimeUSBWC[i]);
      h1_rateCounterUSBWC[i]->Fill(rateCounterUSBWC[i]);
      gr_rateCounterUSBWC[i]->SetPoint(gr_rateCounterUSBWC[i]->GetN(), gr_rateCounterUSBWC[i]->GetN(), rateCounterUSBWC[i]);
      h1_amplMax[i]->Fill(amplMax[i]);
      h1_amplMaxTime[i]->Fill(amplMaxTime[i]);
      h1_amplMin[i]->Fill(amplMin[i]);
      h1_amplMinTime[i]->Fill(amplMinTime[i]);
      h1_amplBaseline[i]->Fill(amplBaseline[i]);
      h1_timeCFD[i]->Fill(timeCFD[i]);
      //h1_deltaTimeCFD[i]->Fill(timeCFD[0] - timeCFD[i]);
      h1_deltaTimeCFD[i]->Fill(timeCFD[i] - timeConstTh[4]);
      h1_timeConstTh[i]->Fill(timeConstTh[i]);
      h1_sigRiseT[i]->Fill(sigRiseT[i]);
      h1_sigWidth[i]->Fill(sigWidth[i]);
      h1_sigFallT[i]->Fill(sigFallT[i]);
      h1_sigChargeInWindowVT[i]->Fill(sigChargeInWindowVT[i]);
      h1_sigChargeInWindowVT_npe[i]->Fill(sigChargeInWindowVT[i]*10e-9/50.0/(1.6*10e-19)/GetBA1511Gain(1000));
      if(amplMax[0]>0.006){
	if(amplMax[1]>0.006){
	  if(sigFallT[0]>350){
	    if(sigFallT[0]<450){
	      if(sigFallT[1]>350){
		if(sigFallT[1]<450){
		  h1_amplMax_cut[i]->Fill(amplMax[i]);
		  h1_timeCFD_cut[i]->Fill(timeCFD[i]);
		  h1_timeConstTh_cut[i]->Fill(timeConstTh[i]);
		  h1_sigRiseT_cut[i]->Fill(sigRiseT[i]);
		  h1_sigWidth_cut[i]->Fill(sigWidth[i]);
		  h1_sigFallT_cut[i]->Fill(sigFallT[i]);
		  h1_deltaTimeCFD_cut[i]->Fill(timeCFD[i] - timeConstTh[4]);
		}
	      }
	    }
	  }
	}
      }
    }
    h2_amplMax_ch0_vs_ch1->Fill(amplMax[1],amplMax[0]);
    h2_amplMax_ch0_vs_dUT->Fill(unixTimeL1 - utStart,amplMax[0]);
    h2_amplMax_ch1_vs_dUT->Fill(unixTimeL1 - utStart,amplMax[1]);
    //if(sigFallT[0]>1800)
    //cout<<"eventIDL1 "<<eventIDL1<<endl;
  }
  //////////////////TS INFO////////////////////
  for(i = 0;i<USBWCnChannels;i++){
    for(jj = 0;jj<nTSpoint;jj++){
      if(nnTSscanMeasR[jj][i] != 0){
	rateAv[jj][i] = rateAv[jj][i]/nnTSscanMeasR[jj][i];
	chrgeAv[jj][i] = chrgeAv[jj][i]/nnTSscanMeasR[jj][i];
	rateChrgeAv[jj][i] = rateChrgeAv[jj][i]/nnTSscanMeasR[jj][i];
	//cout<<nnTSscanMeasR[jj]<<endl;
      }
      else{ 
	rateAv[jj][i] = 0.0;
	chrgeAv[jj][i] = 0.0;
      }
      gr_ts_scanRate[i]->SetPoint(gr_ts_scanRate[i]->GetN(), tspos[jj], rateAv[jj][i]);
      gr_ts_scanChrge[i]->SetPoint(gr_ts_scanChrge[i]->GetN(), tspos[jj], chrgeAv[jj][i]);
      gr_ts_rateChrgeAv[i]->SetPoint(gr_ts_rateChrgeAv[i]->GetN(), tspos[jj], rateChrgeAv[jj][i]);
    }
  }
  //////////////////TS INFO////////////////////
  TFile* rootFile = new TFile(nameF.Data(), "RECREATE", " Histograms", 1);
  rootFile->cd();
  if (rootFile->IsZombie()){
    cout<<"  ERROR ---> file "<<nameF.Data()<<" is zombi"<<endl;
    assert(0);
  }
  else
    cout<<"  Output Histos file ---> "<<nameF.Data()<<endl;
  for(i = 0;i<USBWCnChannels;i++){
    h1_baselineUSBWC[i]->Write();
    h1_amplitudeUSBWC[i]->Write();
    h1_chargeUSBWC[i]->Write();
    h1_leadingEdgeTimeUSBWC[i]->Write();
    h1_deltaLeadingEdgeTimeUSBWC[i]->Write();
    h1_trailingEdgeTimeUSBWC[i]->Write();
    h1_rateCounterUSBWC[i]->Write();
    h1_amplMax[i]->Write();
    h1_amplMaxTime[i]->Write();
    h1_amplMin[i]->Write();
    h1_amplMinTime[i]->Write();
    h1_amplBaseline[i]->Write();
    h1_timeCFD[i]->Write();
    h1_deltaTimeCFD[i]->Write();
    h1_timeConstTh[i]->Write();
    h1_sigRiseT[i]->Write();
    h1_sigWidth[i]->Write();
    h1_sigFallT[i]->Write();
    h1_sigChargeInWindowVT[i]->Write();
    h1_sigChargeInWindowVT_npe[i]->Write();
    gr_rateCounterUSBWC[i]->Write();
    gr_ts_scanRate[i]->Write();
    gr_ts_scanChrge[i]->Write();
    gr_ts_rateChrgeAv[i]->Write();
    h1_amplMax_cut[i]->Write();
    h1_timeCFD_cut[i]->Write();
    h1_timeConstTh_cut[i]->Write();
    h1_sigRiseT_cut[i]->Write();
    h1_sigWidth_cut[i]->Write();
    h1_sigFallT_cut[i]->Write();
    h1_deltaTimeCFD_cut[i]->Write();
  }
  h2_amplMax_ch0_vs_ch1->Write();
  h2_amplMax_ch0_vs_dUT->Write();
  h2_amplMax_ch1_vs_dUT->Write();
  cout<<endl<<endl;
}

Double_t anaL2::GetUnixTimeStop(){
  Long64_t nentries = fChain->GetEntriesFast();
  Long64_t nbytes = 0, nb = 0;
  Long64_t jentry = nentries-1;
  Long64_t ientry = LoadTree(jentry);
  nb = fChain->GetEntry(jentry);   nbytes += nb;
  return unixTimeL1;
}

Double_t anaL2::GetUnixTimeStart(){
  Long64_t nbytes = 0, nb = 0;
  Long64_t jentry=0;
  Long64_t ientry = LoadTree(jentry);
  nb = fChain->GetEntry(jentry);   nbytes += nb;
  return unixTimeL1;
}

Int_t anaL2::GetEntry(Long64_t entry){
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}

Long64_t anaL2::LoadTree(Long64_t entry){
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
   }
   return centry;
}

void anaL2::Init(TTree *tree){
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);
   fChain->SetBranchAddress("eventIDL1", &eventIDL1, &b_eventIDL1);
   fChain->SetBranchAddress("unixTimeL1", &unixTimeL1, &b_unixTimeL1);
   fChain->SetBranchAddress("TDCL1", &TDCL1, &b_TDCL1);
   fChain->SetBranchAddress("baselineUSBWC", baselineUSBWC, &b_baselineUSBWC);
   fChain->SetBranchAddress("amplitudeUSBWC", amplitudeUSBWC, &b_amplitudeUSBWC);
   fChain->SetBranchAddress("chargeUSBWC", chargeUSBWC, &b_chargeUSBWC);
   fChain->SetBranchAddress("leadingEdgeTimeUSBWC", leadingEdgeTimeUSBWC, &b_leadingEdgeTimeUSBWC);
   fChain->SetBranchAddress("trailingEdgeTimeUSBWC", trailingEdgeTimeUSBWC, &b_trailingEdgeTimeUSBWC);
   fChain->SetBranchAddress("rateCounterUSBWC", rateCounterUSBWC, &b_rateCounterUSBWC);
   fChain->SetBranchAddress("amplMax", amplMax, &b_amplMax);
   fChain->SetBranchAddress("amplMaxTime", amplMaxTime, &b_amplMaxTime);
   fChain->SetBranchAddress("amplMin", amplMin, &b_amplMin);
   fChain->SetBranchAddress("amplMinTime", amplMinTime, &b_amplMinTime);
   fChain->SetBranchAddress("amplBaseline", amplBaseline, &b_amplBaseline);
   fChain->SetBranchAddress("timeCFD", timeCFD, &b_timeCFD);
   fChain->SetBranchAddress("timeConstTh", timeConstTh, &b_timeConstTh);
   fChain->SetBranchAddress("sigRiseT", sigRiseT, &b_sigRiseT);
   fChain->SetBranchAddress("sigWidth", sigWidth, &b_sigWidth);
   fChain->SetBranchAddress("sigFallT", sigFallT, &b_sigFallT);
   fChain->SetBranchAddress("sigChargeInWindowVT", sigChargeInWindowVT, &b_sigChargeInWindowVT);
}

void anaL2::Show(Long64_t entry){
   if (!fChain) return;
   fChain->Show(entry);
}
