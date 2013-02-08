#ifndef __L1ITMU_INTERNALTRACKFWD_H__
#define __L1ITMU_INTERNALTRACKFWD_H__

#include <vector>
#include "DataFormats/Common/interface/Ref.h"
#include "DataFormats/Common/interface/Ptr.h"
namespace L1ITMu {
  class InternalTrack;

  typedef std::vector<InternalTrack> InternalTrackCollection;
  typedef edm::Ref<InternalTrackCollection> InternalTrackRef;
  typedef edm::Ptr<InternalTrack> InternalTrackPtr;
}

#endif
