
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
ROOT.gStyle.SetPadRightMargin(0.14)
ROOT.gStyle.SetPadTopMargin(0.06)
ROOT.gStyle.SetPadBottomMargin(0.13)

ROOT.gStyle.SetMarkerStyle(1)


    
global input_dir 
global file_name1 
global file_name2
global file_name3

#_______________________________________________________________
def histDraw1( hist_name, h_bins, Xtitle, Ytitle):

	c1 = TCanvas("c","c",800,600)
	c1.SetGridx()
	c1.SetGridy()
	c1.SetTickx()
	c1.SetTicky()

        nBins = int(h_bins[1:-1].split(',')[0])
        minBin = float(h_bins[1:-1].split(',')[1])
        maxBin = float(h_bins[1:-1].split(',')[2])


	b1 = TH1F("b1","b1", nBins, minBin, maxBin)
	b1.GetYaxis().SetRangeUser(0,1000)
	b1.GetYaxis().SetTitleOffset(1.2)
	b1.GetYaxis().SetNdivisions(520)
	b1.GetYaxis().SetTitle(Ytitle)
	b1.GetXaxis().SetTitle(Xtitle)
	b1.SetTitle(" "*8+"%s"%hist_name+" "*15 + "CMS Simulation Preliminary")
	b1.SetStats(0)

	dir1 = getRootDirectory(input_dir, file_name1)

#	hist_name = "L1cscTrack_eta_stub3pt20hasME2"

	h1 = getH(dir1, "%s"%hist_name)


	h1.SetLineColor(ROOT.kBlack)
	h1.SetLineWidth(2)

	b1.Draw()
	h1.Draw("same")


	legend = TLegend(0.23,0.16,0.52,0.44,"","brNDC")
	legend.SetFillColor(ROOT.kWhite)
        legend.SetFillStyle(0)
	legend.SetHeader("PU140")
        legend.AddEntry(h1,"current","l")
#legend.Draw("same")


	c1.SaveAs("%s_PU140.pdf"%hist_name)
	c1.SaveAs("%s_PU140.png"%hist_name)

#_______________________________________________________________
def histDraw2( hist_name, h_bins, Xtitle, Ytitle):

	c1 = TCanvas("c","c",800,600)
	c1.SetGridx()
	c1.SetGridy()
	c1.SetTickx()
	c1.SetTicky()

        nBins = int(h_bins[1:-1].split(',')[0])
        minBin = float(h_bins[1:-1].split(',')[1])
        maxBin = float(h_bins[1:-1].split(',')[2])


	b1 = TH1F("b1","b1", nBins, minBin, maxBin)
	b1.GetYaxis().SetRangeUser(0.6,1.02)
	b1.GetYaxis().SetTitleOffset(1.2)
	b1.GetYaxis().SetNdivisions(520)
	b1.GetYaxis().SetTitle(Ytitle)
	b1.GetXaxis().SetTitle(Xtitle)
	b1.SetTitle(" "*15+"%s"%hist_name+" "*10 + "CMS Simulation Preliminary")
	b1.SetStats(0)

	dir1 = getRootDirectory(input_dir, file_name1)
	dir2 = getRootDirectory(input_dir, file_name2)

#	hist_name = "L1cscTrack_eta_stub3pt20hasME2"

	h1 = getH(dir1, "%s"%hist_name)
	h2 = getH(dir2, "%s"%hist_name)


	h1.SetLineColor(ROOT.kBlack)
	h1.SetLineWidth(2)
	h2.SetLineColor(ROOT.kRed)
	h2.SetLineWidth(2)

	b1.Draw()
	h1.Draw("same")
	h2.Draw("same")


	legend = TLegend(0.23,0.16,0.52,0.34,"","brNDC")
	legend.SetFillColor(ROOT.kWhite)
        #legend.SetFillStyle(0)
	legend.SetHeader("PU140")
        legend.AddEntry(h1,"dphicut on simTrack Pt","l")
	legend.AddEntry(h2,"dphicut on CSCTFtrack Pt","l")
	legend.Draw("same")


	c1.SaveAs("%s_PU140_dphicut.pdf"%hist_name)
	c1.SaveAs("%s_PU140_dphicut.png"%hist_name)



