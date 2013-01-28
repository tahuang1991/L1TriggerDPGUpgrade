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
  switch(tp.subsystem()) {
  case TriggerPrimitive::kDT:
    return calcDTSpecificEta(tp);
    break;
  case TriggerPrimitive::kCSC:
    return calcCSCSpecificEta(tp);
    break;
  case TriggerPrimitive::kRPC:
    return calcRPCSpecificEta(tp);
    break;
  default:
    return std::nan("Invalid TP type!"); 
    break;
  }
}

double 
GeometryTranslator::calculateGlobalPhi(const TriggerPrimitive& tp) const {
  switch(tp.subsystem()) {
  case TriggerPrimitive::kDT:
    return calcDTSpecificPhi(tp);
    break;
  case TriggerPrimitive::kCSC:
    return calcCSCSpecificPhi(tp);
    break;
  case TriggerPrimitive::kRPC:
    return calcRPCSpecificPhi(tp);
    break;
  default:
    return std::nan("Invalid TP type!");
    break;
  }
}

double 
GeometryTranslator::calculateBendAngle(const TriggerPrimitive& tp) const {
  switch(tp.subsystem()) {
  case TriggerPrimitive::kDT:
    return calcDTSpecificBend(tp);
    break;
  case TriggerPrimitive::kCSC:
    return calcCSCSpecificBend(tp);
    break;
  case TriggerPrimitive::kRPC:
    return calcRPCSpecificBend(tp);
    break;
  default:
    return std::nan("Invalid TP type!");
    break;
  }
}

void GeometryTranslator::checkAndUpdateGeometry(const edm::EventSetup& es) {
  unsigned long long geomid = es.get<MuonGeometryRecord>().cacheIdentifier();
  if( _geom_cache_id != geomid ) {
    const MuonGeometryRecord& geom = es.get<MuonGeometryRecord>();
    geom.get(_georpc);  
    geom.get(_geocsc);
    geom.get(_geodt);
    _geom_cache_id = geomid;
  }  
}

double 
GeometryTranslator::calcRPCSpecificEta(const TriggerPrimitive& tp) const {
  RPCDetId id(tp.detId<RPCDetId>());
  const RPCRoll*  roll = _georpc->roll(id);

  uint16_t strip = tp.getRPCData().strip;
  LocalPoint lp = roll->centreOfStrip(strip);
  GlobalPoint gp = roll->toGlobal(lp);

  return gp.eta();
}

double 
GeometryTranslator::calcRPCSpecificPhi(const TriggerPrimitive& tp) const {
  RPCDetId id(tp.detId<RPCDetId>());
  const RPCRoll*  roll = _georpc->roll(id);

  uint16_t strip = tp.getRPCData().strip;
  LocalPoint lp = roll->centreOfStrip(strip);
  GlobalPoint gp = roll->toGlobal(lp);

  return gp.phi();
}

double 
GeometryTranslator::calcRPCSpecificBend(const TriggerPrimitive& tp) const {
  return 0.0;
}

double 
GeometryTranslator::calcCSCSpecificEta(const TriggerPrimitive& tp) const {
  return 0.0;
}

double 
GeometryTranslator::calcCSCSpecificPhi(const TriggerPrimitive& tp) const {
  return 0.0;
}

double 
GeometryTranslator::calcCSCSpecificBend(const TriggerPrimitive& tp) const {
  return 0.0;
}

double 
GeometryTranslator::calcDTSpecificEta(const TriggerPrimitive& tp) const {
  return 0.0;
}

double 
GeometryTranslator::calcDTSpecificPhi(const TriggerPrimitive& tp) const {
  return 0.0;
}

double 
GeometryTranslator::calcDTSpecificBend(const TriggerPrimitive& tp) const {
  return 0.0;
}
