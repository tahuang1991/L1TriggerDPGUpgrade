#include "L1Trigger/L1IntegratedMuonTrigger/interface/GeometryTranslator.h"
#include "L1Trigger/L1IntegratedMuonTrigger/interface/TriggerPrimitive.h"

// event setup stuff / geometries
#include "FWCore/Framework/interface/EventSetup.h"
#include "Geometry/Records/interface/MuonGeometryRecord.h"

#include "Geometry/CSCGeometry/interface/CSCGeometry.h"
#include "L1Trigger/CSCCommonTrigger/interface/CSCConstants.h"

#include "Geometry/DTGeometry/interface/DTGeometry.h"
#include "Geometry/RPCGeometry/interface/RPCGeometry.h"

using namespace L1ITMu;

GeometryTranslator::GeometryTranslator():  
  _geom_cache_id(0ULL) {
}

GeometryTranslator::~GeometryTranslator() {  
}

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

GlobalPoint 
GeometryTranslator::getRPCSpecificPoint(const TriggerPrimitive& tp) const {
  RPCDetId id(tp.detId<RPCDetId>());
  const RPCRoll*  roll = _georpc->roll(id);
  uint16_t strip = tp.getRPCData().strip;
  LocalPoint lp = roll->centreOfStrip(strip);
  return roll->toGlobal(lp);
}

double 
GeometryTranslator::calcRPCSpecificEta(const TriggerPrimitive& tp) const {  
  return getRPCSpecificPoint(tp).eta();
}

double 
GeometryTranslator::calcRPCSpecificPhi(const TriggerPrimitive& tp) const {  
  return getRPCSpecificPoint(tp).phi();
}

// this function actually does nothing since RPC
// hits are point-like objects
double 
GeometryTranslator::calcRPCSpecificBend(const TriggerPrimitive& tp) const {
  return 0.0;
}

GlobalPoint 
GeometryTranslator::getCSCSpecificPoint(const TriggerPrimitive& tp) const {
  CSCDetId id(tp.detId<CSCDetId>());  
  const CSCChamber* chamb = _geocsc->chamber(id);
  const CSCLayerGeometry* layer_geom = 
    chamb->layer(CSCConstants::KEY_ALCT_LAYER)->geometry();
  const CSCLayer* layer = chamb->layer(CSCConstants::KEY_ALCT_LAYER);

  uint16_t strip = tp.getCSCData().strip;
  uint16_t keyWG = tp.getCSCData().keywire;

  // some magic needs to go here to scale some chamber's strips due to
  // local phi lut scaling

  LocalPoint lp  = layer_geom->stripWireGroupIntersection(strip,keyWG);
  
  return layer->surface().toGlobal(lp);
}

double 
GeometryTranslator::calcCSCSpecificEta(const TriggerPrimitive& tp) const {  
  return getCSCSpecificPoint(tp).eta();
}

double 
GeometryTranslator::calcCSCSpecificPhi(const TriggerPrimitive& tp) const {  
  return getCSCSpecificPoint(tp).phi();
}

double 
GeometryTranslator::calcCSCSpecificBend(const TriggerPrimitive& tp) const {  
  return 0.0;
}

// need to get eta position of the TRACO in some non-insane way
double 
GeometryTranslator::calcDTSpecificEta(const TriggerPrimitive& tp) const {
  DTChamberId id(tp.detId<DTChamberId>());
  return 0.0;
}

// phi of a hit seems to be straightforward
double 
GeometryTranslator::calcDTSpecificPhi(const TriggerPrimitive& tp) const {
  DTChamberId id(tp.detId<DTChamberId>());
  return 0.0;
}

// we have the bend except for station 3
double 
GeometryTranslator::calcDTSpecificBend(const TriggerPrimitive& tp) const {
  DTChamberId id(tp.detId<DTChamberId>());
  return 0.0;
}
