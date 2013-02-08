

#include "DataFormats/MuonDetId/interface/CSCTriggerNumbering.h"

#include "DataFormats/MuonDetId/interface/CSCDetId.h"
#include "DataFormats/MuonDetId/interface/RPCDetId.h"
#include "DataFormats/MuonDetId/interface/DTChamberId.h"

#include "L1Trigger/L1IntegratedMuonTrigger/interface/helpers.h"

namespace L1ITMu {
  namespace helpers {
    
    TriggerPrimitiveList 
    getPrimitivesByCSCTriggerInfo(const int endcap,
				  const int sector,
				  const TriggerPrimitiveCollection& tps,
				  const std::vector<unsigned>& trkNmbs) {
      TriggerPrimitiveList result;
      auto tp = tps.cbegin();
      auto tbeg = tps.cbegin();
      auto tend = tps.cend();
      
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
		result.push_back(TriggerPrimitiveRef(&tps,tp - tbeg));
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
		result.push_back(TriggerPrimitiveRef(&tps,tp-tbeg));
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

  }
}
