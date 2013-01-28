// 
// Class: L1ITMu::GeometryTranslator
//
// Info: This class implements a the translations from packed bits or
//       digi information into local or global CMS coordinates for all
//       types of L1 trigger primitives that we want to consider for
//       use in the integrated muon trigger.
//       
// Note: This should be considered as a base class to some sort of global
//       look-up table
//
// Author: L. Gray (FNAL)
// Some pieces of code lifted from: Matt Carver & Bobby Scurlock (UF)
//

#include "FWCore/Framework/interface/ESHandle.h"

// forwards
namespace edm {  
  class EventSetup;
}

class RPCGeometry;
class CSCGeometry;
class DTGeometry;

namespace L1ITMu{
  class TriggerPrimitive;

  class GeometryTranslator {
  public:
    GeometryTranslator() : _geom_cache_id(0ULL) {}

    double calculateGlobalEta(const TriggerPrimitive&) const;
    double calculateGlobalPhi(const TriggerPrimitive&) const;
    double calculateBendAngle(const TriggerPrimitive&) const;    

    void checkAndUpdateGeometry(const edm::EventSetup&);

  private:
    // pointers to the current geometry records
    edm::ESHandle<RPCGeometry> _georpc;    
    edm::ESHandle<CSCGeometry> _geocsc;    
    edm::ESHandle<DTGeometry>  _geodt;
    unsigned long long _geom_cache_id;
    
    double calcRPCSpecificEta(const TriggerPrimitive&) const;
    double calcRPCSpecificPhi(const TriggerPrimitive&) const;
    double calcRPCSpecificBend(const TriggerPrimitive&) const;

    double calcCSCSpecificEta(const TriggerPrimitive&) const;
    double calcCSCSpecificPhi(const TriggerPrimitive&) const;
    double calcCSCSpecificBend(const TriggerPrimitive&) const;

    double calcDTSpecificEta(const TriggerPrimitive&) const;
    double calcDTSpecificPhi(const TriggerPrimitive&) const;
    double calcDTSpecificBend(const TriggerPrimitive&) const;
  };
}
