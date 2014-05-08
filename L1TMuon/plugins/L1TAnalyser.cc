// system include files
#include <memory>
#include <cmath>
#include <iostream>
#include <sstream>
#include <boost/foreach.hpp>
#define foreach BOOST_FOREACH

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "L1Trigger/CSCTrackFinder/interface/CSCTrackFinderDataTypes.h"
#include "L1Trigger/CSCTrackFinder/interface/CSCSectorReceiverLUT.h"
#include "L1Trigger/CSCTrackFinder/test/src/RunSRLUTs.h"

#include "DataFormats/CSCDigi/interface/CSCCorrelatedLCTDigiCollection.h"
#include "DataFormats/L1CSCTrackFinder/interface/TrackStub.h"//
#include "DataFormats/L1CSCTrackFinder/interface/CSCTriggerContainer.h"
#include "DataFormats/L1CSCTrackFinder/interface/L1CSCTrackCollection.h"
#include "DataFormats/MuonDetId/interface/CSCTriggerNumbering.h"

#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/VertexReco/interface/Vertex.h"

#include "L1TriggerDPGUpgrade/DataFormats/interface/L1TMuonTriggerPrimitive.h"
#include "L1TriggerDPGUpgrade/DataFormats/interface/L1TMuonTriggerPrimitiveFwd.h"

#include "L1TriggerDPGUpgrade/DataFormats/interface/L1TMuonInternalTrack.h"
#include "L1TriggerDPGUpgrade/DataFormats/interface/L1TMuonInternalTrackFwd.h"
#include <SimDataFormats/Track/interface/SimTrackContainer.h>

#include "CondFormats/L1TObjects/interface/L1MuTriggerScales.h"
#include "CondFormats/DataRecord/interface/L1MuTriggerScalesRcd.h"
#include "CondFormats/DataRecord/interface/L1MuTriggerPtScaleRcd.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "TH1F.h"
#include "TH2F.h"
#include "TFile.h"
#include "TGraphAsymmErrors.h"
#include "TLorentzVector.h"

using namespace std;
using namespace edm;
using namespace L1TMuon;

class L1TAnalyser : public edm::EDAnalyzer {
public:
  explicit L1TAnalyser(const edm::ParameterSet&);
  ~L1TAnalyser();

private:
  virtual void beginJob() ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;

  // ----------member data ---------------------------
  edm::Service<TFileService> fs;
  const float ptscale[33] = { 
    -1.,   0.0,   1.5,   2.0,   2.5,   3.0,   3.5,   4.0,
    4.5,   5.0,   6.0,   7.0,   8.0,  10.0,  12.0,  14.0,  
    16.0,  18.0,  20.0,  25.0,  30.0,  35.0,  40.0,  45.0, 
    50.0,  60.0,  70.0,  80.0,  90.0, 100.0, 120.0, 140.0, 1.E6 };

  double min_pt;
  double max_pt;
  double min_aEta;
  double max_aEta;

  TH1F* h_GEMDPhi;
  TH1F* h_nStation;
  TH1F* h_nStationTF;
  TH1F* h_nStationTFloweta;
  TH1F* h_nStationTFlowetalowphi;  

  TH1F* h_StubQuality;
  TH1F* h_StubQualityGEM;

  TH1F* h_TFStubQuality;
  TH1F* h_TFStubQualityGEM;

  TH1F* h_TFnStubinTrack;
  TH1F* h_TFnStubinTrackGEM;

  TH2F* h_TFnStubvsPTAdd;
  TH2F* h_TFnStubvsPTAddGEM;

  TH2F* h_TFnStubvsQualityPacked;
  TH2F* h_TFnStubvsQualityPackedGEM;

  TH2F* h_TFPTpackedvsPTAdd;
  TH2F* h_TFPTpackedvsPTAddGEM;

  TH2F* h_TFPTpackedvsdPhi12;
  TH2F* h_TFPTpackedvsdPhi23;

  TH2F* h_TFPTAddvsdPhi12;
  TH2F* h_TFPTAddvsdPhi23;

