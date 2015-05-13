from ROOT import *
from PlotHelper import *
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

global input_dir 
global file_name1 
global file_name2
global file_name3

#_______________________________________________________________
def ThreeStubsWith():

	c1 = TCanvas("c","c",800,600)
	c1.SetGridx()
	c1.SetGridy()
	c1.SetTickx()
	c1.SetTicky()

	b1 = TH1F("b1","b1",50,0,50)
	b1.GetYaxis().SetRangeUser(0.60,1)
	b1.GetYaxis().SetTitleOffset(1.2)
	b1.GetYaxis().SetNdivisions(520)
	b1.GetYaxis().SetTitle("TrackFinder Reco efficiency")
	b1.GetXaxis().SetTitle("#eta of simulated muon track")
	b1.SetTitle(" "*64 + "CMS Simulation Preliminary")
	b1.SetStats(0)

	dir1 = getRootDirectory(input_dir, file_name1)
	dir2 = getRootDirectory(input_dir, file_name2)

	hist_name = "L1CSCTrack_3Stubs_pt_me2"

	h1 = getH(dir1, "%s"%hist_name)
	h2 = getH(dir2, "%s"%hist_name)


	h1.SetLineColor(ROOT.kRed)
	h1.SetLineWidth(2)
	h2.SetLineColor(ROOT.kBlue)
	h2.SetLineWidth(2)

	b1.Draw()
	h1.Draw("same")
	h2.Draw("same")


	legend = TLegend(0.23,0.16,0.52,0.44,"","brNDC")
	legend.SetFillColor(ROOT.kWhite)
        legend.SetFillStyle(0)
	legend.SetHeader("PU0")
	legend.AddEntry(h1,"current+GE11","l")
	legend.AddEntry(h2,"current+GE11+GE21","l")
	legend.Draw("same")

	c1.SaveAs("%s_PU0.pdf"%hist_name)
	c1.SaveAs("%s_PU0.png"%hist_name)

#_______________________________________________________________
def ThreeStubsWithME2():

	c1 = TCanvas("c","c",800,600)
	c1.SetGridx()
	c1.SetGridy()
	c1.SetTickx()
	c1.SetTicky()

	b1 = TH1F("b1","b1",50,0,50)
	b1.GetYaxis().SetRangeUser(0.60,1)
	b1.GetYaxis().SetTitleOffset(1.2)
	b1.GetYaxis().SetNdivisions(520)
	b1.GetYaxis().SetTitle("TrackFinder Reco efficiency")
	b1.GetXaxis().SetTitle("#eta of simulated muon track")
	b1.SetTitle(" "*64 + "CMS Simulation Preliminary")
	b1.SetStats(0)

	dir1 = getRootDirectory(input_dir, file_name1)
	dir2 = getRootDirectory(input_dir, file_name2)

	hist_name = "L1CSCTrack_3StubsME2_pt_me2"

	h1 = getH(dir1, "%s"%hist_name)
	h2 = getH(dir2, "%s"%hist_name)


	h1.SetLineColor(ROOT.kRed)
	h1.SetLineWidth(2)
	h2.SetLineColor(ROOT.kBlue)
	h2.SetLineWidth(2)

	b1.Draw()
	h1.Draw("same")
	h2.Draw("same")


	legend = TLegend(0.23,0.16,0.52,0.44,"","brNDC")
	legend.SetFillColor(ROOT.kWhite)
        legend.SetFillStyle(0)
	legend.SetHeader("PU0")
	legend.AddEntry(h1,"current+GE11","l")
	legend.AddEntry(h2,"current+GE11+GE21","l")
	legend.Draw("same")

	c1.SaveAs("%s_PU0.pdf"%hist_name)
	c1.SaveAs("%s_PU0.png"%hist_name)

