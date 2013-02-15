// 
// Class: L1ITMuInternalTrackPlotter
//
// Info: Processes a track into histograms of delta-phis and such
//
// Author: L. Gray (FNAL)
//

#include <memory>
#include <map>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "L1Trigger/L1IntegratedMuonTrigger/interface/TriggerPrimitive.h"
#include "L1Trigger/L1IntegratedMuonTrigger/interface/TriggerPrimitiveFwd.h"

#include "L1Trigger/L1IntegratedMuonTrigger/interface/InternalTrack.h"
#include "L1Trigger/L1IntegratedMuonTrigger/interface/InternalTrackFwd.h"

#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "TTree.h"
#include "TH1F.h"
#include "TH2F.h"
#include <map>

#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

using namespace L1ITMu;

typedef edm::ParameterSet PSet;

class L1ITMuInternalTrackPlotter : public edm::EDAnalyzer {  
  typedef TH1F* hist1dfp;
  typedef TH2F* hist2dfp;
  typedef std::map<std::string,hist1dfp> hist1dmap;
  typedef std::map<std::string,hist2dfp> hist2dmap;
  typedef std::map<std::string,double> deltas_map;
  
public:
  L1ITMuInternalTrackPlotter(const PSet&);
  ~L1ITMuInternalTrackPlotter() {}

  void analyze(const edm::Event&, const edm::EventSetup&);  
private:
  std::string convertSubModeToName(const TriggerPrimitive&,
				   const TriggerPrimitive&) const;
  deltas_map makeCombinations(const InternalTrack& ) const;
  bool _dogen;
  edm::InputTag _geninput;
  std::vector<edm::InputTag> _trkInput;
  // gen histograms
  hist1dfp gen_eta, gen_phi, gen_pt;  
  // track histograms
  hist1dmap detahists;
  hist1dmap dphihists;
};

L1ITMuInternalTrackPlotter::L1ITMuInternalTrackPlotter(const PSet& p) {
  if( (_dogen = p.getUntrackedParameter<bool>("doGen",false)) ) {
    _geninput = p.getParameter<edm::InputTag>("genSrc");
  }
  _trkInput = p.getParameter<std::vector<edm::InputTag> >("trackSrc");

  edm::Service<TFileService> _fs;

  if( _dogen ) {
    gen_eta = _fs->make<TH1F>("hgen_eta","Generated Eta",100,-2.5,2.5);
    gen_phi = _fs->make<TH1F>("hgen_phi","Generated Eta",100,-M_PI,M_PI);
    gen_pt = _fs->make<TH1F>("hgen_pt","Generated Eta",400,0,200);
  }
  
}

void L1ITMuInternalTrackPlotter::analyze(const edm::Event& ev, 
					const edm::EventSetup& es) {
  //dump the generated muons in the event (if requested)
  if( _dogen ) {
    edm::Handle<reco::GenParticleCollection> genps;
    ev.getByLabel(_geninput,genps);
    auto bgen = genps->cbegin();
    auto egen = genps->cend();
    for( ; bgen != egen; ++bgen ) {
      if( std::abs(bgen->pdgId()) == 13 ) {
	gen_eta->Fill(bgen->eta());
	gen_phi->Fill(bgen->phi());
	gen_pt->Fill(bgen->pt());
      }
    }
  }
  
  edm::Handle<InternalTrackCollection> trks;
  ev.getByLabel(_trkInput,trks);
}

// take a pair of positions within a mode and make a name!
std::string L1ITMuInternalTrackPlotter::
convertStubsToName(const TriggerPrimitive& tp1,
		   const TriggerPrimitive& tp2) const {
  std::string name1,name2;
  unsigned station1,station2;
  TriggerPrimitive::subsystem_type type1, type2;
  CSCDetId cscid;
  RPCDetId rpcid;
  DTChamberId dtid;

  type1 = tp1.subsystem();
  type2 = tp2.subsystem();
  
  switch(type1) {
  case TriggerPrimitive::kDT:
    name1 = std::string("DT");
    dtid = tp1.detId<DTChamberId>();
    station1 = dtid.station();
  case TriggerPrimitive::kCSC:
    name1 = std::string("CSC");
    cscid = tp1.detId<CSCDetId>();
    station1 = cscid.station();
  case TriggerPrimitive::kRPC:
    rpcid = tp1.detId<RPCDetId>();
    name1 = (rpcid.region() == 0 ? std::string("RPCb") : std::string("RPCf"));
    station1 = rpcid.station();
  }

  switch(type2) {
  case TriggerPrimitive::kDT:
    name2 = std::string("DT");
    dtid = tp2.detId<DTChamberId>();
    station2 = dtid.station();
  case TriggerPrimitive::kCSC:
    name2 = std::string("CSC");
    cscid = tp2.detId<CSCDetId>();
    station2 = cscid.station();
  case TriggerPrimitive::kRPC:
    rpcid = tp2.detId<RPCDetId>();
    name2 = (rpcid.region() == 0 ? std::string("RPCb") : std::string("RPCf"));
    station2 = rpcid.station();
  }

  return std::string(Form("%s%i_%s%i",name1,station1,name2,station2));
}

// this function takes the list of used 
deltas_map L1ITMuInternalTrackPlotter::
makeCombinations(const InternalTrack& ) const {
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(L1ITMuInternalTrackPlotter);
