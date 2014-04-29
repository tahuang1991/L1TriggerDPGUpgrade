
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

#include "TH1F.h"
#include "TH2F.h"
#include "TFile.h"

using namespace std;
using namespace edm;
using namespace L1TMuon;

class L1TUpgradeAnalyser : public edm::EDAnalyzer {
public:
  explicit L1TUpgradeAnalyser(const edm::ParameterSet&);
  ~L1TUpgradeAnalyser();


private:
  virtual void beginJob() ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;

  // ----------member data ---------------------------

  //TH1F* rateHist;

  edm::InputTag lctsTag;	
  edm::InputTag vertexColTag;	
  edm::Service<TFileService> fs;
  
  //  csctf_analysis::RunSRLUTs* runSRLUTs;
  int nVertex;
  bool haveRECO;
  int singleSectorNum;
  string outTreeFileName;

  TH1F* h_GEMDPhi;
  TH1F* h_nStation;  

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

  TH1F* h_truth_pt;
  TH1F* h_truth_eta;
  TH1F* h_truth_phi;
  TH1F* h_truth_30_eta;
  TH1F* h_truth_30_phi;

  TH1F* h_L1CSCTrack__pt;
  TH1F* h_L1CSCTrack__eta;
  TH1F* h_L1CSCTrack__phi;

  TH1F* h_L1CSCTrack_Q3_pt;
  TH1F* h_L1CSCTrack_Q3_eta;
  TH1F* h_L1CSCTrack_Q3_phi;

  TH1F* h_L1CSCTrack_20_pt;
  TH1F* h_L1CSCTrack_20_eta;
  TH1F* h_L1CSCTrack_20_phi;

  TH1F* h_L1CSCTrack_20ME1_pt;
  TH1F* h_L1CSCTrack_20ME1_eta;
  TH1F* h_L1CSCTrack_20ME1_phi;

  TH1F* h_truth_phi_fwd;
  TH1F* h_truth_phi_bwd;
  TH1F* h_L1CSCTrack_Q3_phi_fwd;
  TH1F* h_L1CSCTrack_Q3_phi_bwd;

  TH1F* h_L1TMtracks__pt;
  TH1F* h_L1TMtracks__eta;
  TH1F* h_L1TMtracks__phi;

  TH1F* h_L1CSCTrack_ME1_pt;
  TH1F* h_L1CSCTrack_ME1_eta;
  TH1F* h_L1CSCTrack_ME1_phi;
  TH1F* h_L1TMtracks_ME1_pt;
  TH1F* h_L1TMtracks_ME1_eta;
  TH1F* h_L1TMtracks_ME1_phi;

  TH1F* h_L1CSCTrack_3Stubs_pt;
  TH1F* h_L1CSCTrack_3Stubs_eta;
  TH1F* h_L1CSCTrack_3Stubs_phi;
  TH1F* h_L1TMtracks_3Stubs_pt;
  TH1F* h_L1TMtracks_3Stubs_eta;
  TH1F* h_L1TMtracks_3Stubs_phi;

  TH1F* h_L1CSCTrack_3StubsME1_pt;
  TH1F* h_L1CSCTrack_3StubsME1_eta;
  TH1F* h_L1CSCTrack_3StubsME1_phi;
  TH1F* h_L1TMtracks_3StubsME1_pt;
  TH1F* h_L1TMtracks_3StubsME1_eta;
  TH1F* h_L1TMtracks_3StubsME1_phi;

  TH1F* h_L1CSCTrack_3Stubs20_pt;
  TH1F* h_L1CSCTrack_3Stubs20_eta;
  TH1F* h_L1CSCTrack_3Stubs20_phi;
  TH1F* h_L1CSCTrack_3Stubs20ME1_pt;
  TH1F* h_L1CSCTrack_3Stubs20ME1_eta;
  TH1F* h_L1CSCTrack_3Stubs20ME1_phi;

  TH1F* h_L1CSCTrack_2Stubs_pt;
  TH1F* h_L1CSCTrack_2Stubs_eta;
  TH1F* h_L1CSCTrack_2Stubs_phi;
  TH1F* h_L1TMtracks_2Stubs_pt;
  TH1F* h_L1TMtracks_2Stubs_eta;
  TH1F* h_L1TMtracks_2Stubs_phi;

  TH1F* h_L1CSCTrack_2StubsME1_pt;
  TH1F* h_L1CSCTrack_2StubsME1_eta;
  TH1F* h_L1CSCTrack_2StubsME1_phi;
  TH1F* h_L1TMtracks_2StubsME1_pt;
  TH1F* h_L1TMtracks_2StubsME1_eta;
  TH1F* h_L1TMtracks_2StubsME1_phi;

