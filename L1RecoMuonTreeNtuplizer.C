/*
    This macro will can run on a L1ntuple that contains a L1RecoMuonTree and
    extract the quantities in the Tree. These quantities are then added to a
    flat ntuple for further analysis.
*/

#include "L1Ntuple.h"
#include "hist.C"
#include "Style.C"
#include <algorithm>
#include <TMath.h>
#include <sstream>
#include "TNtuple.h"
#include "TFile.h"

using namespace L1Analysis;
// --------------------------------------------------------------------
// description: Macro to check the charge assignment for J/Psi muons.
// --------------------------------------------------------------------
#define PHI 0
#define ETA 1
#define PT 2
#define PTETA 3

#define DT 0
#define RPCb 1
#define CSC 2
#define RPCf 3
#define GMT 4

class L1RecoMuonTreeNtuplizer : public L1Ntuple {
 public:
  // constructor
  L1RecoMuonTreeNtuplizer(std::string filename) : L1Ntuple(filename) {}
  L1RecoMuonTreeNtuplizer() : L1Ntuple() {}
  ~L1RecoMuonTreeNtuplizer() {}

  // main function macro : arguments can be adpated to your need
  void run(Long64_t nevents);

 private:
  TFile* fout;

  // your private methods can be declared here
  TNtuple* ntuple;
  void fillNtuple(int recoMu1, int N, std::vector<std::string> contentList,
                  Float_t ntupleValues[]);
  void toggleBranches();
};

// --------------------------------------------------------------------
// run function
// --------------------------------------------------------------------
void L1RecoMuonTreeNtuplizer::run(Long64_t nevents) {
  toggleBranches();

  // Create ntuple
  std::ostringstream ntupleContStream;
  std::vector<std::string> contentList;

  std::vector<std::string> physicsQuantities;
  physicsQuantities.push_back("N");
  physicsQuantities.push_back("eta");
  physicsQuantities.push_back("phi");
  physicsQuantities.push_back("pt");
  physicsQuantities.push_back("ch");
  std::vector<std::string> particleList;
  particleList.push_back("_reco");
  for (std::vector<std::string>::iterator name = physicsQuantities.begin();
       name != physicsQuantities.end(); ++name) {
    for (std::vector<std::string>::iterator particle = particleList.begin();
         particle != particleList.end(); ++particle) {
      std::string quantityName = *name + *particle;
      ntupleContStream << ":" << quantityName;
      contentList.push_back(quantityName);
    }
  }
  std::string fname("L1RecoMuTreeNtuple.root");
  TFile* out = new TFile(fname.c_str(), "RECREATE");
  out->cd();
  std::string ntupleContent(ntupleContStream.str());
  ntuple = new TNtuple("ntuple", "ntupledump", ntupleContent.c_str());

  // number of events to process
  if (nevents == -1 || nevents > GetEntries()) {
    nevents = GetEntries();
  }
  std::cout << nevents << " to process ..." << std::endl;

  // loop over the events
  for (Long64_t i = 0; i < nevents; i++) {
    // load the i-th event
    Long64_t ientry = LoadTree(i);
    if (ientry < 0) break;
    GetEntry(i);

    // process progress
    if (i != 0 && (i % 50000) == 0) {
      std::cout << "- processing event " << i << "\n" << std::endl;
    }

    std::cout << "Number of reco muons: " << recoMuon_->nMuons << std::endl;

    for (int mu1 = 0; mu1 < recoMuon_->nMuons; ++mu1) {
      Float_t ntupleValues[contentList.size()];

      int N;
      if (mu1 == 0) {
        N = recoMuon_->nMuons;
      } else {
        N = -1;
      }

      fillNtuple(mu1, N, contentList, ntupleValues);

      ntuple->Fill(ntupleValues);
    }
  }  // loop over events
  out->Write();
}

void L1RecoMuonTreeNtuplizer::fillNtuple(int recoMu1, int N,
                                         std::vector<std::string> contentList,
                                         Float_t ntupleValues[]) {
  for (size_t varIt = 0; varIt < contentList.size(); ++varIt) {
    if (contentList[varIt] == "N_reco") {
      ntupleValues[varIt] = N;
    }
    if (contentList[varIt] == "pt_reco") {
      ntupleValues[varIt] = recoMuon_->pt[recoMu1];
    }
    if (contentList[varIt] == "eta_reco") {
      ntupleValues[varIt] = recoMuon_->eta[recoMu1];
    }
    if (contentList[varIt] == "phi_reco") {
      ntupleValues[varIt] = recoMuon_->phi[recoMu1];
    }
    if (contentList[varIt] == "ch_reco") {
      ntupleValues[varIt] = recoMuon_->ch[recoMu1];
    }
  }
}

void L1RecoMuonTreeNtuplizer::toggleBranches() {
  // Select only needed branches:
  fChain->SetBranchStatus("*", 0);

  fChain->SetBranchStatus("nMuons", 1);
  fChain->SetBranchStatus("pt", 1);
  fChain->SetBranchStatus("eta", 1);
  fChain->SetBranchStatus("phi", 1);
  fChain->SetBranchStatus("ch", 1);
}