#_______________________________________________________________
def histDraw( hist_name, h_bins, Xtitle, Ytitle):

	c1 = TCanvas("c","c",800,600)
	c1.SetGridx()
	c1.SetGridy()
	c1.SetTickx()
	c1.SetTicky()

        nBins = int(h_bins[1:-1].split(',')[0])
        minBin = float(h_bins[1:-1].split(',')[1])
        maxBin = float(h_bins[1:-1].split(',')[2])


	b1 = TH1F("b1","b1", nBins, minBin, maxBin)
	b1.GetYaxis().SetRangeUser(0,1.02)
	b1.GetYaxis().SetTitleOffset(1.2)
	b1.GetYaxis().SetNdivisions(520)
	b1.GetYaxis().SetTitle(Ytitle)
	b1.GetXaxis().SetTitle(Xtitle)
	b1.SetTitle(" "*64 + "CMS Simulation Preliminary")
	b1.SetStats(0)

	dir1 = getRootDirectory(input_dir, file_name1)
	dir2 = getRootDirectory(input_dir, file_name2)
	dir3 = getRootDirectory(input_dir, file_name3)

#	hist_name = "L1cscTrack_eta_stub3pt20hasME2"

	h1 = getH(dir1, "%s"%hist_name)
	h2 = getH(dir2, "%s"%hist_name)
	h3 = getH(dir3, "%s"%hist_name)


	h1.SetLineColor(ROOT.kBlack)
	h1.SetLineWidth(2)
	h2.SetLineColor(ROOT.kRed)
	h2.SetLineWidth(2)
	h3.SetLineColor(ROOT.kBlue)
	h3.SetLineWidth(2)

	b1.Draw()
	h1.Draw("same")
	h2.Draw("same")
	h3.Draw("same")


	legend = TLegend(0.23,0.16,0.52,0.44,"","brNDC")
	legend.SetFillColor(ROOT.kWhite)
#legend.SetFillStyle(0)
	legend.SetHeader("PU140, Phase II detector: CSC-RPC ILT algo")
        legend.AddEntry(h1,"scenario 1","l")
	legend.AddEntry(h2,"scenario 2","l")
	legend.AddEntry(h3,"scenario 3","l")
	legend.Draw("same")


	c1.SaveAs("%s_PU140.pdf"%hist_name)
	c1.SaveAs("%s_PU140.png"%hist_name)

#_______________________________________________________________
def hist2DDraw( hist_name, x_bins,y_bins, Xtitle, Ytitle):

	c1 = TCanvas("c","c",800,600)
	c1.SetGridx()
	c1.SetGridy()
	c1.SetTickx()
	c1.SetTicky()

        xBins = int(x_bins[1:-1].split(',')[0])
        xmin = float(x_bins[1:-1].split(',')[1])
        xmax = float(x_bins[1:-1].split(',')[2])
        yBins = int(y_bins[1:-1].split(',')[0])
        ymin = float(y_bins[1:-1].split(',')[1])
        ymax = float(y_bins[1:-1].split(',')[2])


	b1 = TH2F("b1","b1", xBins, xmin, xmax,yBins, ymin, ymax)
	b1.GetYaxis().SetTitle(Ytitle)
	b1.GetXaxis().SetTitle(Xtitle)
	b1.SetTitle(" "*15+"%s"%hist_name+" "*10 + "CMS Simulation Preliminary")
	b1.SetStats(0)

	dir1 = getRootDirectory(input_dir, file_name3)
#	dir2 = getRootDirectory(input_dir, file_name2)
#	dir3 = getRootDirectory(input_dir, file_name3)