#_______________________________________________________________
def ThreeStubsWithME2PtCut():

	c1 = TCanvas("c","c",800,600)
	c1.SetGridx()
	c1.SetGridy()
	c1.SetTickx()
	c1.SetTicky()

	b1 = TH1F("b1","b1",50,0,50)
	b1.GetYaxis().SetRangeUser(0.60,1)
	b1.GetYaxis().SetTitleOffset(1.2)
	b1.GetYaxis().SetNdivisions(520)
	b1.GetYaxis().SetTitle("TrackFinder Reco efficiency")
	b1.GetXaxis().SetTitle("#eta of simulated muon track")
	b1.SetTitle(" "*64 + "CMS Simulation Preliminary")
	b1.SetStats(0)

	dir1 = getRootDirectory(input_dir, file_name1)
	dir2 = getRootDirectory(input_dir, file_name2)

	hist_name = "L1CSCTrack_3Stubs20ME2_pt_me2"

	h1 = getH(dir1, "%s"%hist_name)
	h2 = getH(dir2, "%s"%hist_name)


	h1.SetLineColor(ROOT.kRed)
	h1.SetLineWidth(2)
	h2.SetLineColor(ROOT.kBlue)
	h2.SetLineWidth(2)

	b1.Draw()
	h1.Draw("same")
	h2.Draw("same")


	legend = TLegend(0.23,0.16,0.52,0.44,"","brNDC")
	legend.SetFillColor(ROOT.kWhite)
        legend.SetFillStyle(0)
	legend.SetHeader("PU0")
	legend.AddEntry(h1,"current+GE11","l")
	legend.AddEntry(h2,"current+GE11+GE21","l")
	legend.Draw("same")


	c1.SaveAs("%s_PU0.pdf"%hist_name)
	c1.SaveAs("%s_PU0.png"%hist_name)


#_______________________________________________________________
def EffVsPtME2( hist_name):

	c1 = TCanvas("c","c",800,600)
	c1.SetGridx()
	c1.SetGridy()
	c1.SetTickx()
	c1.SetTicky()

	b1 = TH1F("b1","b1",50,0,50)
	b1.GetYaxis().SetRangeUser(0.60,1)
	b1.GetYaxis().SetTitleOffset(1.2)
	b1.GetYaxis().SetNdivisions(520)
	b1.GetYaxis().SetTitle("TrackFinder Reco efficiency")
	b1.GetXaxis().SetTitle("#Pt of simulated muon track")
	b1.SetTitle(" "*8+"%s"%hist_name+" "*10 + "CMS Simulation Preliminary")
	b1.SetStats(0)

	dir1 = getRootDirectory(input_dir, file_name1)
	dir2 = getRootDirectory(input_dir, file_name2)
	dir3 = getRootDirectory(input_dir, file_name3)


	h1 = getH(dir1, "%s"%hist_name)
	h2 = getH(dir2, "%s"%hist_name)
	h3 = getH(dir3, "%s"%hist_name)


	h1.SetLineColor(ROOT.kBlack)
	h1.SetLineWidth(2)
	h2.SetLineColor(ROOT.kBlue)
	h2.SetLineWidth(2)
	h3.SetLineColor(ROOT.kRed)
	h3.SetLineWidth(2)

	b1.Draw()
	h1.Draw("same")
	h2.Draw("same")
	h3.Draw("same")


	legend = TLegend(0.15,0.16,0.80,0.45)
	legend.SetFillColor(ROOT.kWhite)
#legend.SetFillStyle(0)
	legend.SetHeader("PU140")
        legend.AddEntry(h1,"1:old+add buildLCTfromlowQstubandrpc","l")
	legend.AddEntry(h2,"2:1+add fixCopad","l")
	legend.AddEntry(h3,"3:2+add suitable matchingwindow","l")
	legend.Draw("same")
	legend.Draw("same")


	c1.SaveAs("%s_PU0.pdf"%hist_name)
	c1.SaveAs("%s_PU0.png"%hist_name)



#_______________________________________________________________________________
if __name__ == "__main__":
	
	input_dir = "files/"
	file_name1 = "histo_PU140_SLHC13_100k_lowQstubandrpc.root"
	file_name2 = "histo_PU140_SLHC13_100k_FixCopad.root"
	file_name3 = "histo_PU140_SLHC13_100k_MatchingWindows.root"
	"""
	ThreeStubsWith()
	ThreeStubsWithME2()
	ThreeStubsWithME2PtCut()
        """
#EffVsPtME2("L1cscTrack_stub3_pt")
	EffVsPtME2("L1cscTrack_stub3hasME2_pt")
	EffVsPtME2("L1cscTrack_stub3pt20hasME2_pt")
#	EffVsPtME2("L1cscTrack_stub3pt20eta1_pt")
	EffVsPtME2("L1cscTrack_stub3hasME2eta1_pt")
	EffVsPtME2("L1cscTrack_stub3pt20hasME2eta1_pt")
#	EffVsPtME2("L1cscTrack_stub3pt20eta2_pt")
	EffVsPtME2("L1cscTrack_stub3hasME2eta2_pt")
	EffVsPtME2("L1cscTrack_stub3pt20hasME2eta2_pt")