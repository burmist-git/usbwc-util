////////////////////////////////////////////////////////////////////
//                Date: Thu Oct 16 14:36:45 CEST 2014             //
//               Autor: Leonid Burmistrov                         //
// Program description: Convertion of the txt data into the root  //
//                      format. Initioal txt data file produced   //
//                      by the single board 8 channel             //
//                      USB-WaveCatcher.                          //
////////////////////////////////////////////////////////////////////


/////////////////
// APRIL 2014 BTF 500 MeV electrons
//=== DATA FILE SAVED WITH SOFTWARE VERSION: V2.3.6 ===
//=== WAVECATCHER SYSTEM OF TYPE 0 WITH 8 CHANNELS AND GAIN: 1.0 ===
//=== Other Parameters *** ===
//=== DATA SAMPLES [1024] in Volts == NB OF CHANNELS ACQUIRED: 8 == Sampling Period: 312.5 ps == INL Correction: 1 ===
//=== EVENT 1 ===
//=== UnixTime = 1397415775.125 date = 2014.4.13 time = 21h.2m.55s.125ms == TDC = 34277721 ===
//=== CH: 0 EVENTID: 1 FCR: 352 Baseline: 0.010761 V Amplitude: -0.770447 V Charge: -911.107 pC LeadingEdgeTime: 113.720 ns TrailingEdgeTime: 314.276 ns ===
//0.011514 0.010216 0.011777 0.010813 0.009761 0.010714 0.011085 0.012230 0.012467 0.010143 0.009911
//.....
//=== CH: 1 EVENTID: 1 FCR: 352 Baseline: -0.001572 V Amplitude: -0.086693 V Charge:  -25.092 pC LeadingEdgeTime: 259.856 ns TrailingEdgeTime: 272.280 ns ===
//.....
//=== EVENT 2 ===
//=== UnixTime = 1397415775.281 date = 2014.4.13 time = 21h.2m.55s.281ms == TDC = 42283950 ===
//=== CH: 0 EVENTID: 2 FCR: 752 Baseline: 0.010220 V Amplitude: -0.768751 V Charge: -911.650 pC LeadingEdgeTime: 109.138 ns TrailingEdgeTime: 309.731 ns ===
//.....
/////////////////

/////////////////
// OCTOBER 2014 H8 180 GeV pions
//=== DATA FILE SAVED WITH SOFTWARE VERSION: V2.4.11 ===
//=== WAVECATCHER SYSTEM OF TYPE 0 WITH 8 CHANNELS AND GAIN: 1.0 ===
//=== Other Parameters *** ===
//=== DATA SAMPLES [1024] in Volts == NB OF CHANNELS ACQUIRED: 8 == Sampling Period: 625.0 ps == INL Correction: 1 ===
//=== EVENT 1 ===
//=== UnixTime = 1413408850.125 date = 2014.10.15 time = 23h.34m.10s.125ms == TDC = 17872195 == TDC corrected time = 08h08m08s,648.648.648ns ===
//=== CH: 0 EVENTID: 1 FCR: 416 Baseline: 0.000035 V Amplitude: -0.002288 V Charge: 0.552 pC LeadingEdgeTime: 11.741 ns TrailingEdgeTime: 12.215 ns RateCounter 0.00 ===
//0.001849 -0.001136 -0.000150 0.000038 0.000763 0.000649 0.000190 -0.000038 -0.000382 -0.000258 0.000433 0.000774 -0.001257
//.....
//=== CH: 1 EVENTID: 1 FCR: 416 Baseline: 0.000637 V Amplitude: -0.148895 V Charge:   -1.500 pC LeadingEdgeTime: 460.566 ns TrailingEdgeTime: 462.984 ns RateCounter       0.00 ===
//0.000616 0.002671 0.000106 0.000039 0.000381 0.000114 0.000192 0.000688 0.001020 0.000028
//.....
//=== EVENT 2 ===
//=== UnixTime = 1413408850.983 date = 2014.10.15 time = 23h.34m.10s.983ms == TDC = 103944994 == TDC corrected time = 08h08m08s,648.648.648ns ===
//=== CH: 0 EVENTID: 2 FCR: 704 Baseline: 0.000236 V Amplitude: 0.002725 V Charge:   -0.715 pC LeadingEdgeTime: 73.368 ns TrailingEdgeTime: 73.877 ns RateCounter       0.00 ===
//.....
/////////////////

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

void convBunchDataUSB2root(TString inputDataFileList, TString inputDataFileFolder, TString outputrootFile);

