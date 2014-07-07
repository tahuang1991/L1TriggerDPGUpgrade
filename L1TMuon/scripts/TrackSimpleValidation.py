import ROOT
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

    
c1 = ROOT.TCanvas()
c1.SetGridx()
c1.SetGridy()
c1.SetTickx()
c1.SetTicky()

def getPurity(file,dir,den,num,xaxis,h_bins):
    c1 = ROOT.TCanvas()
    c1.SetGridx()
    c1.SetGridy()
    c1.SetTickx()
    c1.SetTicky()

    f = ROOT.TFile(file)
    t = f.Get(dir)
    nBins = int(h_bins[1:-1].split(',')[0])
    minBin = float(h_bins[1:-1].split(',')[1])
    maxBin = float(h_bins[1:-1].split(',')[2])
    
    b1 = ROOT.TH1F("b1","b1",nBins,minBin,maxBin)
    b1.GetYaxis().SetRangeUser(0.0,1)
    b1.GetYaxis().SetTitleOffset(1.2)
    b1.GetYaxis().SetNdivisions(520)
    b1.GetYaxis().SetTitle("CSCTF Track purity")
    b1.GetXaxis().SetTitle("%s of simulated muon track"%xaxis)
    b1.SetTitle("CSCTF track reco"+" "*16 + "CMS Simulation Preliminary")
    b1.SetStats(0)

    h1 = ROOT.TH1F("h1","h1",nBins,minBin,maxBin)
    t.Draw("abs(%s) >> h1"%xaxis,den)
    h2 = ROOT.TH1F("h2","h2",nBins,minBin,maxBin)
    t.Draw("abs(%s) >> h2"%xaxis,num)
    e = ROOT.TEfficiency(h2,h1)
    
    b1.Draw()
    e.Draw("same")
    legend = ROOT.TLegend(0.23,0.56,0.52,0.64)
    legend.SetFillColor(ROOT.kWhite)
    legend.SetHeader("PU140")
#legend.AddEntry(e1,"","l")
    legend.Draw("same")
	
    c1.SaveAs("CSCTF_reco_purity_3stubs_%s_GE21dphicut_PU140.pdf"%xaxis)
    c1.SaveAs("CSCTF_reco_purity_3stubs_%s_GE21dphicut_PU140.png"%xaxis)

#_______________________________________________________________________________
if __name__ == "__main__":

    file = "files/histo_PU140_SLHC13_100k_TEST.root"
    dir = "L1TAnalyser/CSCTFTrackTree"
    den = "nstubs>2 && match>=0"
    num = "nstubs>2 && match>0 && passGE21"

    xaxis1 = "truept"
    Pt_bins = "(50,0,50)"
    getPurity(file,dir,den,num,xaxis1,Pt_bins)

    xaxis2 = "trueeta"
    Eta_bins = "(40,1.5,2.5)"
    getPurity(file,dir,den,num,xaxis2,Eta_bins)
   
