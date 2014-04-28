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

  void intializeTree(TriggerTrack trk);

  // ----------member data ---------------------------

  //TH1F* rateHist;
  edm::InputTag lctsTag;	
  edm::InputTag vertexColTag;	
  edm::Service<TFileService> fs;
  
  TTree* l1tmuon;

  int mc_n;
  vector<float> mc_pt, mc_eta, mc_phi, mc_type;

};