int main(int argc, char *argv[]){  
  TString inputDataFileList;
  TString inputDataFileFolder;
  TString outputrootFile;
  if(argc == 5 && atoi(argv[1])==0){
    inputDataFileList = argv[2];
    inputDataFileFolder = argv[3];
    outputrootFile = argv[4];
    cout<<"In data file list : "<<inputDataFileList<<endl
        <<"In data path      : "<<inputDataFileFolder<<endl
        <<"Out root file     : "<<outputrootFile<<endl;
    convBunchDataUSB2root(inputDataFileList, inputDataFileFolder, outputrootFile);
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

void convBunchDataUSB2root(TString inputDataFileList, TString inputDataFileFolder, TString outputrootFile){
  TString inputDataFile;

  const Int_t nChannels     = 8;
  const Int_t nSamplesPoint = 1024;

  //USBWC data in one ev//
  Int_t _eventID;
  Float_t _SamplingPeriod;
  Double_t _UnixTime;
  Double_t _TDC;
  Float_t _ch[nChannels][nSamplesPoint];
  Float_t _Baseline[nChannels], baseline;
  Float_t _Amplitude[nChannels], amplitude;
  Float_t _Charge[nChannels], charge;
  Float_t _LeadingEdgeTime[nChannels], leadingEdgeTime;
  Float_t _TrailingEdgeTime[nChannels], trailingEdgeTime;
  Float_t _RateCounter[nChannels], rateCounter; 
 ///////////////////////
  Int_t eventidTest[nChannels];

  ///////////////////Root file with data/////////////////
  TFile *hfile = new TFile( outputrootFile, "RECREATE", "USBWC Data", 1);
  if (hfile->IsZombie()) {
    cout << "PROBLEM with the initialization of the output ROOT ntuple " 
	 << outputrootFile << ": check that the path is correct!!!"
	 << endl;
    exit(-1);
  }
  TTree *tree = new TTree("T", "USBWC Data Tree");
  hfile->SetCompressionLevel(2);
  tree->SetAutoSave(1000000);
  // Create new event
  TTree::SetBranchStyle(0);

  // for chekin the file
  Int_t nChannels_chek_ii = -999;
  TString nSamplingPoints_chek_ss = "["; nSamplingPoints_chek_ss += nSamplesPoint; nSamplingPoints_chek_ss += "]";

  TString chName_ss = "ch[";
  chName_ss += nChannels;
  chName_ss += "][";
  chName_ss += nSamplesPoint;
  chName_ss +="]/F";
  //cout<<"vvv_"<<chName_ss<<"_www"<<endl;

  TString baseline_ss = "Baseline[";
  TString amplitude_ss = "Amplitude[";
  TString charge_ss = "Charge[";
  TString leadingEdgeTime_ss = "LeadingEdgeTime[";
  TString trailingEdgeTime_ss = "TrailingEdgeTime[";
  TString rateCounter_ss = "RateCounter[";

  baseline_ss+=nChannels; baseline_ss+="]/F";
  amplitude_ss+=nChannels; amplitude_ss+="]/F";
  charge_ss+=nChannels; charge_ss+="]/F";
  leadingEdgeTime_ss+=nChannels; leadingEdgeTime_ss+="]/F";
  trailingEdgeTime_ss+=nChannels; trailingEdgeTime_ss+="]/F";
  rateCounter_ss+=nChannels; rateCounter_ss+="]/F";

  //cout<<"vvv_"<<baseline_ss<<"_www"<<endl;
  //cout<<"vvv_"<<amplitude_ss<<"_www"<<endl;
  //cout<<"vvv_"<<charge_ss<<"_www"<<endl;
  //cout<<"vvv_"<<leadingEdgeTime_ss<<"_www"<<endl;
  //cout<<"vvv_"<<trailingEdgeTime_ss<<"_www"<<endl;

  //Event USBWC
  tree->Branch("eventID",&_eventID, "eventID/I");
  tree->Branch("SamplingPeriod", &_SamplingPeriod,  "SamplingPeriod/F");
  tree->Branch("UnixTime",&_UnixTime, "UnixTime/D");
  tree->Branch("TDC",&_TDC, "TDC/D");
  tree->Branch("ch", _ch, chName_ss.Data());
  tree->Branch("Baseline", _Baseline, baseline_ss.Data());
  tree->Branch("Amplitude", _Amplitude, amplitude_ss.Data());
  tree->Branch("Charge", _Charge, charge_ss.Data());
  tree->Branch("LeadingEdgeTime", _LeadingEdgeTime, leadingEdgeTime_ss.Data());
  tree->Branch("TrailingEdgeTime", _TrailingEdgeTime, trailingEdgeTime_ss.Data());
  tree->Branch("RateCounter", _RateCounter, rateCounter_ss.Data());
  ///////////////////////////////////////////////////////

  Int_t i = 0;
  Int_t j = 0;
  string mot;
  Double_t amplVal;
  Int_t chID = 0;

  Int_t nEv = 0;
  Int_t nEv_Tot = 0;
  
  Double_t timeNpevEv = -999.0; //Unix time of previous event

  // Init event variables
  _eventID = -999;
  _SamplingPeriod = -999.0;
  _UnixTime = -999.0;
  _TDC = -999;
  for(j = 0;j<nChannels;j++){
    eventidTest[j] = -999;
    _Baseline[j] = -999.0;
    _Amplitude[j] = -999.0;
    _Charge[j] = -999.0;
    _LeadingEdgeTime[j] = -999.0;
    _TrailingEdgeTime[j] = -999.0;
    _RateCounter[j] = -999.0;
    for(i = 0;i<nSamplesPoint;i++){
      _ch[j][i] = -999.0;
    }
  }

  ifstream indata;
  ifstream indataList;

  Int_t nPointsRead = 0;
  Int_t nChRead = 0;

  indataList.open(inputDataFileList.Data());
  assert(indataList.is_open());
  while (indataList >> mot ){
    inputDataFile = inputDataFileFolder;
    inputDataFile += mot;
    indata.open(inputDataFile.Data()); 
    assert(indata.is_open());
    //cout<<indata.is_open()<<endl;
    cout<<" ---> Conversion of "<<inputDataFile<<endl;
    nEv = 0; // Event counter for a given file
    while (indata >> mot ){
      //cout<<" New word: "<<mot<<endl;

      if(mot == "SAMPLES"){
	indata  >> mot;
	//cout<<mot<<endl;
	if(mot != nSamplingPoints_chek_ss){
	  cout<<" ERROR     --->      mot != nSamplingPoints_chek_ss"<<endl
	      <<"                     mot  = "<<mot<<endl
	      <<"  nSamplingPoints_chek_ss = "<<nSamplingPoints_chek_ss<<endl;
	  assert(0);
	}
      }
      
      if(mot == "CHANNELS"){
	indata  >> mot;
	//cout<<mot<<endl;
	if(mot == "ACQUIRED:"){
	  indata  >> nChannels_chek_ii;
	  //cout<<nChannels_chek_ii<<endl;
	  if(nChannels_chek_ii != nChannels){
	    cout<<" ERROR ---> nChannels_chek_ii != nChannels"<<endl
		<<"            nChannels_chek_ii  = "<<nChannels_chek_ii<<endl
		<<"                     nChannels = "<<nChannels<<endl;
	    assert(0);
	  }
	}
      }
      
      if(mot == "Sampling"){
	indata >> mot;
	if(mot == "Period:"){
	  //cout<<mot<<endl;
	  indata >> _SamplingPeriod;
	  //cout<<_SamplingPeriod<<endl;
	}
      }

      if(mot == "EVENT"){
	nEv++;
	indata  >> _eventID;
	//cout<<"_eventID "<<_eventID<<endl;
      }    

      //=== UnixTime = 1397131148.328 date = 2014.4.10 time = 13h.59m.8s.328ms == TDC = 3593461 ===
      if(mot == "UnixTime"){
	indata  >> mot;
	indata  >> _UnixTime;
	//printf("UnixTime: %20.4f \n", _UnixTime);
	if(timeNpevEv != -999.0 && (_UnixTime - timeNpevEv)<0.0){
	  cout<<endl<<" events (files) in data list are not ordered in time:"<<endl;
	  printf("\t * previous unix time: %20.4f \n", timeNpevEv);
	  printf("\t *  current unix time: %20.4f \n", _UnixTime);
	  assert(0);
	}
	timeNpevEv = _UnixTime;
      }
                 
      if(mot == "TDC"){
	indata  >> mot;
	if(mot == "="){
	  indata  >> _TDC;
	  //cout<<"_TDC = "<<_TDC<<endl;
	  //printf("_TDC: %20.0f \n", _TDC);
	}
      }

      if(mot == "CH:"){
	indata  >> chID;
	//cout<<"chID "<<chID<<endl;
	nChRead++;	
	if( chID < 0 || chID > nChannels){
	  cout<<"chID      = " << chID << " should be between 0 and nChannels!"<<endl;
	  cout<<"nChannels = "<<nChannels<<endl;
	  assert(0);
	}

	///////////////
	//EVENTID:
	indata  >> mot;
	assert( mot == "EVENTID:" );
	indata  >> eventidTest[chID];
	indata  >> mot;
	indata  >> mot;

	///////////////
	//Baseline
	indata  >> mot;
	assert( mot == "Baseline:" );
	indata  >> baseline;
	indata  >> mot;

	///////////////
	//Amplitude
	indata  >> mot;
	assert( mot == "Amplitude:" );
	indata  >> amplitude;
	indata  >> mot;
	
	///////////////
	//Charge:
	indata  >> mot;
	assert( mot == "Charge:" );
	indata  >> charge;
	indata  >> mot;
	
	///////////////
	//LeadingEdgeTime:
	indata  >> mot;
	assert( mot == "LeadingEdgeTime:" );
	indata  >> leadingEdgeTime;
	indata  >> mot;
	
	///////////////
	//TrailingEdgeTime:
	indata  >> mot;
	assert( mot == "TrailingEdgeTime:" );
	indata  >> trailingEdgeTime;
	indata  >> mot;
	
	///////////////
	//RateCounter
	indata  >> mot;
	assert( mot == "RateCounter" );
	indata  >> rateCounter;
	indata  >> mot;

	///////////////////////////////

	_Baseline[chID] = baseline;
	_Amplitude[chID] = amplitude;
	_Charge[chID] = charge;
	_LeadingEdgeTime[chID] = leadingEdgeTime;
	_TrailingEdgeTime[chID] = trailingEdgeTime;
	_RateCounter[chID] = rateCounter;

	nPointsRead = 0;
	for(i = 0;i<nSamplesPoint;i++){
	  indata  >> amplVal;	  
	  nPointsRead++;
	  _ch[chID][i] = amplVal;
	  if(indata.tellg() == -1){
	    cout<<" ---> ERROR : indata.tellg() == -1"<<endl
		<<" amplVal   = "<<amplVal<<endl;
	    cout<<" In file : "<<inputDataFile<<" structure error is expected"<<endl;
	    assert(0);
	  }
	}
	if(nPointsRead != nSamplesPoint){
	  cout<<" ---> ERROR : nPointsRead != nSamplesPoint"<<endl
	      <<" nPointsRead   = "<<nPointsRead<<endl
	      <<" nSamplesPoint = "<<nSamplesPoint<<endl;
	  assert(0);	  
	}
      }

      //cout << "chID = " << chID << " i = " << i << endl;
      if(chID == (nChannels-1)){
      //if(chID == (2-1)){
	  for(j = 0;j<nChannels;j++){
	//for(j = 0;j<2;j++){
	  if(eventidTest[0] != eventidTest[j]){
	    cout<<" eventidTest[0] != eventidTest[j] "<<endl
		<<" eventidTest[0]  = "<<eventidTest[0]<<endl
		<<" eventidTest[j]  = "<<eventidTest[j]<<endl
		<<"             j   = "<<j<<endl;
	    assert(0);	  
	    //if(eventidTest[j] != _eventID){
	    //cout<<" eventidTest[j] != _eventID "<<endl
	    //    <<" eventidTest["<<j<<"]  = "<<eventidTest[j]<<endl
	    //    <<" _eventID        = "<<_eventID<<endl;
	    //assert(0);
	  }
	}

	if(nChRead != nChannels){
	  cout<<" ---> ERROR : nChRead != nChannels"<<endl
	      <<" nChRead   = "<<nChRead<<endl
	      <<" nChannels = "<<nChannels<<endl;
	  cout<<" In file : "<<inputDataFile<<" structure error is expected"<<endl;
	  assert(0);	  
	}
	else{
	  nChRead = 0;
	}
	
	tree->Fill();
	//cout<<_TDC<<endl;
	_eventID = -999;
	_UnixTime = -999.0;
	_TDC = -999;
	for(j = 0;j<nChannels;j++){
	  eventidTest[j] = -999;
	  _Baseline[j] = -999.0;
	  _Amplitude[j] = -999.0;
	  _Charge[j] = -999.0;
	  _LeadingEdgeTime[j] = -999.0;
	  _TrailingEdgeTime[j] = -999.0;
	  _RateCounter[j] = -999.0;
	  for(i = 0;i<nSamplesPoint;i++){
	    _ch[j][i] = -999.0;
	  }
	}
	chID = -999; 
      }

    } // while (indata  >> mot ){
    //cout<<" close "<<endl;

    indata.close();
  }
  indataList.close();
  
  /////////////////////////////
  
  hfile = tree->GetCurrentFile();
  hfile->Write();
  hfile->Close();
  
}
