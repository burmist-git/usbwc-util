//C, C++
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

#include <time.h>

using namespace std;

Int_t plots(){

  TString fileN;
  fileN = "../root_data_L2/Run_1_Data_12_9_2014_Ascii/Run_1_Data_12_9_2014_Ascii.dat.L1.root.L2.root";


  TFile *f1 = new TFile(fileN.Data());

  TH1D *h1_1 = (TH1D*)f1->Get("h1_amplBaseline_ch_4");
  TH1D *h1_2 = (TH1D*)f1->Get("h1_amplBaseline_ch_5");
  TH1D *h1_3 = (TH1D*)f1->Get("h1_amplBaseline_ch_6");
  TH1D *h1_4 = (TH1D*)f1->Get("h1_amplBaseline_ch_7");

  //TH1D *h1_1 = (TH1D*)f1->Get("h1_sigChargeInWindowVT_ch_2");
  //TH1D *h1_2 = (TH1D*)f1->Get("h1_sigChargeInWindowVT_cut_ch_2");
  //TH1D *h1_3 = (TH1D*)f1->Get("h1_sigChargeInWindowVT_cutCpFM2_ch_2");

  //Double_t nPions = h1_sigChargeInWindowVT_cut_ch_1->GetEntries();
  //Double_t nSigCpFM1 = h1_sigChargeInWindowVT_cutCpFM1_ch_1->GetEntries();
  //Double_t nSigCpFM2 = h1_sigChargeInWindowVT_cutCpFM2_ch_2->GetEntries();

  //cout<<"                 Number of selected pions  = "<<nPions<<endl
  //  <<"             Number of triggers from CpFM1 = "<<nSigCpFM1<<endl
  //  <<"             Number of triggers from CpFM2 = "<<nSigCpFM2<<endl;

  //Double_t nPE_CpFM1 = -TMath::Log((nPions - nSigCpFM1)/nPions);
  //Double_t nPE_CpFM2 = -TMath::Log((nPions - nSigCpFM2)/nPions);

  //cout<<" Number of detected p.e. per pion by CpFM1 = "<<nPE_CpFM1<<endl
  //  <<" Number of detected p.e. per pion by CpFM2 = "<<nPE_CpFM2<<" "<<endl
  //  <<"              Crostalk from CpFM1 to CpFM2 = "<<nPE_CpFM2*100/nPE_CpFM1<<" %"<<endl;

  //TH1D *h1_1 = (TH1D*)f1->Get("h1_amplMax_ch_2");
  //TH1D *h1_2 = (TH1D*)f1->Get("h1_amplMax_cut_ch_2");
  //TH1D *h1_3 = (TH1D*)f1->Get("h1_amplMax_cutCpFM2_ch_2");

  //TH1D *h1_1 = (TH1D*)f1->Get("h1_delta_timeCFD_CpFM2_m_trig");
  //TH1D *h1_2 = (TH1D*)f1->Get("h1_delta_timeCFD_CpFM2_m_trig_cut");
  //TH1D *h1_3 = (TH1D*)f1->Get("h1_delta_timeCFD_CpFM2_m_trig_cutCpFM2");

  //TH1D *h1_1 = (TH1D*)f1->Get("h1_amplMaxTime_ch_0");
  //TH1D *h1_1 = (TH1D*)f1->Get("h1_timeCFD_ch_0");
  //TH1D *h1_1 = (TH1D*)f1->Get("h1_amplMax_ch_0");
  //TH1D *h1_2 = (TH1D*)f1->Get("h1_amplMax_cut_ch_0");
  //TH1D *h1_1 = (TH1D*)f1->Get("h1_sigChargeInWindowVT_ch_1");
  //TH1D *h1_2 = (TH1D*)f1->Get("h1_sigChargeInWindowVT_cut_ch_1");

  //h1_1->Rebin(4);
  //h1_2->Rebin(4);
  //h1_3->Rebin(4);

  h1_1->SetTitle("");

  //TCanvas *c1 = new TCanvas("c1",fileN.Data(),10,10,800,800);
  TCanvas *c1 = new TCanvas("c1",fileN.Data(),10,10,600,600);
  gStyle->SetPalette(1);
  gStyle->SetFrameBorderMode(0);
  gROOT->ForceStyle();
  gStyle->SetStatColor(kWhite);
  //gStyle->SetOptStat(kFALSE);
 
  h1_1->SetLineColor(kBlack);
  h1_2->SetLineColor(kRed);
  h1_3->SetLineColor(kBlue);
  h1_4->SetLineColor(kMagenta);
  h1_1->SetLineWidth(3.0);
  h1_2->SetLineWidth(3.0);
  h1_3->SetLineWidth(3.0);
  h1_4->SetLineWidth(3.0);


  //c1->Divide(2,1);
  //c1->cd(1);
  h1_1->Draw();
  //c1->cd(2);
  h1_2->Draw("sames");
  h1_3->Draw("sames");
  h1_4->Draw("sames");

  //h1_1->Draw();
  //h1_2->Draw("same");

  //h1_2->Draw();
  //h1_2->Fit("gaus");
  h1_1->GetXaxis()->SetTitle("Baseline amplitude, V");
  h1_1->GetXaxis()->SetRangeUser(-0.13,-0.12);

  /*
  h1_1->Fit("gaus");
  TLegend *leg = new TLegend(0.6,0.6,0.9,0.9,"","brNDC");
  leg->AddEntry(h1_1, "Without corrections","l");
  leg->AddEntry(h1_2, "With corrections","l");
  leg->Draw();
  */

  return 0;
}
