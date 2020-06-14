//root
#include <TH1D.h>
#include <TStyle.h>
#include <TString.h>
#include <TCanvas.h>
#include <TFile.h>
#include <TTree.h>
#include <TMath.h>

//C, C++
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

using namespace std;

const Int_t nCh = 8;
const Int_t nMaxDataFiles = 100000;

//const Int_t nPointMax = 100000;
//const Int_t nSegmentsMax = 100000;//per file

void convertUSBWCMeas2root8ChannelsBin(TString dataFileList,
				       TString datPreff, 
				       TString rootFileN);

int main(int argc, char *argv[] ){
  if(argc == 4){
    TString dataFileList = argv[1];
    TString datPreff = argv[2];
    TString rootFileN = argv[3];
    cout<<endl
	<<" File containing the list of input data files: "<<dataFileList<<endl
	<<" Path of the directory containing these input files: "<<datPreff<<endl
	<<" Name of the output ROOT file: "<<rootFileN<<endl<<endl;
    convertUSBWCMeas2root8ChannelsBin(dataFileList,
				      datPreff, 
				      rootFileN);
  }
  else{
    cout<<endl
	<<"  ERROR ---> in input arguments "<<endl
	<<"             [1] : name of a text file containing the list of binary files with measurements to be converted to ROOT"<<endl
	<<"             [2] : path of the directory containing the input USBWC files"<<endl
	<<"             [3] : name of the output ROOT file"<<endl;
  }
  return 0;
}

