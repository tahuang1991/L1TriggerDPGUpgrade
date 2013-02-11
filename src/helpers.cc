

#include "DataFormats/MuonDetId/interface/CSCTriggerNumbering.h"

#include "DataFormats/MuonDetId/interface/CSCDetId.h"
#include "DataFormats/MuonDetId/interface/RPCDetId.h"
#include "DataFormats/MuonDetId/interface/DTChamberId.h"

#include "L1Trigger/L1IntegratedMuonTrigger/interface/helpers.h"

namespace {
  // DT TF relative segment address explanation
  //
  // a code of '15' (stations 2,3,4) or '3' (station 1 only)
  // means there was no valid track extrapolation in this DT station
  //
  // schematic diagram of DT codes with corresponding VHDL addresses in ():
  // the phi direction seems to be the direction with respect to the
  // track's sector processor. 
  // ( this is why station one only has addresses 1,2 )
  //         --------------------------------------
  //         |   4 (10)  5 (11) |   6 (2)  7 (3)  |   ( next sector )
  //      P  ------------+------------------------- 
  //      H  |   0 (8)   1 (9)  |   2 (0)  3 (1)  |   ( this sector )
  //      I  ------------+-------------------------
  //         |   8 (12)  9 (13) |  10 (4) 11 (5)  |   ( prev sector )
  //         ------------+-------------------------
  //               this Wheel       next Wheel
 
  bool isExtrapAcrossWheel(const int addr, const int station) {     
    if( station != 1 ) {
      switch(addr) {
      case 8:
      case 9:
      case 10:
      case 11:
      case 12:
      case 13:
	return false;
	break;
      default:
	return true;
      }
    } else {
      return !((bool)addr);
    }
    return false;
  } 
  
  int relativeSector(const int addr, const int station) {
    if( station != 1 ){
      switch(addr) {
      case 12:
      case 13:
      case 4:
      case 5:
	return -1;
	break;
      case 8:
      case 9:
      case 0:
      case 1:
	return 0;
	break;
      case 10:
      case 11:
      case 2:
      case 3:
	return 1;
	break;
      default:
	break;
      }
    }
    return 0;
  }
  
  

}

namespace L1ITMu {
  namespace helpers {
    
    TriggerPrimitiveList 
    getPrimitivesByCSCTriggerInfo(const int endcap,
				  const int sector,
			    const edm::Handle<TriggerPrimitiveCollection>& tps,
				  const std::vector<unsigned>& trkNmbs) {
      TriggerPrimitiveList result;
      auto tp = tps->cbegin();
      auto tbeg = tps->cbegin();
      auto tend = tps->cend();
      
      std::vector<unsigned>::const_iterator ista;
      auto sbeg = trkNmbs.cbegin();
      auto send = trkNmbs.cend();
      
      // the station
      int station;
      // csc chamber identifiers
      CSCDetId cscid;      
      int csector,csubsector,cendcap,cstation;
      unsigned ctrkNmb;
      // dt chamber identifiers
      DTChamberId dtid;
      int twheel, tsector, dwheel, dsector;
      //unsigned dtrkNmb;
      
      for( ; tp != tend; ++tp ) {
	for( ista = sbeg; ista != send; ++ista ) {	  
	  if( *ista == 0 ) continue; // if no stub don't process
	  station = (ista - sbeg) + 1;
	  switch( station ) {
	  case 1:
	  case 2:
	  case 3:
	  case 4:	    
	    if( tp->subsystem() == TriggerPrimitive::kCSC ) {
	      cscid = tp->detId<CSCDetId>();
	      csector = CSCTriggerNumbering::triggerSectorFromLabels(cscid);
	      csubsector = 
		CSCTriggerNumbering::triggerSubSectorFromLabels(cscid);
	      cendcap = cscid.endcap();
	      cstation = cscid.station();
	      ctrkNmb = ( tp->getCSCData().mpclink + 
			  (csubsector!=0)*(csubsector-1)*3 );	    
	      if( cendcap == endcap && cstation == station && 
		  csector == sector && ctrkNmb == *ista ) {
		result.push_back(TriggerPrimitiveRef(tps,tp - tbeg));
	      }	  
	    }
	    break;
	  case 5:
	    if( tp->subsystem() == TriggerPrimitive::kDT ) {
	      dtid = tp->detId<DTChamberId>();	      
	      if( std::abs(dtid.wheel()) != 2 && dtid.station() != 1 )
		continue;
	      twheel = ( endcap == 1 ? 2 : -2 );
	      // sectors go from 1-12
	      tsector = 2*sector + *ista -1;
	      tsector = (tsector == 13 ? 1 : tsector);
	      dwheel = dtid.wheel();
	      dsector = dtid.sector();
	      //dtrkNmb = tp->getDTData().segment_number;	      
	      if( twheel == dwheel && 
		  (dsector == tsector ) ) {
		result.push_back(TriggerPrimitiveRef(tps,tp-tbeg));
	      }
	    }	    
	    break;
	  default:
	    break;
	  }
	}	
      }
      return result;
    }

