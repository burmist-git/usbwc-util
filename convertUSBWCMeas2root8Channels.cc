////////////////////////////////////////////////////////////////////
//                Date: Sat Oct 18 16:04:35 CEST 2014             //
//               Autor: Leonid Burmistrov                         //
// Program description: Convertion of the txt data                //
//                      with measurements into the root           //
//                      format. Initioal txt data file produced   //
//                      by the single board 8 channel             //
//                      USB-WaveCatcher.                          //
////////////////////////////////////////////////////////////////////

//root
#include <TH1D.h>
#include <TStyle.h>
#include <TString.h>
#include <TCanvas.h>
#include <TFile.h>
#include <TTree.h>
#include <TH1D.h>

//C, C++
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

using namespace std;

void convBunchDataUSBmeas2root(TString inputDataFile, TString inputDataFilePath, TString outputrootFile);

int main(int argc, char *argv[]){
  
  TString inData;
  TString dataPath;
  TString outData;
  if(argc == 5 && atoi(argv[1])==0){
    inData   = argv[2];
    dataPath = argv[3];
    outData  = argv[4];
    cout<<"In data file list : "<<inData<<endl
	<<"In data path      : "<<dataPath<<endl
        <<"Out root file     : "<<outData<<endl;
    convBunchDataUSBmeas2root( inData, dataPath, outData);
  }
  else{
    cout<<" ERROR --->  in input arguments "<<endl
        <<"  runID [1] = 0  "<<endl
        <<"        [2] - in data file list"<<endl
	<<"        [3] - in data path"<<endl
        <<"        [4] - out root file"<<endl;
  }

  return 0;
}

