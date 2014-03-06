#include "L1TriggerDPGUpgrade/L1TMuon/interface/NullPtRefinement.h"
#include "L1TriggerDPGUpgrade/DataFormats/interface/L1TMuonInternalTrack.h"

#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include <TFile.h>
#include <TGraph.h>
#include <TString.h>

using namespace L1TMuon;

namespace {
  typedef std::vector<double> vdouble;
}

NullPtRefinement::NullPtRefinement(const edm::ParameterSet& ps):
  PtRefinementUnit(ps) {
}

#include "L1TriggerDPGUpgrade/L1TMuon/interface/PtRefinementUnitFactory.h"
DEFINE_EDM_PLUGIN( PtRefinementUnitFactory, 
		   NullPtRefinement, 
		   "NullPtRefinement");
