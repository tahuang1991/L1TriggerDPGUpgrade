// 
// Class: L1TMuonAssignedInternalTrackProducer
//
// Info: A configurable track processor where the pt assignment/refinement 
//       algorithms are controlled by config files.
//       The closest representation of this in the old hardware is 
//       the pt lookup table
//
// Author: L. Gray (FNAL)
//

#include <memory>
#include <map>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "L1TriggerDPGUpgrade/DataFormats/interface/L1TMuonTriggerPrimitive.h"
#include "L1TriggerDPGUpgrade/DataFormats/interface/L1TMuonTriggerPrimitiveFwd.h"

#include "L1TriggerDPGUpgrade/DataFormats/interface/L1TMuonInternalTrack.h"
#include "L1TriggerDPGUpgrade/DataFormats/interface/L1TMuonInternalTrackFwd.h"

// interface to Pt assignment and refinement
#include "L1TriggerDPGUpgrade/L1TMuon/interface/PtAssignmentUnitFactory.h"
#include "L1TriggerDPGUpgrade/L1TMuon/interface/PtRefinementUnitFactory.h"

#include "FWCore/ServiceRegistry/interface/Service.h"

using namespace L1TMuon;

typedef edm::ParameterSet PSet;
typedef std::unique_ptr<PtAssignmentUnit> pPAU;
typedef std::unique_ptr<PtRefinementUnit> pPRU;

class L1TMuonAssignedInternalTrackProducer : public edm::EDProducer {    
public:
  L1TMuonAssignedInternalTrackProducer(const PSet&);
  ~L1TMuonAssignedInternalTrackProducer() {}

  void produce(edm::Event&, const edm::EventSetup&);  
private:
  pPAU _ptassign;
  pPRU _ptrefine;
  edm::InputTag _trackInput;
};

L1TMuonAssignedInternalTrackProducer::
L1TMuonAssignedInternalTrackProducer(const PSet& p) {
  // name of the track source
  _trackInput = p.getParameter<edm::InputTag>("src");
  // configure and build pt assignment unit
  std::unique_ptr<PtAssignmentUnitFactory> 
    fPtAU(PtAssignmentUnitFactory::get());
  if( p.existsAs<PSet>("PtAssignmentUnit") ) {
    PSet PtAU_config = p.getParameterSet("PtAssignmentUnit");
    std::string PtAU_type = 
      PtAU_config.getParameter<std::string>("AssignmentType");
    _ptassign.reset( fPtAU->create( PtAU_type,
				    PtAU_config) );
  } else {
    _ptassign.reset(NULL);
  }
  // configure and build pt refinement unit
  std::unique_ptr<PtRefinementUnitFactory> 
    fPtRU(PtRefinementUnitFactory::get());
  if( p.existsAs<PSet>("PtRefinementUnit") ) {
    PSet PtRU_config = p.getParameterSet("PtRefinementUnit");
    std::string PtRU_type =
      PtRU_config.getParameter<std::string>("RefinementType");
    _ptrefine.reset( fPtRU->create( PtRU_type,
				    PtRU_config) );
  } else {
    _ptrefine.reset(NULL);
  }
  
  produces<InternalTrackCollection>("AssignmentOnly");
  produces<InternalTrackCollection>();

  fPtAU.release();
  fPtRU.release();
}

void L1TMuonAssignedInternalTrackProducer::produce(edm::Event& ev, 
						   const edm::EventSetup& es) {
  edm::Handle<edm::View<L1MuRegionalCand> > tracks;
  ev.getByLabel(_trackInput,tracks);
  auto trackRefToBases = tracks->refVector();
  
  std::auto_ptr<InternalTrackCollection> 
    assignedOnly(new InternalTrackCollection);
  std::auto_ptr<InternalTrackCollection> 
    assignedAndRefined(new InternalTrackCollection);

  // setup pt assignment
  if( _ptassign ) _ptassign->updateEventSetup(es);
  // run the pt assignment
  for( auto tk : trackRefToBases ) {
    InternalTrack toassign(tk.castTo<InternalTrackRef>());
    if( _ptassign ) _ptassign->assignPt(toassign);
    assignedOnly->push_back(toassign);
  }
  auto assignedTkHandle = ev.put(assignedOnly,"AssignmentOnly");

  // setup pt refinement
  if( _ptrefine) _ptrefine->updateEventSetup(es);
  // run the pt refinement
  auto atkbeg = assignedTkHandle->begin();
  auto atk = atkbeg;
  auto atkend = assignedTkHandle->end();  
  for( ; atk != atkend; ++atk ) {
    InternalTrackRef atkref = InternalTrackRef(assignedTkHandle,atk-atkbeg);
    InternalTrack refined(atkref);
    if( _ptrefine ) _ptrefine->refinePt(refined);
    assignedAndRefined->push_back(refined);
  }
  ev.put(assignedAndRefined);
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(L1TMuonAssignedInternalTrackProducer);
