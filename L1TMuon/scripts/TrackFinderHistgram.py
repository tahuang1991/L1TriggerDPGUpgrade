
from ROOT import *
from PlotHelper import *
global input_dir 
global file_name1 
global file_name2
global file_name3


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
	b1.GetYaxis().SetRangeUser(0,35000)
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
        legend.SetFillStyle(0)
	legend.SetHeader("PU0")
        legend.AddEntry(h1,"current","l")
	legend.AddEntry(h2,"current+GE11","l")
	legend.AddEntry(h3,"current+GE11+GE21","l")
	legend.Draw("same")


	c1.SaveAs("%s_new_PU0.pdf"%hist_name)
	c1.SaveAs("%s_new_PU0.png"%hist_name)


#_______________________________________________________________________________
if __name__ == "__main__":
	
	input_dir = "files/"
	file_name1 = "TFAna_histo_CSConly_NonUpgradeTMB.root"
	file_name2 = "TFAna_histo_UpgradeME11only.root"
	file_name3 = "TFAna_histo_UpgradeME11ME21.root"

	stationbins = "(10,-5,5)"
        Xtitle1 = "station in Tracker"
	Ytitle1 = "count"
	histDraw( "nStationTF",stationbins, Xtitle1, Ytitle1)
