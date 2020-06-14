{
  gROOT->LoadMacro("vis.C");
  gROOT->LoadMacro("./wfSimulation/src/wfSim.cpp");
  gROOT->LoadMacro("./wfSimulation/src/waveform.cpp");

  vis t("../root_data/Run_2_Data_12_9_2014_Ascii/Run_2_Data_12_9_2014_Ascii.dat.root");

  t.DrawAllChannels(9);
  //t.DrawOneChannel(10, 0);
  //t.DrawOneChannel_waveform(9, 1);
  //t.DrawOneChannel_Mamy(101, 0);
  //t.Loop();
}