  TH1F* h_L1CSCTrack_2Stubs20_pt;
  TH1F* h_L1CSCTrack_2Stubs20_eta;
  TH1F* h_L1CSCTrack_2Stubs20_phi;
  TH1F* h_L1CSCTrack_2Stubs20ME1_pt;
  TH1F* h_L1CSCTrack_2Stubs20ME1_eta;
  TH1F* h_L1CSCTrack_2Stubs20ME1_phi;


};
//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
L1TUpgradeAnalyser::L1TUpgradeAnalyser(const edm::ParameterSet& iConfig)
{
  //now do what ever initialization is needed
  //  runSRLUTs = new csctf_analysis::RunSRLUTs();
  lctsTag= iConfig.getParameter<edm::InputTag>("lctsTag");
  vertexColTag= iConfig.getParameter<edm::InputTag>("vertexColTag");
  outTreeFileName= iConfig.getUntrackedParameter<string>("outTreeFileName");
  haveRECO = iConfig.getUntrackedParameter<bool>("haveRECO");
  singleSectorNum = iConfig.getUntrackedParameter<int>("singleSectorNum");
}
L1TUpgradeAnalyser::~L1TUpgradeAnalyser()
{
 
  // do anything here that needs to be done at desctruction time
  // (e.g. close files, deallocate resources etc.)
  //  delete runSRLUTs;
}
//
// member functions
// ------------ method called to for each event  ------------
void
L1TUpgradeAnalyser::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  ///////////////////
  //Setup Stuff//////
  ///////////////////
  //  edm::RefProd<CSCTrackCollection> csctfProd = ev.getRefBeforePut<CSCTrackCollection>("input");  
  const float ptscale[33] = { 
    -1.,   0.0,   1.5,   2.0,   2.5,   3.0,   3.5,   4.0,
    4.5,   5.0,   6.0,   7.0,   8.0,  10.0,  12.0,  14.0,  
    16.0,  18.0,  20.0,  25.0,  30.0,  35.0,  40.0,  45.0, 
    50.0,  60.0,  70.0,  80.0,  90.0, 100.0, 120.0, 140.0, 1.E6 };

  edm::Handle<L1CSCTrackCollection> l1csctracks;
  iEvent.getByLabel("simCsctfTrackDigis",l1csctracks);
  edm::Handle<TriggerPrimitiveCollection> trigPrims;
  iEvent.getByLabel("L1TMuonTriggerPrimitives",trigPrims);
  edm::Handle<CSCCorrelatedLCTDigiCollection> lcts;
  iEvent.getByLabel("simCscTriggerPrimitiveDigis","MPCSORTED", lcts);

  edm::Handle<InternalTrackCollection> L1TMtracks;
  iEvent.getByLabel( "L1TMuonText" , "DataITC"  , L1TMtracks );

  edm::Handle<edm::SimTrackContainer> BaseSimTracks;
  iEvent.getByLabel("g4SimHits",BaseSimTracks);
  edm::SimTrackContainer::const_iterator BaseSimTrk;

  float min_pt = 2;
  float max_pt = 100;
  float min_aEta = 0.9;
  float max_aEta = 2.4;
  //float max_aEta = 1.8;
  float etaFwd = 0;
  float etaBwd = 0;
  float ptFwd = 0;
  float ptBwd = 0;
  float phiFwd = 0;
  float phiBwd = 0;

  //  cout << "BaseSimTracks->size() = "<< BaseSimTracks->size() << endl;
  int nSimMu = 0;
  for(BaseSimTrk=BaseSimTracks->begin(); BaseSimTrk != BaseSimTracks->end(); BaseSimTrk++){
    if ((fabs(BaseSimTrk->type()) == 13) and
	(BaseSimTrk->momentum().pt() >= min_pt) and
	(BaseSimTrk->momentum().pt() <= max_pt) and
	(fabs(BaseSimTrk->momentum().eta()) >= min_aEta) and 
	(fabs(BaseSimTrk->momentum().eta()) <= max_aEta) ){
      // cout << "BaseSimTrk = "<< BaseSimTrk->type()
      // 	   << " pt " << BaseSimTrk->momentum().pt()
      // 	   << " eta " << BaseSimTrk->momentum().eta()
      // 	   << endl;
      nSimMu++;
      if (BaseSimTrk->momentum().eta() > 0){
	ptFwd = BaseSimTrk->momentum().pt();
	etaFwd = BaseSimTrk->momentum().eta();
	phiFwd = BaseSimTrk->momentum().phi();
      }
      if (BaseSimTrk->momentum().eta() < 0){
	ptBwd = BaseSimTrk->momentum().pt();
	etaBwd = BaseSimTrk->momentum().eta();
	phiBwd = BaseSimTrk->momentum().phi();
      }
    }
  }
  if (nSimMu > 2){
    cout << "nSimMu = "<< nSimMu << endl;
    for(BaseSimTrk=BaseSimTracks->begin(); BaseSimTrk != BaseSimTracks->end(); BaseSimTrk++){
      if ((fabs(BaseSimTrk->type()) == 13) ){
	std::cout << "BaseSimTrk = "<< BaseSimTrk->type()
		  << " pt " << BaseSimTrk->momentum().pt()
		  << " eta " << BaseSimTrk->momentum().eta()
		  << std::endl;
	// if (!BaseSimTrk->noVertex())
	//   std::cout << "BaseSimTrk = "<< BaseSimTrk->type()
	// 	    << " pos( " << BaseSimTrk->trackerSurfaceMomentum().x()
	// 	    << ", " << BaseSimTrk->trackerSurfaceMomentum().y()
	// 	    << ", " << BaseSimTrk->trackerSurfaceMomentum().z()
	// 	    << ") " 
	// 	    << std::endl;
      }
    }
  }
  if (ptFwd == 0) 
    return;

  //using abs
  etaFwd = fabs(etaFwd);
  etaBwd = fabs(etaBwd);
  h_truth_pt->Fill(ptFwd);
  h_truth_pt->Fill(ptBwd);
  h_truth_eta->Fill(etaFwd);
  h_truth_eta->Fill(etaBwd);
  h_truth_phi->Fill(phiFwd);
  h_truth_phi->Fill(phiBwd);

  if (ptFwd > 30){
    h_truth_30_eta->Fill(etaFwd);
    h_truth_30_phi->Fill(phiFwd);
  }
  if (ptBwd > 30){
    h_truth_30_eta->Fill(etaBwd);
    h_truth_30_phi->Fill(phiBwd);
  }
  h_truth_phi_fwd->Fill(phiFwd);
  h_truth_phi_bwd->Fill(phiBwd);

  bool hasTrkFwd = false;
  bool hasTrkBwd = false;
  bool hasTrkFwd20 = false;
  bool hasTrkBwd20 = false;
  bool hasQ3TrkFwd = false;
  bool hasQ3TrkBwd = false;
  bool hasTrkFwdME1 = false;
  bool hasTrkBwdME1 = false;
  int nTrkStubsFwd = 0;
  int nTrkStubsBwd = 0;

  // lcts used in tracks
  L1CSCTrackCollection::const_iterator tmp_trk = l1csctracks->begin();
  for(; tmp_trk != l1csctracks->end(); tmp_trk++){
    auto l1track = tmp_trk->first;
    unsigned int quality_packed;
    unsigned int rank=l1track.rank();
    unsigned int pt_packed;
    unsigned m_ptAddress = l1track.ptLUTAddress();
    l1track.decodeRank(rank,pt_packed,quality_packed); //get the pt and gaulity packed
    // cout << "l1track.decodeRank = "
    //  	 << ", rank " << rank
    //  	 << ", pt_packed " << pt_packed
    //  	 << ", quality_packed " << quality_packed
    //  	 << endl;
    int nstubs=0;
    bool hasGEM = false;
    float GEMdPhi = -99;
    for(CSCCorrelatedLCTDigiCollection::DigiRangeIterator csc=tmp_trk->second.begin(); csc!=tmp_trk->second.end(); csc++){
      auto lctdigi = (*csc).second.first;
      h_GEMDPhi->Fill((*csc).second.first->getGEMDPhi());

      if ((*csc).first.endcap() == 1){
	h_nStation->Fill((*csc).first.station());
	hasTrkFwd = true;
	nTrkStubsFwd++;
	if (quality_packed >= 3) hasQ3TrkFwd = true;
	if ((*csc).first.station() == 1) hasTrkFwdME1 = true;
	if (ptscale[pt_packed] >= 20) hasTrkFwd20 = true;
      }
      if ((*csc).first.endcap() == 2){
	h_nStation->Fill(-((*csc).first.station()));
	hasTrkBwd = true;
	nTrkStubsBwd++;
	if (quality_packed >= 3) hasQ3TrkBwd = true;
	if ((*csc).first.station() == 1) hasTrkBwdME1 = true;
	if (ptscale[pt_packed] >= 20) hasTrkBwd20 = true;
      }

      if ((*csc).first.station()==1 and l1track.me1ID() == 0)
	cout << "(*csc).first.station() " << (*csc).first.station() 
	     << " l1track.me1ID() "<< l1track.me1ID() <<endl;
      if ((*csc).first.station()==2 and l1track.me2ID() == 0) 
	cout << "(*csc).first.station() " << (*csc).first.station() 
	     << " l1track.me2ID() "<< l1track.me2ID() <<endl;
      if ((*csc).first.station()==3 and l1track.me3ID() == 0)
	cout << "(*csc).first.station() " << (*csc).first.station() 
	     << " l1track.me3ID() "<< l1track.me3ID() <<endl;
      if ((*csc).first.station()==4 and l1track.me4ID() == 0)
	cout << "(*csc).first.station() " << (*csc).first.station() 
	     << " l1track.me4ID() "<< l1track.me4ID() <<endl;

      // if ((*csc).first.station() == 1){
      // 	// cout << "Station = " << (*csc).first.station()
      // 	//      << ", getGEMDPhi " << lctdigi->getGEMDPhi()
      // 	//      << ", getQuality " << lctdigi->getQuality()
      // 	//      << ", getKeyWG " << lctdigi->getKeyWG()
      // 	//      << ", getStrip " << lctdigi->getStrip()
      // 	//      << ", getPattern " << lctdigi->getPattern()
      // 	//      << ", getBend " << lctdigi->getBend()
      // 	//      << ", getCLCTPattern " << lctdigi->getCLCTPattern()
      // 	//      << ", getMPCLink " << lctdigi->getMPCLink()
      // 	//      << endl;
      // }
      nstubs++;
      if ((*csc).second.first->getGEMDPhi() > -99){
	hasGEM = true;
	GEMdPhi = (*csc).second.first->getGEMDPhi();
	h_TFStubQualityGEM->Fill(lctdigi->getQuality());
      }
      h_TFStubQuality->Fill(lctdigi->getQuality());
    }

    // cout << "m_ptAddress = " << m_ptAddress
    // 	 << ", deltaPhi12 " << l1track.deltaPhi12()
    // 	 << ", deltaPhi23 " << l1track.deltaPhi23()
    // 	 << ", addressEta " << l1track.addressEta()
    // 	 << ", mode " << l1track.mode()
    // 	 << ", sign " << l1track.sign()
    // 	 << endl;
    // cout << "nstubs = " << nstubs
    // 	 << ", me1Tbin = " << l1track.me1Tbin()
    // 	 << ", me2Tbin = " << l1track.me2Tbin()
    // 	 << ", me3Tbin = " << l1track.me3Tbin()
    // 	 << ", me4Tbin = " << l1track.me4Tbin()
    // 	 << ", mb1Tbin = " << l1track.mb1Tbin()
    // 	 << endl;

    h_TFnStubinTrack->Fill(nstubs);
    h_TFnStubvsPTAdd->Fill(nstubs,m_ptAddress);
    h_TFnStubvsQualityPacked->Fill(nstubs,quality_packed);
    h_TFPTpackedvsPTAdd->Fill(pt_packed,m_ptAddress);
    h_TFPTpackedvsdPhi12->Fill(pt_packed,l1track.deltaPhi12());
    h_TFPTpackedvsdPhi23->Fill(pt_packed,l1track.deltaPhi23());
    h_TFPTAddvsdPhi12->Fill(m_ptAddress,l1track.deltaPhi12());
    h_TFPTAddvsdPhi23->Fill(m_ptAddress,l1track.deltaPhi23());

    if (hasGEM){
      h_TFnStubinTrackGEM->Fill(nstubs);
      h_TFnStubvsPTAddGEM->Fill(nstubs,m_ptAddress);
      h_TFnStubvsQualityPackedGEM->Fill(nstubs,quality_packed);
      h_TFPTpackedvsPTAddGEM->Fill(pt_packed,m_ptAddress);
      h_TFPTpackedvsdPhi12GEM->Fill(pt_packed,l1track.deltaPhi12());
      h_TFPTpackedvsdPhi23GEM->Fill(pt_packed,l1track.deltaPhi23());
      h_TFPTpackedvsGEMdPhi->Fill(pt_packed,GEMdPhi);
    }
  }

  // all lcts
  int nFwdStubs = 0;
  int nBwdStubs = 0;
  bool hasFwdGE11 = false;
  bool hasBwdGE11 = false;

  CSCCorrelatedLCTDigiCollection::DigiRangeIterator Citer;
  for(Citer = lcts->begin(); Citer != lcts->end(); Citer++){
    CSCCorrelatedLCTDigiCollection::const_iterator Diter = (*Citer).second.first;
    CSCCorrelatedLCTDigiCollection::const_iterator Dend = (*Citer).second.second;
    for(; Diter != Dend; Diter++){
      if ((*Citer).first.endcap() == 1){
	nFwdStubs++;
	if (Diter->getGEMDPhi() > -99) hasFwdGE11 = true;
      }
      if ((*Citer).first.endcap() == 2){
	nBwdStubs++;
	if (Diter->getGEMDPhi() > -99) hasBwdGE11 = true;
      }
      if (Diter->getGEMDPhi() > -99){
	h_StubQualityGEM->Fill(Diter->getQuality());
      }
      h_StubQuality->Fill(Diter->getQuality());
    }
  }
  if (!hasTrkFwd){
    h_noTFnStub->Fill(nFwdStubs);
    if (hasFwdGE11)
      h_noTFnStubGEM->Fill(nFwdStubs);
  }
  if (!hasTrkBwd){
    h_noTFnStub->Fill(nBwdStubs);
    if (hasBwdGE11)
      h_noTFnStubGEM->Fill(nBwdStubs);
  }
  if (!hasQ3TrkFwd){
    h_noTFQ3nStub->Fill(nFwdStubs);
    if (hasFwdGE11)
      h_noTFQ3nStubGEM->Fill(nFwdStubs);
  }
  if (!hasQ3TrkBwd){
    h_noTFQ3nStub->Fill(nBwdStubs);
    if (hasBwdGE11)
      h_noTFQ3nStubGEM->Fill(nBwdStubs);
  }
  if (hasTrkFwd){
    h_L1CSCTrack__pt->Fill(ptFwd);
    h_L1CSCTrack__eta->Fill(etaFwd);
    h_L1CSCTrack__phi->Fill(phiFwd);
  }
  if (hasTrkBwd){
    h_L1CSCTrack__pt->Fill(ptBwd);
    h_L1CSCTrack__eta->Fill(etaBwd);
    h_L1CSCTrack__phi->Fill(phiBwd);
  }
  if (hasTrkFwdME1){
    h_L1CSCTrack_ME1_pt->Fill(ptFwd);
    h_L1CSCTrack_ME1_eta->Fill(etaFwd);
    h_L1CSCTrack_ME1_phi->Fill(phiFwd);
  }
  if (hasTrkBwdME1){
    h_L1CSCTrack_ME1_pt->Fill(ptBwd);
    h_L1CSCTrack_ME1_eta->Fill(etaBwd);
    h_L1CSCTrack_ME1_phi->Fill(phiBwd);
  }
  if (nTrkStubsFwd>2){
    h_L1CSCTrack_3Stubs_pt->Fill(ptFwd);
    h_L1CSCTrack_3Stubs_eta->Fill(etaFwd);
    h_L1CSCTrack_3Stubs_phi->Fill(phiFwd);
  }
  if (nTrkStubsBwd>2){
    h_L1CSCTrack_3Stubs_pt->Fill(ptBwd);
    h_L1CSCTrack_3Stubs_eta->Fill(etaBwd);
    h_L1CSCTrack_3Stubs_phi->Fill(phiBwd);
  }
  if (hasTrkFwdME1 && nTrkStubsFwd>2){
    h_L1CSCTrack_3StubsME1_pt->Fill(ptFwd);
    h_L1CSCTrack_3StubsME1_eta->Fill(etaFwd);
    h_L1CSCTrack_3StubsME1_phi->Fill(phiFwd);
  }
  if (hasTrkBwdME1 && nTrkStubsBwd>2){
    h_L1CSCTrack_3StubsME1_pt->Fill(ptBwd);
    h_L1CSCTrack_3StubsME1_eta->Fill(etaBwd);
    h_L1CSCTrack_3StubsME1_phi->Fill(phiBwd);
  }
  if (nTrkStubsFwd>1){
    h_L1CSCTrack_2Stubs_pt->Fill(ptFwd);
    h_L1CSCTrack_2Stubs_eta->Fill(etaFwd);
    h_L1CSCTrack_2Stubs_phi->Fill(phiFwd);
  }
  if (nTrkStubsBwd>1){
    h_L1CSCTrack_2Stubs_pt->Fill(ptBwd);
    h_L1CSCTrack_2Stubs_eta->Fill(etaBwd);
    h_L1CSCTrack_2Stubs_phi->Fill(phiBwd);
  }
  if (hasTrkFwdME1 && nTrkStubsFwd>1){
    h_L1CSCTrack_2StubsME1_pt->Fill(ptFwd);
    h_L1CSCTrack_2StubsME1_eta->Fill(etaFwd);
    h_L1CSCTrack_2StubsME1_phi->Fill(phiFwd);
  }
  if (hasTrkBwdME1 && nTrkStubsBwd>1){
    h_L1CSCTrack_2StubsME1_pt->Fill(ptBwd);
    h_L1CSCTrack_2StubsME1_eta->Fill(etaBwd);
    h_L1CSCTrack_2StubsME1_phi->Fill(phiBwd);
  }

  if (hasQ3TrkFwd){
    h_L1CSCTrack_Q3_pt->Fill(ptFwd);
    h_L1CSCTrack_Q3_eta->Fill(etaFwd);
    h_L1CSCTrack_Q3_phi->Fill(phiFwd);
    h_L1CSCTrack_Q3_phi_fwd->Fill(phiFwd);
  }
  if (hasQ3TrkBwd){
    h_L1CSCTrack_Q3_pt->Fill(ptBwd);
    h_L1CSCTrack_Q3_eta->Fill(etaBwd);
    h_L1CSCTrack_Q3_phi->Fill(phiBwd);
    h_L1CSCTrack_Q3_phi_bwd->Fill(phiBwd);
  }
  if (hasTrkFwd20){
    h_L1CSCTrack_20_pt->Fill(ptFwd);
    if (hasTrkFwdME1) h_L1CSCTrack_20ME1_pt->Fill(ptFwd);
    if (ptFwd > 30){
      h_L1CSCTrack_20_eta->Fill(etaFwd);
      h_L1CSCTrack_20_phi->Fill(phiFwd);
      if (hasTrkFwdME1) h_L1CSCTrack_20ME1_eta->Fill(etaFwd);
      if (hasTrkFwdME1) h_L1CSCTrack_20ME1_phi->Fill(phiFwd);
    }
    if (nTrkStubsFwd>2){
      h_L1CSCTrack_3Stubs20_pt->Fill(ptFwd);
      if (hasTrkFwdME1) h_L1CSCTrack_3Stubs20ME1_pt->Fill(ptFwd);
      if (ptFwd > 30){
	h_L1CSCTrack_3Stubs20_eta->Fill(etaFwd);
	h_L1CSCTrack_3Stubs20_phi->Fill(phiFwd);
	if (hasTrkFwdME1) h_L1CSCTrack_3Stubs20ME1_eta->Fill(etaFwd);
	if (hasTrkFwdME1) h_L1CSCTrack_3Stubs20ME1_phi->Fill(phiFwd);
      }
    }
    if (nTrkStubsFwd>1){
      h_L1CSCTrack_2Stubs20_pt->Fill(ptFwd);
      if (hasTrkFwdME1) h_L1CSCTrack_2Stubs20ME1_pt->Fill(ptFwd);
      if (ptFwd > 30){
	h_L1CSCTrack_2Stubs20_eta->Fill(etaFwd);
	h_L1CSCTrack_2Stubs20_phi->Fill(phiFwd);
	if (hasTrkFwdME1) h_L1CSCTrack_2Stubs20ME1_eta->Fill(etaFwd);
	if (hasTrkFwdME1) h_L1CSCTrack_2Stubs20ME1_phi->Fill(phiFwd);
      }
    }
  }

  if (hasTrkBwd20){
    h_L1CSCTrack_20_pt->Fill(ptBwd);
    if (hasTrkBwdME1) h_L1CSCTrack_20ME1_pt->Fill(ptBwd);
    if (ptBwd > 30){
      h_L1CSCTrack_20_eta->Fill(etaBwd);
      h_L1CSCTrack_20_phi->Fill(phiBwd);
      if (hasTrkBwdME1) h_L1CSCTrack_20ME1_eta->Fill(etaBwd);
      if (hasTrkBwdME1) h_L1CSCTrack_20ME1_phi->Fill(phiBwd);
    }
    if (nTrkStubsBwd>2){
      h_L1CSCTrack_3Stubs20_pt->Fill(ptBwd);
      if (hasTrkBwdME1) h_L1CSCTrack_3Stubs20ME1_pt->Fill(ptBwd);
      if (ptBwd > 30){
	h_L1CSCTrack_3Stubs20_eta->Fill(etaBwd);
	h_L1CSCTrack_3Stubs20_phi->Fill(phiBwd);
	if (hasTrkBwdME1) h_L1CSCTrack_3Stubs20ME1_eta->Fill(etaBwd);
	if (hasTrkBwdME1) h_L1CSCTrack_3Stubs20ME1_phi->Fill(phiBwd);
      }
    }
    if (nTrkStubsBwd>1){
      h_L1CSCTrack_2Stubs20_pt->Fill(ptBwd);
      if (hasTrkBwdME1) h_L1CSCTrack_2Stubs20ME1_pt->Fill(ptBwd);
      if (ptBwd > 30){
	h_L1CSCTrack_2Stubs20_eta->Fill(etaBwd);
	h_L1CSCTrack_2Stubs20_phi->Fill(phiBwd);
	if (hasTrkBwdME1) h_L1CSCTrack_2Stubs20ME1_eta->Fill(etaBwd);
	if (hasTrkBwdME1) h_L1CSCTrack_2Stubs20ME1_phi->Fill(phiBwd);
      }
    }
  }

  bool hasL1TrkFwd = false;
  bool hasL1TrkBwd = false;
  bool hasL1TrkFwdME1 = false;
  bool hasL1TrkBwdME1 = false;
  int nL1TrkStubsFwd = 0;
  int nL1TrkStubsBwd = 0;
  /// upgrade L1T muon track finder
  auto itData    = L1TMtracks->cbegin();
  auto itendData = L1TMtracks->cend(); 
  for( ; itData != itendData ; ++itData){      
    TriggerPrimitiveStationMap tpsmData = itData->getStubs();
    // Getting the unique station ID number for ME1
    const unsigned id = 4*L1TMuon::InternalTrack::kCSC;
    // Looping over all four stations
    for(unsigned meNum=id; meNum<(id+4); meNum++){
      // Getting the trig prim lists for this station
      TriggerPrimitiveList tplData = tpsmData[meNum];
      //      cout << "ME " << meNum-id+1 << " -  # Trig Prims = " << tplData.size() << endl;
      for(unsigned tpNum = 0; tpNum < tplData.size() ; tpNum++){
	//	cout << " ----- tp #" << tpNum << endl; 
	// Creating references to the trig prim info
	TriggerPrimitiveRef tprData = tplData.at(tpNum);
	if ((*tprData).getCMSGlobalEta() > 0){
	  hasL1TrkFwd = true;
	  nL1TrkStubsFwd++;
	  if ((meNum-id+1) == 1) hasL1TrkFwdME1 = true;
	}
	if ((*tprData).getCMSGlobalEta() < 0){
	  hasL1TrkBwd = true;
	  nL1TrkStubsBwd++;
	  if ((meNum-id+1) == 1) hasL1TrkBwdME1 = true;
	}
	
      }
    }
  }
  if (hasL1TrkFwd){
    h_L1TMtracks__pt->Fill(ptFwd);
    h_L1TMtracks__eta->Fill(etaFwd);
    h_L1TMtracks__phi->Fill(phiFwd);
  }
  if (hasL1TrkBwd){
    h_L1TMtracks__pt->Fill(ptBwd);
    h_L1TMtracks__eta->Fill(etaBwd);
    h_L1TMtracks__phi->Fill(phiBwd);
  }
  if (hasL1TrkFwdME1){
    h_L1TMtracks_ME1_pt->Fill(ptFwd);
    h_L1TMtracks_ME1_eta->Fill(etaFwd);
    h_L1TMtracks_ME1_phi->Fill(phiFwd);
  }
  if (hasL1TrkBwdME1){
    h_L1TMtracks_ME1_pt->Fill(ptBwd);
    h_L1TMtracks_ME1_eta->Fill(etaBwd);
    h_L1TMtracks_ME1_phi->Fill(phiBwd);
  }
  if (nL1TrkStubsFwd>2){
    h_L1TMtracks_3Stubs_pt->Fill(ptFwd);
    h_L1TMtracks_3Stubs_eta->Fill(etaFwd);
    h_L1TMtracks_3Stubs_phi->Fill(phiFwd);
  }
  if (nL1TrkStubsBwd>2){
    h_L1TMtracks_3Stubs_pt->Fill(ptBwd);
    h_L1TMtracks_3Stubs_eta->Fill(etaBwd);
    h_L1TMtracks_3Stubs_phi->Fill(phiBwd);
  }
  if (hasL1TrkFwdME1 && nL1TrkStubsFwd>2){
    h_L1TMtracks_3StubsME1_pt->Fill(ptFwd);
    h_L1TMtracks_3StubsME1_eta->Fill(etaFwd);
    h_L1TMtracks_3StubsME1_phi->Fill(phiFwd);
  }
  if (hasL1TrkBwdME1 && nL1TrkStubsBwd>2){
    h_L1TMtracks_3StubsME1_pt->Fill(ptBwd);
    h_L1TMtracks_3StubsME1_eta->Fill(etaBwd);
    h_L1TMtracks_3StubsME1_phi->Fill(phiBwd);
  }

  if (nL1TrkStubsFwd>1){
    h_L1TMtracks_2Stubs_pt->Fill(ptFwd);
    h_L1TMtracks_2Stubs_eta->Fill(etaFwd);
    h_L1TMtracks_2Stubs_phi->Fill(phiFwd);
  }
  if (nL1TrkStubsBwd>1){
    h_L1TMtracks_2Stubs_pt->Fill(ptBwd);
    h_L1TMtracks_2Stubs_eta->Fill(etaBwd);
    h_L1TMtracks_2Stubs_phi->Fill(phiBwd);
  }
  if (hasL1TrkFwdME1 && nL1TrkStubsFwd>1){
    h_L1TMtracks_2StubsME1_pt->Fill(ptFwd);
    h_L1TMtracks_2StubsME1_eta->Fill(etaFwd);
    h_L1TMtracks_2StubsME1_phi->Fill(phiFwd);
  }
  if (hasL1TrkBwdME1 && nL1TrkStubsBwd>1){
    h_L1TMtracks_2StubsME1_pt->Fill(ptBwd);
    h_L1TMtracks_2StubsME1_eta->Fill(etaBwd);
    h_L1TMtracks_2StubsME1_phi->Fill(phiBwd);
  }

}


