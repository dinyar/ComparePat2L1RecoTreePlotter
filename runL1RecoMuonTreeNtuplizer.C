{
  gROOT->ProcessLine(".x ../L1TriggerDPG/L1Ntuples/macros/initL1Analysis.C");
  gROOT->ProcessLine(".L L1RecoMuonTreeNtuplizer.C+");
  gROOT->ProcessLine(
      "L1RecoMuonTreeNtuplizer macro = L1RecoMuonTreeNtuplizer(\"murecotree_list\");");
   //   "L1RecoMuonTreeNtuplizer(\"ntuple_list\");");
  gROOT->ProcessLine("macro.run(-1);");
}
