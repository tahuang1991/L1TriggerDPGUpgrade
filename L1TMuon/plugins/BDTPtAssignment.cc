#include "L1TriggerDPGUpgrade/L1TMuon/interface/BDTPtAssignment.h"
#include "L1TriggerDPGUpgrade/DataFormats/interface/L1TMuonInternalTrack.h"

#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/EventSetup.h"
 
using namespace L1TMuon;

BDTPtAssignment::BDTPtAssignment( const edm::ParameterSet& ps ):
  PtAssignmentUnit(ps) {
}

// this modifies the track in place!!!
void BDTPtAssignment::assignPt(const edm::EventSetup& es, 
			       InternalTrack&) const {
  
}

#include "L1TriggerDPGUpgrade/L1TMuon/interface/PtAssignmentUnitFactory.h"
DEFINE_EDM_PLUGIN( PtAssignmentUnitFactory, 
		   BDTPtAssignment, 
		   "BDTPtAssignment");
