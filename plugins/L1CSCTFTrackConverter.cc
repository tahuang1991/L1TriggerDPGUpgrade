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

#include "DataFormats/L1CSCTrackFinder/interface/L1CSCTrackCollection.h"

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "L1Trigger/L1IntegratedMuonTrigger/interface/helpers.h"

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

  edm::Handle<L1CSCTrackCollection> cscTracks;
  ev.getByLabel(_cscTrackSrc,cscTracks);
  
  edm::Handle<TriggerPrimitiveCollection> trigPrims;
  ev.getByLabel(_trigPrimSrc,trigPrims);
  
  auto btrk = cscTracks->cbegin();
  auto etrk = cscTracks->cend();
  for( ; btrk != etrk; ++btrk ) {
    InternalTrack trk(btrk->first);
    
    std::cout << "Track endcap:" << btrk->first.endcap() 
	      << " sector: " << btrk->first.sector() << std::endl;
    std::cout << "ME 1: " << btrk->first.me1ID() << std::endl 
	      << "ME 2: " << btrk->first.me2ID() << std::endl
	      << "ME 3: " << btrk->first.me3ID() << std::endl
	      << "ME 4: " << btrk->first.me4ID() << std::endl
	      << "MB 1: " << btrk->first.mb1ID() << std::endl;

    std::vector<unsigned> trkNmbs;
    trkNmbs.reserve(5);
    trkNmbs.push_back(btrk->first.me1ID());
    trkNmbs.push_back(btrk->first.me2ID());
    trkNmbs.push_back(btrk->first.me3ID());
    trkNmbs.push_back(btrk->first.me4ID());
    trkNmbs.push_back(btrk->first.mb1ID());

    TriggerPrimitiveList tplist =
      helpers::getPrimitivesByCSCTriggerInfo(btrk->first.endcap(),
					     btrk->first.sector(),
					     *trigPrims,
					     trkNmbs);
    //assert(tplist.size() <= 5);
    
    auto stub = tplist.cbegin();
    auto stend = tplist.cend();
    for( ; stub != stend; ++stub ) {
      (*stub)->print(std::cout);
    }

  }

  ev.put(convertedTracks);
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(L1CSCTFTrackConverter);
