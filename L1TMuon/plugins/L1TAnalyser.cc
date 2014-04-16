
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

#include "L1TriggerDPGUpgrade/DataFormats/interface/L1TMuonTriggerPrimitiveFwd.h"

#include "TH1F.h"
#include "TH2F.h"
#include "TFile.h"

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

  TH2F* h_TFPTpackedvsdPhi12GEM;
  TH2F* h_TFPTpackedvsdPhi23GEM;
  TH2F* h_TFPTpackedvsGEMdPhi;

  TH1F* h_noTFnStub;
  TH1F* h_noTFnStubGEM;

  TH1F* h_noTFQ3nStub;
  TH1F* h_noTFQ3nStubGEM;
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
L1TAnalyser::L1TAnalyser(const edm::ParameterSet& iConfig)
{
  //now do what ever initialization is needed
  //  runSRLUTs = new csctf_analysis::RunSRLUTs();
  lctsTag= iConfig.getParameter<edm::InputTag>("lctsTag");
  vertexColTag= iConfig.getParameter<edm::InputTag>("vertexColTag");
  outTreeFileName= iConfig.getUntrackedParameter<string>("outTreeFileName");
  haveRECO = iConfig.getUntrackedParameter<bool>("haveRECO");
  singleSectorNum = iConfig.getUntrackedParameter<int>("singleSectorNum");
}
L1TAnalyser::~L1TAnalyser()
{
 
  // do anything here that needs to be done at desctruction time
  // (e.g. close files, deallocate resources etc.)
  //  delete runSRLUTs;
}
//
// member functions
//

// ------------ method called to for each event  ------------
void
L1TAnalyser::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  ///////////////////
  //Setup Stuff//////
  ///////////////////
  //  edm::RefProd<CSCTrackCollection> csctfProd = ev.getRefBeforePut<CSCTrackCollection>("input");  
  edm::Handle<L1CSCTrackCollection> l1csctracks;
  iEvent.getByLabel("simCsctfTrackDigis",l1csctracks);
  edm::Handle<TriggerPrimitiveCollection> trigPrims;
  iEvent.getByLabel("L1TMuonTriggerPrimitives",trigPrims);
  edm::Handle<CSCCorrelatedLCTDigiCollection> lcts;
  iEvent.getByLabel("simCscTriggerPrimitiveDigis","MPCSORTED", lcts);

  bool hasTrkFwd = false;
  bool hasTrkBwd = false;
  bool hasQ3TrkFwd = false;
  bool hasQ3TrkBwd = false;
  // bool hasTrkFwdGE11 = false;
  // bool hasTrkBwdGE11 = false;

  // lcts used in tracks
  L1CSCTrackCollection::const_iterator tmp_trk = l1csctracks->begin();
  for(; tmp_trk != l1csctracks->end(); tmp_trk++){
    auto l1track = tmp_trk->first;
    // cout << "track = "
    // 	 << ", ptValue " << l1track.ptValue()
    // 	 << ", etaValue " << l1track.etaValue()
    // 	 << ", phiValue " << l1track.phiValue()
    // 	 << ", ptLUTAddress " << l1track.ptLUTAddress()
    // 	 << ", quality " << l1track.quality()
    // 	 << ", front_rear " << l1track.front_rear()
    // 	 << endl;
    // cout << "track = "
    // 	 << ", pt_packed " << l1track.pt_packed()
    // 	 << ", eta_packed " << l1track.eta_packed()
    // 	 << ", phi_packed " << l1track.phi_packed()
    // 	 << ", ptLUTAddress " << l1track.ptLUTAddress()
    // 	 << endl;
    // //      l1track.Print();

    unsigned int quality_packed;
    unsigned int rank=l1track.rank();
    unsigned int pt_packed;
    unsigned m_ptAddress = l1track.ptLUTAddress();

    l1track.decodeRank(rank,pt_packed,quality_packed); //get the pt and gaulity packed
    cout << "l1track.decodeRank = "
     	 << ", rank " << rank
     	 << ", pt_packed " << pt_packed
     	 << ", quality_packed " << quality_packed
     	 << endl;

    int nstubs=0;
    bool hasGEM = false;
    float GEMdPhi = -99;
    for(CSCCorrelatedLCTDigiCollection::DigiRangeIterator csc=tmp_trk->second.begin(); csc!=tmp_trk->second.end(); csc++){
      auto lctdigi = (*csc).second.first;
      h_GEMDPhi->Fill((*csc).second.first->getGEMDPhi());

      if ((*csc).first.endcap() == 1){
	h_nStation->Fill((*csc).first.station());
	hasTrkFwd = true;
	if (quality_packed >= 3)
	  hasQ3TrkFwd = true;
	// if ((*csc).first.station() == 1)
	//   hasTrkFwdGE11 = true;
      }
      if ((*csc).first.endcap() == 2){
	h_nStation->Fill(-((*csc).first.station()));
	hasTrkBwd = true;
	if (quality_packed >= 3)
	  hasQ3TrkBwd = true;
	// if ((*csc).first.station() == 1)
	//   hasTrkBwdGE11 = true;
      }

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

    cout << "m_ptAddress = " << m_ptAddress
	 << ", deltaPhi12 " << l1track.deltaPhi12()
	 << ", deltaPhi23 " << l1track.deltaPhi23()
	 << ", addressEta " << l1track.addressEta()
	 << ", mode " << l1track.mode()
	 << ", sign " << l1track.sign()
	 << endl;
    cout << "nstubs = " << nstubs
	 << ", me1Tbin = " << l1track.me1Tbin()
	 << ", me2Tbin = " << l1track.me2Tbin()
	 << ", me3Tbin = " << l1track.me3Tbin()
	 << ", me4Tbin = " << l1track.me4Tbin()
	 << ", mb1Tbin = " << l1track.mb1Tbin()
	 << endl;

    h_TFnStubinTrack->Fill(nstubs);
    h_TFnStubvsPTAdd->Fill(nstubs,m_ptAddress);
    h_TFnStubvsQualityPacked->Fill(nstubs,quality_packed);
    h_TFPTpackedvsPTAdd->Fill(pt_packed,m_ptAddress);
    h_TFPTpackedvsdPhi12->Fill(pt_packed,l1track.deltaPhi12());
    h_TFPTpackedvsdPhi23->Fill(pt_packed,l1track.deltaPhi23());

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
}


// ------------ method called once each job just before starting event loop  ------------
void 
L1TAnalyser::beginJob()
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

  h_nStation=fs->make<TH1F>("nStation","Number of station",11,-5,6);
  h_nStation->GetXaxis()->SetTitle("Station mumber");
  h_nStation->GetYaxis()->SetTitle("Counts");
}

// ------------ method called once each job just after ending the event loop  ------------
void L1TAnalyser::endJob() 
{
}

//define this as a plug-in
DEFINE_FWK_MODULE(L1TAnalyser);
