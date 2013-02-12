// 
// Class: L1RPCTFTrackConverter
//
// Info: This producer eats RPCTF tracks (pre GMT) and matches them to 
//       L1ITMu::TriggerPrimitives. In the process of doing so it
//       converts the RPCTF tracks into a collection L1ITMu::InternalTrack
//       each of which contains the track stubs it was matched to.
//
// Author: L. Gray (FNAL)
//

#include "L1Trigger/L1IntegratedMuonTrigger/interface/InternalTrackFwd.h"
#include "L1Trigger/L1IntegratedMuonTrigger/interface/InternalTrack.h"

#include "L1Trigger/L1IntegratedMuonTrigger/interface/TriggerPrimitiveFwd.h"
#include "L1Trigger/L1IntegratedMuonTrigger/interface/TriggerPrimitive.h"

#include "L1Trigger/L1IntegratedMuonTrigger/interface/RegionalTracksFwd.h"
#include "DataFormats/L1GlobalMuonTrigger/interface/L1MuRegionalCand.h"

#include "DataFormats/RPCDigi/interface/RPCDigiL1Link.h"

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Utilities/interface/InputTag.h"

using namespace L1ITMu;

typedef edm::ParameterSet PSet;

class L1RPCTFTrackConverter : public edm::EDProducer {
public:
  L1RPCTFTrackConverter(const PSet&);
  ~L1RPCTFTrackConverter() {}

  void produce(edm::Event&, const edm::EventSetup&);  
private:
  edm::InputTag _rpcTrackSrc, _rpcLinkSrc, _trigPrimSrc;
};

L1RPCTFTrackConverter::L1RPCTFTrackConverter(const PSet& ps):
  _rpcTrackSrc(ps.getParameter<edm::InputTag>("RPCTrackSrc")),
  _rpcLinkSrc(ps.getParameter<edm::InputTag>("RPCL1LinkSrc")),
  _trigPrimSrc(ps.getParameter<edm::InputTag>("TriggerPrimitiveSrc")) {
  produces<InternalTrackCollection>();
}

void L1RPCTFTrackConverter::produce(edm::Event& ev, 
				    const edm::EventSetup& es) {
  std::auto_ptr<InternalTrackCollection> 
    convertedTracks (new InternalTrackCollection());
  std::auto_ptr<RegionalCandCollection>
    inputTracks(new RegionalCandCollection);

  edm::RefProd<RegionalCandCollection> rpcpacProd = 
    ev.getRefBeforePut<RegionalCandCollection>("input");  

  edm::Handle<std::vector<RPCDigiL1Link> > rpclinks;
  ev.getByLabel(_rpcLinkSrc,rpclinks);

  edm::Handle<RegionalCandCollection> rpctracks;
  ev.getByLabel(_rpcTrackSrc,rpctracks);

  edm::Handle<TriggerPrimitiveCollection> trigPrims;
  ev.getByLabel(_trigPrimSrc,trigPrims);

  assert(rpclinks.size() == rpctracks.size() && ""

  ev.put(inputTracks,"input");
  ev.put(convertedTracks);
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(L1RPCTFTrackConverter);
