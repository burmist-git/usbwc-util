//My
#include "anaL1.hh"
#include "wfSimulation/src/waveformSimple.hh"

//root
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

//C, C++
#include <iostream>
#include <stdlib.h>
#include <assert.h>
#include <fstream>
#include <iomanip>
#include <time.h>

using namespace std;

anaL1::anaL1(TString name){
  //cout<<endl<<endl<<name<<endl;
  ifstream indata;
  TString rootFileName;
  TChain *theChain = new TChain("T");
  indata.open(name.Data()); 
  assert(indata.is_open());  
  while (indata  >> rootFileName ){
    if(indata.eof()){
      std::cout<<"EOF"<<std::endl;
      break;
    }
    cout<<"        adding "<<rootFileName<<endl;
    theChain->Add(rootFileName.Data(),-1);
  }
  indata.close();
  Init(theChain);
}

anaL1::anaL1(TString name, Int_t executeModeID){
  if(executeModeID == 1){
    //cout<<endl<<endl<<name<<endl;
    TString rootFileName;
    TChain *theChain = new TChain("T");
    rootFileName = name;
    theChain->Add(rootFileName.Data(),-1);
    Init(theChain);
  }
  else{
    cout<<" ERROR --> executeModeID = "<<executeModeID<<endl;
    assert(0);
  }
}

anaL1::~anaL1()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

