// 
// Class: L1DTTFTrackConverter
//
// Info: This producer eats DTTF tracks (pre GMT) and matches them to 
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

#include "DataFormats/L1DTTrackFinder/interface/L1MuDTTrackContainer.h"

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "L1Trigger/L1IntegratedMuonTrigger/interface/helpers.h"

using namespace L1ITMu;

typedef edm::ParameterSet PSet;

class L1DTTFTrackConverter : public edm::EDProducer {
public:
  L1DTTFTrackConverter(const PSet&);
  ~L1DTTFTrackConverter() {}

  void produce(edm::Event&, const edm::EventSetup&);  
private:
  edm::InputTag _dtTrackSrc, _trigPrimSrc;
  const int min_bx, max_bx;
};

L1DTTFTrackConverter::L1DTTFTrackConverter(const PSet& ps):
  _dtTrackSrc(ps.getParameter<edm::InputTag>("DTTrackSrc")),
  _trigPrimSrc(ps.getParameter<edm::InputTag>("TriggerPrimitiveSrc")),
  min_bx(ps.getParameter<int>("MinBx")),
  max_bx(ps.getParameter<int>("MaxBx")) {
  produces<InternalTrackCollection>();
}

void L1DTTFTrackConverter::produce(edm::Event& ev, 
				    const edm::EventSetup& es) {
  std::auto_ptr<InternalTrackCollection> 
    convertedTracks (new InternalTrackCollection());

  edm::Handle<L1MuDTTrackContainer> dtTracks;
  ev.getByLabel(_dtTrackSrc,dtTracks);
  
  edm::Handle<TriggerPrimitiveCollection> trigPrims;
  ev.getByLabel(_trigPrimSrc,trigPrims);
    
  for( int wheel = -2 ; wheel <= 2; ++wheel ) {
    for( int sector = 1; sector <= 12; ++sector ) {
      for( int bx = min_bx; bx <= max_bx; ++bx ) {
	for( int itrk = 1; itrk <=2; ++itrk ) {
	  std::unique_ptr<L1MuDTTrackCand> dttrk;
	  if( itrk == 1 ) dttrk.reset(dtTracks->dtTrackCand1(wheel,sector,bx));
	  else            dttrk.reset(dtTracks->dtTrackCand2(wheel,sector,bx));
	  
	  if( dttrk ) {
	    InternalTrack trk(*dttrk);
	    std::vector<unsigned> addrs;
	    addrs.reserve(4);
	    
	    for( int station = 1; station <= 4; ++ station ) {
	      std::cout << "MB " << station << " : " 
			<< dttrk->stNum(station) << std::endl;
	      addrs.push_back(dttrk->stNum(station));
	    }	    

	    TriggerPrimitiveList tplist;
	    
	    auto stub = tplist.cbegin();
	    auto stend = tplist.cend();
	    for( ; stub != stend; ++stub ) {
	      trk.addStub(*stub);      
	    }
	  }
	  dttrk.release();
	}
      }
    }
  }

  ev.put(convertedTracks);
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(L1DTTFTrackConverter);
