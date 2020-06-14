#include <iostream>
#include <fstream>
#include <stdlib.h>

using namespace std;

bool fexists(const char *filename);

Int_t fileListAndBashGen(){

  const Int_t nMaxFiles = 10000;
  const Int_t nn = 1;
  TString analisysPreff = "/home/leonid/SPS_CpFM_test_July_2015/";
  TString GenerationOfTheListsWithMeasurements = "yes";                      // should be yes all the time
  TString GenerationOfTheListsWithRates = "yes";                             // should be yes all the time (added : SD)
  TString GenerationOfTheListsWithWF = "yes";                                // should be yes all the time
  TString CreatOf_RunFolders_in_root_data_root_data_L1_root_data_L2_Folders = "yes";   // should be yes all the time
  TString CreatOf_RunFolders_in_root_data_Rates_Folders = "yes";             // should be yes all the time (added : SD)
  TString GenerationOfTheBashScreeptToConvertMeasurements = "yes";           // should be yes all the time
  TString GenerationOfTheBashScreeptToConvertWF = "yes";                     // should be yes all the time
  TString GenerationOfTheBashScreeptToConvertRates = "yes";                  // should be yes all the time (added : SD)
  TString GenerationOfTheListsWithRootFilesForL1Conversion = "yes";          // should be yes all the time
  TString GenerationOfTheBashScreeptToGetParamatersFromWF_L1 = "yes";        // should be yes all the time
  TString GenerationOfTheBashScreeptToGetHistogramsFromWF_L2 = "yes";        // should be yes all the time
  TString GenerationOfTheBashScreeptToGetHistogramsFromMeasurements = "yes"; // should be yes all the time
  TString Make_tar_gzArchivationOfTheDataFiles = "yes";                      // should be yes all the time
  ///////////////////////////////////////////////////////////////
  ifstream fileRunList("runList.dat");
  TString measurementsListName = "measurementsList.dat";
  TString ratesListName    = "ratesList.dat"; // (added by SD)
  TString wfListName       = "dataFileList.dat";
  TString rootFilesList    = "rootFilesList.dat";
  ///////////////////////////////////////////////////////////////
  TString anaSetup        = "anaSetup.dat";
  TString convBashMeas    = analisysPreff; convBashMeas   += "ana/convertUSBWCMeas2root8Channels.bash";
  TString convBashRates   = analisysPreff; convBashRates  += "ana/convertUSBWCRates2root8Channels.bash"; // (added by SD)
  TString convBashWf      = analisysPreff; convBashWf     += "ana/convertUSBWC2root8Channels.bash";
  TString dataFilePath    = analisysPreff; dataFilePath   += "data/";
  TString anaFilesPath    = analisysPreff; anaFilesPath   += "ana/";
  TString rootFilePath    = analisysPreff; rootFilePath   += "root_data/";
  TString rootL1FilePath  = analisysPreff; rootL1FilePath += "root_data_L1/";
  TString rootL2FilePath  = analisysPreff; rootL2FilePath += "root_data_L2/";
  TString runMeascript    = analisysPreff; runMeascript   += "ana/runMeasures.bash";
  TString runL1script     = analisysPreff; runL1script    += "ana/runL1.bash";
  TString runL2script     = analisysPreff; runL2script    += "ana/runL2.bash";
  ///////////////////////////////////////////////////////////////
  TString  targzscript    = dataFilePath; targzscript += "targzscript.bash";
  TString  firstMeasFileNameArr[nn]; 
  TString  firstMeasFileName;
  TString  firstMeasFileNameTest;
  Int_t    nFilesMeas[nn];
  TString  fileMeasName;
  ofstream fileMeas;
  TString  firstRatesFileNameArr[nn]; // (added by SD)
  TString  firstRatesFileName; // (added by SD)
  TString  firstRatesFileNameTest;// (added by LB)
  Int_t    nFilesRates[nn]; // (added by SD)
  TString  fileRatesName; // (added by SD)
  ofstream fileRates; // (added by SD)
  TString  firstWfFileNameArr[nn]; 
  TString  firstWfFileName;
  TString  firstWfFileNameTest;
  Int_t    nFilesWf[nn];
  TString  fileWfName;
  ofstream fileWf;
  ofstream fileConvBashMeas;
  TString  fileConvBashMeasLine;
  ofstream fileConvBashRates; // (added by SD)
  TString  fileConvBashRatesLine; // (added by SD)
  ofstream fileConvBashWf;
  TString  fileConvBashWfLine;
  ofstream fileRunMeasScript;
  TString  fileRunMeasScriptLine;
  ofstream fileRunL1script;
  TString  fileRunL1scriptLine;
  ofstream fileRunL2script;
  TString  fileRunL2scriptLine;
  ofstream fileTargzScript;
  TString  fileTargzScriptLine;

  TString runFolder[nn]; 
  Int_t nFiles = 0;
  TString mot;
  Int_t i = 0;
  Int_t j = 0;
  TString commandName;
  ifstream indata;

  Int_t nwfRootFiles;

  ifstream rootListExistFile;
  TString rootListExistName;

  // Get all the runs
  cout<<" Read Run folders "<<endl;
  if (fileRunList.is_open()){
    while (fileRunList>>mot){
      //cout << mot << endl;
      if(i>=nn){
	cout<<" --> Error: i>=nn "<<endl
	    <<"            i = "<<i<<endl
	    <<"           nn = "<<nn<<endl;
	exit(-1);
      }
      runFolder[i] = mot;
      i++;
    }
    fileRunList.close();
  }

  /////////////////////////////////////////////
  // Generation of the lists with measurements
  //----------------------------------
  // 01 Get number of meas files
  if(GenerationOfTheListsWithMeasurements == "yes"){
    cout<<" Generation of the lists with measurements "<<endl;
    cout<<"    ---> 01 "<<endl;
    for(i = 0;i<nn;i++){
      commandName  = "ls -lrt ../data/";
      commandName += runFolder[i];
      commandName += " | grep Meas | wc -l > a.tmp";
      system(commandName.Data());
      indata.open("a.tmp");
      indata>>nFiles;
      nFilesMeas[i] = nFiles;
      indata.close();
      system("rm a.tmp");
      //cout<<commandName<<endl;
    }
    //for(i = 0;i<nn;i++){
    //cout<<nFilesMeas[i]<<endl;
    //}
    // 02 Get first name of the file with meas data
    cout<<"    ---> 02 "<<endl;
    for(i = 0;i<nn;i++){
      commandName  = "ls ../data/";
      commandName += runFolder[i];
      commandName += " | grep Meas | grep -v dat_ > a.tmp";
      system(commandName.Data());
      indata.open("a.tmp");
      indata>>mot;
      firstMeasFileNameArr[i] = mot;
      indata.close();
      system("rm a.tmp");
      //cout<<commandName<<endl;
    }
    //for(i = 0;i<nn;i++){
    //cout<<firstMeasFileNameArr[i]<<endl;
    //}  
    // 03 Fill the file list for measurements
    cout<<"    ---> 03 "<<endl;
    for(i = 0;i<nn;i++){
      fileMeasName = "../data/";
      fileMeasName += runFolder[i];
      fileMeasName += "/";
      fileMeasName += measurementsListName;
      fileMeas.open(fileMeasName.Data());
      // LB 27.10.2015
      //for(j = 0;j<nFilesMeas[i];j++){
      for(j = 0;j<nMaxFiles;j++){
	firstMeasFileName      = firstMeasFileNameArr[i];
	firstMeasFileName     += GetFileNameEnd(j);
	firstMeasFileNameTest  = "../data/";
	firstMeasFileNameTest += runFolder[i];
	firstMeasFileNameTest += "/";
	firstMeasFileNameTest += firstMeasFileName;
	if(fexists(firstMeasFileNameTest.Data())){
	  //cout<<firstMeasFileNameTest<<endl;
	  fileMeas<<firstMeasFileName<<endl;
	}
      }
      fileMeas.close();
    }
  }


  /////////////////////////////////////////////
  // Generation of the lists with rates (added by SD)
  //----------------------------------
  // 01 Get number of rates files
  if(GenerationOfTheListsWithRates == "yes"){
    cout<<" Generation of the lists with rates "<<endl;
    cout<<"    ---> 01 "<<endl;
    for(i = 0;i<nn;i++){
      commandName  = "ls -lrt ../data/";
      commandName += runFolder[i];
      commandName += " | grep Rates | wc -l > a.tmp";
      system(commandName.Data());
      indata.open("a.tmp");
      indata>>nFiles;
      nFilesRates[i] = nFiles;
      indata.close();
      system("rm a.tmp");
    }

    // 02 Get first name of the file with rates data
    cout<<"    ---> 02 "<<endl;
    for(i = 0;i<nn;i++){
      commandName  = "ls ../data/";
      commandName += runFolder[i];
      commandName += " | grep Rates | grep -v dat_ > a.tmp";
      system(commandName.Data());
      indata.open("a.tmp");
      indata>>mot;
      firstRatesFileNameArr[i] = mot;
      indata.close();
      system("rm a.tmp");
    }

    // 03 Fill the file list for rates
    cout<<"    ---> 03 "<<endl;
    for(i = 0;i<nn;i++){
      fileRatesName = "../data/";
      fileRatesName += runFolder[i];
      fileRatesName += "/";
      fileRatesName += ratesListName;
      fileRates.open(fileRatesName.Data());
      // LB 27.10.2015
      //for(j = 0;j<nFilesRates[i];j++){
      for(j = 0;j<nMaxFiles;j++){
	firstRatesFileName      = firstRatesFileNameArr[i];
	firstRatesFileName     += GetFileNameEnd(j);
	firstRatesFileNameTest  = "../data/";
	firstRatesFileNameTest += runFolder[i];
	firstRatesFileNameTest += "/";
	firstRatesFileNameTest += firstRatesFileName;
	if(fexists(firstRatesFileNameTest.Data())){
	  //cout<<firstRatesFileNameTest<<endl;
	  fileRates<<firstRatesFileName<<endl;
	} 
     }
      fileRates.close();
    }
  }

  
  /////////////////////////////////////////////
  // Generation of the lists with wf
  //----------------------------------
  // 01 Get number of wf files
  if(GenerationOfTheListsWithWF == "yes"){
    cout<<" Generation of the lists with wf "<<endl;
    cout<<"    ---> 01 "<<endl;
    for(i = 0;i<nn;i++){
      commandName  = "ls -lrt ../data/";
      commandName += runFolder[i];
      commandName += " | grep -v Meas | grep -v Rates | grep Run | grep Ascii | grep Data | wc -l > a.tmp";
      system(commandName.Data());
      indata.open("a.tmp");
      indata>>nFiles;
      nFilesWf[i] = nFiles;
      indata.close();
      system("rm a.tmp");
      //cout<<commandName<<endl;
    }
    //for(i = 0;i<nn;i++){
    //cout<<nFilesWf[i]<<endl;
    //}
    // 02 Get first name of the file with wf data
    cout<<"    ---> 02 "<<endl;
    for(i = 0;i<nn;i++){
      commandName  = "ls ../data/";
      commandName += runFolder[i];
      commandName += " | grep -v Meas | grep -v Rates | grep Run | grep Ascii | grep Data | grep -v dat_ > a.tmp";
      system(commandName.Data());
      indata.open("a.tmp");
      indata>>mot;
      firstWfFileNameArr[i] = mot;
      indata.close();
      system("rm a.tmp");
      //cout<<commandName<<endl;
    }
    //for(i = 0;i<nn;i++){
    //cout<<firstWfFileNameArr[i]<<endl;
    //}
    // 03 Fill the file list for wf
    cout<<"    ---> 03 "<<endl;
    for(i = 0;i<nn;i++){
      fileWfName = "../data/";
      fileWfName += runFolder[i];
      fileWfName += "/";
      fileWfName += wfListName;
      fileWf.open(fileWfName.Data());
      // LB 27.10.2015
      //for(j = 0;j<nFilesWf[i];j++){
      for(j = 0;j<nMaxFiles;j++){
	firstWfFileName      = firstWfFileNameArr[i];
	firstWfFileName     += GetFileNameEnd(j);
	firstWfFileNameTest  = "../data/";
	firstWfFileNameTest += runFolder[i];
	firstWfFileNameTest += "/";
	firstWfFileNameTest += firstWfFileName;
	if(fexists(firstWfFileNameTest.Data())){
	  fileWf<<firstWfFileName<<endl;
	}
      }
      fileWf.close();
    }
  }
  
  /////////////////////////////////////////////
  // Creat of root_data root_data_L1 root_data_L2 folders
  //----------------------------------
  if(CreatOf_RunFolders_in_root_data_root_data_L1_root_data_L2_Folders == "yes"){
    cout<<" Creat of root_data root_data_L1 root_data_L2 folders "<<endl;
    gSystem->MakeDirectory(rootFilePath);
    gSystem->MakeDirectory(rootL1FilePath);
    gSystem->MakeDirectory(rootL2FilePath);
    for(i = 0;i<nn;i++){
      commandName = rootFilePath;
      commandName += runFolder[i];
      //cout<<"   "<<commandName<<endl;
      gSystem->MakeDirectory(commandName);
    }
    for(i = 0;i<nn;i++){
      //commandName  = "mkdir ";
      //commandName += rootL1FilePath;
      //commandName += runFolder[i];
      //system(commandName.Data());
      commandName = rootL1FilePath;
      commandName += runFolder[i];
      //cout<<"   "<<commandName<<endl;
      gSystem->MakeDirectory(commandName);
    }
    for(i = 0;i<nn;i++){
      //commandName  = "mkdir ";
      //commandName += rootL2FilePath;
      //commandName += runFolder[i];
      //system(commandName.Data());
      commandName = rootL2FilePath;
      commandName += runFolder[i];
      //cout<<"   "<<commandName<<endl;
      gSystem->MakeDirectory(commandName);
    }
  }

  
  /////////////////////////////////////////////
  // Generation of the bash screept to convert measurements
  //----------------------------------
  //./convertUSBWCMeas2root8Channels 0 
  //            /home/leonid/H8-test_2014_November/data/Run_0_Data_11_18_2014_Ascii/measurementsList.dat 
  //            /home/leonid/H8-test_2014_November/data/Run_0_Data_11_18_2014_Ascii/ 
  //            /home/leonid/H8-test_2014_November/root_data_L1/Run_0_Data_11_18_2014_Ascii/Run_0_Measurements_Only_11_18_2014.dat.root
  if(GenerationOfTheBashScreeptToConvertMeasurements == "yes"){
    cout<<" Generation of the bash screept to convert measurements "<<endl;
    fileConvBashMeas.open(convBashMeas.Data());
    fileConvBashMeas<<"make clean; make convertUSBWCMeas2root8Channels;"<<endl;
    for(i = 0;i<nn;i++){
      if(firstMeasFileNameArr[i] != ""){
	fileConvBashMeas<<endl;
	fileConvBashMeas<<"echo \"\""<<endl;
	fileConvBashMeasLine  = "./convertUSBWCMeas2root8Channels 0 ";
	fileConvBashMeasLine += dataFilePath;
	fileConvBashMeasLine += runFolder[i];
	fileConvBashMeasLine += "/";
	fileConvBashMeasLine += measurementsListName;
	fileConvBashMeasLine += " ";
	fileConvBashMeasLine += dataFilePath;
	fileConvBashMeasLine += runFolder[i];
	fileConvBashMeasLine += "/ ";
	fileConvBashMeasLine += rootL1FilePath;
	fileConvBashMeasLine += runFolder[i];
	fileConvBashMeasLine += "/";
	fileConvBashMeasLine += firstMeasFileNameArr[i];
	fileConvBashMeasLine += ".root";
	fileConvBashMeas<<fileConvBashMeasLine<<endl;
	fileConvBashMeas<<endl;
      }
    }
    fileConvBashMeas.close();
  }

  /////////////////////////////////////////////
  // Generation of the bash screept to convert rates (added by SD)
  //----------------------------------
  //./convertUSBWCMeas2root8Channels 0 
  //            /home/leonid/H8-test_2014_November/data/Run_0_Data_11_18_2014_Ascii/measurementsList.dat 
  //            /home/leonid/H8-test_2014_November/data/Run_0_Data_11_18_2014_Ascii/ 
  //            /home/leonid/H8-test_2014_November/root_data_L1/Run_0_Data_11_18_2014_Ascii/Run_0_Measurements_Only_11_18_2014.dat.root
  if(GenerationOfTheBashScreeptToConvertRates == "yes"){
    cout<<" Generation of the bash screept to convert rates "<<endl;
    fileConvBashRates.open(convBashRates.Data());
    fileConvBashRates<<"make clean; make convertUSBWCRates2root8Channels;"<<endl;
    for(i = 0;i<nn;i++){
      if(firstRatesFileNameArr[i] != ""){
	fileConvBashRates<<endl;
	fileConvBashRates<<"echo \"\""<<endl;
	fileConvBashRatesLine  = "./convertUSBWCRates2root8Channels 0 ";
	fileConvBashRatesLine += dataFilePath;
	fileConvBashRatesLine += runFolder[i];
	fileConvBashRatesLine += "/";
	fileConvBashRatesLine += ratesListName;
	fileConvBashRatesLine += " ";
	fileConvBashRatesLine += dataFilePath;
	fileConvBashRatesLine += runFolder[i];
	fileConvBashRatesLine += "/ ";
	fileConvBashRatesLine += rootL1FilePath;
	fileConvBashRatesLine += runFolder[i];
	fileConvBashRatesLine += "/";
	fileConvBashRatesLine += firstRatesFileNameArr[i];
	fileConvBashRatesLine += ".root";
	fileConvBashRates<<fileConvBashRatesLine<<endl;
	fileConvBashRates<<endl;
      }
    }
    fileConvBashRates.close();
  }

  /////////////////////////////////////////////
  // Generation of the bash screept to convert wf
  //----------------------------------
  //./convertUSBWC2root8Channels 0 
  //             /home/leonid/H8-test_2014_October/data/Run_0_Data_10_13_2014_Ascii/dataFileList.dat 
  //             /home/leonid/H8-test_2014_October/data/Run_0_Data_10_13_2014_Ascii/ 
  //             /home/leonid/H8-test_2014_October/root_data/Run_0_Data_10_13_2014_Ascii/Run_0_Data_11_18_2014_Ascii.dat.root
  if(GenerationOfTheBashScreeptToConvertWF == "yes"){
    cout<<" Generation of the bash screept to convert wf "<<endl;
    fileConvBashWf.open(convBashWf.Data());
    fileConvBashWf<<"make clean; make convertUSBWC2root8Channels;"<<endl;
    for(i = 0;i<nn;i++){
      if(firstWfFileNameArr[i] != ""){
	fileConvBashWf<<endl;
	fileConvBashWf<<"echo \"\""<<endl;
	fileConvBashWfLine  = "./convertUSBWC2root8Channels 0 ";
	fileConvBashWfLine += dataFilePath;
	fileConvBashWfLine += runFolder[i];
	fileConvBashWfLine += "/";
	fileConvBashWfLine += wfListName;
	fileConvBashWfLine += " ";
	fileConvBashWfLine += dataFilePath;
	fileConvBashWfLine += runFolder[i];
	fileConvBashWfLine += "/ ";
	fileConvBashWfLine += rootFilePath;
	fileConvBashWfLine += runFolder[i];
	fileConvBashWfLine += "/";
	fileConvBashWfLine += firstWfFileNameArr[i];
	fileConvBashWfLine += ".root";
	fileConvBashWf<<fileConvBashWfLine<<endl;
	fileConvBashWf<<endl;
      }
    }
    fileConvBashWf.close();
  }

  /////////////////////////////////////////////
  // Generation of the lists with root files
  // for L1 conversion
  //----------------------------------
  //ls /home/leonid/H8-test_2014_November/root_data/Run_0_Data_11_18_2014_Ascii/ | grep .root | wc -l
  if(GenerationOfTheListsWithRootFilesForL1Conversion == "yes"){
    cout<<" Generation of the lists with root files for L1 conversion"<<endl;
    for(i = 0;i<nn;i++){
      commandName  = "ls ";
      commandName += rootFilePath;
      commandName += runFolder[i];
      commandName += "/";
      commandName += " | grep .root | wc -l > a.tmp";
      system(commandName.Data());
      indata.open("a.tmp");
      indata>>nwfRootFiles;
      if(nwfRootFiles == 0){
	cout<<" ERROR ---> file :  "<<endl;
	cout<<rootFilePath<<runFolder[i]<<"/"<<firstWfFileNameArr[i]<<".root"<<endl;
	cout<<" doesnot exist"<<endl;
      }
      else if(nwfRootFiles == 1){
	commandName  = "echo \"";
	commandName += rootFilePath;
	commandName += runFolder[i];
	commandName += "/";
	//commandName += firstWfFileNameArr[i];
	commandName += runFolder[i];
	commandName += ".dat";
	commandName += ".root\"";
	commandName += " > ";
	commandName += rootFilePath;
	commandName += runFolder[i];
	commandName += "/";
	commandName += rootFilesList;
	//cout<<commandName<<endl;
	system(commandName.Data());
      }
      else if(nwfRootFiles > 1){
	cout<<" ERROR ---> nwfRootFiles > 1"<<endl
	    <<"            nwfRootFiles = "<<nwfRootFiles<<endl;
      }
      else{
	cout<<" ERROR ---> nwfRootFiles = "<<nwfRootFiles<<endl;
      }
      indata.close();
      system("rm a.tmp");
      //cout<<commandName<<endl;
    }
  }
  
  /////////////////////////////////////////////
  // Generation of the bash screept to get paramaters
  // from wf (L1)
  //----------------------------------
  //./runL1 0 
  //        /home/leonid/H8-test_2014_November/root_data/Run_0_Data_11_18_2014_Ascii/rootFilesList.dat 
  //        /home/leonid/H8-test_2014_November/root_data/anaSetup.dat 
  //        /home/leonid/H8-test_2014_November/root_data_L1/Run_0_Data_11_18_2014_Ascii/Run_0_Data_11_18_2014_Ascii.dat.L1.root
  if(GenerationOfTheBashScreeptToGetParamatersFromWF_L1 == "yes"){
    cout<<" Generation of the bash screept to get paramaters from wf (L1) "<<endl;
    fileRunL1script.open(runL1script.Data());
    fileRunL1script<<"make clean; make runL1;"<<endl;
    for(i = 0;i<nn;i++){
      rootListExistName = rootFilePath;
      rootListExistName += runFolder[i];
      rootListExistName += "/";
      rootListExistName += rootFilesList;
      rootListExistFile.open(rootListExistName.Data());
      if(rootListExistFile.good()) {
	rootListExistFile.close();      
	fileRunL1script<<endl;
	fileRunL1script<<"echo \"\""<<endl;
	fileRunL1scriptLine  = "./runL1 0 ";
	fileRunL1scriptLine += rootFilePath;
	fileRunL1scriptLine += runFolder[i];
	fileRunL1scriptLine += "/";
	fileRunL1scriptLine += rootFilesList;
	fileRunL1scriptLine += " ";
	fileRunL1scriptLine += anaFilesPath;
	fileRunL1scriptLine += anaSetup;
	fileRunL1scriptLine += " ";
	fileRunL1scriptLine += rootL1FilePath;
	fileRunL1scriptLine += runFolder[i];
	fileRunL1scriptLine += "/";
	//fileRunL1scriptLine += firstWfFileNameArr[i];
	fileRunL1scriptLine += runFolder[i];
	fileRunL1scriptLine += ".dat";
	fileRunL1scriptLine += ".L1.root";
	//cout<<fileRunL1scriptLine<<endl;
	fileRunL1script<<fileRunL1scriptLine<<endl;
	fileRunL1script<<endl;
      }
    }
    fileRunL1script.close();
  }
  
  /////////////////////////////////////////////
  // Generation of the bash screept to get histograms
  // from wf (L2)
  //----------------------------------
  //./runL2 0 
  //        /home/leonid/H8-test_2014_November/root_data_L1/Run_0_Data_11_18_2014_Ascii/Run_0_Data_11_18_2014_Ascii.dat.L1.root 
  //        /home/leonid/H8-test_2014_November/root_data_L2/Run_0_Data_11_18_2014_Ascii/Run_0_Data_11_18_2014_Ascii.dat.L1.L2.root
  if(GenerationOfTheBashScreeptToGetHistogramsFromWF_L2 == "yes"){
    cout<<" Generation of the bash screept to get histograms from wf (L2) "<<endl;
    fileRunL2script.open(runL2script.Data());
    fileRunL2script<<"make clean; make runL2;"<<endl;
    for(i = 0;i<nn;i++){
      fileRunL2script<<endl;
      fileRunL2script<<"#echo \"\""<<endl;
      fileRunL2scriptLine  = "#./runL2 0 ";
      fileRunL2scriptLine += rootL1FilePath;
      fileRunL2scriptLine += runFolder[i];
      fileRunL2scriptLine += "/";
      commandName  = "ls ";
      commandName +=rootL1FilePath;
      commandName += runFolder[i];
      commandName += " | grep -v Meas | grep -v Rates | grep .root > a.tmp";
      system(commandName.Data());
      indata.open("a.tmp");
      indata>>mot;
      indata.close();
      system("rm a.tmp");
      if(mot != ""){
	//fileRunL2scriptLine += firstWfFileNameArr[i];
	fileRunL2scriptLine += mot;
	fileRunL2scriptLine += " ";
	fileRunL2scriptLine += rootL2FilePath;
	fileRunL2scriptLine += runFolder[i];
	fileRunL2scriptLine += "/";
	fileRunL2scriptLine += mot;
	fileRunL2scriptLine += ".L2.root ";
	//cout<<fileRunL2scriptLine<<endl;
	fileRunL2script<<fileRunL2scriptLine<<endl;
	fileRunL2script<<endl;
      }
    }
    fileRunL2script.close();
  }
  
  /////////////////////////////////////////////
  // Generation of the bash screept to get histograms
  // from measurements
  //----------------------------------
  //./runMeasures 0 
  //              ../root_data_L1/Run_1_Data_12_13_2014_Ascii/Run_1_Measurements_Only_12_13_2014.dat.root 
  //              ../root_data_L2/Run_1_Data_12_13_2014_Ascii/Run_1_Measurements_Only_12_13_2014.dat.root_hist.root
  if(GenerationOfTheBashScreeptToGetHistogramsFromMeasurements == "yes"){
    cout<<" Generation of the bash screept to get histograms from Measurements "<<endl;
    fileRunMeasScript.open(runMeascript.Data());
    fileRunMeasScript<<"make clean; make runMeasures;"<<endl;
    for(i = 0;i<nn;i++){
      fileRunMeasScript<<endl;
      fileRunMeasScript<<"echo \"\""<<endl;
      fileRunMeasScriptLine  = "./runMeasures 0 ";
      fileRunMeasScriptLine += rootL1FilePath;
      fileRunMeasScriptLine += runFolder[i];
      fileRunMeasScriptLine += "/";
      commandName  = "ls ";
      commandName +=rootL1FilePath;
      commandName += runFolder[i];
      commandName += " | grep Meas | grep -v Rates | grep .root > a.tmp";
      system(commandName.Data());
      indata.open("a.tmp");
      indata>>mot;
      indata.close();
      system("rm a.tmp");
      if(mot != ""){
	//fileRunMeasScriptLine += firstMeasFileNameArr[i];
	//fileRunMeasScriptLine += ".root ";
	fileRunMeasScriptLine += mot;
	fileRunMeasScriptLine += " ";
	fileRunMeasScriptLine += rootL2FilePath;
	fileRunMeasScriptLine += runFolder[i];
	fileRunMeasScriptLine += "/";
	//fileRunMeasScriptLine += firstMeasFileNameArr[i];
	//fileRunMeasScriptLine += ".root";
	fileRunMeasScriptLine += mot;
	fileRunMeasScriptLine += "_hist.root";
	//cout<<fileRunMeasScriptLine<<endl;
      fileRunMeasScript<<fileRunMeasScriptLine<<endl;
      fileRunMeasScript<<endl;
      }
    }
    fileRunMeasScript.close();
  }
  
  /////////////////////////////////////////////
  // Make .tar.gz archivation of the data files 
  //----------------------------------
  //tar -pczf Run_0_Data_11_18_2014_Ascii.tar.gz Run_0_Data_11_18_2014_Ascii
  if(Make_tar_gzArchivationOfTheDataFiles == "yes"){
    cout<<" Make .tar.gz archivation of the data files "<<endl;
    fileTargzScript.open(targzscript.Data());
    for(i = 0;i<nn;i++){
      fileTargzScriptLine  = "echo \"";
      fileTargzScriptLine += runFolder[i];
      fileTargzScriptLine += "\"";
      fileTargzScript<<fileTargzScriptLine<<endl;
      fileTargzScriptLine  = "tar -pczf ";
      fileTargzScriptLine += runFolder[i];
      fileTargzScriptLine += ".tar.gz ";
      fileTargzScriptLine += runFolder[i];
      //cout<<fileTargzScriptLine<<endl;
      fileTargzScript<<fileTargzScriptLine<<endl;
      fileTargzScript<<endl;
    }
    fileTargzScript.close();
  }
  
  return 0;
}

TString GetFileNameEnd(Int_t i){
  TString endN;
  if(i == 0){
    endN = "";
  }
  else if(i>0 && i<10){
    endN  = "_";
    endN += "000";
    endN += i;
  }
  else if(i>9 && i<100){
    endN  = "_";
    endN += "00";
    endN += i;
  }
  else if(i>99 && i<1000){
    endN  = "_";
    endN += "0";
    endN += i;
  }
  else if(i>999 && i<10000){
    endN  = "_";
    endN += i;
  }
  else{
    cerr<<" ---> ERROR : too many files per run, maximum is 1000."<<endl
	<<"          i = "<<i<<endl;
    //assert(0);
    //exit (EXIT_FAILURE);
    exit(-1);
  }
  return endN;
}

bool fexists(const char *filename){
  ifstream ifile;
  ifile.open(filename);
  if(!ifile)
    return false;
  return true;
}