  TH2F* h_TFPTpackedvsdPhi12GEM;
  TH2F* h_TFPTpackedvsdPhi23GEM;
  TH2F* h_TFPTpackedvsGEMdPhi;

  TH1F* h_noTFnStub;
  TH1F* h_noTFnStubGEM;
  TH1F* h_noTFQ3nStub;
  TH1F* h_noTFQ3nStubGEM;

  enum etabins{eta_all, eta_me1, eta_me2, netabins};
  enum ptbins{pt_all, pt_20, nptbins};
  enum stubbins{stub_2, stub_3, nstubbins};
  enum MEbins{ME_all, ME_1, ME_2, nMEbins};
  TH1F* h_truth_pt[netabins][nptbins][nstubbins][nMEbins];
  TH1F* h_truth_eta[nptbins][nstubbins][nMEbins];
  TH1F* h_truth_phi[netabins][nptbins][nstubbins][nMEbins];
  TH1F* h_L1CSCTrack_pt[netabins][nptbins][nstubbins][nMEbins];
  TH1F* h_L1CSCTrack_eta[nptbins][nstubbins][nMEbins];
  TH1F* h_L1CSCTrack_phi[netabins][nptbins][nstubbins][nMEbins];

  TH1F* h_TFnStubinTrack_phihole;

};

