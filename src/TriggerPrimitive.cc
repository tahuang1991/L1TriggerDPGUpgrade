#include "L1Trigger/L1IntegratedMuonTrigger/interface/TriggerPrimitive.h"

// the primitive types we can use
#include "DataFormats/CSCDigi/interface/CSCCorrelatedLCTDigi.h"
#include "DataFormats/L1DTTrackFinder/interface/L1MuDTChambPhDigi.h"
#include "DataFormats/L1DTTrackFinder/interface/L1MuDTChambThDigi.h"
#include "DataFormats/RPCDigi/interface/RPCDigi.h"

// detector ID types
#include "DataFormats/MuonDetId/interface/DTChamberId.h"
#include "DataFormats/MuonDetId/interface/CSCDetId.h"
#include "DataFormats/MuonDetId/interface/RPCDetId.h"

using namespace L1ITMu;

//constructors from DT data
TriggerPrimitive::TriggerPrimitive(const DTChamberId& detid,
				   const L1MuDTChambPhDigi& digi) {
}

TriggerPrimitive::TriggerPrimitive(const DTChamberId& detid,
				   const L1MuDTChambThDigi& digi) {
}

//constructor from CSC data
TriggerPrimitive::TriggerPrimitive(const CSCDetId& detid,
				   const CSCCorrelatedLCTDigi& digi) {
}

// constructor from RPC data
TriggerPrimitive::TriggerPrimitive(const RPCDetId& detid,
				   const RPCDigi& digi) {
}