void convBunchDataUSBmeas2root(TString inputDataFileList, TString inputDataFilePath, TString outputrootFile){
  const Int_t nChannels = 8;
  //=== DATA FILE SAVED WITH SOFTWARE VERSION: V2.4.11 ===
  //=== WAVECATCHER SYSTEM OF TYPE 0 WITH 8 CHANNELS AND GAIN: 1.0 ===
  //=== Other Parameters *** ===
  //=== ChannelNb EventID MeasuredBaseline[Volts] Amplitude[Volts] Charge[pC] LeadingEdgeTime[ns] TrailingEdgeTime[ns] RateCounter[Hz]=== 
  //=== Event 1 UnixTime = 1413384064.877 date = 2014.10.15 time = 16h.41m.4s.877ms == TDC = 74011 ===
  //0 1 -0.000111 0.002667    0.199  0.097  0.479       0.00
  //1 1 0.000144 0.002370    0.099  9.930 10.757       0.00
  //2 1 0.000185 0.002005   -0.194  5.239  5.420       0.00
  //3 1 -0.000452 -0.002366    0.397  0.914  1.053       0.00
  //4 1 0.000212 0.001783   -0.213  6.214  0.138       0.00
  //5 1 -0.000448 -0.002371    0.421  7.452  7.580       0.00
  //6 1 0.000276 0.002199    0.164 45.933 45.939       0.00
  //7 1 0.000036 0.002341    0.105  7.687  7.928       0.00
  //=== Event 2 UnixTime = 1413384064.950 date = 2014.10.15 time = 16h.41m.4s.950ms == TDC = 99078 ===
  //0 2 -0.000046 0.002868   -0.103  7.658  8.115       0.00
  //1 2 0.000282 -0.002392   -0.234  6.884  7.353       0.00
  //2 2 -0.000139 0.002181   -0.021  1.033  1.316       0.00
  //3 2 -0.000178 -0.002225    0.098  4.619  4.905       0.00
  //USBWC data in one ev//
  Int_t eventID;
  Double_t UnixTime;
  Double_t TDC;
  Int_t nChusbwc = nChannels;
  Float_t MeasuredBaseline[nChannels], baseline;
  Float_t Amplitude[nChannels], amplitude;
  Float_t Charge[nChannels], charge;
  Float_t LeadingEdgeTime[nChannels], leadingEdgeTime;
  Float_t TrailingEdgeTime[nChannels], trailingEdgeTime;
  Float_t RateCounter[nChannels], rateCount;
  ///////////////////Root file with data/////////////////
  TFile *hfile = new TFile( outputrootFile, "RECREATE", "USBWC Data", 1);
  if (hfile->IsZombie()) {
    cout << "PROBLEM with the initialization of the output ROOT ntuple " 
	 << outputrootFile << ": check that the path is correct!!!"
	 << endl;
    exit(-1);
  }
  TTree *tree = new TTree("T", "USBWC measuremtns Data Tree");
  hfile->SetCompressionLevel(2);
  tree->SetAutoSave(1000000);
  // Create new event
  TTree::SetBranchStyle(0);
  //Event USBWC
  tree->Branch("eventID",&eventID, "eventID/I");
  tree->Branch("UnixTime",&UnixTime, "UnixTime/D");
  tree->Branch("TDC",&TDC, "TDC/D");
  tree->Branch("nChusbwc",&nChusbwc, "nChusbwc/I");
  tree->Branch("MeasuredBaseline", MeasuredBaseline, "MeasuredBaseline[nChusbwc]/F");
  tree->Branch("Amplitude", Amplitude, "Amplitude[nChusbwc]/F");
  tree->Branch("Charge", Charge, "Charge[nChusbwc]/F");
  tree->Branch("LeadingEdgeTime", LeadingEdgeTime, "LeadingEdgeTime[nChusbwc]/F");
  tree->Branch("TrailingEdgeTime", TrailingEdgeTime, "TrailingEdgeTime[nChusbwc]/F");
  tree->Branch("RateCounter",RateCounter ,"RateCounter[nChusbwc]/F");
  ///////////////////////////////////////////////////////
  Int_t i = 0;
  Int_t j = 0;
  Int_t chCheck = 0;
  Int_t evIDCheckFirst = 0;
  Int_t evIDCheck = 0;
  string mot;
  TString fileToConvert;
  Int_t chID = 0;
  Int_t nEv = 0;
  Int_t nEv_Tot = 0;  
  Double_t timeNpevEv = -999.0; //Unix time of previous event
  //cout<<" ---> Conversion of "<<inputDataFile<<endl;
  // Init event variables
  eventID = -999;
  UnixTime = -999.0;
  TDC = -999;
  for(j = 0;j<nChannels;j++){
    MeasuredBaseline[j] = -999.0;
    Amplitude[j] = -999.0;
    Charge[j] = -999.0;
    LeadingEdgeTime[j] = -999.0;
    TrailingEdgeTime[j] = -999.0;
    RateCounter[j] = -999.0;
  }
  ifstream indataList;
  ifstream indata;
  indataList.open(inputDataFileList.Data()); 
  assert(indataList.is_open());
  nEv = 0; // Event counter for a given file
  while (indataList  >> mot ){
    fileToConvert = inputDataFilePath;
    fileToConvert += mot;
    cout<<" ---> Conversion of "<<fileToConvert<<endl;
    indata.open(fileToConvert.Data()); 
    while (indata  >> mot ){
      if(mot == "Event"){
	indata >> eventID;
	//cout<<"eventID = "<<eventID<<endl;
	indata >> mot;
	if(mot != "UnixTime") assert(0);
	indata >> mot;
	indata >> UnixTime;
	//=== Event 1 UnixTime = 1413384064.877 date = 2014.10.15 time = 16h.41m.4s.877ms == TDC = 74011 ===
	indata >> mot; if (mot != "date") assert(0);
	indata >> mot >> mot;
	indata >> mot; if (mot != "time") assert(0);
	indata >> mot >> mot;
	indata >> mot >> mot; 
	if(mot != "TDC") assert(0);
	indata >> mot;
	indata >> TDC;
	indata >> mot;      
	for(i = 0;i<8;i++){
	  indata>>chCheck;
	  if(chCheck != i){
	    cout<<" i       = "<<i<<endl
		<<" chCheck = "<<chCheck<<endl
		<<"eventID  = "<<eventID<<endl;
	    assert(0);
	  }
	  indata>>evIDCheck;
	  if(i == 0){
	    evIDCheckFirst = evIDCheck;
	  }
	  if(i != 0){
	    if(evIDCheckFirst != evIDCheck){
	      cout<<"ERROR ---> evIDCheckFirst != evIDCheck"<<endl
		  <<"           evIDCheckFirst  = "<<evIDCheckFirst<<endl
		  <<"                evIDCheck  = "<<evIDCheck<<endl
		  <<"                 eventID  = "<<eventID<<endl;
	      assert(0);
	    }
	  }
	  //if(evIDCheck != eventID){
	    //cout<<"eventID  = "<<eventID<<endl
	    //<<"evIDCheck = "<<evIDCheck<<endl;
	    //assert(0);
	  //}
	  //cout<<"mot = "<<mot<<endl;
	  indata>>baseline;
	  indata>>amplitude;
	  indata>>charge;
	  indata>>leadingEdgeTime;
	  indata>>trailingEdgeTime;
	  indata>>rateCount;
	  if(indata.tellg() == -1){
	    cout<<" ---> ERROR : indata.tellg() == -1"<<endl
		<<" rateCount   = "<<rateCount<<endl;
	    cout<<" In file : "<<fileToConvert<<" structure error is expected"<<endl;
	    assert(0);
	  }
	  MeasuredBaseline[i] = baseline;
	  Amplitude[i] = amplitude;
	  Charge[i] = charge;
	  LeadingEdgeTime[i] = leadingEdgeTime;
	  TrailingEdgeTime[i] = trailingEdgeTime;
	  RateCounter[i] = rateCount;
	}
	tree->Fill();
	//cout<< " eventID  = "<<eventID<<endl
	//  << " UnixTime = "<<UnixTime<<endl
	//  << " TDC      = "<<TDC<<endl;
	//for(i = 0;i<8;i++){
	//cout<<setw(12)<<MeasuredBaseline[i]
	//    <<setw(12)<<Amplitude[i]
	//    <<setw(12)<<Charge[i]
	//    <<setw(12)<<LeadingEdgeTime[i]
	//    <<setw(12)<<TrailingEdgeTime[i]
	//    <<setw(12)<<RateCounter[i]<<endl;
	//}
      }
    }
    indata.close(); 
  }
  indataList.close();
  hfile = tree->GetCurrentFile();
  hfile->Write();
  hfile->Close();
}
