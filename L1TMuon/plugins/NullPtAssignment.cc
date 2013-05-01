#include "L1TriggerDPGUpgrade/L1TMuon/interface/NullPtAssignment.h"
#include "L1TriggerDPGUpgrade/DataFormats/interface/L1TMuonInternalTrack.h"

#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/EventSetup.h"
 
using namespace L1TMuon;

NullPtAssignment::NullPtAssignment( const edm::ParameterSet& ps ):
  PtAssignmentUnit(ps) {
}

#include "L1TriggerDPGUpgrade/L1TMuon/interface/PtAssignmentUnitFactory.h"
DEFINE_EDM_PLUGIN( PtAssignmentUnitFactory, 
		   NullPtAssignment, 
		   "NullPtAssignment");
