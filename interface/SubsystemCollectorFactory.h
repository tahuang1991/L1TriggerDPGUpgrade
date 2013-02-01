#ifndef __L1ITMU_SUBSYSTEMCOLLECTORFACTORY_H__
#define __L1ITMU_SUBSYSTEMCOLLECTORFACTORY_H__

// 
// Class: L1ITMu::SubsystemCollectorFactory
//
// Info: Factory that produces a specified type of SubsystemCollector
//
// Author: L. Gray (FNAL)
//

#include "L1Trigger/L1IntegratedMuonTrigger/interface/SubsystemCollector.h"
#include "FWCore/PluginManager/interface/PluginFactory.h"

namespace L1ITMu {
  typedef 
    edmplugin::PluginFactory<SubsystemCollector*(const edm::ParameterSet&)>
    SubsystemCollectorFactory;
}

#endif
