

#include "DataFormats/MuonDetId/interface/CSCTriggerNumbering.h"

#include "DataFormats/MuonDetId/interface/CSCDetId.h"
#include "DataFormats/MuonDetId/interface/RPCDetId.h"
#include "DataFormats/MuonDetId/interface/DTChamberId.h"

#include "L1Trigger/L1IntegratedMuonTrigger/interface/helpers.h"

namespace {
  // note: this helper function has been reverse engineered and 
  //       may not be entirely correct
  // from the DT track is the VHDL address
  // not the raw address
  // for raw addresses (addr/2)%2 == 0 means extrapolation in
  // same wheel (not that the track is in the same wheel as the segment)  
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
      int twheel, tfirstsector, dwheel, dsector;
      unsigned dtrkNmb;
      
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
	      tfirstsector = 2*sector - 1;
	      dwheel = dtid.wheel();
	      dsector = dtid.sector();
	      dtrkNmb = tp->getDTData().segment_number;	      
	      if( twheel == dwheel && 
		  (dsector == tfirstsector || dsector == tfirstsector+1) &&
		  dtrkNmb == *ista ) {
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
      int dwheel, dsector,calcwheel,calcsector;
      unsigned dtrkNmb;
      
      for( ; tp != tend; ++tp ) {
	for( ista = sbeg; ista != send; ++ista ) {
	  if( tp->subsystem() != TriggerPrimitive::kDT ) continue;
	  station = (ista - sbeg) + 1;
	  bool station_used = mode & ( 0x1 << (station-1) );
	  dtid = tp->detId<DTChamberId>();
	  if( station_used && station == dtid.station() ) {	    
	    
	    address = *ista;
	    calcwheel = wheel + (int)isExtrapAcrossWheel(address,station);
	    dtrkNmb = address%2 + 1;
	    
	    std::cout <<"Track wheel" << sp_wheel  
		      <<" DT wheel: " << wheel 
		      << " cross-wheel extrap : " 
		      << isExtrapAcrossWheel(address,station)
		      << " exp. trk. #: " << dtrkNmb
		      << " sector: " << sector 
		      << " station:address " 
		      << station << ":" << address << std::endl;
	    std::cout << "Segment DetId: " << dtid << std::endl;
	    tp->print(std::cout);
	    if( false ) {
	      result.push_back(TriggerPrimitiveRef(tps,tp - tbeg));
	    }	
	  }		  	   
	}	
      }
      return result;
    }

  }
}