// ------------ method called once each job just before starting event loop  ------------
void 
L1TUpgradeAnalyser::beginJob()
{
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

  h_nStation=fs->make<TH1F>("nStation","stations in track",11,-5,6);
  h_nStation->GetXaxis()->SetTitle("Station number");
  h_nStation->GetYaxis()->SetTitle("Counts");

  h_truth_pt=fs->make<TH1F>("truth_pt","",20,0,50);
  h_truth_pt->GetXaxis()->SetTitle("sim muon p_T [GeV]");
  h_truth_pt->GetYaxis()->SetTitle("Efficiency");
  h_truth_eta=fs->make<TH1F>("truth_eta","",35,1.5,2.2);
  h_truth_eta->GetXaxis()->SetTitle("sim muon #eta");
  h_truth_eta->GetYaxis()->SetTitle("Efficiency");
  h_truth_phi=fs->make<TH1F>("truth_phi","",70,-3.5,3.5);
  h_truth_phi->GetXaxis()->SetTitle("sim muon #phi");
  h_truth_phi->GetYaxis()->SetTitle("Efficiency");

  h_truth_30_eta=fs->make<TH1F>("truth_30_eta","",35,1.5,2.2);
  h_truth_30_eta->GetXaxis()->SetTitle("sim muon #eta");
  h_truth_30_eta->GetYaxis()->SetTitle("Efficiency");
  h_truth_30_phi=fs->make<TH1F>("truth_30_phi","",70,-3.5,3.5);
  h_truth_30_phi->GetXaxis()->SetTitle("sim muon #phi");
  h_truth_30_phi->GetYaxis()->SetTitle("Efficiency");

  h_L1CSCTrack__pt=fs->make<TH1F>("L1CSCTrack__pt","",20,0,50);
  h_L1CSCTrack__pt->GetXaxis()->SetTitle("sim muon p_T [GeV]");
  h_L1CSCTrack__pt->GetYaxis()->SetTitle("Efficiency");
  h_L1CSCTrack__eta=fs->make<TH1F>("L1CSCTrack__eta","",35,1.5,2.2);
  h_L1CSCTrack__eta->GetXaxis()->SetTitle("sim muon #eta");
  h_L1CSCTrack__eta->GetYaxis()->SetTitle("Efficiency");
  h_L1CSCTrack__phi=fs->make<TH1F>("L1CSCTrack__phi","",70,-3.5,3.5);
  h_L1CSCTrack__phi->GetXaxis()->SetTitle("sim muon #phi");
  h_L1CSCTrack__phi->GetYaxis()->SetTitle("Efficiency");

  h_L1TMtracks__pt=fs->make<TH1F>("L1TMtracks__pt","",20,0,50);
  h_L1TMtracks__pt->GetXaxis()->SetTitle("sim muon p_T [GeV]");
  h_L1TMtracks__pt->GetYaxis()->SetTitle("Efficiency");
  h_L1TMtracks__eta=fs->make<TH1F>("L1TMtracks__eta","",35,1.5,2.2);
  h_L1TMtracks__eta->GetXaxis()->SetTitle("sim muon #eta");
  h_L1TMtracks__eta->GetYaxis()->SetTitle("Efficiency");
  h_L1TMtracks__phi=fs->make<TH1F>("L1TMtracks__phi","",70,-3.5,3.5);
  h_L1TMtracks__phi->GetXaxis()->SetTitle("sim muon #phi");
  h_L1TMtracks__phi->GetYaxis()->SetTitle("Efficiency");

  h_L1CSCTrack_Q3_pt=fs->make<TH1F>("L1CSCTrack_Q3_pt","",20,0,50);
  h_L1CSCTrack_Q3_pt->GetXaxis()->SetTitle("sim muon p_T [GeV]");
  h_L1CSCTrack_Q3_pt->GetYaxis()->SetTitle("Efficiency");
  h_L1CSCTrack_Q3_eta=fs->make<TH1F>("L1CSCTrack_Q3_eta","",35,1.5,2.2);
  h_L1CSCTrack_Q3_eta->GetXaxis()->SetTitle("sim muon #eta");
  h_L1CSCTrack_Q3_eta->GetYaxis()->SetTitle("Efficiency");
  h_L1CSCTrack_Q3_phi=fs->make<TH1F>("L1CSCTrack_Q3_phi","",70,-3.5,3.5);
  h_L1CSCTrack_Q3_phi->GetXaxis()->SetTitle("sim muon #phi");
  h_L1CSCTrack_Q3_phi->GetYaxis()->SetTitle("Efficiency");

  h_L1CSCTrack_20_pt=fs->make<TH1F>("L1CSCTrack_20_pt","",20,0,50);
  h_L1CSCTrack_20_pt->GetXaxis()->SetTitle("sim muon p_T [GeV]");
  h_L1CSCTrack_20_pt->GetYaxis()->SetTitle("Efficiency");
  h_L1CSCTrack_20_eta=fs->make<TH1F>("L1CSCTrack_20_eta","",35,1.5,2.2);
  h_L1CSCTrack_20_eta->GetXaxis()->SetTitle("sim muon #eta");
  h_L1CSCTrack_20_eta->GetYaxis()->SetTitle("Efficiency");
  h_L1CSCTrack_20_phi=fs->make<TH1F>("L1CSCTrack_20_phi","",70,-3.5,3.5);
  h_L1CSCTrack_20_phi->GetXaxis()->SetTitle("sim muon #phi");
  h_L1CSCTrack_20_phi->GetYaxis()->SetTitle("Efficiency");

  h_L1CSCTrack_20ME1_pt=fs->make<TH1F>("L1CSCTrack_20ME1_pt","",20,0,50);
  h_L1CSCTrack_20ME1_pt->GetXaxis()->SetTitle("sim muon p_T [GeV]");
  h_L1CSCTrack_20ME1_pt->GetYaxis()->SetTitle("Efficiency");
  h_L1CSCTrack_20ME1_eta=fs->make<TH1F>("L1CSCTrack_20ME1_eta","",35,1.5,2.2);
  h_L1CSCTrack_20ME1_eta->GetXaxis()->SetTitle("sim muon #eta");
  h_L1CSCTrack_20ME1_eta->GetYaxis()->SetTitle("Efficiency");
  h_L1CSCTrack_20ME1_phi=fs->make<TH1F>("L1CSCTrack_20ME1_phi","",70,-3.5,3.5);
  h_L1CSCTrack_20ME1_phi->GetXaxis()->SetTitle("sim muon #phi");
  h_L1CSCTrack_20ME1_phi->GetYaxis()->SetTitle("Efficiency");

  h_truth_phi_fwd=fs->make<TH1F>("truth_phi_fwd","",70,-3.5,3.5);
  h_truth_phi_fwd->GetXaxis()->SetTitle("sim muon #phi");
  h_truth_phi_fwd->GetYaxis()->SetTitle("Efficiency");

  h_truth_phi_bwd=fs->make<TH1F>("truth_phi_bwd","",70,-3.5,3.5);
  h_truth_phi_bwd->GetXaxis()->SetTitle("sim muon #phi");
  h_truth_phi_bwd->GetYaxis()->SetTitle("Efficiency");

  h_L1CSCTrack_Q3_phi_fwd=fs->make<TH1F>("L1CSCTrack_Q3_phi_fwd","",70,-3.5,3.5);
  h_L1CSCTrack_Q3_phi_fwd->GetXaxis()->SetTitle("sim muon #phi");
  h_L1CSCTrack_Q3_phi_fwd->GetYaxis()->SetTitle("Efficiency");
  h_L1CSCTrack_Q3_phi_bwd=fs->make<TH1F>("L1CSCTrack_Q3_phi_bwd","",70,-3.5,3.5);
  h_L1CSCTrack_Q3_phi_bwd->GetXaxis()->SetTitle("sim muon #phi");
  h_L1CSCTrack_Q3_phi_bwd->GetYaxis()->SetTitle("Efficiency");

  h_L1CSCTrack_ME1_pt=fs->make<TH1F>("L1CSCTrack_ME1_pt","",20,0,50);
  h_L1CSCTrack_ME1_pt->GetXaxis()->SetTitle("sim muon p_T [GeV]");
  h_L1CSCTrack_ME1_pt->GetYaxis()->SetTitle("Efficiency");
  h_L1CSCTrack_ME1_eta=fs->make<TH1F>("L1CSCTrack_ME1_eta","",35,1.5,2.2);
  h_L1CSCTrack_ME1_eta->GetXaxis()->SetTitle("sim muon #eta");
  h_L1CSCTrack_ME1_eta->GetYaxis()->SetTitle("Efficiency");
  h_L1CSCTrack_ME1_phi=fs->make<TH1F>("L1CSCTrack_ME1_phi","",70,-3.5,3.5);
  h_L1CSCTrack_ME1_phi->GetXaxis()->SetTitle("sim muon #phi");
  h_L1CSCTrack_ME1_phi->GetYaxis()->SetTitle("Efficiency");

  h_L1TMtracks_ME1_pt=fs->make<TH1F>("L1TMtracks_ME1_pt","",20,0,50);
  h_L1TMtracks_ME1_pt->GetXaxis()->SetTitle("sim muon p_T [GeV]");
  h_L1TMtracks_ME1_pt->GetYaxis()->SetTitle("Efficiency");
  h_L1TMtracks_ME1_eta=fs->make<TH1F>("L1TMtracks_ME1_eta","",35,1.5,2.2);
  h_L1TMtracks_ME1_eta->GetXaxis()->SetTitle("sim muon #eta");
  h_L1TMtracks_ME1_eta->GetYaxis()->SetTitle("Efficiency");
  h_L1TMtracks_ME1_phi=fs->make<TH1F>("L1TMtracks_ME1_phi","",70,-3.5,3.5);
  h_L1TMtracks_ME1_phi->GetXaxis()->SetTitle("sim muon #phi");
  h_L1TMtracks_ME1_phi->GetYaxis()->SetTitle("Efficiency");

  h_L1CSCTrack_3Stubs_pt=fs->make<TH1F>("L1CSCTrack_3Stubs_pt","",20,0,50);
  h_L1CSCTrack_3Stubs_pt->GetXaxis()->SetTitle("sim muon p_T [GeV]");
  h_L1CSCTrack_3Stubs_pt->GetYaxis()->SetTitle("Efficiency");
  h_L1CSCTrack_3Stubs_eta=fs->make<TH1F>("L1CSCTrack_3Stubs_eta","",35,1.5,2.2);
  h_L1CSCTrack_3Stubs_eta->GetXaxis()->SetTitle("sim muon #eta");
  h_L1CSCTrack_3Stubs_eta->GetYaxis()->SetTitle("Efficiency");
  h_L1CSCTrack_3Stubs_phi=fs->make<TH1F>("L1CSCTrack_3Stubs_phi","",70,-3.5,3.5);
  h_L1CSCTrack_3Stubs_phi->GetXaxis()->SetTitle("sim muon #phi");
  h_L1CSCTrack_3Stubs_phi->GetYaxis()->SetTitle("Efficiency");

  h_L1TMtracks_3Stubs_pt=fs->make<TH1F>("L1TMtracks_3Stubs_pt","",20,0,50);
  h_L1TMtracks_3Stubs_pt->GetXaxis()->SetTitle("sim muon p_T [GeV]");
  h_L1TMtracks_3Stubs_pt->GetYaxis()->SetTitle("Efficiency");
  h_L1TMtracks_3Stubs_eta=fs->make<TH1F>("L1TMtracks_3Stubs_eta","",35,1.5,2.2);
  h_L1TMtracks_3Stubs_eta->GetXaxis()->SetTitle("sim muon #eta");
  h_L1TMtracks_3Stubs_eta->GetYaxis()->SetTitle("Efficiency");
  h_L1TMtracks_3Stubs_phi=fs->make<TH1F>("L1TMtracks_3Stubs_phi","",70,-3.5,3.5);
  h_L1TMtracks_3Stubs_phi->GetXaxis()->SetTitle("sim muon #phi");
  h_L1TMtracks_3Stubs_phi->GetYaxis()->SetTitle("Efficiency");

  h_L1CSCTrack_3Stubs20_pt=fs->make<TH1F>("L1CSCTrack_3Stubs20_pt","",20,0,50);
  h_L1CSCTrack_3Stubs20_pt->GetXaxis()->SetTitle("sim muon p_T [GeV]");
  h_L1CSCTrack_3Stubs20_pt->GetYaxis()->SetTitle("Efficiency");
  h_L1CSCTrack_3Stubs20_eta=fs->make<TH1F>("L1CSCTrack_3Stubs20_eta","",35,1.5,2.2);
  h_L1CSCTrack_3Stubs20_eta->GetXaxis()->SetTitle("sim muon #eta");
  h_L1CSCTrack_3Stubs20_eta->GetYaxis()->SetTitle("Efficiency");
  h_L1CSCTrack_3Stubs20_phi=fs->make<TH1F>("L1CSCTrack_3Stubs20_phi","",70,-3.5,3.5);
  h_L1CSCTrack_3Stubs20_phi->GetXaxis()->SetTitle("sim muon #phi");
  h_L1CSCTrack_3Stubs20_phi->GetYaxis()->SetTitle("Efficiency");

  h_L1CSCTrack_3StubsME1_pt=fs->make<TH1F>("L1CSCTrack_3StubsME1_pt","",20,0,50);
  h_L1CSCTrack_3StubsME1_pt->GetXaxis()->SetTitle("sim muon p_T [GeV]");
  h_L1CSCTrack_3StubsME1_pt->GetYaxis()->SetTitle("Efficiency");
  h_L1CSCTrack_3StubsME1_eta=fs->make<TH1F>("L1CSCTrack_3StubsME1_eta","",35,1.5,2.2);
  h_L1CSCTrack_3StubsME1_eta->GetXaxis()->SetTitle("sim muon #eta");
  h_L1CSCTrack_3StubsME1_eta->GetYaxis()->SetTitle("Efficiency");
  h_L1CSCTrack_3StubsME1_phi=fs->make<TH1F>("L1CSCTrack_3StubsME1_phi","",70,-3.5,3.5);
  h_L1CSCTrack_3StubsME1_phi->GetXaxis()->SetTitle("sim muon #phi");
  h_L1CSCTrack_3StubsME1_phi->GetYaxis()->SetTitle("Efficiency");

  h_L1CSCTrack_3Stubs20ME1_pt=fs->make<TH1F>("L1CSCTrack_3Stubs20ME1_pt","",20,0,50);
  h_L1CSCTrack_3Stubs20ME1_pt->GetXaxis()->SetTitle("sim muon p_T [GeV]");
  h_L1CSCTrack_3Stubs20ME1_pt->GetYaxis()->SetTitle("Efficiency");
  h_L1CSCTrack_3Stubs20ME1_eta=fs->make<TH1F>("L1CSCTrack_3Stubs20ME1_eta","",35,1.5,2.2);
  h_L1CSCTrack_3Stubs20ME1_eta->GetXaxis()->SetTitle("sim muon #eta");
  h_L1CSCTrack_3Stubs20ME1_eta->GetYaxis()->SetTitle("Efficiency");
  h_L1CSCTrack_3Stubs20ME1_phi=fs->make<TH1F>("L1CSCTrack_3Stubs20ME1_phi","",70,-3.5,3.5);
  h_L1CSCTrack_3Stubs20ME1_phi->GetXaxis()->SetTitle("sim muon #phi");
  h_L1CSCTrack_3Stubs20ME1_phi->GetYaxis()->SetTitle("Efficiency");

  h_L1TMtracks_3StubsME1_pt=fs->make<TH1F>("L1TMtracks_3StubsME1_pt","",20,0,50);
  h_L1TMtracks_3StubsME1_pt->GetXaxis()->SetTitle("sim muon p_T [GeV]");
  h_L1TMtracks_3StubsME1_pt->GetYaxis()->SetTitle("Efficiency");
  h_L1TMtracks_3StubsME1_eta=fs->make<TH1F>("L1TMtracks_3StubsME1_eta","",35,1.5,2.2);
  h_L1TMtracks_3StubsME1_eta->GetXaxis()->SetTitle("sim muon #eta");
  h_L1TMtracks_3StubsME1_eta->GetYaxis()->SetTitle("Efficiency");
  h_L1TMtracks_3StubsME1_phi=fs->make<TH1F>("L1TMtracks_3StubsME1_phi","",70,-3.5,3.5);
  h_L1TMtracks_3StubsME1_phi->GetXaxis()->SetTitle("sim muon #phi");
  h_L1TMtracks_3StubsME1_phi->GetYaxis()->SetTitle("Efficiency");

  h_L1CSCTrack_2Stubs_pt=fs->make<TH1F>("L1CSCTrack_2Stubs_pt","",20,0,50);
  h_L1CSCTrack_2Stubs_pt->GetXaxis()->SetTitle("sim muon p_T [GeV]");
  h_L1CSCTrack_2Stubs_pt->GetYaxis()->SetTitle("Efficiency");
  h_L1CSCTrack_2Stubs_eta=fs->make<TH1F>("L1CSCTrack_2Stubs_eta","",35,1.5,2.2);
  h_L1CSCTrack_2Stubs_eta->GetXaxis()->SetTitle("sim muon #eta");
  h_L1CSCTrack_2Stubs_eta->GetYaxis()->SetTitle("Efficiency");
  h_L1CSCTrack_2Stubs_phi=fs->make<TH1F>("L1CSCTrack_2Stubs_phi","",70,-3.5,3.5);
  h_L1CSCTrack_2Stubs_phi->GetXaxis()->SetTitle("sim muon #phi");
  h_L1CSCTrack_2Stubs_phi->GetYaxis()->SetTitle("Efficiency");

  h_L1TMtracks_2Stubs_pt=fs->make<TH1F>("L1TMtracks_2Stubs_pt","",20,0,50);
  h_L1TMtracks_2Stubs_pt->GetXaxis()->SetTitle("sim muon p_T [GeV]");
  h_L1TMtracks_2Stubs_pt->GetYaxis()->SetTitle("Efficiency");
  h_L1TMtracks_2Stubs_eta=fs->make<TH1F>("L1TMtracks_2Stubs_eta","",35,1.5,2.2);
  h_L1TMtracks_2Stubs_eta->GetXaxis()->SetTitle("sim muon #eta");
  h_L1TMtracks_2Stubs_eta->GetYaxis()->SetTitle("Efficiency");
  h_L1TMtracks_2Stubs_phi=fs->make<TH1F>("L1TMtracks_2Stubs_phi","",70,-3.5,3.5);
  h_L1TMtracks_2Stubs_phi->GetXaxis()->SetTitle("sim muon #phi");
  h_L1TMtracks_2Stubs_phi->GetYaxis()->SetTitle("Efficiency");

  h_L1CSCTrack_2Stubs20_pt=fs->make<TH1F>("L1CSCTrack_2Stubs20_pt","",20,0,50);
  h_L1CSCTrack_2Stubs20_pt->GetXaxis()->SetTitle("sim muon p_T [GeV]");
  h_L1CSCTrack_2Stubs20_pt->GetYaxis()->SetTitle("Efficiency");
  h_L1CSCTrack_2Stubs20_eta=fs->make<TH1F>("L1CSCTrack_2Stubs20_eta","",35,1.5,2.2);
  h_L1CSCTrack_2Stubs20_eta->GetXaxis()->SetTitle("sim muon #eta");
  h_L1CSCTrack_2Stubs20_eta->GetYaxis()->SetTitle("Efficiency");
  h_L1CSCTrack_2Stubs20_phi=fs->make<TH1F>("L1CSCTrack_2Stubs20_phi","",70,-3.5,3.5);
  h_L1CSCTrack_2Stubs20_phi->GetXaxis()->SetTitle("sim muon #phi");
  h_L1CSCTrack_2Stubs20_phi->GetYaxis()->SetTitle("Efficiency");

  h_L1CSCTrack_2StubsME1_pt=fs->make<TH1F>("L1CSCTrack_2StubsME1_pt","",20,0,50);
  h_L1CSCTrack_2StubsME1_pt->GetXaxis()->SetTitle("sim muon p_T [GeV]");
  h_L1CSCTrack_2StubsME1_pt->GetYaxis()->SetTitle("Efficiency");
  h_L1CSCTrack_2StubsME1_eta=fs->make<TH1F>("L1CSCTrack_2StubsME1_eta","",35,1.5,2.2);
  h_L1CSCTrack_2StubsME1_eta->GetXaxis()->SetTitle("sim muon #eta");
  h_L1CSCTrack_2StubsME1_eta->GetYaxis()->SetTitle("Efficiency");
  h_L1CSCTrack_2StubsME1_phi=fs->make<TH1F>("L1CSCTrack_2StubsME1_phi","",70,-3.5,3.5);
  h_L1CSCTrack_2StubsME1_phi->GetXaxis()->SetTitle("sim muon #phi");
  h_L1CSCTrack_2StubsME1_phi->GetYaxis()->SetTitle("Efficiency");

  h_L1CSCTrack_2Stubs20ME1_pt=fs->make<TH1F>("L1CSCTrack_2Stubs20ME1_pt","",20,0,50);
  h_L1CSCTrack_2Stubs20ME1_pt->GetXaxis()->SetTitle("sim muon p_T [GeV]");
  h_L1CSCTrack_2Stubs20ME1_pt->GetYaxis()->SetTitle("Efficiency");
  h_L1CSCTrack_2Stubs20ME1_eta=fs->make<TH1F>("L1CSCTrack_2Stubs20ME1_eta","",35,1.5,2.2);
  h_L1CSCTrack_2Stubs20ME1_eta->GetXaxis()->SetTitle("sim muon #eta");
  h_L1CSCTrack_2Stubs20ME1_eta->GetYaxis()->SetTitle("Efficiency");
  h_L1CSCTrack_2Stubs20ME1_phi=fs->make<TH1F>("L1CSCTrack_2Stubs20ME1_phi","",70,-3.5,3.5);
  h_L1CSCTrack_2Stubs20ME1_phi->GetXaxis()->SetTitle("sim muon #phi");
  h_L1CSCTrack_2Stubs20ME1_phi->GetYaxis()->SetTitle("Efficiency");

  h_L1TMtracks_2StubsME1_pt=fs->make<TH1F>("L1TMtracks_2StubsME1_pt","",20,0,50);
  h_L1TMtracks_2StubsME1_pt->GetXaxis()->SetTitle("sim muon p_T [GeV]");
  h_L1TMtracks_2StubsME1_pt->GetYaxis()->SetTitle("Efficiency");
  h_L1TMtracks_2StubsME1_eta=fs->make<TH1F>("L1TMtracks_2StubsME1_eta","",35,1.5,2.2);
  h_L1TMtracks_2StubsME1_eta->GetXaxis()->SetTitle("sim muon #eta");
  h_L1TMtracks_2StubsME1_eta->GetYaxis()->SetTitle("Efficiency");
  h_L1TMtracks_2StubsME1_phi=fs->make<TH1F>("L1TMtracks_2StubsME1_phi","",70,-3.5,3.5);
  h_L1TMtracks_2StubsME1_phi->GetXaxis()->SetTitle("sim muon #phi");
  h_L1TMtracks_2StubsME1_phi->GetYaxis()->SetTitle("Efficiency");

}

