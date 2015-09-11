{
  gROOT->ProcessLine(".x ../L1Trigger/L1TNtuples/macros/initL1Analysis.C");
  gROOT->ProcessLine(".L L1RecoMuonTreeNtuplizer.C+");
  gROOT->ProcessLine(
      "L1RecoMuonTreeNtuplizer macro = "
      "L1RecoMuonTreeNtuplizer(\"murecotree_list\");");
  gROOT->ProcessLine("macro.run(-1);");
}
