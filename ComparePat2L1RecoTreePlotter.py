#! /usr/bin/env python

import ROOT
import math
import tdrstyle
from itertools import izip
from collections import namedtuple
from DataFormats.FWLite import Events, Handle


def makeHistogram(title, binning, quantity):
    histo = ROOT.TH1F(title, "", binning[0],
                      binning[1], binning[2])
    histo.SetMinimum(0)
    if quantity == "eta":
        histo.GetXaxis().SetTitle("#mbox{#eta}")
        histo.GetYaxis().SetTitleOffset(1.3)
    elif quantity == "phi":
        histo.GetXaxis().SetTitle("#mbox{#phi}")
    elif quantity == "pt":
        histo.GetXaxis().SetTitle("#mbox{p}_{#mbox{T}} #mbox{[GeV/c]}")
        histo.GetYaxis().SetTitleOffset(1.3)
    elif quantity == "ch":
        histo.GetXaxis().SetTitle("#mbox{Charge}")
    else:
        histo.GetXaxis().SetTitle("")
    histo.GetYaxis().SetTitle("Counts")

    return histo


# Create histograms, etc.
def plotMuQuantities_pat(events, label, handle, plottingVariables):
    events_noOS.toBegin()
    events_OSrequired.toBegin()

    # Book histograms
    particleHistorgrams = []
    for plottingVar in plottingVariables:
        histo = makeHistogram(plottingVar.title, plottingVar.binning,
                              plottingVar.quantity)
        particleHistorgrams.append(histo)

    # loop over events
    for event in events:
        # use getByLabel, just like in cmsRun
        event.getByLabel(label, handle)
        # get the product
        muons = handle.product()
        # use muons to make J/Psi peak
        numMuons = len(muons)
        print "Number of muons: " + numMuons

        for mu in xrange(numMuons - 1):
            muon = muons[mu]
            for plottingVar, hist in izip(plottingVariables,
                                          particleHistorgrams):
                if plottingVar.quantity == "eta":
                    hist.Fill(muon.eta())
                elif plottingVar.quantity == "phi":
                    hist.Fill(muon.phi())
                elif plottingVar.quantity == "pt":
                    hist.Fill(muon.pt())
                elif plottingVar.quantity == "ch":
                    hist.Fill(muon.ch())
    return particleHistorgrams

def plotMuQuantities_l1(ntuple, plottingVariables):
    # Book histograms
    particleHistorgrams = []
    for plottingVar in plottingVariables:
        histo = makeHistogram(plottingVar.title, plottingVar.binning,
                              plottingVar.quantity)
        ntuple.Project(plottingVar.title, plottingVar.quantity + "_reco", "")
        particleHistorgrams.append(histo)
    return particleHistorgrams

def makeComparisonPlots(label, handle, plottingVariables):
    ROOT.gROOT.SetBatch()        # don't pop up canvases
    ROOT.gStyle.SetErrorX(0)

    tdrstyle.setTDRStyle()

    muonHists_patTree = plotMuQuantities_pat(events_patTree, label, handle,
                                             plottingVariables)
    muonHists_l1muRecoTree = plotMuQuantities_l1(ntuple_l1recoMuonTree,
                                                 plottingVariables)

    for muonHist_patTree, muonHist_l1muRecoTree, plotVar in izip(muonHists_patTree,
                                                                 muonHists_l1muRecoTree,
                                                                 plottingVariables):
        # Draw histograms
        c1 = ROOT.TCanvas("", "", 1024, 786)
        muonHist_patTree.SetLineColor(ROOT.kRed)
        muonHist_patTree.DrawCopy("E1HIST")
        muonHist_l1muRecoTree.SetLineColor(ROOT.kBlue)
        muonHist_l1muRecoTree.DrawCopy("E1HISTSAME")
        c1.Print(plotVar.title + ".pdf")

f = ROOT.TFile.Open(".root")
ntuple_l1recoMuonTree = f.Get("ntuple")
# How to combine several pat tuple files here?
f_patTree = ROOT.TFile.Open(".root")
events_patTree = Events(f_patTree)

# create handle outside of loop
handle = Handle("std::vector<pat::Muon>")

# for now, label is just a tuple of strings that is initialized just
# like an edm::InputTag
label = ("selectedPatMuons")

# Structure to hold plotting data
PlottingVars = namedtuple("PlottingVars", ["quantity", "binning",
                                           "title"])

# Binning lists
binning_eta = [16, -1.6, 1.6]
binning_pT = [35, 0, 35]

defaultLegendPositions = [upperRight, upperRight]

# Construct plotting variables
plotPt = PlottingVars(quantity="pt", binning=binning_pT, title="muVsPt")
plotEta = PlottingVars(quantity="eta", binning=binning_eta, title="muVsEta")

# Construct plotting lists
plotVars = [plotPt, plotEta]

makeComparisonPlots(label, handle, plotVars)
