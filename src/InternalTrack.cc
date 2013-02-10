#include "L1Trigger/L1IntegratedMuonTrigger/interface/InternalTrack.h"

#include "DataFormats/L1DTTrackFinder/interface/L1MuDTTrackCand.h"
#include "DataFormats/L1CSCTrackFinder/interface/L1Track.h"

#include "DataFormats/MuonDetId/interface/RPCDetId.h"
#include "DataFormats/MuonDetId/interface/CSCDetId.h"
#include "DataFormats/MuonDetId/interface/DTChamberId.h"

using namespace L1ITMu;

InternalTrack::InternalTrack(const L1MuDTTrackCand& dttrack):
  L1MuRegionalCand(dttrack) {
  _mode = 0;
  _wheel = dttrack.whNum();
  _endcap = (_wheel < 0) ? -1 : 1;
  _sector = dttrack.scNum()/2 + 1; // 0-11 -> 1 - 6  
}

InternalTrack::InternalTrack(const csc::L1Track& csctrack):
  L1MuRegionalCand(csctrack) {
  _mode = 0;
  _endcap = (csctrack.endcap() == 2) ? -1 : 1;
  _wheel = (_endcap < 0) ? -4 : 4;
  _sector = csctrack.sector();
}

InternalTrack::InternalTrack(const L1MuRegionalCand& rpctrack):
  L1MuRegionalCand(rpctrack) {
  _mode = 0;
}

void InternalTrack::addStub(const TriggerPrimitiveRef& stub) { 
  unsigned station;
  subsystem_offset offset;
  TriggerPrimitive::subsystem_type type = stub->subsystem();
  switch(type){
  case TriggerPrimitive::kCSC:    
    offset = kCSC;
    station = stub->detId<CSCDetId>().station();
    break;
  case TriggerPrimitive::kDT:    
    offset = kDT;
    station = stub->detId<DTChamberId>().station();
    break;
  case TriggerPrimitive::kRPC:    
    offset = kRPCb;
    if(stub->detId<RPCDetId>().region() != 0) 
      offset = kRPCf;
    station = stub->detId<RPCDetId>().station();    
    break;
  default:
    throw cms::Exception("Invalid Subsytem") 
      << "The specified subsystem for this track stub is out of range"
      << std::endl;
  }
  const unsigned shift = 4*offset + station - 1;
  const unsigned bit = 1 << shift;
   // add this track to the mode
  _mode = _mode | bit;
  if( _associatedStubs.count(offset) == 0 ) {
    _associatedStubs[shift] = TriggerPrimitiveList();
  }   
  _associatedStubs[shift].push_back(stub);
}

// this magic file contains a DT TrackClass -> mode LUT
#include "L1Trigger/DTTrackFinder/src/L1MuDTTrackAssParam.h"

void InternalTrack::print(std::ostream& out) const {
  std::cout << "Internal Track -- endcap: " << std::dec << _endcap
	    << " wheel: " << _wheel 
	    << " sector: " << _sector << std::endl;
  std::cout << "\tMode: " << std::hex  
	    << mode() << std::dec << std::endl;
  std::cout << "\tMode Breakdown: " << std::hex
	    << " DT: " << dtMode() << " RPCb: " << rpcbMode()
	    << " CSC: " << cscMode() << " RPCf: " << rpcfMode() 
	    << std::dec << std::endl;
  DTTrackRef dtparent;
  CSCTrackRef cscparent;
  switch( type_idx() ) {
  case 0: // DT    
    dtparent = _parent.castTo<DTTrackRef>();
    std::cout << "\tParent is a DT Track!" << std::endl;
    std::cout << "\t Parent Mode: " << std::hex
	      << tc2bitmap((TrackClass)dtparent->TCNum()) 
	      << std::dec << std::endl;
    std::cout << "\t  MB 1: " << dtparent->stNum(1)
	      << "\tMB 2: " << dtparent->stNum(2)
	      << "\tMB 3: " << dtparent->stNum(3)
	      << "\tMB 4: " << dtparent->stNum(4) << std::endl;
    break;
  case 1: // RPCb 
    break;
  case 2: // CSC    
    cscparent = _parent.castTo<CSCTrackRef>();
    std::cout << "\tParent is a CSC Track!" << std::endl;
    std::cout << "\t Parent Mode: " << std::hex
	      << cscparent->mode() 
	      << std::dec << std::endl
	      << "\t  ME 1: " << cscparent->me1ID() 
	      << "\tME 2: " << cscparent->me2ID() 
	      << "\tME 3: " << cscparent->me3ID() 
	      << "\tME 4: " << cscparent->me4ID() 
	      << "\tMB 1: " << cscparent->mb1ID() << std::endl;
    break;
  case 3: // RPCf
    break;
  case 4: // L1ITMu ?
    break;
  default:
    throw cms::Exception("Unknown Track Type") 
      << "L1ITMu::InternalTrack is of unknown track type: " << type_idx()
      << std::endl;
  }
}
