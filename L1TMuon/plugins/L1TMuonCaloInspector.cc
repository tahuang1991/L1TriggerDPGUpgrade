// -*- C++ -*-
//
// Package:    L1TMuon
// Class:      L1TMuonCaloInspector
// 
/**\class L1TMuonCaloInspector L1TMuonCaloInspector.cc L1TriggerDPGUpgrade/L1TMuon/plugins/L1TMuonCaloInspector.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Alberto Belloni
//         Created:  Wed, 12 Jun 2013 16:42:12 GMT
// $Id$
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/TrackReco/interface/Track.h"

#include "DataFormats/CaloTowers/interface/CaloTower.h"
#include "DataFormats/CaloTowers/interface/CaloTowerFwd.h"

#include "L1TriggerDPGUpgrade/DataFormats/interface/L1TMuonTriggerPrimitive.h"
#include "L1TriggerDPGUpgrade/DataFormats/interface/L1TMuonTriggerPrimitiveFwd.h"

#include "L1TriggerDPGUpgrade/DataFormats/interface/L1TMuonInternalTrack.h"
#include "L1TriggerDPGUpgrade/DataFormats/interface/L1TMuonInternalTrackFwd.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "TTree.h"
#include "TH1F.h"
#include "TH2F.h"

using namespace L1TMuon;

//
// class declaration
//

class L1TMuonCaloInspector : public edm::EDAnalyzer {
public:
  explicit L1TMuonCaloInspector(const edm::ParameterSet&);
  ~L1TMuonCaloInspector();
  
  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);
  
  
private:
  virtual void beginJob() override;
  virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
  virtual void endJob() override;
  
  edm::Service<TFileService> _fileService;
  
  bool _doGen;
  edm::InputTag _genInput;
  edm::InputTag _bestInput;
  edm::InputTag _worstInput;
  //std::vector<edm::InputTag> _trackInput;
  
  TH1F* _detaGB; // generated-standalone
  TH1F* _dphiGB;

  TH1F* _detaCG; // calo tower-generated
  TH1F* _dphiCG;

  TH1F* _caloM; // tower energy: muon-matched
  TH1F* _caloN; //               noise

  TH1F* _caloE;
  TH1F* _caloH;
  TH1F* _caloO;

  TH2F* _rpcEtaPhi;
  TH2F* _hoEtaPhi;

  TH2F* _caloXY; // sanity check: location of HO deposits
  TH2F* _caloZY;

  TriggerPrimitiveRef getBestTriggerPrimitive(const TriggerPrimitiveList& list, 
					      unsigned subsystem) const;

  // ----------member data ---------------------------
};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
L1TMuonCaloInspector::L1TMuonCaloInspector(const edm::ParameterSet& iConfig)

{
  //now do what ever initialization is needed
  if( (_doGen = iConfig.getUntrackedParameter<bool>("doGen",false)) ) {
    _genInput = iConfig.getParameter<edm::InputTag>("genSrc");
  }
  //_trackInput = iConfig.getParameter<std::vector<edm::InputTag> >("trackSrcs");

  _bestInput  = iConfig.getParameter<edm::InputTag>("bestSrc"); //standalone muons?
  _worstInput = iConfig.getParameter<edm::InputTag>("worstSrc"); //RPC trig prims?


  _detaGB = _fileService->make<TH1F>("genbesteta","Generated-Best Eta",200,
				     -0.10,0.10);
  _dphiGB = _fileService->make<TH1F>("genbestphi","Generated-Best Phi",200,
				     -M_PI/100.,M_PI/100.);

  _detaCG = _fileService->make<TH1F>("calogeneta","CaloTower-Generated Eta",200,
				     -0.50,0.50);
  _dphiCG = _fileService->make<TH1F>("calogenphi","CaloTower-Generated Phi",200,
				     -M_PI/10.,M_PI/10.);

  _caloM = _fileService->make<TH1F>("calobest","CaloTower w muon Energy",500,
				     -0.200,0.800);
  _caloN = _fileService->make<TH1F>("calonoise","CaloTower wo muon Energy",500,
				     -0.200,0.800);

  _caloE = _fileService->make<TH1F>("emcalo","EM energy",500,
				     -3,7.);
  _caloH = _fileService->make<TH1F>("hadcalo","HAD energy",500,
				     -3,7.);
  _caloO = _fileService->make<TH1F>("hocalo","HO energy",500,
				     -3,7.);

  _rpcEtaPhi = _fileService->make<TH2F>("rpcetaphi","RpcB TP eta vs phi",
					260,-1.3,1.3,
					200,-M_PI,M_PI);
  _hoEtaPhi = _fileService->make<TH2F>("hoetaphi","HO TP eta vs phi",
				       260,-1.3,1.3,
				       200,-M_PI,M_PI);

  _caloXY = _fileService->make<TH2F>("caloxy","CaloTower XY",
				     600,-300.,300.,
				     600,-300.,300.);
  _caloZY = _fileService->make<TH2F>("calozy","CaloTower ZY",
				     600,-1500.,1500.,
				     600,-300.,300.);
  
}


L1TMuonCaloInspector::~L1TMuonCaloInspector()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
void
L1TMuonCaloInspector::analyze(const edm::Event& iEvent, 
			      const edm::EventSetup& iSetup)
{
  //using namespace edm;
  
  edm::Handle<reco::GenParticleCollection> genParticles;
  iEvent.getByLabel(_genInput,genParticles);

  edm::Handle<reco::TrackCollection> bestOnes;  // standalone muons
  iEvent.getByLabel(_bestInput,bestOnes);
  edm::Handle<InternalTrackCollection> worstOnes; // L1RPCbTFTrackConverter
  iEvent.getByLabel(_worstInput,worstOnes);

  edm::Handle<CaloTowerCollection> caloTowers;
  iEvent.getByLabel("towerMaker",caloTowers);


  auto bgen = genParticles->cbegin();
  auto egen = genParticles->cend();

  auto bbest = bestOnes->cbegin();
  auto ebest = bestOnes->cend();

  auto bworst = worstOnes->cbegin();
  auto eworst = worstOnes->cend();

  auto bcalo = caloTowers->begin();
  auto ecalo = caloTowers->end();
 
   for( ; bgen != egen; ++bgen ) {
     if (bgen->pt()>15 && 
	 std::abs(bgen->pdgId()) == 13 && 
	 fabs(bgen->eta())<1.24) {
       bool matched = false;
       for( ; bbest != ebest; ++bbest ) {
	 _detaGB->Fill(bgen->eta()-bbest->eta());
	 _dphiGB->Fill(bgen->phi()-bbest->phi());
	 if(sqrt(pow(bgen->eta()-bbest->eta(),2)+
		 pow(bgen->phi()-bbest->phi(),2))<0.01) {
	   // decide I have match
	   std::cout << iEvent.id().event() << " Match with best" << std::endl;
	   matched = true;
	 }
       }
       for( ; bworst != eworst; ++bworst ) {
	 // loop on trigger primitives and check if we get RPC and HO
	 // trigger primitives
	 TriggerPrimitiveStationMap stubs = bworst->getStubs();

	 // Loop on RPC stations, for each of which you can get a TP
	 unsigned station;
	 for( station = 1; station <= 4; ++station ) {
	   const unsigned idx = 4*1+station-1; // RPCb=1
	   if( !stubs.count(idx) ) continue;
	   TriggerPrimitiveList tpRpcB = stubs[idx];
	   TriggerPrimitiveRef bestRpcB = getBestTriggerPrimitive(tpRpcB,1);
	   // plot eta and phi
	   std::cout << iEvent.id().event() << " RpcB eta, phi, station:"
		     << " " << bestRpcB->getCMSGlobalEta()
		     << " " << bestRpcB->getCMSGlobalPhi()
		     << " " << station
		     << std::endl;
	   _rpcEtaPhi->Fill(bestRpcB->getCMSGlobalEta(),
			    bestRpcB->getCMSGlobalPhi());
	 }
	 // Loop on HO stations, for each of which you can get a TP
	 // will change the code because it makes so much more sense that
	 // the HO station is always the same: I have one layer (or two)

	 for (unsigned i=0;i<20;i++)
	   std::cout << " i=" << i << ", " << stubs.count(i) << std::endl;

	 for( station = 1; station <= 4; ++station ) {
	   const unsigned idx = 4*4+station-1; // Hcal=4
	   if( !stubs.count(idx) ) continue;
	   TriggerPrimitiveList tpHo = stubs[idx];
	   TriggerPrimitiveRef bestHo = getBestTriggerPrimitive(tpHo,4);
	   // plot eta and phi
	   std::cout << iEvent.id().event() << " Ho eta, phi, station:"
		     << " " << bestHo->getCMSGlobalEta()
		     << " " << bestHo->getCMSGlobalPhi()
		     << " " << station
		     << std::endl;
	   _hoEtaPhi->Fill(bestHo->getCMSGlobalEta(),
			   bestHo->getCMSGlobalPhi());
	 }
       }

       for( ; bcalo != ecalo; ++bcalo ) {

	 if (fabs(bcalo->hadPosition().eta())<0.9)
	   _caloXY->Fill(bcalo->hadPosition().x(),bcalo->hadPosition().y());
	 _caloZY->Fill(bcalo->hadPosition().z(),bcalo->hadPosition().y());

	 _detaCG->Fill(bcalo->hadPosition().eta()-bgen->eta());
	 _dphiCG->Fill(bcalo->hadPosition().phi()-bgen->phi());

	 std::cout << iEvent.id().event()
		   << " Calo outer, had, em: " << bcalo->outerEnergy()
		   << " " << bcalo->hadEnergy()
		   << " " << bcalo->emEnergy()
		   << std::endl;
	 _caloE->Fill(bcalo->emEnergy());
	 _caloH->Fill(bcalo->hadEnergy());
	 _caloO->Fill(bcalo->outerEnergy());
	 if (sqrt(pow(bcalo->hadPosition().eta()-bgen->eta(),2)+
		  pow(bcalo->hadPosition().phi()-bgen->phi(),2))<0.2)
	   _caloM->Fill(bcalo->outerEnergy());
	 else
	   _caloN->Fill(bcalo->outerEnergy());
       }

       if (!matched)
	   std::cout << iEvent.id().event() << " Not matched" << std::endl;
     }
   }
}


// ------------ method called once each job just before starting event loop  ------------
void 
L1TMuonCaloInspector::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
L1TMuonCaloInspector::endJob() 
{
}

// ------------ method called when starting to processes a run  ------------
/*
void 
L1TMuonCaloInspector::beginRun(edm::Run const&, edm::EventSetup const&)
{
}
*/

