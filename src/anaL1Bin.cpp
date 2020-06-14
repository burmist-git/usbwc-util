//My
#include "anaL1Bin.hh"
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

anaL1Bin::anaL1Bin(TString name){
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

anaL1Bin::anaL1Bin(TString name, Int_t executeModeID){
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

anaL1Bin::~anaL1Bin(){
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

void anaL1Bin::anaWFfillRootFile(TString fileAnaSetups,TString outRootFileF){
  Int_t i,j;
  Int_t eventIDL1;
  Double_t unixTimeL1;
  Int_t TDCL1;
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
  outTree->Branch("TDCL1",&TDCL1, "TDCL1/I");
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
  /////////WF analiz////////////
  readAnaSetup(fileAnaSetups);
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
    //if(jentry>20000)
    //break;    
    eventIDL1 = EventNumber_usbwc;
    unixTimeL1 = EpochTime_usbwc;
    TDCL1 = EventIDsamIndex_usbwc;
    for(j = 0;j<USBWCnChannels;j++){
      for(i = 0;i<USBWCnSamplesPoint;i++){
	ch_A_usb[i] = amplValues_usbwc[j][i]/10.0*0.00061;
	ch_T_usb[i] = i*SamplingPeriod_usbwc/1000.0;
      }  
      //wf = new waveformSimple( ch_T_usb, ch_A_usb, USBWCnSamplesPoint, nPointBaseLine[j]);
      wf = new waveformSimple( ch_T_usb, ch_A_usb, USBWCnSamplesPoint, _nPointBaseLine[j], _nSplinePoints[j], _sigPolarity[j]);
      wf->findPar(_chargeWinL[j],_chargeWinR[j],_sigConstTh[j],_cfdVal[j]);
      baselineUSBWC[j] = MeasuredBaseline_usbwc[j];
      amplitudeUSBWC[j] = AmplitudeValue_usbwc[j];
      chargeUSBWC[j] = ComputedCharge_usbwc[j];
      leadingEdgeTimeUSBWC[j] = RiseTimeInstant_usbwc[j];
      trailingEdgeTimeUSBWC[j] = FallTimeInstant_usbwc[j];
      rateCounterUSBWC[j] = RawTriggerRate_usbwc[j];
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

void anaL1Bin::Loop(){
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

Int_t anaL1Bin::GetEntry(Long64_t entry){
  // Read contents of entry.
  if (!fChain) return 0;
  return fChain->GetEntry(entry);
}

Long64_t anaL1Bin::LoadTree(Long64_t entry){
  // Set the environment to read one entry
  if (!fChain) return -5;
  Long64_t centry = fChain->LoadTree(entry);
  if (centry < 0) return centry;
  if (fChain->GetTreeNumber() != fCurrent) {
    fCurrent = fChain->GetTreeNumber();
  }
  return centry;
}

void anaL1Bin::Init(TTree *tree){
  if (!tree) return;
  fChain = tree;
  fCurrent = -1;
  fChain->SetMakeClass(1);
  fChain->SetBranchAddress("EventNumber_usbwc", &EventNumber_usbwc, &b_EventNumber_usbwc);
  fChain->SetBranchAddress("SamplingPeriod_usbwc", &SamplingPeriod_usbwc, &b_SamplingPeriod_usbwc);
  fChain->SetBranchAddress("EpochTime_usbwc", &EpochTime_usbwc, &b_EpochTime_usbwc);
  fChain->SetBranchAddress("Year_usbwc", &Year_usbwc, &b_Year_usbwc);
  fChain->SetBranchAddress("Month_usbwc", &Month_usbwc, &b_Month_usbwc);
  fChain->SetBranchAddress("Day_usbwc", &Day_usbwc, &b_Day_usbwc);
  fChain->SetBranchAddress("Hour_usbwc", &Hour_usbwc, &b_Hour_usbwc);
  fChain->SetBranchAddress("Minute_usbwc", &Minute_usbwc, &b_Minute_usbwc);
  fChain->SetBranchAddress("Second_usbwc", &Second_usbwc, &b_Second_usbwc);
  fChain->SetBranchAddress("Millisecond_usbwc", &Millisecond_usbwc, &b_Millisecond_usbwc);
  fChain->SetBranchAddress("channel_usbwc", &channel_usbwc, &b_channel_usbwc);
  fChain->SetBranchAddress("EventIDsamIndex_usbwc", &EventIDsamIndex_usbwc, &b_EventIDsamIndex_usbwc);
  fChain->SetBranchAddress("FirstCellToPlotsamIndex_usbwc", &FirstCellToPlotsamIndex_usbwc, &b_FirstCellToPlotsamIndex_usbwc);
  fChain->SetBranchAddress("MeasuredBaseline_usbwc", MeasuredBaseline_usbwc, &b_MeasuredBaseline_usbwc);
  fChain->SetBranchAddress("AmplitudeValue_usbwc", AmplitudeValue_usbwc, &b_AmplitudeValue_usbwc);
  fChain->SetBranchAddress("ComputedCharge_usbwc", ComputedCharge_usbwc, &b_ComputedCharge_usbwc);
  fChain->SetBranchAddress("RiseTimeInstant_usbwc", RiseTimeInstant_usbwc, &b_RiseTimeInstant_usbwc);
  fChain->SetBranchAddress("FallTimeInstant_usbwc", FallTimeInstant_usbwc, &b_RawTriggerRate_usbwc);
  fChain->SetBranchAddress("RawTriggerRate_usbwc", RawTriggerRate_usbwc, &b_RawTriggerRate_usbwc);
  fChain->SetBranchAddress("amplValues_usbwc", amplValues_usbwc, &b_amplValues_usbwc);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//         channel  nPointBaseLine      sigConstTh          cfdVal      chargeWinL      chargeWinR   nSplinePoints     sigPolarity //
//               0              20            0.01             0.5              10              20               0             neg //
//               1              20            0.01             0.5              10              20               0             neg //
//               2              20            0.01             0.5              10              20               0             neg //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void anaL1Bin::readAnaSetup(TString fileAnaSetups){
  ////////////////WF analiz////////////////
  ifstream indataSetup;
  TString mot;
  TString mot_sigPolarity;
  indataSetup.open(fileAnaSetups.Data()); 
  assert(indataSetup.is_open());  
  indataSetup>>mot; indataSetup>>mot; indataSetup>>mot;
  indataSetup>>mot; indataSetup>>mot; indataSetup>>mot;
  indataSetup>>mot; indataSetup>>mot;
  for(Int_t i = 0;i<USBWCnChannels;i++){
    indataSetup>>mot;
    indataSetup>>_nPointBaseLine[i];
    indataSetup>>_sigConstTh[i];
    indataSetup>>_cfdVal[i];
    indataSetup>>_chargeWinL[i];
    indataSetup>>_chargeWinR[i];
    indataSetup>>_nSplinePoints[i];
    indataSetup>>mot_sigPolarity;
    if(mot_sigPolarity == "neg")
      _sigPolarity[i] = -1;
    if(mot_sigPolarity == "pos")
      _sigPolarity[i] =  1;
  }
  /////////////////////////////////////////
  Int_t spaceAllocation = 16;
  cout<<setw(spaceAllocation)<<"channel"
      <<setw(spaceAllocation)<<"nPointBaseLine"
      <<setw(spaceAllocation)<<"sigConstTh"
      <<setw(spaceAllocation)<<"cfdVal"
      <<setw(spaceAllocation)<<"chargeWinL"
      <<setw(spaceAllocation)<<"chargeWinR"
      <<setw(spaceAllocation)<<"nSplinePoints"
      <<setw(spaceAllocation)<<"sigPolarity"<<endl;
  for(Int_t i = 0;i<USBWCnChannels;i++){
    cout<<setw(spaceAllocation)<<i
        <<setw(spaceAllocation)<<_nPointBaseLine[i]
        <<setw(spaceAllocation)<<_sigConstTh[i]
        <<setw(spaceAllocation)<<_cfdVal[i]
        <<setw(spaceAllocation)<<_chargeWinL[i]
        <<setw(spaceAllocation)<<_chargeWinR[i]
        <<setw(spaceAllocation)<<_nSplinePoints[i];
    if(_sigPolarity[i]==-1)
      cout<<setw(spaceAllocation)<<"neg"<<endl;
    if(_sigPolarity[i]==1)
      cout<<setw(spaceAllocation)<<"pos"<<endl;
  }
  ////////////////////////////////////////
}
