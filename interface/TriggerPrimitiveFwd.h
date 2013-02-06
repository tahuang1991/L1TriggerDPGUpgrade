#include <vector>
#include "DataFormats/Common/interface/Ref.h"
#include "DataFormats/Common/interface/Ptr.h"

namespace L1ITMu{
  class TriggerPrimitive;

  typedef std::vector<TriggerPrimitive> TriggerPrimitiveCollection;
  
  typedef edm::Ref<TriggerPrimitiveCollection> TriggerPrimitiveRef;
  typedef edm::Ptr<TriggerPrimitive> TriggerPrimitivePtr;
}


