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
				   const L1MuDTChambPhDigi& digi):
  _id(detid),
  _subsystem(TriggerPrimitive::kDT) {
  calculateDTGlobalSector(detid,_globalsector,_subsector);
}

TriggerPrimitive::TriggerPrimitive(const DTChamberId& detid,
				   const L1MuDTChambThDigi& digi):
  _id(detid),
  _subsystem(TriggerPrimitive::kDT) {
  calculateDTGlobalSector(detid,_globalsector,_subsector);  
}

TriggerPrimitive::TriggerPrimitive(const DTChamberId& detid,
				   const L1MuDTChambPhDigi& digi_phi,
				   const L1MuDTChambThDigi& digi_th):
  _id(detid),
  _subsystem(TriggerPrimitive::kDT) {
  calculateDTGlobalSector(detid,_globalsector,_subsector);  
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