L1TAnalyser::L1TAnalyser(const edm::ParameterSet& iConfig)
{
  //now do what ever initialization is needed
  //  runSRLUTs = new csctf_analysis::RunSRLUTs();
  min_pt = iConfig.getUntrackedParameter<double>("minPt", 2);
  max_pt = iConfig.getUntrackedParameter<double>("maxPt", 100);
  min_aEta = iConfig.getUntrackedParameter<double>("minEta", 1.6);
  max_aEta = iConfig.getUntrackedParameter<double>("maxEta", 2.4);

}
L1TAnalyser::~L1TAnalyser()
{
 
  // do anything here that needs to be done at desctruction time
  // (e.g. close files, deallocate resources etc.)
  //  delete runSRLUTs;
}
//
// member functions
// ------------ method called to for each event  ------------
void
L1TAnalyser::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  edm::Handle<L1CSCTrackCollection> l1csctracks;
  iEvent.getByLabel("simCsctfTrackDigis",l1csctracks);
  edm::Handle<TriggerPrimitiveCollection> trigPrims;
  iEvent.getByLabel("L1TMuonTriggerPrimitives",trigPrims);
  edm::Handle<CSCCorrelatedLCTDigiCollection> lcts;
  iEvent.getByLabel("simCscTriggerPrimitiveDigis","MPCSORTED", lcts);
  edm::Handle<edm::SimTrackContainer> BaseSimTracks;
  iEvent.getByLabel("g4SimHits",BaseSimTracks);

  float minDRMatch = 0.5;

  bool loweta = false;
  bool lowphi = false;
  edm::SimTrackContainer::const_iterator BaseSimTrk;
  for(BaseSimTrk=BaseSimTracks->begin(); BaseSimTrk != BaseSimTracks->end(); BaseSimTrk++){
    if ((fabs(BaseSimTrk->type()) == 13) and
	(BaseSimTrk->momentum().pt() >= min_pt) and
	(BaseSimTrk->momentum().pt() <= max_pt) and
	(fabs(BaseSimTrk->momentum().eta()) >= min_aEta) and 
	(fabs(BaseSimTrk->momentum().eta()) <= max_aEta) ){

      TLorentzVector truemuon; 
      truemuon.SetPtEtaPhiE(BaseSimTrk->momentum().pt(), BaseSimTrk->momentum().eta(), BaseSimTrk->momentum().phi(), BaseSimTrk->momentum().E());
      TLorentzVector l1muon;      
      int nstubs=0;
      float tempDRMatch = 10;
      bool hasME1=false;
      bool hasME2=false;

      L1CSCTrackCollection::const_iterator tmp_trk = l1csctracks->begin();
      for(; tmp_trk != l1csctracks->end(); tmp_trk++){
	float pt=0, eta=-9, phi=-9;
	unsigned int quality_packed=0, rank=0;// ptLUTAddress=0;
	unsigned int pt_packed=0;

	auto l1track = tmp_trk->first;
	rank=l1track.rank();
	//ptLUTAddress = l1track.ptLUTAddress();
	l1track.decodeRank(rank,pt_packed,quality_packed); //get the pt and gaulity packed
	pt = ptscale[pt_packed];
	
	unsigned int sector = l1track.sector();// get sector
	edm::ESHandle< L1MuTriggerScales > scales;//get structures for scales (phi and eta
	iSetup.get< L1MuTriggerScalesRcd >().get(scales); // get scales from EventSetup
	const L1MuTriggerScales  *ts;// the trigger scales 
	ts = scales.product();
	unsigned gbl_phi = l1track.localPhi() + ((sector - 1)*24) + 6;
	if(gbl_phi > 143) gbl_phi -= 143;
	phi = ts->getPhiScale()->getLowEdge( gbl_phi&0xff );
	unsigned eta_sign = (l1track.endcap() == 1 ? 0 : 1);
	eta = ts->getRegionalEtaScale(2)->getCenter( ((l1track.eta_packed()) | (eta_sign<<5)) & 0x3f );

	int tempnstubs = 0;
	bool temphasME1=false;
	bool temphasME2=false;

	TLorentzVector templ1muon;
	templ1muon.SetPtEtaPhiM(pt, eta, phi, 0.1057);
	CSCCorrelatedLCTDigiCollection::DigiRangeIterator csc=tmp_trk->second.begin();
	for(; csc!=tmp_trk->second.end(); csc++){
	  if ((*csc).first.station()==1) temphasME1 = true;
	  if ((*csc).first.station()==2) temphasME2 = true;
	  tempnstubs++;
	}

	if (truemuon.DeltaR(templ1muon) < minDRMatch){
	  if (tempnstubs >= nstubs){
	    if (truemuon.DeltaR(templ1muon) < tempDRMatch){
	      nstubs = tempnstubs;
	      tempDRMatch = truemuon.DeltaR(templ1muon);
	      l1muon = templ1muon;
	      hasME1 = temphasME1;
	      hasME2 = temphasME2;
	    }
	  }
	}
      }
      // testing for eff drop at phi~0
      if (BaseSimTrk->momentum().eta() > 1.6 and BaseSimTrk->momentum().eta() < 1.75){
	loweta = true;
	if (BaseSimTrk->momentum().phi() > 0.0 and BaseSimTrk->momentum().phi() < 0.4){
	  lowphi = true;
	  if (nstubs == 2)
      	    cout << "drop in 3 stub efficiency, pt = "<< BaseSimTrk->momentum().pt()
      		 << ", eta = "<< BaseSimTrk->momentum().eta()
      		 << ", phi = "<< BaseSimTrk->momentum().phi()
      		 << endl;
	}
      }

      float trueEta = fabs(truemuon.Eta());
      for (int netabin = 0; netabin < netabins; netabin++){
	if ((netabin == eta_all) ||
	    ((netabin == eta_me1) && (trueEta > 1.6 && trueEta < 2.1)) ||
	    ((netabin == eta_me2) && (trueEta > 2.1 && trueEta < 2.4))){	    
	  for (int nptbin = 0; nptbin < nptbins; nptbin++){
	    for (int nMEbin = 0; nMEbin < nMEbins; nMEbin++){
	      for (int nstubbin = 0; nstubbin < nstubbins; nstubbin++){

		if (nptbin == 0 and netabin == 0 and nMEbin == 0 and nstubbin == 0)
		  if (truemuon.Phi() > 0 and truemuon.Phi() < 0.4)
		    if (trueEta < 1.75)
		      h_TFnStubinTrack_phihole->Fill(nstubs);

		h_truth_pt[netabin][nptbin][nstubbin][nMEbin]->Fill(truemuon.Pt());
		if ((nptbin == pt_all && truemuon.Pt() >= 10) ||
		    ((nptbin == pt_20) && (truemuon.Pt() >= 30))){
		  if (netabin == eta_all) h_truth_eta[nptbin][nstubbin][nMEbin]->Fill(trueEta);
		  h_truth_phi[netabin][nptbin][nstubbin][nMEbin]->Fill(truemuon.Phi());
		}
		
		if ((nptbin == pt_all) ||
		    ((nptbin == pt_20) && (l1muon.Pt() >= 20))){

		  if (((nstubbin == stub_2) && (nstubs > 1)) ||
		      ((nstubbin == stub_3) && (nstubs > 2))){

		    if ((nMEbin == ME_all) ||
			((nMEbin == ME_1) && (hasME1)) ||
			((nMEbin == ME_2) && (hasME2))){

		      h_L1CSCTrack_pt[netabin][nptbin][nstubbin][nMEbin]->Fill(truemuon.Pt());
		      
		      if ((nptbin == pt_all && truemuon.Pt() >= 10) ||
			  ((nptbin == pt_20) && (truemuon.Pt() >= 30))){
			if (netabin == eta_all) h_L1CSCTrack_eta[nptbin][nstubbin][nMEbin]->Fill(trueEta);
			h_L1CSCTrack_phi[netabin][nptbin][nstubbin][nMEbin]->Fill(truemuon.Phi());
		      }
		    }
		  }
		}
	      }
	    }
	  }
	}
      }
    }
  }
  
  // all lcts
  for(CSCCorrelatedLCTDigiCollection::DigiRangeIterator csc=lcts->begin(); csc!=lcts->end(); csc++){
    if ((*csc).first.endcap() == 1){
      h_nStation->Fill((*csc).first.station());
    }
    if ((*csc).first.endcap() == 2){
      h_nStation->Fill(-((*csc).first.station()));
    }
  }

  // lcts used in tracks
  L1CSCTrackCollection::const_iterator tmp_trk = l1csctracks->begin();
  for(; tmp_trk != l1csctracks->end(); tmp_trk++){
    float GEMdPhi = -99;
    unsigned int quality_packed=0, rank=0, ptLUTAddress=0;
    unsigned int pt_packed=0;
    auto l1track = tmp_trk->first;
    rank=l1track.rank();
    ptLUTAddress = l1track.ptLUTAddress();
    l1track.decodeRank(rank,pt_packed,quality_packed); //get the pt and gaulity packed
    int nstubs = 0;
    bool hasGEM = false;
    for(CSCCorrelatedLCTDigiCollection::DigiRangeIterator csc=tmp_trk->second.begin(); csc!=tmp_trk->second.end(); csc++){
      auto lctdigi = (*csc).second.first;
      nstubs++;
      h_GEMDPhi->Fill((*csc).second.first->getGEMDPhi());

      if ((*csc).first.endcap() == 1){
	h_nStationTF->Fill((*csc).first.station());
	if (loweta){
	  h_nStationTFloweta->Fill((*csc).first.station());
	  if (lowphi) h_nStationTFlowetalowphi->Fill((*csc).first.station());
	}
      }
      if ((*csc).first.endcap() == 2){
	h_nStationTF->Fill(-((*csc).first.station()));
      }
      if ((*csc).second.first->getGEMDPhi() > -99){
	hasGEM = true;
	GEMdPhi = (*csc).second.first->getGEMDPhi();
	h_TFStubQualityGEM->Fill(lctdigi->getQuality());
      }
      h_TFStubQuality->Fill(lctdigi->getQuality());
    }

    h_TFnStubinTrack->Fill(nstubs);
    h_TFnStubvsPTAdd->Fill(nstubs,ptLUTAddress);
    h_TFnStubvsQualityPacked->Fill(nstubs,quality_packed);
    h_TFPTpackedvsPTAdd->Fill(pt_packed,ptLUTAddress);
    h_TFPTpackedvsdPhi12->Fill(pt_packed,l1track.deltaPhi12());
    h_TFPTpackedvsdPhi23->Fill(pt_packed,l1track.deltaPhi23());
    h_TFPTAddvsdPhi12->Fill(ptLUTAddress,l1track.deltaPhi12());
    h_TFPTAddvsdPhi23->Fill(ptLUTAddress,l1track.deltaPhi23());

    if (hasGEM){
      h_TFnStubinTrackGEM->Fill(nstubs);
      h_TFnStubvsPTAddGEM->Fill(nstubs,ptLUTAddress);
      h_TFnStubvsQualityPackedGEM->Fill(nstubs,quality_packed);
      h_TFPTpackedvsPTAddGEM->Fill(pt_packed,ptLUTAddress);
      h_TFPTpackedvsdPhi12GEM->Fill(pt_packed,l1track.deltaPhi12());
      h_TFPTpackedvsdPhi23GEM->Fill(pt_packed,l1track.deltaPhi23());
      h_TFPTpackedvsGEMdPhi->Fill(pt_packed,GEMdPhi);
    }
  }
}


