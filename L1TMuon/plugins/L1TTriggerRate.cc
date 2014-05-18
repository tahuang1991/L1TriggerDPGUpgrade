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

class L1TTriggerRate : public edm::EDAnalyzer {
public:
  explicit L1TTriggerRate(const edm::ParameterSet&);
  ~L1TTriggerRate();


private:
  virtual void beginJob() ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;

  // ----------member data ---------------------------

  edm::Service<TFileService> fs;
  //  csctf_analysis::RunSRLUTs* runSRLUTs;
  
  double min_pt;
  double max_pt;
  double min_aEta;
  double max_aEta;
  float ntotalEvents = 0.0;
  const float ptscale[33] = { 
    -1.,   0.0,   1.5,   2.0,   2.5,   3.0,   3.5,   4.0,
    4.5,   5.0,   6.0,   7.0,   8.0,  10.0,  12.0,  14.0,  
    16.0,  18.0,  20.0,  25.0,  30.0,  35.0,  40.0,  45.0, 
    50.0,  60.0,  70.0,  80.0,  90.0, 100.0, 120.0, 140.0, 1.E6 };
  const float xptscale[30] = { 
    2.0,   2.5,   3.0,   3.5,   4.0,
    4.5,   5.0,   6.0,   7.0,   8.0,  10.0,  12.0,  14.0,  
    16.0,  18.0,  20.0,  25.0,  30.0,  35.0,  40.0,  45.0, 
    50.0,  60.0,  70.0,  80.0,  90.0, 100.0, 120.0, 140.0, 200.0};
  unsigned maxPTbins = 33;

const double ME11GEMdPhi[8][3] = {
 {3 , 0.03971647, 0.01710244 },
 {5 , 0.02123785, 0.00928431 },
 {7 , 0.01475524, 0.00650928 },
 {10, 0.01023299, 0.00458796 },
 {15, 0.00689220, 0.00331313 },
 {20, 0.00535176, 0.00276152 },
 {30, 0.00389050, 0.00224959 },
 {40, 0.00329539, 0.00204670 }
};
const double ME21GEMdPhi[8][3] = {
  {3 , 0.01832829, 0.01003643 },
  {5 , 0.01095490, 0.00631625 },
  {7 , 0.00786026, 0.00501017 },
  {10, 0.00596349, 0.00414560 },
  {15, 0.00462411, 0.00365550 },
  {20, 0.00435298, 0.00361550 },
  {30, 0.00465160, 0.00335700 },
  {40, 0.00372145, 0.00366262 }
};

  enum etabins{eta_all, eta_me1, eta_me2, eta_me3, netabins};
  enum ptbins{pt_all, pt_20, nptbins};
  enum stubbins{stub_2, stub_3, nstubbins};
  enum MEbins{ME_all, ME_1, GE_1, ME_2, GE_2, nMEbins};
  TH1F* h_L1CSCTrack_pt[netabins][nptbins][nstubbins][nMEbins];
  TH1F* h_L1CSCTrack_eta[nptbins][nstubbins][nMEbins];
  TH1F* h_L1CSCTrack_phi[netabins][nptbins][nstubbins][nMEbins];

  TH1F* h_nStation;

};

L1TTriggerRate::L1TTriggerRate(const edm::ParameterSet& iConfig)
{
  //now do what ever initialization is needed
  //  runSRLUTs = new csctf_analysis::RunSRLUTs();
  min_pt = iConfig.getParameter<double>("minPt");
  max_pt = iConfig.getParameter<double>("maxPt");
  min_aEta = iConfig.getParameter<double>("minEta");
  max_aEta = iConfig.getParameter<double>("maxEta");

}

L1TTriggerRate::~L1TTriggerRate()
{
}

