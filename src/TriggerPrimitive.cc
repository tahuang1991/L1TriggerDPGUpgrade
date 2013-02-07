#include "L1Trigger/L1IntegratedMuonTrigger/interface/TriggerPrimitive.h"

// the primitive types we can use
#include "DataFormats/CSCDigi/interface/CSCCorrelatedLCTDigi.h"
#include "DataFormats/L1DTTrackFinder/interface/L1MuDTChambPhDigi.h"
#include "DataFormats/L1DTTrackFinder/interface/L1MuDTChambThDigi.h"
#include "DataFormats/RPCDigi/interface/RPCDigiL1Link.h"

// detector ID types
#include "DataFormats/MuonDetId/interface/DTChamberId.h"
#include "DataFormats/MuonDetId/interface/CSCDetId.h"
#include "DataFormats/MuonDetId/interface/RPCDetId.h"

using namespace L1ITMu;

//constructors from DT data
TriggerPrimitive::TriggerPrimitive(const DTChamberId& detid,
				   const L1MuDTChambPhDigi& digi_phi,
				   const int segment_number):
  _id(detid),
  _subsystem(TriggerPrimitive::kDT) {
  calculateDTGlobalSector(detid,_globalsector,_subsector);
  // fill in information from theta trigger
  _dt.theta_bti_group = -1;
  _dt.segment_number = segment_number;  
  _dt.theta_code = -1;
  _dt.theta_quality = -1;
  // now phi trigger
  _dt.bx = digi_phi.bxNum();
  _dt.wheel = digi_phi.whNum();
  _dt.sector = digi_phi.scNum();
  _dt.station = digi_phi.stNum();
  _dt.radialAngle = digi_phi.phi();
  _dt.bendingAngle = digi_phi.phiB();
  _dt.qualityCode = digi_phi.code();
  _dt.Ts2TagCode = digi_phi.Ts2Tag();
  _dt.BxCntCode = digi_phi.BxCnt();  
}

TriggerPrimitive::TriggerPrimitive(const DTChamberId& detid,
				   const L1MuDTChambPhDigi& digi_phi,
				   const L1MuDTChambThDigi& digi_th,
				   const int theta_bti_group):
  _id(detid),
  _subsystem(TriggerPrimitive::kDT) {
  calculateDTGlobalSector(detid,_globalsector,_subsector);
  // fill in information from theta trigger
  _dt.theta_bti_group = theta_bti_group;
  _dt.segment_number = digi_th.position(theta_bti_group);
  _dt.theta_code = digi_th.code(theta_bti_group);
  _dt.theta_quality = digi_th.quality(theta_bti_group);
  // now phi trigger
  _dt.bx = digi_phi.bxNum();
  _dt.wheel = digi_phi.whNum();
  _dt.sector = digi_phi.scNum();
  _dt.station = digi_phi.stNum();
  _dt.radialAngle = digi_phi.phi();
  _dt.bendingAngle = digi_phi.phiB();
  _dt.qualityCode = digi_phi.code();
  _dt.Ts2TagCode = digi_phi.Ts2Tag();
  _dt.BxCntCode = digi_phi.BxCnt();    
}

//constructor from CSC data
TriggerPrimitive::TriggerPrimitive(const CSCDetId& detid,
				   const CSCCorrelatedLCTDigi& digi):
  _id(detid),
  _subsystem(TriggerPrimitive::kCSC) {
  calculateCSCGlobalSector(detid,_globalsector,_subsector);
  _csc.trknmb  = digi.getTrknmb();
  _csc.valid   = digi.isValid();
  _csc.quality = digi.getQuality();
  _csc.keywire = digi.getKeyWG();
  _csc.strip   = digi.getStrip();
  _csc.pattern = digi.getPattern();
  _csc.bend    = digi.getBend();
  _csc.bx      = digi.getBX();
  _csc.mpclink = digi.getMPCLink();
  _csc.bx0     = digi.getBX0();
  _csc.syncErr = digi.getSyncErr();
  _csc.cscID   = digi.getCSCID();
}

// constructor from RPC data
TriggerPrimitive::TriggerPrimitive(const RPCDetId& detid,
				   const unsigned strip,
				   const unsigned layer,
				   const uint16_t bx):
  _id(detid),
  _subsystem(TriggerPrimitive::kRPC) {
  calculateRPCGlobalSector(detid,_globalsector,_subsector);
  _rpc.strip = strip;
  _rpc.layer = layer;
  _rpc.bx = bx;
}

TriggerPrimitive::TriggerPrimitive(const TriggerPrimitive& tp):
  _dt(tp._dt),
  _csc(tp._csc),
  _rpc(tp._rpc),
  _id(tp._id),
  _subsystem(tp._subsystem),  
  _globalsector(tp._globalsector),
  _subsector(tp._subsector),
  _eta(tp._eta),
  _phi(tp._phi),
  _theta(tp._theta){
}

TriggerPrimitive& TriggerPrimitive::operator=(const TriggerPrimitive& tp) {
  this->_dt = tp._dt;
  this->_csc = tp._csc;
  this->_rpc = tp._rpc;
  this->_id = tp._id;
  this->_subsystem = tp._subsystem;
  this->_globalsector = tp._globalsector;
  this->_subsector = tp._subsector;
  this->_eta = tp._eta;
  this->_phi = tp._phi;
  return *this;
}

const int TriggerPrimitive::getBX() const {
  switch(_subsystem) {
  case kDT:
    return _dt.bx;
  case kCSC:
    return _csc.bx;
  case kRPC:
    return _rpc.bx;
  default:
    throw cms::Exception("Invalid Subsytem") 
      << "The specified subsystem for this track stub is out of range"
      << std::endl;
  }
  return -1;
}

void TriggerPrimitive::calculateDTGlobalSector(const DTChamberId& chid, 
					       unsigned& global_sector, 
					       unsigned& subsector ) {
}

void TriggerPrimitive::calculateCSCGlobalSector(const CSCDetId& chid, 
						unsigned& global_sector, 
						unsigned& subsector ) {
}

void TriggerPrimitive::calculateRPCGlobalSector(const RPCDetId& chid, 
						unsigned& global_sector, 
						unsigned& subsector ) {
}