// ------------ method called once each job just before starting event loop  ------------
void L1TAnalyser::beginJob()
{

  TString etabinsName[] = {"", "eta1", "eta2"};
  TString ptbinsName[] = {"", "pt20"};
  TString stubbinsName[] = {"stub2", "stub3"};
  TString MEbinsName[] = {"", "hasME1", "hasME2"};
  for (int nstubbin = 0; nstubbin < nstubbins; nstubbin++){
    for (int netabin = 0; netabin < netabins; netabin++){
      for (int nptbin = 0; nptbin < nptbins; nptbin++){
	for (int nMEbin = 0; nMEbin < nMEbins; nMEbin++){
	  h_truth_pt[netabin][nptbin][nstubbin][nMEbin] 
	    = new TH1F("truth_"+stubbinsName[nstubbin]+ptbinsName[nptbin]+MEbinsName[nMEbin]+etabinsName[netabin]+"_pt", "", 20, 0, 50);
	  h_truth_phi[netabin][nptbin][nstubbin][nMEbin]
	    = new TH1F("truth_"+stubbinsName[nstubbin]+ptbinsName[nptbin]+MEbinsName[nMEbin]+etabinsName[netabin]+"_phi", "", 70,-3.5,3.5);

	  if (netabin == eta_all){
	    h_truth_eta[nptbin][nstubbin][nMEbin]
	    = new TH1F("truth_"+stubbinsName[nstubbin]+ptbinsName[nptbin]+MEbinsName[nMEbin]+"_eta", "", 50,1.5,2.5);

	    h_L1CSCTrack_eta[nptbin][nstubbin][nMEbin]
	      = fs->make<TH1F>("L1cscTrack_"+stubbinsName[nstubbin]+ptbinsName[nptbin]+MEbinsName[nMEbin]+"_eta", "", 50,1.5,2.5);
	    h_L1CSCTrack_eta[nptbin][nstubbin][nMEbin]->GetXaxis()->SetTitle("simulated muon #eta");
	    h_L1CSCTrack_eta[nptbin][nstubbin][nMEbin]->GetYaxis()->SetTitle("Efficiency");
	  }

	  h_L1CSCTrack_pt[netabin][nptbin][nstubbin][nMEbin]
	    = fs->make<TH1F>("L1cscTrack_"+stubbinsName[nstubbin]+ptbinsName[nptbin]+MEbinsName[nMEbin]+etabinsName[netabin]+"_pt", "", 20, 0, 50);
	  h_L1CSCTrack_pt[netabin][nptbin][nstubbin][nMEbin]->GetXaxis()->SetTitle("simulated muon p_{T} [GeV]");
	  h_L1CSCTrack_pt[netabin][nptbin][nstubbin][nMEbin]->GetYaxis()->SetTitle("Efficiency");

	  h_L1CSCTrack_phi[netabin][nptbin][nstubbin][nMEbin]
	    = fs->make<TH1F>("L1cscTrack_"+stubbinsName[nstubbin]+ptbinsName[nptbin]+MEbinsName[nMEbin]+etabinsName[netabin]+"_phi", "", 70,-3.5,3.5);
	  h_L1CSCTrack_phi[netabin][nptbin][nstubbin][nMEbin]->GetXaxis()->SetTitle("simulated muon #phi");
	  h_L1CSCTrack_phi[netabin][nptbin][nstubbin][nMEbin]->GetYaxis()->SetTitle("Efficiency");
	}
      }
    }
  }
  h_StubQuality=fs->make<TH1F>("StubQuality","Stub Quality",20,0,20);
  h_StubQuality->GetXaxis()->SetTitle("Stub Quality");
  h_StubQuality->GetYaxis()->SetTitle("Counts");

  h_StubQualityGEM=fs->make<TH1F>("StubQualityGEM","Stub Quality",20,0,20);
  h_StubQualityGEM->GetXaxis()->SetTitle("Stub Quality");
  h_StubQualityGEM->GetYaxis()->SetTitle("Counts");

  h_TFStubQuality=fs->make<TH1F>("TFStubQuality","Stub Quality",20,0,20);
  h_TFStubQuality->GetXaxis()->SetTitle("Stub Quality");
  h_TFStubQuality->GetYaxis()->SetTitle("Counts");

  h_TFStubQualityGEM=fs->make<TH1F>("TFStubQualityGEM","Stub Quality",20,0,20);
  h_TFStubQualityGEM->GetXaxis()->SetTitle("Stub Quality");
  h_TFStubQualityGEM->GetYaxis()->SetTitle("Counts");

  h_TFnStubinTrack=fs->make<TH1F>("TFnStubinTrack","N Stubs in Track",6,0,6);
  h_TFnStubinTrack->GetXaxis()->SetTitle("N Stubs in Track");
  h_TFnStubinTrack->GetYaxis()->SetTitle("Counts");

  h_TFnStubinTrack_phihole=fs->make<TH1F>("TFnStubinTrack_phihole","N Stubs in Track",6,0,6);
  h_TFnStubinTrack_phihole->GetXaxis()->SetTitle("N Stubs in Track");
  h_TFnStubinTrack_phihole->GetYaxis()->SetTitle("Counts");

  h_TFnStubinTrackGEM=fs->make<TH1F>("TFnStubinTrackGEM","N Stubs in Track",6,0,6);
  h_TFnStubinTrackGEM->GetXaxis()->SetTitle("N Stubs in Track");
  h_TFnStubinTrackGEM->GetYaxis()->SetTitle("Counts");

  h_TFnStubvsPTAdd=fs->make<TH2F>("TFnStubvsPTAdd","N Stubs in Track Vs Pt address",6,0,6,500,0,4500000);
  h_TFnStubvsPTAdd->GetXaxis()->SetTitle("N Stubs in Track");
  h_TFnStubvsPTAdd->GetYaxis()->SetTitle("Pt address");

  h_TFnStubvsPTAddGEM=fs->make<TH2F>("TFnStubvsPTAddGEM","N Stubs in Track Vs Pt address",6,0,6,500,0,4500000);
  h_TFnStubvsPTAddGEM->GetXaxis()->SetTitle("N Stubs in Track");
  h_TFnStubvsPTAddGEM->GetYaxis()->SetTitle("Pt address");

  h_TFnStubvsQualityPacked=fs->make<TH2F>("TFnStubvsQualityPacked","N Stubs in Track vs Quality",6,0,6,5,0,5);
  h_TFnStubvsQualityPacked->GetXaxis()->SetTitle("N Stubs in Track");
  h_TFnStubvsQualityPacked->GetYaxis()->SetTitle("Quality");

  h_TFnStubvsQualityPackedGEM=fs->make<TH2F>("TFnStubvsQualityPackedGEM","N Stubs in Track Vs Quality",6,0,6,5,0,5);
  h_TFnStubvsQualityPackedGEM->GetXaxis()->SetTitle("N Stubs in Track");
  h_TFnStubvsQualityPackedGEM->GetYaxis()->SetTitle("Quality");

  h_TFPTpackedvsPTAdd=fs->make<TH2F>("TFPTpackedvsPTAdd","Pt packed Vs Pt address",40,0,40,500,0,4500000);
  h_TFPTpackedvsPTAdd->GetXaxis()->SetTitle("Pt packed");
  h_TFPTpackedvsPTAdd->GetYaxis()->SetTitle("Pt address");

  h_TFPTpackedvsPTAddGEM=fs->make<TH2F>("TFPTpackedvsPTAddGEM","Pt packed Vs Pt address",40,0,40,500,0,4500000);
  h_TFPTpackedvsPTAddGEM->GetXaxis()->SetTitle("Pt packed");
  h_TFPTpackedvsPTAddGEM->GetYaxis()->SetTitle("Pt address");

  h_TFPTpackedvsdPhi12=fs->make<TH2F>("TFPTpackedvsdPhi12","Pt packed Vs dPhi12",40,0,40,200,0,300);
  h_TFPTpackedvsdPhi12->GetXaxis()->SetTitle("Pt packed");
  h_TFPTpackedvsdPhi12->GetYaxis()->SetTitle("dPhi12");

  h_TFPTpackedvsdPhi23=fs->make<TH2F>("TFPTpackedvsdPhi23","Pt packed Vs dPhi23",40,0,40,200,0,20);
  h_TFPTpackedvsdPhi23->GetXaxis()->SetTitle("Pt packed");
  h_TFPTpackedvsdPhi23->GetYaxis()->SetTitle("dPhi23");

  h_TFPTAddvsdPhi12=fs->make<TH2F>("TFPTAddvsdPhi12","Pt Add Vs dPhi12",500,0,4500000,200,0,300);
  h_TFPTAddvsdPhi12->GetXaxis()->SetTitle("Pt Add");
  h_TFPTAddvsdPhi12->GetYaxis()->SetTitle("dPhi12");

  h_TFPTAddvsdPhi23=fs->make<TH2F>("TFPTAddvsdPhi23","Pt Add Vs dPhi23",500,0,4500000,200,0,20);
  h_TFPTAddvsdPhi23->GetXaxis()->SetTitle("Pt Add");
  h_TFPTAddvsdPhi23->GetYaxis()->SetTitle("dPhi23");

  h_TFPTpackedvsdPhi12GEM=fs->make<TH2F>("TFPTpackedvsdPhi12GEM","Pt packed Vs dPhi12",40,0,40,200,0,300);
  h_TFPTpackedvsdPhi12GEM->GetXaxis()->SetTitle("Pt packed");
  h_TFPTpackedvsdPhi12GEM->GetYaxis()->SetTitle("dPhi12");

  h_TFPTpackedvsdPhi23GEM=fs->make<TH2F>("TFPTpackedvsdPhi23GEM","Pt packed Vs dPhi23",40,0,40,200,0,20);
  h_TFPTpackedvsdPhi23GEM->GetXaxis()->SetTitle("Pt packed");
  h_TFPTpackedvsdPhi23GEM->GetYaxis()->SetTitle("dPhi23");

  h_TFPTpackedvsGEMdPhi=fs->make<TH2F>("TFPTpackedvsGEMdPhi","Pt packed Vs GEMdPhi",40,0,40,200,-0.5,0.5);
  h_TFPTpackedvsGEMdPhi->GetXaxis()->SetTitle("Pt packed");
  h_TFPTpackedvsGEMdPhi->GetYaxis()->SetTitle("GEMdPhi");

  h_noTFnStub=fs->make<TH1F>("noTFnStub","no track, No. Stubs",6,0,6);
  h_noTFnStub->GetXaxis()->SetTitle("No. Stubs");
  h_noTFnStub->GetYaxis()->SetTitle("Counts");

  h_noTFnStubGEM=fs->make<TH1F>("noTFnStubGEM","no track, No. Stubs",6,0,6);
  h_noTFnStubGEM->GetXaxis()->SetTitle("No. Stubs");
  h_noTFnStubGEM->GetYaxis()->SetTitle("Counts");

  h_noTFQ3nStub=fs->make<TH1F>("noTFQ3nStub","no Q3 track, No. Stubs",6,0,6);
  h_noTFQ3nStub->GetXaxis()->SetTitle("No. Stubs");
  h_noTFQ3nStub->GetYaxis()->SetTitle("Counts");

  h_noTFQ3nStubGEM=fs->make<TH1F>("noTFQ3nStubGEM","no Q3 track, No. Stubs",6,0,6);
  h_noTFQ3nStubGEM->GetXaxis()->SetTitle("No. Stubs");
  h_noTFQ3nStubGEM->GetYaxis()->SetTitle("Counts");

  h_GEMDPhi=fs->make<TH1F>("GemDPhi","GEM-SCS dPhi in station 1",100,-0.1,0.1);
  h_GEMDPhi->GetXaxis()->SetTitle("dPhi");
  h_GEMDPhi->GetYaxis()->SetTitle("Counts");

  h_nStationTF=fs->make<TH1F>("nStationTF","stations in track",11,-5,6);
  h_nStationTF->GetXaxis()->SetTitle("Station number");
  h_nStationTF->GetYaxis()->SetTitle("Counts");
  h_nStationTFloweta=fs->make<TH1F>("nStationTFloweta","stations in track",11,-5,6);
  h_nStationTFloweta->GetXaxis()->SetTitle("Station number");
  h_nStationTFloweta->GetYaxis()->SetTitle("Counts");
  h_nStationTFlowetalowphi=fs->make<TH1F>("nStationTFlowetalowphi","stations in track",11,-5,6);
  h_nStationTFlowetalowphi->GetXaxis()->SetTitle("Station number");
  h_nStationTFlowetalowphi->GetYaxis()->SetTitle("Counts");

  h_nStation=fs->make<TH1F>("nStation","LCT stations",11,-5,6);
  h_nStation->GetXaxis()->SetTitle("Station number");
  h_nStation->GetYaxis()->SetTitle("Counts");

}