    TriggerPrimitiveList 
    getPrimitivesByDTTriggerInfo(const int wheel,
				 const int sp_wheel,
				 const int sector,
			    const edm::Handle<TriggerPrimitiveCollection>& tps,
				 const unsigned mode,
				 const std::vector<unsigned>& trkNmbs) {
      TriggerPrimitiveList result;
      auto tp = tps->cbegin();
      auto tbeg = tps->cbegin();
      auto tend = tps->cend();
      
      std::vector<unsigned>::const_iterator ista;
      auto sbeg = trkNmbs.cbegin();
      auto send = trkNmbs.cend();
      
      // the station and relative address
      int station, address;      
      // dt chamber identifiers
      DTChamberId dtid;
      int wheel_incr;
      int expectedwheel, dwheel, expectedsector, dsector;
      unsigned expectedtrkNmb,dtrkNmb;
      // csc chamber ids
      CSCDetId cscid;
      unsigned csector, csubsector;
      int csector_asdt;

      for( ; tp != tend; ++tp ) {
	for( ista = sbeg; ista != send; ++ista ) {
	  station = (ista - sbeg) + 1;
	  bool station_used = mode & ( 0x1 << (station-1) );
	  address = *ista;
	  switch( tp->subsystem() ) {
	  case TriggerPrimitive::kDT:	       	    
	    dtid = tp->detId<DTChamberId>();
	    if( !station_used || station != dtid.station() ) continue;
	    wheel_incr = (isExtrapAcrossWheel(address,station) ? 1 : 0);
	    expectedwheel = ( sp_wheel < 0 ? 
			      wheel - wheel_incr :
			      wheel + wheel_incr   );
	    
	    dwheel = dtid.wheel();
	    expectedsector = sector + relativeSector(address,station);
	    expectedsector = ( expectedsector == 0 ? 12 : expectedsector);
	    expectedsector = ( expectedsector == 13 ? 1 : expectedsector);
	    dsector = dtid.sector();
	    expectedtrkNmb = address%2 + 1;
	    dtrkNmb = tp->getDTData().segment_number;
	    
	    if( expectedsector == dsector &&
		expectedwheel  == dwheel  && 
		expectedtrkNmb == dtrkNmb    ) {
	      result.push_back(TriggerPrimitiveRef(tps,tp - tbeg));
	    }
	    break;
	  case TriggerPrimitive::kCSC:
	    cscid = tp->detId<CSCDetId>();
	    // see if the track expects a segment in "wheel 4" which is 
	    // CSC station one	    
	    wheel_incr = (isExtrapAcrossWheel(address,station) ? 1 : 0);
	    expectedwheel = ( sp_wheel < 0 ? 
			      wheel - wheel_incr :
			      wheel + wheel_incr   );	    
	    // station 1 in CSCs is 3 in DTs for the trigger	    
	    // the relative address for CSC segments is always 0
	    // matching endcap to DT SP wheel means product > 0
	    // mode bits still apply so we can lazy continue
	    if(  !station_used || station != 3 || 
		 cscid.station() != 1 || address != 0  ) continue;
	     csector = CSCTriggerNumbering::triggerSectorFromLabels(cscid);
	     csubsector = 
	       CSCTriggerNumbering::triggerSubSectorFromLabels(cscid);
	     csector_asdt = 2*csector + csubsector - 1;
	     expectedsector = sector + relativeSector(address,station);
	     expectedsector = ( expectedsector == 0 ? 12 : expectedsector);
	     expectedsector = ( expectedsector == 13 ? 1 : expectedsector);
	     std::cout << " DT - CSC match: " << expectedsector 
		       << ' ' << csector_asdt << std::endl;
	     if( expectedsector == csector_asdt ) {
	       result.push_back(TriggerPrimitiveRef(tps,tp - tbeg));
	     }
	    break;
	  default: // don't care about RPCs
	    continue;
	  }		  	   
	}	
      }
      return result;
    }

  }
}