void anaL1::anaWFfillRootFile(TString fileAnaSetups,TString outRootFileF){

  Int_t i,j;

  Int_t eventIDL1;
  Double_t unixTimeL1;
  Double_t TDCL1;
  Float_t baselineUSBWC[USBWCnChannels];
  Float_t amplitudeUSBWC[USBWCnChannels];
  Float_t chargeUSBWC[USBWCnChannels];
  Float_t leadingEdgeTimeUSBWC[USBWCnChannels];
  Float_t trailingEdgeTimeUSBWC[USBWCnChannels];
  Float_t rateCounterUSBWC[USBWCnChannels];

  Float_t amplMax[USBWCnChannels];
  Float_t amplMaxTime[USBWCnChannels];
  Float_t amplMin[USBWCnChannels];
  Float_t amplMinTime[USBWCnChannels];
  Float_t amplBaseline[USBWCnChannels];
  Float_t timeCFD[USBWCnChannels];
  Float_t timeConstTh[USBWCnChannels];
  Float_t sigRiseT[USBWCnChannels];
  Float_t sigWidth[USBWCnChannels];
  Float_t sigFallT[USBWCnChannels];
  Float_t sigChargeInWindowVT[USBWCnChannels];
  
  TFile* outL1rootFile = new TFile(outRootFileF.Data(), "RECREATE", "Root file with parameters L1", 1);
  outL1rootFile->cd();
  if (outL1rootFile->IsZombie()){
    cout<<"  ERROR ---> file "<<outRootFileF<<" is zombi"<<endl;
    assert(0);
  }
  
  TTree *outTree = new TTree("T", "USBWC L1 Data Tree");
  outL1rootFile->SetCompressionLevel(2);
  outTree->SetAutoSave(1000000);
  // Create new event
  TTree::SetBranchStyle(0);
  
  TString var_ss = "";
  TString end_ss = "[";
  end_ss+=USBWCnChannels;
  end_ss+="]/F";
  //cout<<"vvv_"<<end_ss<<"_www"<<endl;
  outTree->Branch("eventIDL1",&eventIDL1, "eventIDL1/I");
  outTree->Branch("unixTimeL1",&unixTimeL1, "unixTimeL1/D");
  outTree->Branch("TDCL1",&TDCL1, "TDCL1/D");
  var_ss = "baselineUSBWC"; var_ss += end_ss;
  outTree->Branch("baselineUSBWC", baselineUSBWC, var_ss.Data());
  var_ss = "amplitudeUSBWC"; var_ss += end_ss;
  outTree->Branch("amplitudeUSBWC", amplitudeUSBWC, var_ss.Data());
  var_ss = "chargeUSBWC"; var_ss += end_ss;
  outTree->Branch("chargeUSBWC", chargeUSBWC, var_ss.Data());
  var_ss = "leadingEdgeTimeUSBWC"; var_ss += end_ss;
  outTree->Branch("leadingEdgeTimeUSBWC", leadingEdgeTimeUSBWC, var_ss.Data());
  var_ss = "trailingEdgeTimeUSBWC"; var_ss += end_ss;
  outTree->Branch("trailingEdgeTimeUSBWC", trailingEdgeTimeUSBWC, var_ss.Data());
  var_ss = "rateCounterUSBWC"; var_ss += end_ss;
  outTree->Branch("rateCounterUSBWC", rateCounterUSBWC, var_ss.Data());
  var_ss = "amplMax"; var_ss += end_ss;
  outTree->Branch("amplMax", amplMax, var_ss.Data());
  var_ss = "amplMaxTime"; var_ss += end_ss;
  outTree->Branch("amplMaxTime", amplMaxTime, var_ss.Data());
  var_ss = "amplMin"; var_ss += end_ss;
  outTree->Branch("amplMin", amplMin, var_ss.Data());
  var_ss = "amplMinTime"; var_ss += end_ss;
  outTree->Branch("amplMinTime", amplMinTime, var_ss.Data());
  var_ss = "amplBaseline"; var_ss += end_ss;
  outTree->Branch("amplBaseline", amplBaseline, var_ss.Data());
  var_ss = "timeCFD"; var_ss += end_ss;
  outTree->Branch("timeCFD", timeCFD, var_ss.Data());
  var_ss = "timeConstTh"; var_ss += end_ss;
  outTree->Branch("timeConstTh", timeConstTh, var_ss.Data());
  var_ss = "sigRiseT"; var_ss += end_ss;
  outTree->Branch("sigRiseT", sigRiseT, var_ss.Data());
  var_ss = "sigWidth"; var_ss += end_ss;
  outTree->Branch("sigWidth", sigWidth, var_ss.Data());
  var_ss = "sigFallT"; var_ss += end_ss;
  outTree->Branch("sigFallT", sigFallT, var_ss.Data());
  var_ss = "sigChargeInWindowVT"; var_ss += end_ss;
  outTree->Branch("sigChargeInWindowVT", sigChargeInWindowVT, var_ss.Data());
  
  /////////////////////////
  
  /////////WF analiz////////////
  Int_t nPointBaseLine[USBWCnChannels];
  Double_t sigConstTh[USBWCnChannels];
  Double_t chargeWinL[USBWCnChannels];
  Double_t chargeWinR[USBWCnChannels];

  ifstream indataSetup;
  TString mot;
  indataSetup.open(fileAnaSetups.Data()); 
  assert(indataSetup.is_open());  
  indataSetup>>mot; indataSetup>>mot; indataSetup>>mot;
  indataSetup>>mot; indataSetup>>mot;
  for(i = 0;i<USBWCnChannels;i++){
    indataSetup>>mot;
    indataSetup>>nPointBaseLine[i];
    indataSetup>>sigConstTh[i];
    indataSetup>>chargeWinL[i];
    indataSetup>>chargeWinR[i];
  }

  /////////////////////////////////////////
  cout<<setw(20)<<"channel"<<setw(20)<<"nPointBaseLine"<<setw(20)<<"sigConstTh"<<setw(20)<<"chargeWinL"<<setw(20)<<"chargeWinR"<<endl;
  for(i = 0;i<USBWCnChannels;i++){
    cout<<setw(20)<<i
	<<setw(20)<<nPointBaseLine[i]
	<<setw(20)<<sigConstTh[i]
	<<setw(20)<<chargeWinL[i]
	<<setw(20)<<chargeWinR[i]<<endl;
  }
  //////////////////////////////
    
  Double_t ch_A_usb[USBWCnSamplesPoint];
  Double_t ch_T_usb[USBWCnSamplesPoint];

  Double_t t_left = 0;
  Double_t t_right = 0;
  
  waveformSimple *wf;
  
  if (fChain == 0) return;
  Long64_t nentries = fChain->GetEntriesFast();
  cout<<"nentries "<<nentries<<endl;
  Long64_t nbytes = 0, nb = 0;
  Double_t unixTimePrev = 0.0;
  for (Long64_t jentry=0; jentry<nentries;jentry++) {
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);   nbytes += nb;
    if(jentry%10000 == 0)
      cout<<jentry<<endl;
    
    eventIDL1 = eventID;
    unixTimeL1 = UnixTime;
    TDCL1 = TDC;
    
    for(j = 0;j<USBWCnChannels;j++){
      for(i = 0;i<USBWCnSamplesPoint;i++){
	ch_A_usb[i] = ch[j][i];
	ch_T_usb[i] = i*SamplingPeriod/1000.0;
      }  
      wf = new waveformSimple( ch_T_usb, ch_A_usb, USBWCnSamplesPoint, nPointBaseLine[j]);
      wf->findPar(chargeWinL[j],chargeWinR[j],sigConstTh[j],0.5);
      baselineUSBWC[j] = Baseline[j];
      amplitudeUSBWC[j] = Amplitude[j];
      chargeUSBWC[j] = Charge[j];
      leadingEdgeTimeUSBWC[j] = LeadingEdgeTime[j];
      trailingEdgeTimeUSBWC[j] = TrailingEdgeTime[j];
      rateCounterUSBWC[j] = RateCounter[j];

      amplMax[j] = wf->GetAmax();
      amplMaxTime[j] = wf->GetTmax();
      amplMin[j] = wf->GetAmin();
      amplMinTime[j] = wf->GetTmin();
      amplBaseline[j] = wf->GetAbasel();
      timeCFD[j] = wf->GetTimeCFD();
      timeConstTh[j] = wf->GetTimeConstThreas();
      sigRiseT[j] = wf->GetSigRise();
      sigWidth[j] = wf->GetSigWidth();
      sigFallT[j] = wf->GetSigFall();
      sigChargeInWindowVT[j] = wf->GetChargeInW();
     
      delete wf;
    }
    
    outTree->Fill();
  }
  
  //outL1rootFile = outTree->GetCurrentFile();
  outL1rootFile->Write();
  outL1rootFile->Close();

  //cout<<" Output root file with parameters ---> "<<outRootFileF<<endl;

}

void anaL1::Loop()
{
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

Int_t anaL1::GetEntry(Long64_t entry)
{
  // Read contents of entry.
  if (!fChain) return 0;
  return fChain->GetEntry(entry);
}

Long64_t anaL1::LoadTree(Long64_t entry)
{
  // Set the environment to read one entry
  if (!fChain) return -5;
  Long64_t centry = fChain->LoadTree(entry);
  if (centry < 0) return centry;
  if (fChain->GetTreeNumber() != fCurrent) {
    fCurrent = fChain->GetTreeNumber();
  }
  return centry;
}

void anaL1::Init(TTree *tree)
{
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
}
