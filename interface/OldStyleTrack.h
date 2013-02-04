#ifndef __L1ITMU_OLDSTYLETRACK_H__
#define __L1ITMU_OLDSTYLETRACK_H__
// 
// Class: L1ITMu::OldStyleTrack
//
// Info: This class implements a common layer on top of the existing 
//       level-one trigger track types (CSC,DT,RPC) and allows you
//       add edm::Refs to L1ITMu::TriggerPrimitives to the track
//       so that additional delta phi and other such information
//       can be added.
// TODO: add in containers for the various old track types that are
//       internal to this piece of code
//
// Author: L. Gray (FNAL)
//

#include "DataFormats/Common/interface/RefVector.h"

namespace L1ITMu{
  class OldStyleTrack {
  public:
    OldStyleTrack() {}
  private:
    edm::RefVector<TriggerPrimitiveCollection> _associatedStubs;
  };
}

#endif
