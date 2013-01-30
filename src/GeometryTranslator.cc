#include "L1Trigger/L1IntegratedMuonTrigger/interface/GeometryTranslator.h"
#include "L1Trigger/L1IntegratedMuonTrigger/interface/TriggerPrimitive.h"

// event setup stuff / geometries
#include "FWCore/Framework/interface/EventSetup.h"
#include "Geometry/Records/interface/MuonGeometryRecord.h"

#include "Geometry/CSCGeometry/interface/CSCGeometry.h"
#include "L1Trigger/CSCCommonTrigger/interface/CSCConstants.h"
#include "L1Trigger/CSCCommonTrigger/interface/CSCPatternLUT.h"

#include "Geometry/DTGeometry/interface/DTGeometry.h"
#include "Geometry/RPCGeometry/interface/RPCGeometry.h"

#include <cmath> // for pi

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
  std::unique_ptr<const RPCRoll>  roll(_georpc->roll(id));
  uint16_t strip = tp.getRPCData().strip;
  LocalPoint lp = roll->centreOfStrip(strip);
  GlobalPoint gp = roll->toGlobal(lp);

  roll.release();
 
  return gp;
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


// alot of this is transcription and consolidation of the CSC
// global phi calculation code
// this works directly with the geometry 
// rather than using the old phi luts
GlobalPoint 
GeometryTranslator::getCSCSpecificPoint(const TriggerPrimitive& tp) const {
  const CSCDetId id(tp.detId<CSCDetId>()); 
  // we should change this to weak_ptrs at some point
  // requires introducing std::shared_ptrs to geometry
  std::unique_ptr<const CSCChamber> chamb(_geocsc->chamber(id));
  std::unique_ptr<const CSCLayerGeometry> layer_geom(
    chamb->layer(CSCConstants::KEY_ALCT_LAYER)->geometry()
    );
  std::unique_ptr<const CSCLayer> layer(
    chamb->layer(CSCConstants::KEY_ALCT_LAYER)
    );
  
  const uint16_t halfstrip = tp.getCSCData().strip;
  const uint16_t pattern = tp.getCSCData().pattern;
  const uint16_t keyWG = tp.getCSCData().keywire; 
  //const unsigned maxStrips = layer_geom->numberOfStrips();  

  // so we can extend this later 
  // assume TMB2007 half-strips only as baseline
  double offset = 0.0;
  switch(1) {
  case 1:
    offset = CSCPatternLUT::get2007Position(pattern);
  }
  const unsigned halfstrip_offs = unsigned(0.5 + halfstrip + offset);
  const unsigned strip = halfstrip_offs/2 + 1; // geom starts from 1

  // the rough location of the hit at the ALCT key layer
  // we will refine this using the half strip information
  const LocalPoint coarse_lp = 
    layer_geom->stripWireGroupIntersection(strip,keyWG);  
  const GlobalPoint coarse_gp = layer->surface().toGlobal(coarse_lp);  
  
  // the strip width/4.0 gives the offset of the half-strip
  // center with respect to the strip center
  const double hs_offset = layer_geom->stripPhiPitch()/4.0;
  
  // determine if we need to add or subtract the half-strip center
  const bool ccw = isCSCCounterClockwise(layer);
  const double phi_offset = ( ccw ? -hs_offset : hs_offset );
  
  // the global eta calculation uses the middle of the strip
  // so no need to increment it
  const GlobalPoint final_gp( GlobalPoint::Polar( coarse_gp.theta(),
						  coarse_gp.phi() + phi_offset,
						  coarse_gp.theta() ) );
    
  // We need to add in some notion of the 'error' on trigger primitives
  // like the width of the wire group by the width of the strip
  // or something similar      

  // release ownership of the pointers
  chamb.release();
  layer_geom.release();
  layer.release();
  
  return final_gp;
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

bool GeometryTranslator::
isCSCCounterClockwise(const std::unique_ptr<const CSCLayer>& layer) const {
  const int nStrips = layer->geometry()->numberOfStrips();
  const double phi1 = layer->centerOfStrip(1).phi();
  const double phiN = layer->centerOfStrip(nStrips).phi();
  return ( (std::abs(phi1 - phiN) < M_PI  && phi1 >= phiN) || 
	   (std::abs(phi1 - phiN) >= M_PI && phi1 < phiN)     );  
}