void convertUSBWCMeas2root8ChannelsBin(TString dataFileList,
				       TString datPreff, 
				       TString rootFileN){
  FILE *pFile;
  long totFileSizeByte;
  long currentPositionByte;
  char *buffer;
  TString buffTmp;

  int EventNumber;
  double EpochTime;
  unsigned int Year;
  unsigned int Month;
  unsigned int Day;
  unsigned int Hour;
  unsigned int Minute;
  unsigned int Second;
  unsigned int Millisecond;
  int channel;
  int EventIDsamIndex;
  float	MeasuredBaselineV[nCh];
  float	AmplitudeValue[nCh];
  float	ComputedCharge[nCh];
  float	RiseTimeInstant[nCh];
  float	FallTimeInstant[nCh];
  float	RawTriggerRate[nCh];
  float floatR;

  Int_t EventNumber_usbwc;
  Double_t EpochTime_usbwc;
  Int_t Year_usbwc;
  Int_t Month_usbwc;
  Int_t Day_usbwc;
  Int_t Hour_usbwc;
  Int_t Minute_usbwc;
  Int_t Second_usbwc;
  Int_t Millisecond_usbwc;
  Int_t channel_usbwc;
  Int_t EventIDsamIndex_usbwc;
  Float_t MeasuredBaseline_usbwc[nCh];
  Float_t AmplitudeValue_usbwc[nCh];
  Float_t ComputedCharge_usbwc[nCh];
  Float_t RiseTimeInstant_usbwc[nCh];
  Float_t FallTimeInstant_usbwc[nCh];
  Float_t RawTriggerRate_usbwc[nCh];
  Int_t nCh_usbwc = nCh;
  //USBWC data in one ev//
  Int_t eventID;
  Double_t UnixTime;
  Double_t TDC;
  Int_t nChusbwc = nCh_usbwc;
  Float_t MeasuredBaseline[nCh];
  Float_t Amplitude[nCh];
  Float_t Charge[nCh];
  Float_t LeadingEdgeTime[nCh];
  Float_t TrailingEdgeTime[nCh];
  Float_t RateCounter[nCh];
  TFile *hfile = new TFile( rootFileN, "RECREATE", "Data", 1);
  if (hfile->IsZombie()) {
    cout << "PROBLEM with the initialization of the output ROOT ntuple " 
	 << rootFileN << ": check that the path is correct!!!"
	 << endl;
    exit(-1);
  }
  TTree *tree = new TTree("T", "Data Tree");
  hfile->SetCompressionLevel(2);
  tree->SetAutoSave(1000000);
  // Create new event
  TTree::SetBranchStyle(0);
  //Event
  // 16.02.2015 new form of the event - same as in convertUSBWCMeas2root8Channels
  //tree->Branch("EventNumber_usbwc",&EventNumber_usbwc, "EventNumber_usbwc/I");
  //tree->Branch("EpochTime_usbwc",&EpochTime_usbwc, "EpochTime_usbwc/D");
  //tree->Branch("Year_usbwc",&Year_usbwc, "Year_usbwc/I");
  //tree->Branch("Month_usbwc",&Month_usbwc, "Month_usbwc/I");
  //tree->Branch("Day_usbwc",&Day_usbwc, "Day_usbwc/I");
  //tree->Branch("Hour_usbwc",&Hour_usbwc, "Hour_usbwc/I");
  //tree->Branch("Minute_usbwc",&Minute_usbwc, "Minute_usbwc/I");
  //tree->Branch("Second_usbwc",&Second_usbwc, "Second_usbwc/I");
  //tree->Branch("Millisecond_usbwc",&Millisecond_usbwc, "Millisecond_usbwc/I");
  //tree->Branch("nCh_usbwc",&nCh_usbwc, "nCh_usbwc/I");
  //tree->Branch("MeasuredBaseline_usbwc", MeasuredBaseline_usbwc, "MeasuredBaseline_usbwc[nCh_usbwc]/F");
  //tree->Branch("AmplitudeValue_usbwc", AmplitudeValue_usbwc, "AmplitudeValue_usbwc[nCh_usbwc]/F");
  //tree->Branch("ComputedCharge_usbwc", ComputedCharge_usbwc, "ComputedCharge_usbwc[nCh_usbwc]/F");
  //tree->Branch("RiseTimeInstant_usbwc", RiseTimeInstant_usbwc, "RiseTimeInstant_usbwc[nCh_usbwc]/F");
  //tree->Branch("FallTimeInstant_usbwc", FallTimeInstant_usbwc, "FallTimeInstant_usbwc[nCh_usbwc]/F");
  //tree->Branch("RawTriggerRate_usbwc", RawTriggerRate_usbwc, "RawTriggerRate_usbwc[nCh_usbwc]/F");
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

  Int_t i,j;

  ifstream indataFileList;
  string mot;
  TString fileN;

  indataFileList.open(dataFileList.Data()); 
  assert(indataFileList.is_open());
  while (indataFileList >> mot){
    fileN = datPreff + "/" + mot;
    pFile = fopen (fileN.Data(), "rb" );
    if (pFile==NULL) {fputs ("File error",stderr); exit (1);}  
    cout<<" ---> File to convert : "<<fileN<<endl;
    // obtain file size:
    fseek (pFile , 0 , SEEK_END);
    totFileSizeByte = ftell (pFile);
    rewind (pFile);
    //cout<<"totFileSizeByte = "<<totFileSizeByte<<endl;
    // allocate memory to contain the whole file:
    buffer = (char*) malloc (sizeof(char)*totFileSizeByte);
    if (buffer == NULL) {fputs ("Memory error",stderr); exit (2);}
    // copy the file into the buffer:
    //result = fread (buffer,1,lSize,pFile);
    currentPositionByte = 0;
    fread (buffer,1,286,pFile);
    currentPositionByte = currentPositionByte + 286;
    buffTmp = buffer[283];
    if( buffTmp != "="){
      cout<<" ERROR --> Wrong format"<<endl
	  <<"         : buffer[283] != ="<<endl;
      cout<<std::hex<<(int)buffer[283] << '\n';
      cout<<std::dec;
      //cout<<10<< '\n';
      assert(0);
    } 
    while(currentPositionByte<totFileSizeByte){
      //for(j = 0;j<100;j++){
      currentPositionByte += fread (&EventNumber,1,4,pFile);
      currentPositionByte += fread (&EpochTime,1,8,pFile);
      currentPositionByte += fread (&Year,1,4,pFile);
      currentPositionByte += fread (&Month,1,4,pFile);
      currentPositionByte += fread (&Day,1,4,pFile);
      currentPositionByte += fread (&Hour,1,4,pFile);
      currentPositionByte += fread (&Minute,1,4,pFile);
      currentPositionByte += fread (&Second,1,4,pFile);
      currentPositionByte += fread (&Millisecond,1,4,pFile);    
      EventNumber_usbwc = EventNumber;
      EpochTime_usbwc = EpochTime;
      Year_usbwc = Year;
      Month_usbwc = Month;
      Day_usbwc = Day;
      Hour_usbwc = Hour;
      Minute_usbwc = Minute;
      Second_usbwc = Second;
      Millisecond_usbwc = Millisecond;
      eventID = EventNumber_usbwc;
      UnixTime = EpochTime_usbwc;
      for(i = 0;i<nCh;i++){
	currentPositionByte += fread (&channel,1,4,pFile);
	currentPositionByte += fread (&EventIDsamIndex,1,4,pFile);
	TDC = EventIDsamIndex;
	currentPositionByte += fread (&floatR,1,4,pFile); MeasuredBaselineV[i] = floatR;
	currentPositionByte += fread (&floatR,1,4,pFile); AmplitudeValue[i] = floatR;
	currentPositionByte += fread (&floatR,1,4,pFile); ComputedCharge[i] = floatR;
	currentPositionByte += fread (&floatR,1,4,pFile); RiseTimeInstant[i] = floatR;
	currentPositionByte += fread (&floatR,1,4,pFile); FallTimeInstant[i] = floatR;
	currentPositionByte += fread (&floatR,1,4,pFile); RawTriggerRate[i] = floatR;
	EventIDsamIndex_usbwc = EventIDsamIndex;
	MeasuredBaseline_usbwc[i] = MeasuredBaselineV[i];
	AmplitudeValue_usbwc[i]   = AmplitudeValue[i];
	ComputedCharge_usbwc[i]   = ComputedCharge[i];
	RiseTimeInstant_usbwc[i]  = RiseTimeInstant[i];
	FallTimeInstant_usbwc[i]  = FallTimeInstant[i];
	RawTriggerRate_usbwc[i]   = RawTriggerRate[i];
	MeasuredBaseline[i] = MeasuredBaselineV[i];
	Amplitude[i]   = AmplitudeValue[i];
	Charge[i]   = ComputedCharge[i];
	LeadingEdgeTime[i]  = RiseTimeInstant[i];
	TrailingEdgeTime[i]  = FallTimeInstant[i];
	RateCounter[i]   = RawTriggerRate[i];
	//cout<<"Channel    "<<channel<<endl;
	//cout<<"EventIDsamIndex    "<<EventIDsamIndex<<endl;
      }
      //cout<<"EventNumber         "<<EventNumber<<endl;
      //cout<<"totFileSizeByte     "<<totFileSizeByte<<endl
      //<<"currentPositionByte "<<currentPositionByte<<endl;
      //for(i = 0;i<nCh;i++){
      //cout<<"MeasuredBaseline[i] "<<MeasuredBaseline[i]<<endl;
      //cout<<"AmplitudeValue[i]   "<<AmplitudeValue[i]<<endl;
      //}
      //printf ("EpochTime %13.0f \n", EpochTime);
      //cout<<"Year         "<<Year<<endl
      //<<"Month        "<<Month<<endl
      //<<"Day          "<<Day<<endl
      //<<"Hour         "<<Hour<<endl
      //<<"Minute       "<<Minute<<endl
      //<<"Second       "<<Second<<endl
      //<<"Millisecond  "<<Millisecond<<endl;
      tree->Fill();
    } 
    // terminate
    fclose (pFile);
    free (buffer);
  }
  indataFileList.close();
  hfile = tree->GetCurrentFile();
  hfile->Write();
  hfile->Close();
}