#	hist_name = "L1cscTrack_eta_stub3pt20hasME2"

	h1 = getH(dir1, "%s"%hist_name)
#	h2 = getH(dir2, "%s"%hist_name)
#	h3 = getH(dir3, "%s"%hist_name)


        h1.SetMaximum(200)
	b1.Draw()
	h1.Draw("Colzsame")
        c1.SetLogz()
        tex = TLatex(.25,.3,"PU140, fabs(eta)>2.0")
        tex.SetTextSize(0.05)
        tex.SetNDC()
        tex.Draw("same")

        
#legend = TLegend(0.23,0.16,0.52,0.44,"","brNDC")
#	legend.SetFillColor(ROOT.kWhite)
#       legend.SetFillStyle(0)
#legend.Draw("same")


	c1.SaveAs("%s_test2_PU140.pdf"%hist_name)
	c1.SaveAs("%s_test2_PU140.png"%hist_name)



#_______________________________________________________________________________
if __name__ == "__main__":
	
	input_dir = "files/"
	file_name1 = "histo_PU140_SLHC13_100k_MatchingWindows_dphicutonsimPT.root"
	file_name2 = "histo_PU140_SLHC13_100k_MatchingWindows.root"
	file_name3 = "histo_PU140_SLHC13_100k_MatchingWindows_dphicutonCSCTFPt_test2.root"

	stationbins = "(10,-5,5)"
        Xtitle1 = "station in Tracker"
	Ytitle1 = "count"
	#histDraw( "nStationTF",stationbins, Xtitle1, Ytitle1)


	Ptbins = "(50,0,50)"
	DPhibins = "(100,-0.25,0.25)"
	Xtitle2 = "Simulated #mu P_{T} [Gev/c]"
	Ytitle2 = "GEMDPhi"
#	histDraw1( "den_2stubsme1bdphi20", Ptbins, Xtitle2, Ytitle2)
#	histDraw1( "den_3stubsme1bdphi20", Ptbins, Xtitle2, Ytitle2)
#        histDraw2( "L1cscTrack_stub3hasGE11eta1_pt", Ptbins, Xtitle2, Ytitle2)
#        histDraw2( "L1cscTrack_stub3hasGE21eta1_pt", Ptbins, Xtitle2, Ytitle2)
#        histDraw2( "L1cscTrack_stub3hasGE21eta2_pt", Ptbins, Xtitle2, Ytitle2)
        hist2DDraw( "GE11GEMdPhivsPT2stubsodd", Ptbins, DPhibins, Xtitle2, Ytitle2)
        hist2DDraw( "GE11GEMdPhivsPT2stubseven", Ptbins,DPhibins, Xtitle2, Ytitle2)
        hist2DDraw( "GE11GEMdPhivsPT3stubsodd", Ptbins,DPhibins, Xtitle2, Ytitle2)
        hist2DDraw( "GE11GEMdPhivsPT3stubseven", Ptbins,DPhibins, Xtitle2, Ytitle2)
        hist2DDraw( "GE21GEMdPhivsPT2stubseven", Ptbins,DPhibins, Xtitle2, Ytitle2)
        hist2DDraw( "GE21GEMdPhivsPT2stubsodd", Ptbins,DPhibins, Xtitle2, Ytitle2)
        hist2DDraw( "GE21GEMdPhivsPT3stubseven", Ptbins,DPhibins, Xtitle2, Ytitle2)
        hist2DDraw( "GE21GEMdPhivsPT3stubsodd", Ptbins,DPhibins, Xtitle2, Ytitle2)
	Xtitle3 = "Simulated #mu P_{T} [Gev/c]"
	Ytitle3 = "CSCTF track P_{T} [Gev/c]"
#        hist2DDraw("TFtrack2stubsPtVsSimtrackPt", Ptbins,Ptbins, Xtitle3, Ytitle3)
#        hist2DDraw("TFtrack3stubsPtVsSimtrackPt", Ptbins,Ptbins, Xtitle3, Ytitle3)