// ------------ method called once each job just after ending the event loop  ------------
void L1TUpgradeAnalyser::endJob() 
{
  h_truth_pt->Sumw2();
  h_truth_eta->Sumw2();
  h_truth_phi->Sumw2();

  h_L1CSCTrack__pt->Sumw2();
  h_L1CSCTrack__eta->Sumw2();
  h_L1CSCTrack__phi->Sumw2();

  h_L1TMtracks__pt->Sumw2();
  h_L1TMtracks__eta->Sumw2();
  h_L1TMtracks__phi->Sumw2();

  h_L1CSCTrack_Q3_pt->Sumw2();
  h_L1CSCTrack_Q3_eta->Sumw2();
  h_L1CSCTrack_Q3_phi->Sumw2();

  h_L1CSCTrack_20_pt->Sumw2();
  h_L1CSCTrack_20_eta->Sumw2();
  h_L1CSCTrack_20_phi->Sumw2();

  h_L1CSCTrack_20ME1_pt->Sumw2();
  h_L1CSCTrack_20ME1_eta->Sumw2();
  h_L1CSCTrack_20ME1_phi->Sumw2();

  h_truth_phi_fwd->Sumw2();
  h_truth_phi_bwd->Sumw2();
  h_L1CSCTrack_Q3_phi_fwd->Sumw2();
  h_L1CSCTrack_Q3_phi_bwd->Sumw2();

  h_L1CSCTrack_ME1_pt->Sumw2();
  h_L1CSCTrack_ME1_eta->Sumw2();
  h_L1CSCTrack_ME1_phi->Sumw2();

  h_L1TMtracks_ME1_pt->Sumw2();
  h_L1TMtracks_ME1_eta->Sumw2();
  h_L1TMtracks_ME1_phi->Sumw2();

  h_L1CSCTrack_3Stubs_pt->Sumw2();
  h_L1CSCTrack_3Stubs_eta->Sumw2();
  h_L1CSCTrack_3Stubs_phi->Sumw2();

  h_L1CSCTrack_3Stubs20_pt->Sumw2();
  h_L1CSCTrack_3Stubs20_eta->Sumw2();
  h_L1CSCTrack_3Stubs20_phi->Sumw2();

  h_L1TMtracks_3Stubs_pt->Sumw2();
  h_L1TMtracks_3Stubs_eta->Sumw2();
  h_L1TMtracks_3Stubs_phi->Sumw2();

  h_L1CSCTrack_3StubsME1_pt->Sumw2();
  h_L1CSCTrack_3StubsME1_eta->Sumw2();
  h_L1CSCTrack_3StubsME1_phi->Sumw2();

  h_L1CSCTrack_3Stubs20ME1_pt->Sumw2();
  h_L1CSCTrack_3Stubs20ME1_eta->Sumw2();
  h_L1CSCTrack_3Stubs20ME1_phi->Sumw2();

  h_L1TMtracks_3StubsME1_pt->Sumw2();
  h_L1TMtracks_3StubsME1_eta->Sumw2();
  h_L1TMtracks_3StubsME1_phi->Sumw2();

  h_L1CSCTrack_2Stubs_pt->Sumw2();
  h_L1CSCTrack_2Stubs_eta->Sumw2();
  h_L1CSCTrack_2Stubs_phi->Sumw2();

  h_L1CSCTrack_2Stubs20_pt->Sumw2();
  h_L1CSCTrack_2Stubs20_eta->Sumw2();
  h_L1CSCTrack_2Stubs20_phi->Sumw2();

  h_L1CSCTrack_2Stubs20ME1_pt->Sumw2();
  h_L1CSCTrack_2Stubs20ME1_eta->Sumw2();
  h_L1CSCTrack_2Stubs20ME1_phi->Sumw2();

  h_L1TMtracks_2Stubs_pt->Sumw2();
  h_L1TMtracks_2Stubs_eta->Sumw2();
  h_L1TMtracks_2Stubs_phi->Sumw2();

  h_L1CSCTrack_2StubsME1_pt->Sumw2();
  h_L1CSCTrack_2StubsME1_eta->Sumw2();
  h_L1CSCTrack_2StubsME1_phi->Sumw2();

  h_L1TMtracks_2StubsME1_pt->Sumw2();
  h_L1TMtracks_2StubsME1_eta->Sumw2();
  h_L1TMtracks_2StubsME1_phi->Sumw2();

  h_L1CSCTrack__pt->Divide(h_L1CSCTrack__pt, h_truth_pt,1.0,1.0,"B");
  h_L1CSCTrack__eta->Divide(h_L1CSCTrack__eta, h_truth_eta,1.0,1.0,"B");
  h_L1CSCTrack__phi->Divide(h_L1CSCTrack__phi, h_truth_phi,1.0,1.0,"B");
  h_L1TMtracks__pt->Divide(h_L1TMtracks__pt, h_truth_pt,1.0,1.0,"B");
  h_L1TMtracks__eta->Divide(h_L1TMtracks__eta, h_truth_eta,1.0,1.0,"B");
  h_L1TMtracks__phi->Divide(h_L1TMtracks__phi, h_truth_phi,1.0,1.0,"B");
  h_L1CSCTrack_Q3_pt->Divide(h_L1CSCTrack_Q3_pt, h_truth_pt,1.0,1.0,"B");
  h_L1CSCTrack_Q3_eta->Divide(h_L1CSCTrack_Q3_eta, h_truth_eta,1.0,1.0,"B");
  h_L1CSCTrack_Q3_phi->Divide(h_L1CSCTrack_Q3_phi, h_truth_phi,1.0,1.0,"B");
  h_L1CSCTrack_20_pt->Divide(h_L1CSCTrack_20_pt, h_truth_pt,1.0,1.0,"B");
  h_L1CSCTrack_20_eta->Divide(h_L1CSCTrack_20_eta, h_truth_30_eta,1.0,1.0,"B");
  h_L1CSCTrack_20_phi->Divide(h_L1CSCTrack_20_phi, h_truth_30_phi,1.0,1.0,"B");
  h_L1CSCTrack_20ME1_pt->Divide(h_L1CSCTrack_20ME1_pt, h_truth_pt,1.0,1.0,"B");
  h_L1CSCTrack_20ME1_eta->Divide(h_L1CSCTrack_20ME1_eta, h_truth_30_eta,1.0,1.0,"B");
  h_L1CSCTrack_20ME1_phi->Divide(h_L1CSCTrack_20ME1_phi, h_truth_30_phi,1.0,1.0,"B");
  h_L1CSCTrack_Q3_phi_fwd->Divide(h_L1CSCTrack_Q3_phi_fwd, h_truth_phi_fwd,1.0,1.0,"B");
  h_L1CSCTrack_Q3_phi_bwd->Divide(h_L1CSCTrack_Q3_phi_bwd, h_truth_phi_bwd,1.0,1.0,"B");
  h_L1CSCTrack_ME1_pt->Divide(h_L1CSCTrack_ME1_pt, h_truth_pt,1.0,1.0,"B");
  h_L1CSCTrack_ME1_eta->Divide(h_L1CSCTrack_ME1_eta, h_truth_eta,1.0,1.0,"B");
  h_L1CSCTrack_ME1_phi->Divide(h_L1CSCTrack_ME1_phi, h_truth_phi,1.0,1.0,"B");
  h_L1TMtracks_ME1_pt->Divide(h_L1TMtracks_ME1_pt, h_truth_pt,1.0,1.0,"B");
  h_L1TMtracks_ME1_eta->Divide(h_L1TMtracks_ME1_eta, h_truth_eta,1.0,1.0,"B");
  h_L1TMtracks_ME1_phi->Divide(h_L1TMtracks_ME1_phi, h_truth_phi,1.0,1.0,"B");
  h_L1CSCTrack_3Stubs_pt->Divide(h_L1CSCTrack_3Stubs_pt, h_truth_pt,1.0,1.0,"B");
  h_L1CSCTrack_3Stubs_eta->Divide(h_L1CSCTrack_3Stubs_eta, h_truth_eta,1.0,1.0,"B");
  h_L1CSCTrack_3Stubs_phi->Divide(h_L1CSCTrack_3Stubs_phi, h_truth_phi,1.0,1.0,"B");
  h_L1CSCTrack_3Stubs20_pt->Divide(h_L1CSCTrack_3Stubs20_pt, h_truth_pt,1.0,1.0,"B");
  h_L1CSCTrack_3Stubs20_eta->Divide(h_L1CSCTrack_3Stubs20_eta, h_truth_30_eta,1.0,1.0,"B");
  h_L1CSCTrack_3Stubs20_phi->Divide(h_L1CSCTrack_3Stubs20_phi, h_truth_30_phi,1.0,1.0,"B");
  h_L1TMtracks_3Stubs_pt->Divide(h_L1TMtracks_3Stubs_pt, h_truth_pt,1.0,1.0,"B");
  h_L1TMtracks_3Stubs_eta->Divide(h_L1TMtracks_3Stubs_eta, h_truth_eta,1.0,1.0,"B");
  h_L1TMtracks_3Stubs_phi->Divide(h_L1TMtracks_3Stubs_phi, h_truth_phi,1.0,1.0,"B");
  h_L1CSCTrack_3StubsME1_pt->Divide(h_L1CSCTrack_3StubsME1_pt, h_truth_pt,1.0,1.0,"B");
  h_L1CSCTrack_3StubsME1_eta->Divide(h_L1CSCTrack_3StubsME1_eta, h_truth_eta,1.0,1.0,"B");
  h_L1CSCTrack_3StubsME1_phi->Divide(h_L1CSCTrack_3StubsME1_phi, h_truth_phi,1.0,1.0,"B");
  h_L1CSCTrack_3Stubs20ME1_pt->Divide(h_L1CSCTrack_3Stubs20ME1_pt, h_truth_pt,1.0,1.0,"B");
  h_L1CSCTrack_3Stubs20ME1_eta->Divide(h_L1CSCTrack_3Stubs20ME1_eta, h_truth_30_eta,1.0,1.0,"B");
  h_L1CSCTrack_3Stubs20ME1_phi->Divide(h_L1CSCTrack_3Stubs20ME1_phi, h_truth_30_phi,1.0,1.0,"B");
  h_L1TMtracks_3StubsME1_pt->Divide(h_L1TMtracks_3StubsME1_pt, h_truth_pt,1.0,1.0,"B");
  h_L1TMtracks_3StubsME1_eta->Divide(h_L1TMtracks_3StubsME1_eta, h_truth_eta,1.0,1.0,"B");
  h_L1TMtracks_3StubsME1_phi->Divide(h_L1TMtracks_3StubsME1_phi, h_truth_phi,1.0,1.0,"B");
  h_L1CSCTrack_2Stubs20_pt->Divide(h_L1CSCTrack_2Stubs20_pt, h_truth_pt,1.0,1.0,"B");
  h_L1CSCTrack_2Stubs20_eta->Divide(h_L1CSCTrack_2Stubs20_eta, h_truth_30_eta,1.0,1.0,"B");
  h_L1CSCTrack_2Stubs20_phi->Divide(h_L1CSCTrack_2Stubs20_phi, h_truth_30_phi,1.0,1.0,"B");
  h_L1CSCTrack_2Stubs20ME1_pt->Divide(h_L1CSCTrack_2Stubs20ME1_pt, h_truth_pt,1.0,1.0,"B");
  h_L1CSCTrack_2Stubs20ME1_eta->Divide(h_L1CSCTrack_2Stubs20ME1_eta, h_truth_30_eta,1.0,1.0,"B");
  h_L1CSCTrack_2Stubs20ME1_phi->Divide(h_L1CSCTrack_2Stubs20ME1_phi, h_truth_30_phi,1.0,1.0,"B");
  h_L1CSCTrack_2Stubs_pt->Divide(h_L1CSCTrack_2Stubs_pt, h_truth_pt,1.0,1.0,"B");
  h_L1CSCTrack_2Stubs_eta->Divide(h_L1CSCTrack_2Stubs_eta, h_truth_eta,1.0,1.0,"B");
  h_L1CSCTrack_2Stubs_phi->Divide(h_L1CSCTrack_2Stubs_phi, h_truth_phi,1.0,1.0,"B");
  h_L1TMtracks_2Stubs_pt->Divide(h_L1TMtracks_2Stubs_pt, h_truth_pt,1.0,1.0,"B");
  h_L1TMtracks_2Stubs_eta->Divide(h_L1TMtracks_2Stubs_eta, h_truth_eta,1.0,1.0,"B");
  h_L1TMtracks_2Stubs_phi->Divide(h_L1TMtracks_2Stubs_phi, h_truth_phi,1.0,1.0,"B");
  h_L1CSCTrack_2StubsME1_pt->Divide(h_L1CSCTrack_2StubsME1_pt, h_truth_pt,1.0,1.0,"B");
  h_L1CSCTrack_2StubsME1_eta->Divide(h_L1CSCTrack_2StubsME1_eta, h_truth_eta,1.0,1.0,"B");
  h_L1CSCTrack_2StubsME1_phi->Divide(h_L1CSCTrack_2StubsME1_phi, h_truth_phi,1.0,1.0,"B");
  h_L1TMtracks_2StubsME1_pt->Divide(h_L1TMtracks_2StubsME1_pt, h_truth_pt,1.0,1.0,"B");
  h_L1TMtracks_2StubsME1_eta->Divide(h_L1TMtracks_2StubsME1_eta, h_truth_eta,1.0,1.0,"B");
  h_L1TMtracks_2StubsME1_phi->Divide(h_L1TMtracks_2StubsME1_phi, h_truth_phi,1.0,1.0,"B");

}

//define this as a plug-in
DEFINE_FWK_MODULE(L1TUpgradeAnalyser);
