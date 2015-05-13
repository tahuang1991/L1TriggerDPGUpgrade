from ROOT import *
from PlotHelper import *

ROOT.gROOT.SetBatch(1)
ROOT.gStyle.SetStatW(0.07)
ROOT.gStyle.SetStatH(0.06)

ROOT.gStyle.SetOptStat(0)
ROOT.gStyle.SetTitleStyle(0)
ROOT.gStyle.SetTitleAlign(13) ## coord in top left
ROOT.gStyle.SetTitleX(0.)
ROOT.gStyle.SetTitleY(1.)
ROOT.gStyle.SetTitleW(1)
ROOT.gStyle.SetTitleH(0.058)
ROOT.gStyle.SetTitleBorderSize(0)

ROOT.gStyle.SetPadLeftMargin(0.126)
ROOT.gStyle.SetPadRightMargin(0.04)
ROOT.gStyle.SetPadTopMargin(0.06)
ROOT.gStyle.SetPadBottomMargin(0.13)

ROOT.gStyle.SetMarkerStyle(1)

#_______________________________________________________________________________
def getH(dir, histo_name):
  """Get the histogram from a directory"""
  histo = dir.Get("%s;1"%(histo_name))
  if not histo:
    print "No such histogram: ", histo_name
  return histo

#_______________________________________________________________________________
def getRootDirectory(input_dir, file_name, dir_name = "L1TTriggerRate"):
    """Get the ROOT directory from the GEMCSCTriggerEfficiency analyzer. Normally,
    the directory should be called GEMCSCTriggerEfficiency. You should check it anyway"""

    ### get the ROOT file
    file = TFile.Open(input_dir + file_name)
    if not file:
        sys.exit('Input ROOT file %s is missing.' %(file_name))

    ## get the ROOT directory
    dir = file.Get(dir_name)
    if not dir:
        sys.exit('Directory %s does not exist.' %(dir_name))

    return dir


#_______________________________________________________________
def ThreeStubs(stub, pt, station, eta ):

        
        Stringstations = ["","hasME%d"%station, "hasGE%d1"%station, "passGE%d1"%station]
	
        leg = [" ","+ 1 in Station %d "%station, "+ has GEM in GE%d1"%station, "+passGE%d1 dphicut"%station]
	c1 = TCanvas("c","c",800,600)
	c1.SetGridx()
	c1.SetGridy()
	c1.SetTickx()
	c1.SetTicky()
        
	dir = getRootDirectory(input_dir, file_name)
        hs = THStack("hs","Muon Trigger Rate") 

	legend = TLegend(0.60,0.66,0.92,0.92,"","brNDC")
	legend.SetFillColor(ROOT.kWhite)
        legend.SetFillStyle(0)
	legend.SetHeader("PU140, %s"%stringetas[eta])
        for st in range(len(Stringstations)):
	        print "L1cscTrack_%s%s%s%s_pt"%(stub,pt,Stringstations[st],eta)
		htemp = getH(dir,"L1cscTrack_%s%s%s%s_pt"%(stub,pt,Stringstations[st],eta))
		htemp.SetLineColor(Color[st])
		htemp.SetLineWidth(len(Stringstations)-st)
		legend.AddEntry(htemp,"%s%s"%(stub, leg[st]), "pl")
		hs.Add(htemp)

        hs.Draw("nostack")
 	hs.GetHistogram().GetXaxis().SetTitle("L1 Muon P_{T}")
        hs.GetHistogram().GetYaxis().SetTitle("Trigger Rate [kHz]")
	ROOT.gPad.SetLogx()
	ROOT.gPad.SetLogy()
	c1.Update()
	legend.Draw("same")

	c1.SaveAs("TriggerRate_%s_St%d_%s_%s_PU140.pdf"%(stub,station, eta, pt))
	c1.SaveAs("TriggerRate_%s_St%d_%s_%s_PU140.png"%(stub,station, eta, pt))


#_______________________________________________________________________________
if __name__ == "__main__":

        Color = [ROOT.kGreen-3,ROOT.kBlue+1, ROOT.kRed+1 ,ROOT.kMagenta+2]
	stringetas = {'eta1':'1.6<|#eta|<2.4','eta2':'2.1<|#eta|<2.4','eta3':'1.64<|eta|<2.14'}
	stringPts = {'':'No Pt cut', 'Pt15':'L1 Muon P_{T}>15','Pt20':'L1 Muon P_{T}>20'}
	input_dir = "/uscms_data/d3/tahuang/CMSSW_6_2_0_SLHC25_patch1/src/L1TriggerDPGUpgrade/L1TMuon/test/"
	file_name = "Rate_Neutrino_SLHC25_PU140_0511.root"
	ThreeStubs("stub3","",1,"eta1")
	ThreeStubs("stub3","",1,"eta2")
	ThreeStubs("stub3","",1,"eta3")
	ThreeStubs("stub3","",2,"eta1")
	ThreeStubs("stub3","",2,"eta2")
	ThreeStubs("stub3","",2,"eta3")

