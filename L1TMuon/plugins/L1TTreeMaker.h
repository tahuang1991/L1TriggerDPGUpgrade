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
#include "TTree.h"

using namespace std;
using namespace edm;
using namespace L1TMuon;

class L1TTreeMaker : public edm::EDAnalyzer {
public:
  explicit L1TTreeMaker(const edm::ParameterSet&);
  ~L1TTreeMaker();

private:
  virtual void beginJob() ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;

  void intializeTree();

  // ----------member data ---------------------------

  //TH1F* rateHist;
  edm::InputTag lctsTag;	
  edm::InputTag vertexColTag;	
  edm::Service<TFileService> fs;
  
  TTree* l1muon;

  unsigned int event;
  int mc_n;
  vector<float> mc_pt, mc_eta, mc_phi, mc_type;

  int l1csctrk_n;
  vector<float> l1csctrk_pt, l1csctrk_eta, l1csctrk_phi, l1csctrk_GEMdPhi;
  vector<unsigned int> l1csctrk_quality_packed, l1csctrk_rank, l1csctrk_ptLUTAddress;
  vector<unsigned int> l1csctrk_pt_packed, l1csctrk_eta_packed, l1csctrk_phi_packed;
  vector<int> l1csctrk_nstubs;
  vector<bool> l1csctrk_hasGEM, l1csctrk_hasME1, l1csctrk_hasME2, l1csctrk_hasME3, l1csctrk_hasME4;

  int l1tm_n;
  vector<float> l1tm_pt, l1tm_eta, l1tm_phi, l1tm_GEMdPhi;
  vector<int> l1tm_nstubs, l1tm_rank;
  vector<bool> l1tm_hasGEM, l1tm_hasME1, l1tm_hasME2, l1tm_hasME3, l1tm_hasME4;
  
};