// ------------ method called once each job just after ending the event loop  ------------
void L1TAnalyser::endJob() 
{
  for (int nstubbin = 0; nstubbin < nstubbins; nstubbin++){
    for (int netabin = 0; netabin < netabins; netabin++){
      for (int nptbin = 0; nptbin < nptbins; nptbin++){
	for (int nMEbin = 0; nMEbin < nMEbins; nMEbin++){
	  h_truth_pt[netabin][nptbin][nstubbin][nMEbin]->Sumw2();
	  h_truth_phi[netabin][nptbin][nstubbin][nMEbin]->Sumw2();
	  if (netabin == eta_all) {
	    h_truth_eta[nptbin][nstubbin][nMEbin]->Sumw2();
	    h_L1CSCTrack_eta[nptbin][nstubbin][nMEbin]->Sumw2();
	    h_L1CSCTrack_eta[nptbin][nstubbin][nMEbin]->Divide(h_L1CSCTrack_eta[nptbin][nstubbin][nMEbin], 
								      h_truth_eta[nptbin][nstubbin][nMEbin],1.0,1.0,"B");
	  }
	  h_L1CSCTrack_pt[netabin][nptbin][nstubbin][nMEbin]->Sumw2();
	  h_L1CSCTrack_phi[netabin][nptbin][nstubbin][nMEbin]->Sumw2();
	  h_L1CSCTrack_pt[netabin][nptbin][nstubbin][nMEbin]->Divide(h_L1CSCTrack_pt[netabin][nptbin][nstubbin][nMEbin], 
								     h_truth_pt[netabin][nptbin][nstubbin][nMEbin],1.0,1.0,"B");
	  h_L1CSCTrack_phi[netabin][nptbin][nstubbin][nMEbin]->Divide(h_L1CSCTrack_phi[netabin][nptbin][nstubbin][nMEbin],
								      h_truth_phi[netabin][nptbin][nstubbin][nMEbin],1.0,1.0,"B");
	}
      }
    }
  }
}

//define this as a plug-in
DEFINE_FWK_MODULE(L1TAnalyser);
