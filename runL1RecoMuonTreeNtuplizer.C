{
  gROOT->ProcessLine(".x ../L1TriggerDPG/L1Ntuples/macros/initL1Analysis.C");
  gROOT->ProcessLine(".L GMTChargeAssignmentNtupleizer.C+");
  gROOT->ProcessLine(
      "GMTChargeAssignmentNtupleizer macro = "
      "GMTChargeAssignmentNtupleizer(\"murecotree_list\");");
  gROOT->ProcessLine("macro.run(-1);");
}
