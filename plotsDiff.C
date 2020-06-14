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

Int_t plotsDiff(){

  TString fileN1;
  TString fileN2;
  TString fileN3;
  fileN1 = "../root_data_L2/Run_2_Data_12_9_2014_Ascii/Run_2_Measurements_Only_12_9_2014.dat.root_hist.root";
  fileN2 = "../root_data_L2/Run_2_Data_12_9_2014_Ascii/Run_2_Data_12_9_2014_Ascii.dat.L1.root.L2.root";
  //fileN1 = "../root_data_L2/Run_3_Data_12_9_2014_Binary/Run_3_Measurements_Only_12_9_2014.bin.root_hist.root";
  //fileN2 = "../root_data_L2/Run_3_Data_12_9_2014_Binary/Run_3_Data_12_9_2014_Binary.bin.L1.root.L2.root";
  //fileN1 = "../root_data_L2/Run_4_Data_12_9_2014_Binary/Run_4_Measurements_Only_12_9_2014.bin.root_hist.root";
  //fileN2 = "../root_data_L2/Run_4_Data_12_9_2014_Binary/Run_4_Data_12_9_2014_Binary.bin.L1.root.L2.root";
  fileN3 = fileN2;

  TFile *f1 = new TFile(fileN1.Data());
  TFile *f2 = new TFile(fileN2.Data());
  TFile *f3 = new TFile(fileN3.Data());

  //TH1D *h1_1 = (TH1D*)f1->Get("h1_amplMax_ch_4");
  //TH1D *h1_2 = (TH1D*)f2->Get("h1_amplMax_ch_4");
  //TH1D *h1_3 = (TH1D*)f3->Get("h1_amplMax_ch_4");

  //TH1D *h1_1 = (TH1D*)f1->Get("h1_sigChargeInWindowVT_ch_4");
  //TH1D *h1_2 = (TH1D*)f2->Get("h1_sigChargeInWindowVT_ch_4");
  //TH1D *h1_3 = (TH1D*)f3->Get("h1_sigChargeInWindowVT_ch_4");

  //TH1D *h1_1 = (TH1D*)f1->Get("h1_timeCFD_ch_4");
  //TH1D *h1_2 = (TH1D*)f2->Get("h1_timeCFD_ch_4");
  //TH1D *h1_3 = (TH1D*)f3->Get("h1_timeCFD_ch_4");

  //TH1D *h1_1 = (TH1D*)f1->Get("h1_Amplitude_ch_4");
  //TH1D *h1_2 = (TH1D*)f2->Get("h1_amplitudeUSBWC_ch_4");
  //TH1D *h1_3 = (TH1D*)f3->Get("h1_amplitudeUSBWC_ch_4");

  TH1D *h1_1 = (TH1D*)f1->Get("h1_Charge_ch_4");
  TH1D *h1_2 = (TH1D*)f2->Get("h1_chargeUSBWC_ch_4");
  TH1D *h1_3 = (TH1D*)f3->Get("h1_chargeUSBWC_ch_4");

  //normalizeHisto(h1_1);
  //normalizeHisto(h1_2);

  //h1_1->Rebin(8);
  //h1_2->Rebin(8);
  //h1_3->Rebin(8);

  h1_1->SetTitle("");

  h1_1->SetName("1");
  h1_2->SetName("2");
  h1_3->SetName("3");

  TCanvas *c1 = new TCanvas("c1",fileN1.Data(),10,10,800,800);
  gStyle->SetPalette(1);
  gStyle->SetFrameBorderMode(0);
  gROOT->ForceStyle();
  gStyle->SetStatColor(kWhite);
  //gStyle->SetOptStat(kFALSE);
 
  h1_1->SetLineColor(kBlack);
  h1_1->SetLineWidth(3.0);
  h1_2->SetLineColor(kRed);
  h1_2->SetLineWidth(3.0);
  h1_3->SetLineColor(kBlue);
  h1_3->SetLineWidth(3.0);

  //c1->Divide(2,1);
  //c1->cd(1);
  //h1_1->SetMaximum(520);
  //h1_1->GetXaxis()->SetRangeUser(0.224,0.234);
  //h1_1->GetXaxis()->SetRangeUser(156,164);
  //h1_1->GetXaxis()->SetRangeUser(1.9,2.1);
  //h1_1->GetXaxis()->SetRangeUser(-0.05,1.0);
  h1_1->Draw();
  h1_2->Draw("sames");
  //h1_3->Draw("same");
  //h1_1->GetXaxis()->SetTitle("Time (CFD), ns");
  h1_1->GetXaxis()->SetTitle("Charge, nVs");
  //h1_1->GetXaxis()->SetTitle("Amplitude, V");
  //h1_2->GetXaxis()->SetTitle("Number of p.e. per incoming 500 MeV/c e^{-}");

  //h1_1->Draw();
  //h1_2->Draw("same");

  //h1_2->Draw();
  //h1_2->Fit("gaus");
  //h1_2->GetXaxis()->SetTitle("Number of p.e. per incoming 500 MeV/c e^{-}");
  //h1_2->GetXaxis()->SetRangeUser(0,1.0);


  //h1_1->Fit("gaus");
  TLegend *leg = new TLegend(0.6,0.6,0.9,0.9,"","brNDC");
  leg->AddEntry(h1_1, "Measurements only","l");
  leg->AddEntry(h1_2, "Measurements in wf. file","l");
  //leg->AddEntry(h1_3, "Run 3","l");
  leg->Draw();


  return 0;
}

void normalizeHisto(TH1D *h1_1){
  Double_t norm = h1_1->GetEntries();
  for(Int_t i = 0;i<h1_1->GetNbinsX();i++){
    h1_1->SetBinContent(i+1,h1_1->GetBinContent(i+1)/norm);
  }
}
