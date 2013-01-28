#include "L1Trigger/L1IntegratedMuonTrigger/interface/GeometryTranslator.h"
#include "L1Trigger/L1IntegratedMuonTrigger/interface/TriggerPrimitive.h"

// event setup stuff / geometries
#include "FWCore/Framework/interface/EventSetup.h"
#include "Geometry/Records/interface/MuonGeometryRecord.h"
#include "Geometry/CSCGeometry/interface/CSCGeometry.h"
#include "Geometry/DTGeometry/interface/DTGeometry.h"
#include "Geometry/RPCGeometry/interface/RPCGeometry.h"

using namespace L1ITMu;

double 
GeometryTranslator::calculateGlobalEta(const TriggerPrimitive& tp) const {
  return 0.0;
}

double 
GeometryTranslator::calculateGlobalPhi(const TriggerPrimitive& tp) const {
  return 0.0;
}

double 
GeometryTranslator::calculateBendAngle(const TriggerPrimitive& tp) const {
  return 0.0;
}

void GeometryTranslator::checkAndUpdateGeometry(const edm::EventSetup& es) {
  unsigned long long geomid = es.get<MuonGeometryRecord>().cacheIdentifier();
  if( _geom_cache_id != geomid ) { 
    es.get<MuonGeometryRecord>().get(_georpc);  
    es.get<MuonGeometryRecord>().get(_geocsc);
    es.get<MuonGeometryRecord>().get(_geodt);
    _geom_cache_id = geomid;
  }  
}

double GeometryTranslator::calcRpcSpecificEta(const TriggerPrimitive&) const {
  return 0.0;
}
double GeometryTranslator::calcRpcSpecificPhi(const TriggerPrimitive&) const {
  return 0.0;
}
double GeometryTranslator::calcRpcSpecificBend(const TriggerPrimitive&) const {
  return 0.0;
}

double GeometryTranslator::calcCSCSpecificEta(const TriggerPrimitive&) const {
  return 0.0;
}
double GeometryTranslator::calcCSCSpecificPhi(const TriggerPrimitive&) const {
  return 0.0;
}
double GeometryTranslator::calcCSCSpecificBend(const TriggerPrimitive&) const {
  return 0.0;
}

double GeometryTranslator::calcDTSpecificEta(const TriggerPrimitive&) const {
  return 0.0;
}
double GeometryTranslator::calcDTSpecificPhi(const TriggerPrimitive&) const {
  return 0.0;
}
double GeometryTranslator::calcDTSpecificBend(const TriggerPrimitive&) const {
  return 0.0;
}
