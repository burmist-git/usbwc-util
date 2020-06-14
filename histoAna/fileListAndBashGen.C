#include <iostream>
#include <fstream>

using namespace std;

Int_t fileListAndBashGen(){


  TString analisysPreff = "./";
  TString dataFilePath = "../";

  ///////////////////////////////////////////////////////////////
  TString convBashHistos    = analisysPreff; convBashHistos   += "convertUSBWCHistos2root8Channels.bash";
  ofstream fileConvBashHistos;
  TString fileConvBashHistosLine;


  ///////////////////////////////////////////////////////////////
  TString mot;
  TString commandName;



  /////////////////////////////////////////////
  // Generation of the list with histograms
  //----------------------------------

    cout<<"-> Generation of the list with histograms "<<endl;

      commandName  = "ls ../";
      commandName += " | grep Histos | grep .dat | grep -v .root > HistosList.dat";
      system(commandName.Data());

      TString filenameHistosList = "HistosList.dat";


  
  /////////////////////////////////////////////
  // Generation of the bash screept to convert histograms
  //----------------------------------


    cout<<"-> Generation of the bash screept to convert histograms "<<endl;
    fileConvBashHistos.open(convBashHistos.Data());
    fileConvBashHistos<<"make clean; make convertUSBWCHistos2root8Channels;"<<endl;
    
    ifstream fileHistosList;
    fileHistosList.open(filenameHistosList.Data()); 
    assert(fileHistosList.is_open());
    
    while(fileHistosList >> mot){

	fileConvBashHistos<<endl;
	fileConvBashHistos<<"echo \"\""<<endl;
	fileConvBashHistosLine  = "./convertUSBWCHistos2root8Channels 0 ";
	fileConvBashHistosLine += dataFilePath;
	fileConvBashHistosLine += mot;
	fileConvBashHistosLine += " ";
	fileConvBashHistosLine += dataFilePath;
	fileConvBashHistosLine += mot;
	fileConvBashHistosLine += ".root";
	fileConvBashHistos<<fileConvBashHistosLine<<endl;
	fileConvBashHistos<<endl;
    }
    
    fileConvBashHistos.close();
    fileHistosList.close();

  
  return 0;
}
