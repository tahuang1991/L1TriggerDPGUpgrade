#include "L1Trigger/L1IntegratedMuonTrigger/interface/RPCCollector.h"
#include "DataFormats/RPCDigi/interface/RPCDigiL1Link.h"
#include "DataFormats/MuonDetId/interface/RPCDetId.h"

#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"

using namespace L1ITMu;

RPCCollector::RPCCollector( const edm::ParameterSet& ps ):
  SubsystemCollector(ps) {
}

void RPCCollector::
extractPrimitives(const edm::Event& ev, 
		  const edm::EventSetup& es, 
		  std::vector<TriggerPrimitive>& out) const {
  edm::Handle<std::vector<RPCDigiL1Link> > rpcDigis;  
  ev.getByLabel(_src,rpcDigis);
  
  auto digi = rpcDigis->begin();
  auto end  = rpcDigis->end();

  for( ; digi != end; ++digi ) {
    for( unsigned layer = 1; layer <= digi->nlayer(); ++layer ){      
      if( ! digi->empty(layer) ) {
	RPCDetId id(digi->rawdetId(layer));
	out.push_back(TriggerPrimitive(id,
				       digi->strip(layer),
				       layer,
				       digi->bx(layer))
		      );
      }
    }
  }
}

#include "L1Trigger/L1IntegratedMuonTrigger/interface/SubsystemCollectorFactory.h"
DEFINE_EDM_PLUGIN( SubsystemCollectorFactory, RPCCollector, "RPCCollector");
