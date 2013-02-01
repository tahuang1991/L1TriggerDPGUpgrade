// 
// Class: L1ITMuTriggerPrimitiveProducer
//
// Info: This producer runs the subsystem collectors for each subsystem
//       specified in its configuration file.
//       It produces a concatenated common trigger primitive list
//       as well as the trigger primitives for each subsystem.
//
// Author: L. Gray (FNAL)
//

#include <memory>
#include <map>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "L1Trigger/L1IntegratedMuonTrigger/interface/TriggerPrimitive.h"
#include "L1Trigger/L1IntegratedMuonTrigger/interface/TriggerPrimitiveFwd.h"

#include "L1Trigger/L1IntegratedMuonTrigger/interface/SubsystemCollectorFactory.h"

using namespace L1ITMu;

typedef std::vector<std::string> vstring;
typedef std::map<std::string,std::unique_ptr<SubsystemCollector> > collector_list;
typedef std::unique_ptr<SubsystemCollector> collector_ptr;
typedef edm::ParameterSet PSet;

class L1ITMuTriggerPrimitiveProducer : public edm::EDProducer {
public:
  L1ITMuTriggerPrimitiveProducer(const PSet&);
  virtual ~L1ITMuTriggerPrimitiveProducer() {}

  void produce(edm::Event&, const edm::EventSetup&);  
private:
  collector_list collectors;
};

L1ITMuTriggerPrimitiveProducer::L1ITMuTriggerPrimitiveProducer(const PSet& p) {
  std::unique_ptr<SubsystemCollectorFactory> 
    factory(SubsystemCollectorFactory::get());
  
  vstring psetNames;
  p.getParameterSetNames(psetNames);
  auto name = psetNames.cbegin();
  auto nend = psetNames.cend();
  for( ; name != nend; ++name ) {
    PSet collector_cfg = p.getParameterSet(*name);
    std::string collector_type = 
      collector_cfg.getParameter<std::string>("collectorType");
    collectors[*name] = collector_ptr( factory->create( collector_type,
							collector_cfg  ) );
    produces<TriggerPrimitiveCollection>(*name);
  } 
  factory.release();
  produces<TriggerPrimitiveCollection>();
}

void L1ITMuTriggerPrimitiveProducer::produce(edm::Event& ev, 
					     const edm::EventSetup& es) {
  std::auto_ptr<TriggerPrimitiveCollection> 
    master_out(new TriggerPrimitiveCollection);

  auto coll_itr = collectors.cbegin();
  auto cend = collectors.cend();
  
  for( ; coll_itr != cend; ++coll_itr ) {
    std::auto_ptr<TriggerPrimitiveCollection> 
      subs_out;
    auto& collector = coll_itr->second;
    
    collector->extractPrimitives(ev,es,*subs_out);
    
    master_out->insert(master_out->end(),
		       subs_out->begin(),
		       subs_out->end());

    ev.put(subs_out,coll_itr->first);
  }

  ev.put(master_out);
}
