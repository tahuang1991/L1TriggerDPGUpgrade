// 
// Class: L1CSCTFTrackConverter
//
// Info: This producer eats CSCTF tracks (pre GMT) and matches them to 
//       L1ITMu::TriggerPrimitives. In the process of doing so it
//       converts the CSCTF tracks into a collection L1ITMu::InternalTrack
//       each of which contains the track stubs it was matched to.
//
// Author: L. Gray (FNAL)
//

#include "L1Trigger/L1IntegratedMuonTrigger/interface/InternalTrackFwd.h"
#include "L1Trigger/L1IntegratedMuonTrigger/interface/InternalTrack.h"

#include "L1Trigger/L1IntegratedMuonTrigger/interface/TriggerPrimitiveFwd.h"
#include "L1Trigger/L1IntegratedMuonTrigger/interface/TriggerPrimitive.h"

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Utilities/interface/InputTag.h"

using namespace L1ITMu;

typedef edm::ParameterSet PSet;

class L1CSCTFTrackConverter : public edm::EDProducer {
public:
  L1CSCTFTrackConverter(const PSet&);
  ~L1CSCTFTrackConverter() {}

  void produce(edm::Event&, const edm::EventSetup&);  
private:
  edm::InputTag _cscTrackSrc, _trigPrimSrc;
};

L1CSCTFTrackConverter::L1CSCTFTrackConverter(const PSet& ps):
  _cscTrackSrc(ps.getParameter<edm::InputTag>("CSCTrackSrc")),
  _trigPrimSrc(ps.getParameter<edm::InputTag>("TriggerPrimitiveSrc")) {
  produces<InternalTrackCollection>();
}

void L1CSCTFTrackConverter::produce(edm::Event& ev, 
				    const edm::EventSetup& es) {
  std::auto_ptr<InternalTrackCollection> 
    convertedTracks (new InternalTrackCollection());

  edm::Handle<std::vector<csc::L1Track> > cscTracks;
  ev.getByLabel(_cscTrackSrc,cscTracks);

  

  ev.put(convertedTracks);
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(L1CSCTFTrackConverter);
