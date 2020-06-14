{
  gROOT->LoadMacro("vis.C");
  gROOT->LoadMacro("./wfSimulation/src/waveformSimple.cpp");

  vis t("../root_data/Run_0_Data_4_27_2015_Ascii/Run_0_Data_4_27_2015_Ascii.dat.root");

  t.DrawOneChannel_waveformSimple(23, 2,"yes","printInfoTrue");

  //t.Loop();
}
