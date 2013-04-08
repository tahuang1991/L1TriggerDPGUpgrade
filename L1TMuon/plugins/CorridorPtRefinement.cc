#include "L1TriggerDPGUpgrade/L1TMuon/interface/CorridorPtRefinement.h"
#include "L1TriggerDPGUpgrade/DataFormats/interface/L1TMuonInternalTrack.h"

#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/EventSetup.h"
 
using namespace L1TMuon;

CorridorPtRefinement::CorridorPtRefinement( const edm::ParameterSet& ps ):
  PtRefinementUnit(ps) {
}

// this modifies the track in place!!!
void CorridorPtRefinement::refinePt(const edm::EventSetup& es, 
				    InternalTrack&) const {
  
}

#include "L1TriggerDPGUpgrade/L1TMuon/interface/PtRefinementUnitFactory.h"
DEFINE_EDM_PLUGIN( PtRefinementUnitFactory, 
		   CorridorPtRefinement, 
		   "CorridorPtRefinement");
