#include "L1TriggerDPGUpgrade/L1TMuon/interface/PtRefinementUnit.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "CondFormats/DataRecord/interface/L1MuTriggerScalesRcd.h"
#include "CondFormats/DataRecord/interface/L1MuTriggerPtScaleRcd.h"

using namespace L1TMuon;

PtRefinementUnit::PtRefinementUnit(const edm::ParameterSet& ps):
  _name(ps.getParameter<std::string>("RefinementType")),
  position_scalesCacheID(0),
  pt_scaleCacheID(0) {
}

void PtRefinementUnit::updateEventSetup(const edm::EventSetup& es) {
  const unsigned long long poscache = 
    es.get<L1MuTriggerScalesRcd>().cacheIdentifier();
  const unsigned long long ptcache = 
    es.get<L1MuTriggerPtScaleRcd>().cacheIdentifier();
  if(  poscache != position_scalesCacheID || ptcache  != pt_scaleCacheID ) {
    es.get<L1MuTriggerScalesRcd>().get(position_scales);
    es.get<L1MuTriggerPtScaleRcd>().get(pt_scale);
    
    position_scalesCacheID = poscache;
    pt_scaleCacheID = ptcache;
  }
}
