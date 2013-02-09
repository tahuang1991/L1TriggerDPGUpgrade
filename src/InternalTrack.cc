#include "L1Trigger/L1IntegratedMuonTrigger/interface/InternalTrack.h"

#include "DataFormats/L1DTTrackFinder/interface/L1MuDTTrackCand.h"
#include "DataFormats/L1CSCTrackFinder/interface/L1Track.h"

#include "DataFormats/MuonDetId/interface/RPCDetId.h"
#include "DataFormats/MuonDetId/interface/CSCDetId.h"
#include "DataFormats/MuonDetId/interface/DTChamberId.h"

using namespace L1ITMu;

InternalTrack::InternalTrack(const L1MuDTTrackCand& dttrack):
  L1MuRegionalCand(dttrack) {
  _wheel = dttrack.whNum();
  _endcap = (_wheel < 0) ? -1 : 1;
  _sector = dttrack.scNum()/2 + 1; // 0-11 -> 1 - 6  
}

InternalTrack::InternalTrack(const csc::L1Track& csctrack):
  L1MuRegionalCand(csctrack) {  
  _endcap = (csctrack.endcap() == 2) ? -1 : 1;
  _wheel = (_endcap < 0) ? -4 : 4;
  _sector = csctrack.sector();
}

InternalTrack::InternalTrack(const L1MuRegionalCand& rpctrack):
  L1MuRegionalCand(rpctrack) {
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
   // add this track to the mode
  _mode = _mode | ( 1 << (4*offset + (station - 1)) );
  if( _associatedStubs.count(station << 4*offset ) == 0 ) {
    _associatedStubs[station << 4*offset] = TriggerPrimitiveList();
  }   
  _associatedStubs[station << 4*offset].push_back(stub);
}

void InternalTrack::print(std::ostream& out) const {
  std::cout << "Internal Track -- endcap: " << std::dec << _endcap
	    << " wheel: " << _wheel 
	    << " sector: " << _sector << std::endl;
}
