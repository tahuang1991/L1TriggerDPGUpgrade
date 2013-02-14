// 
// Class: L1ITMuInternalTrackTupler
//
// Info: Processes a track into an ntuple entry
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

#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

using namespace L1ITMu;

typedef edm::ParameterSet PSet;

class L1ITMuInternalTrackTupler : public edm::EDAnalyzer {
  typedef TH1F* histfp;
public:
  L1ITMuInternalTrackTupler(const PSet&);
  ~L1ITMuInternalTrackTupler() {}

  void analyze(const edm::Event&, const edm::EventSetup&);  
private:
  bool _dogen;
  edm::InputTag _geninput;
  std::vector<edm::InputTag> _trkInput;
  // gen histograms
  histfp gen_eta, gen_phi, gen_pt;  
  // track histograms
};

L1ITMuInternalTrackTupler::L1ITMuInternalTrackTupler(const PSet& p) {
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

void L1ITMuInternalTrackTupler::analyze(const edm::Event& ev, 
					const edm::EventSetup& es) {
  //dump the generated muons in the event (if requested)
  if( _dogen ) {
    edm::Handle<reco::GenParticleCollection> genps;
    ev.getByLabel(_geninput,genps);
    
  }
  edm::Handle<InternalTrackCollection> trks;
  ev.getByLabel(_trkInput,trks);
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(L1ITMuInternalTrackTupler);
