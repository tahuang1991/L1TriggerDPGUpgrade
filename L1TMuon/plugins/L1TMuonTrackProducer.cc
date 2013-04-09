// 
// Class: L1TMuonTrackProcessor
//
// Info: A configurable track processor where the trackfinding algorithms and 
//       pt assignment/refinement algorithms are controlled by config files.
//       The closest representation of this in the old hardware is 
//       the CSC sector processor.
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

#include "L1TriggerDPGUpgrade/DataFormats/interface/L1TMuonTriggerPrimitive.h"
#include "L1TriggerDPGUpgrade/DataFormats/interface/L1TMuonTriggerPrimitiveFwd.h"

#include "L1TriggerDPGUpgrade/DataFormats/interface/L1TMuonInternalTrack.h"
#include "L1TriggerDPGUpgrade/DataFormats/interface/L1TMuonInternalTrackFwd.h"

// interface to Pt assignment and refinement
#include "L1TriggerDPGUpgrade/L1TMuon/interface/PtAssignmentUnitFactory.h"
#include "L1TriggerDPGUpgrade/L1TMuon/interface/PtRefinementUnitFactory.h"

#include "FWCore/ServiceRegistry/interface/Service.h"

using namespace L1TMuon;

typedef edm::ParameterSet PSet;
typedef std::unique_ptr<PtAssignmentUnit> pPAU;
typedef std::unique_ptr<PtRefinementUnit> pPRU;

class L1TMuonTrackProcessor : public edm::EDProducer {    
public:
  L1TMuonTrackProcessor(const PSet&);
  ~L1TMuonTrackProcessor() {}

  void analyze(edm::Event&, const edm::EventSetup&);  
private:
  pPAU _ptassign;
  pPRU _ptrefine;
};

L1TMuonTrackProcessor::L1TMuonTrackProcessor(const PSet& p) {  
  std::unique_ptr<PtAssignmentUnitFactory> 
    fPAU(PtAssignmentUnitFactory::get());
  if( p.existsAs<PSet>("PtAssignmentUnit") ) {
  } else {
    _ptassign.reset(NULL);
  }
  std::unique_ptr<PtRefinementUnitFactory> 
    fPRU(PtRefinementUnitFactory::get());
  
  fPAU.release();
  fPRU.release();
}

void L1TMuonTrackProcessor::produce(edm::Event& ev, 
				    const edm::EventSetup& es) {  
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(L1TMuonTrackProcessor);
