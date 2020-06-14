//18.06.2015
Int_t graph(){

  Int_t i = 0;
  const Int_t n1 = 4;
  const Int_t n2 = 4;

  Double_t x1_min =  600.0;
  Double_t x1_max = 1100.0;
  Double_t x1[n1];
  Double_t y1[n1];
  Double_t x1Err[n1];
  Double_t y1Err[n1];

  Double_t x2_min =  600.0;
  Double_t x2_max = 1100.0;
  Double_t x2[n2];
  Double_t y2[n2];
  Double_t x2Err[n2];
  Double_t y2Err[n2];

  // CpFM 1
  x1[0] = 1000; y1[0] = 4.29; x1Err[0] = 0; y1Err[0] = 0; //0 //Run  7
  x1[1] =  900; y1[1] =  2.0; x1Err[1] = 0; y1Err[1] = 0; //1 //Run  8
  x1[2] =  800; y1[2] =  0.8; x1Err[2] = 0; y1Err[2] = 0; //2 //Run  9
  x1[3] =  700; y1[3] = 0.28; x1Err[3] = 0; y1Err[3] = 0; //3 //Run 10


  // CpFM 2
  x2[0] = 1000; y2[0] = 0.85; x2Err[0] = 0; y2Err[0] = 0; //0 //Run  7
  x2[1] =  900; y2[1] = 0.34; x2Err[1] = 0; y2Err[1] = 0; //1 //Run  8
  x2[2] =  800; y2[2] = 0.17; x2Err[2] = 0; y2Err[2] = 0; //2 //Run  9
  x2[3] =  700; y2[3] = 0.06; x2Err[3] = 0; y2Err[3] = 0; //3 //Run 10

  //for(i = 0;i<n;i++){
  //x[i] = x_min + (x_max-x_min)*i/(n-1);
  //y[i] = 0.0;
  //}

  TGraphErrors *gr1 = new TGraphErrors(n1,x1,y1,0,0);
  gr1->SetMarkerStyle(21);
  gr1->SetMarkerColor(kBlack);
  gr1->SetTitle("CpFM1");
  gr1->GetXaxis()->SetTitle("High voltage, V");
  gr1->GetYaxis()->SetTitle("Charge, nVs");
  gr1->GetXaxis()->SetTitleOffset(0.8);
  gr1->GetXaxis()->SetTitleSize(0.05);
  gr1->GetXaxis()->CenterTitle();
  gr1->GetYaxis()->SetTitleOffset(0.95);
  gr1->GetYaxis()->SetTitleSize(0.05);
  gr1->GetYaxis()->CenterTitle();

  TGraphErrors *gr2 = new TGraphErrors(n2,x2,y2,0,0);
  gr2->SetMarkerStyle(21);
  gr2->SetMarkerColor(kBlack);
  gr2->SetTitle("CpFM2");
  gr2->GetXaxis()->SetTitle("High voltage, V");
  gr2->GetYaxis()->SetTitle("Charge, nVs");
  gr2->GetXaxis()->SetTitleOffset(0.8);
  gr2->GetXaxis()->SetTitleSize(0.05);
  gr2->GetXaxis()->CenterTitle();
  gr2->GetYaxis()->SetTitleOffset(0.95);
  gr2->GetYaxis()->SetTitleSize(0.05);
  gr2->GetYaxis()->CenterTitle();

  gStyle->SetPalette(1);
  gStyle->SetFrameBorderMode(0);
  gROOT->ForceStyle();
  gStyle->SetStatColor(kWhite);
  //gStyle->SetOptStat(kFALSE);
  
  TCanvas *c1 = new TCanvas("c1","plots",10,10,800,800);
  c1->Clear();
  c1->SetFillColor(kWhite);
  gr1->SetMinimum(0);
  gr1->Draw("AP");
  c1->GetPad(0)->SetLeftMargin(0.12);
  c1->GetPad(0)->SetRightMargin(0.02);
  c1->GetPad(0)->SetTopMargin(0.07);
  //c1->GetPad(0)->SetBottomMargin(0.02);

  TCanvas *c2 = new TCanvas("c2","plots",20,20,800,800);
  c2->Clear();
  c2->SetFillColor(kWhite);
  gr2->SetMinimum(0);
  gr2->Draw("AP");
  c2->GetPad(0)->SetLeftMargin(0.12);
  c2->GetPad(0)->SetRightMargin(0.02);
  c2->GetPad(0)->SetTopMargin(0.07);
  //c1->GetPad(0)->SetBottomMargin(0.02);

  return 0;
}
