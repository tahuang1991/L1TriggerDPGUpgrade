#include "L1Trigger/L1IntegratedMuonTrigger/interface/SubsystemCollector.h"

using namespace L1ITMu;

SubsystemCollector::SubsystemCollector(const edm::ParameterSet& ps):
  _src(ps.getParameter<edm::InputTag>("src")) {
}