void L1TTriggerRate::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  edm::Handle<L1CSCTrackCollection> l1csctracks;
  iEvent.getByLabel("simCsctfTrackDigis",l1csctracks);
  edm::Handle<TriggerPrimitiveCollection> trigPrims;
  iEvent.getByLabel("L1TMuonTriggerPrimitives",trigPrims);
  edm::Handle<CSCCorrelatedLCTDigiCollection> lcts;
  iEvent.getByLabel("simCscTriggerPrimitiveDigis","MPCSORTED", lcts);
  ++ntotalEvents;
  // lcts used in tracks
  L1CSCTrackCollection::const_iterator tmp_trk = l1csctracks->begin();
  for(; tmp_trk != l1csctracks->end(); tmp_trk++){
    //    float GEMdPhi = -99;
    float pt=0, eta=-9, phi=-9;
    unsigned int quality_packed=0, rank=0;//ptLUTAddress=0;
    unsigned int pt_packed=0;

    auto l1track = tmp_trk->first;
    rank=l1track.rank();
    // ptLUTAddress = l1track.ptLUTAddress();
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
    if (phi > 3.14159265359)
      phi = 3.14159265359 - phi;
    int nstubs=0;
    bool hasME1=false;
    bool hasME2=false;
    float GE11dPhi=-99.;
    float GE21dPhi=-99.;
    bool passGE11=false;
    bool passGE21=false;
    CSCCorrelatedLCTDigiCollection::DigiRangeIterator csc=tmp_trk->second.begin();
    for(; csc!=tmp_trk->second.end(); csc++){

      bool is_odd = ((*csc).first.chamber()%2==1);
      
      if ((*csc).first.station()==1){
	hasME1 = true;
	GE11dPhi =  fabs((*csc).second.first->getGEMDPhi());
	for (int b = 0; b < 8; b++){
	  if (double(pt) > ME11GEMdPhi[b][0]){
	    if ((is_odd && ME11GEMdPhi[b][1] > GE11dPhi) || 
		(!is_odd && ME11GEMdPhi[b][2] > GE11dPhi)){
	      passGE11 = true;
	    }
	    else passGE11 = false;
	  }
	}
	if (GE11dPhi == -99) passGE11 = true;
      }

      if ((*csc).first.station()==2){
	hasME2 = true;
	GE21dPhi =  fabs((*csc).second.first->getGEMDPhi());
	for (int b = 0; b < 8; b++){
	  if (double(pt) > ME21GEMdPhi[b][0]){
	    if ((is_odd && ME21GEMdPhi[b][1] > GE21dPhi) || 
		(!is_odd && ME21GEMdPhi[b][2] > GE21dPhi)){
	      passGE21 = true;
	    }
	    else passGE21 = false;
	  }
	}
	if (GE21dPhi == -99) passGE21 = true;
      }
      nstubs++;
    }

    for (int nstubbin = 0; nstubbin < nstubbins; nstubbin++){
      if (((nstubbin == stub_2) && (nstubs > 1)) ||
	  ((nstubbin == stub_3) && (nstubs > 2))){

	for (int nMEbin = 0; nMEbin < nMEbins; nMEbin++){
	  if ((nMEbin == ME_all) ||
	      ((nMEbin == ME_1) && hasME1) ||
	      ((nMEbin == ME_2) && hasME2) ||
	      ((nMEbin == GE_1) && passGE11) ||
	      ((nMEbin == GE_2) && passGE11 && passGE21) )
	    {

	    for (int netabin = 0; netabin < netabins; netabin++){
	      if ((netabin == eta_all) ||
		  ((netabin == eta_me3) && (eta > 1.64 && eta < 2.14)) ||
		  ((netabin == eta_me1) && (eta > 1.6 && eta < 2.1)) ||
		  ((netabin == eta_me2) && (eta > 2.1 && eta < 2.4))){
   
		for (unsigned nptbin = 0; nptbin < maxPTbins; nptbin++){
		  if (pt >= ptscale[nptbin]){
		    h_L1CSCTrack_pt[netabin][0][nstubbin][nMEbin]->Fill(ptscale[nptbin]);
		  }
		}
		for (unsigned nptbin = 0; nptbin < nptbins; nptbin++){
		  if ((nptbin == pt_all) ||
		      ((nptbin == pt_20) && (pt >= 20))){
		    if (netabin == 0) h_L1CSCTrack_eta[nptbin][nstubbin][nMEbin]->Fill(eta);
		    h_L1CSCTrack_phi[netabin][nptbin][nstubbin][nMEbin]->Fill(phi);
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

}


// ------------ method called once each job just before starting event loop  ------------
void L1TTriggerRate::beginJob()
{

  TString etabinsName[] = {"", "eta1", "eta2", "eta3"};
  TString ptbinsName[] = {"", "pt20"};
  TString stubbinsName[] = {"stub2", "stub3"};
  TString MEbinsName[] = {"", "hasME1", "hasGE11", "hasME2", "hasGE21"};
  for (int nstubbin = 0; nstubbin < nstubbins; nstubbin++){
    for (int netabin = 0; netabin < netabins; netabin++){
      for (int nptbin = 0; nptbin < nptbins; nptbin++){
	for (int nMEbin = 0; nMEbin < nMEbins; nMEbin++){
	  h_L1CSCTrack_pt[netabin][nptbin][nstubbin][nMEbin]
	    = fs->make<TH1F>("L1cscTrack_"+stubbinsName[nstubbin]+ptbinsName[nptbin]+MEbinsName[nMEbin]+etabinsName[netabin]+"_pt", "", 29, xptscale);
	  h_L1CSCTrack_pt[netabin][nptbin][nstubbin][nMEbin]->GetXaxis()->SetTitle("L1 muon p_{T} [GeV]");
	  h_L1CSCTrack_pt[netabin][nptbin][nstubbin][nMEbin]->GetYaxis()->SetTitle("Rate [kHz]");

	  if (netabin == 0){
	    h_L1CSCTrack_eta[nptbin][nstubbin][nMEbin]
	      = fs->make<TH1F>("L1cscTrack_"+stubbinsName[nstubbin]+ptbinsName[nptbin]+MEbinsName[nMEbin]+"_eta", "", 50,1.5,2.5);
	    h_L1CSCTrack_eta[nptbin][nstubbin][nMEbin]->GetXaxis()->SetTitle("muon #eta");
	    h_L1CSCTrack_eta[nptbin][nstubbin][nMEbin]->GetYaxis()->SetTitle("Rate [kHz]");
	  }

	  h_L1CSCTrack_phi[netabin][nptbin][nstubbin][nMEbin]
	    = fs->make<TH1F>("L1cscTrack_"+stubbinsName[nstubbin]+ptbinsName[nptbin]+MEbinsName[nMEbin]+etabinsName[netabin]+"_phi", "", 70,-3.5,3.5);
	  h_L1CSCTrack_phi[netabin][nptbin][nstubbin][nMEbin]->GetXaxis()->SetTitle(" muon #phi");
	  h_L1CSCTrack_phi[netabin][nptbin][nstubbin][nMEbin]->GetYaxis()->SetTitle("Rate [kHz]");
	}
      }
    }
  }

  h_nStation=fs->make<TH1F>("nStation","",11,-5,6);
  h_nStation->GetXaxis()->SetTitle("LCT stations");
  h_nStation->GetYaxis()->SetTitle("Rate [kHz]");

}

// ------------ method called once each job just after ending the event loop  ------------
void L1TTriggerRate::endJob() 
{
  for (int nstubbin = 0; nstubbin < nstubbins; nstubbin++){
    for (int netabin = 0; netabin < netabins; netabin++){
      for (int nptbin = 0; nptbin < nptbins; nptbin++){
	for (int nMEbin = 0; nMEbin < nMEbins; nMEbin++){
	  h_L1CSCTrack_pt[netabin][nptbin][nstubbin][nMEbin]->Sumw2();
	  if (netabin == 0) h_L1CSCTrack_eta[nptbin][nstubbin][nMEbin]->Sumw2();
	  h_L1CSCTrack_phi[netabin][nptbin][nstubbin][nMEbin]->Sumw2();

	  h_L1CSCTrack_pt[netabin][nptbin][nstubbin][nMEbin]->Scale(40000./ntotalEvents/3.*0.795);
	  if (netabin == 0) h_L1CSCTrack_eta[nptbin][nstubbin][nMEbin]->Scale(40000./ntotalEvents/3.*0.795);
	  h_L1CSCTrack_phi[netabin][nptbin][nstubbin][nMEbin]->Scale(40000./ntotalEvents/3.*0.795);
	}
      }
    }
  }
  h_nStation->Sumw2();
  h_nStation->Scale(40000./ntotalEvents/3.*0.795);
  cout << "L1TTriggerRate::endJob ntotalEvents " << ntotalEvents<<endl;
}

//define this as a plug-in
DEFINE_FWK_MODULE(L1TTriggerRate);