// ------------ method called when ending the processing of a run  ------------
/*
void 
L1TMuonCaloInspector::endRun(edm::Run const&, edm::EventSetup const&)
{
}
*/

// ------------ method called when starting to processes a luminosity block  ------------
/*
void 
L1TMuonCaloInspector::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/

// ------------ method called when ending the processing of a luminosity block  ------------
/*
void 
L1TMuonCaloInspector::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
L1TMuonCaloInspector::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

TriggerPrimitiveRef L1TMuonCaloInspector::
getBestTriggerPrimitive(const TriggerPrimitiveList& list, 
			unsigned subsystem) const {
  TriggerPrimitiveRef result;
  unsigned bestquality = 0, qualtemp; // for CSCs / DTs / HCAL (just for fun)
  float phiavg, bestdphi, lsize; // average strip for RPCS
  auto tp = list.cbegin();
  auto tpend = list.cend();
  
  switch( subsystem ) {
  case 0: // DTs
    for( ; tp != tpend; ++tp ) {
      qualtemp = 0;
      if( (*tp)->getDTData().qualityCode != -1 ) {
	qualtemp += (~((*tp)->getDTData().qualityCode)&0x7) << 1;	
      }
      if( (*tp)->getDTData().theta_quality != -1 ) {
	qualtemp += (~((*tp)->getDTData().theta_quality)&0x1);
      }
      if( qualtemp > bestquality ) {
	bestquality = qualtemp;
	result = *tp;
      }
    }
      break;
  case 2: // CSCs
    for( ; tp != tpend; ++tp ) {
      qualtemp = (*tp)->getCSCData().quality;      
      if ( qualtemp > bestquality ) {
	bestquality = qualtemp;
	result = *tp;
      }
    }
    break;
  case 1:
  case 3: // RPCb/f
    phiavg = 0;
    lsize = list.size();
    for( ; tp != tpend; ++tp ) {
      phiavg += (*tp)->getCMSGlobalPhi();
    }
    phiavg = phiavg/lsize;    
    tp = list.cbegin();
    bestdphi = 100;
    for( ; tp != tpend; ++tp ) {      
      if( std::abs((*tp)->getCMSGlobalPhi() - phiavg) < bestdphi ) {
	result = *tp;
	bestdphi = std::abs((*tp)->getCMSGlobalPhi() - phiavg);
      }
    }
    break;
  case 4: // HCAL
    for( ; tp != tpend; ++tp ) {
      qualtemp = (*tp)->getHCALData().size;
      if ( qualtemp > bestquality ) {
	bestquality = qualtemp;
	result = *tp;
      }
    }
    break;
  default:
    break;
  }
  return result;
}




//define this as a plug-in
DEFINE_FWK_MODULE(L1TMuonCaloInspector);
