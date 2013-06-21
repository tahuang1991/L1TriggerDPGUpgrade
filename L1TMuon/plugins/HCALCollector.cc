#include "L1TriggerDPGUpgrade/L1TMuon/interface/HCALCollector.h"
#include "DataFormats/HcalDigi/interface/HcalTriggerPrimitiveDigi.h"
#include "DataFormats/HcalDigi/interface/HcalDigiCollections.h"
#include "DataFormats/HcalDetId/interface/HcalTrigTowerDetId.h"

#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"

using namespace L1TMuon;

HCALCollector::HCALCollector( const edm::ParameterSet& ps ):
  SubsystemCollector(ps) {
}

void HCALCollector::
extractPrimitives(const edm::Event& ev, 
		  const edm::EventSetup& es, 
		  std::vector<TriggerPrimitive>& out) const {
  edm::Handle<HcalTrigPrimDigiCollection> hcalDigis;  
  ev.getByLabel(_src,hcalDigis);

  auto chamber = hcalDigis->begin();
  auto chend  = hcalDigis->end();
  // simple flag: save digi if at least one sample is not empty
  // check at some point that SampleOfInterest is the non-zero one!
  bool save;
  for( ; chamber != chend; ++chamber ) {
    save = false;
    
    for(int i=0 ; i < (*chamber).size(); ++i ) {
      if ((*chamber).sample(i).compressedEt()!=0) {
	save = true; break;
      }  
    }
    
    if (save)
      out.push_back(TriggerPrimitive((*chamber).id(),
				     (*chamber)));
    
  }
}

#include "L1TriggerDPGUpgrade/L1TMuon/interface/SubsystemCollectorFactory.h"
DEFINE_EDM_PLUGIN( SubsystemCollectorFactory, HCALCollector, "HCALCollector");
