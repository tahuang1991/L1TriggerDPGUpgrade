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

#include "DataFormats/Math/interface/deltaR.h"

#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/TrackReco/interface/Track.h"

#include "DataFormats/CaloTowers/interface/CaloTower.h"
#include "DataFormats/CaloTowers/interface/CaloTowerFwd.h"

#include "DataFormats/HcalRecHit/interface/HORecHit.h"
#include "DataFormats/HcalRecHit/interface/HBHERecHit.h"
#include "DataFormats/HcalRecHit/interface/HFRecHit.h"
#include "DataFormats/HcalRecHit/interface/ZDCRecHit.h"
#include "DataFormats/HcalRecHit/interface/CastorRecHit.h"
#include "DataFormats/HcalRecHit/interface/HcalCalibRecHit.h"
#include "DataFormats/HcalRecHit/interface/HcalRecHitFwd.h"

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

  edm::InputTag _rpcInput;
  edm::InputTag _dttfInput;
  edm::InputTag _hcalInput;
  edm::InputTag _stdmuInput;
  edm::InputTag _glbmuInput;

  // Delta R values to be used for matching
  double _dRtruthToRpc, _dRrpcToDttf, _dRdttfToHcal, 
    _dRhcalToStdMu, _dRdttfToStdMu;
  edm::InputTag _truthToRpc;
  edm::InputTag _rpcToDttf;
  edm::InputTag _dttfToHcal;
  edm::InputTag _hcalToStdMu;
  edm::InputTag _dttfToStdMu; // or global: can't be too different

  // map with histograms: all deltaEta and deltaPhi plots will
  // have same boundaries (very generous), then work out useful
  // ranges with plotting macro
  std::map<std::string,TH1F*> _h1dDeltaEta;
  std::map<std::string,TH1F*> _h1dDeltaPhi;
  std::map<std::string,TH1F*> _h1dDeltaR;
  std::map<std::string,TH2F*> _h2dDeltaEtaPhi;

  std::map<std::string,TH1F*> _h1dEta;
  std::map<std::string,TH1F*> _h1dPhi;
  std::map<std::string,TH1F*> _h1dPt;

  std::map<std::string,TH2F*> _h2dEtaPhi;

  std::map<std::string,TH1F*> _h1dStations;

  TH1F* _counters;
  enum { ALL=0, TRUTH, RPC, DTTF, HCAL, STDMU, GLBMU };
  
  TriggerPrimitiveRef getBestTriggerPrimitive
  (const TriggerPrimitiveList& list, unsigned subsystem) const;

  void fillDeltaEtaPhiHistograms(float eta1, float phi1,
				 float eta2, float phi2,
				 std::string key);

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
  _rpcInput =   iConfig.getParameter<edm::InputTag>("rpcSrc");
  _dttfInput =  iConfig.getParameter<edm::InputTag>("dttfSrc");
  _hcalInput =  iConfig.getParameter<edm::InputTag>("hcalSrc");
  _stdmuInput = iConfig.getParameter<edm::InputTag>("stdmuSrc");
  _glbmuInput = iConfig.getParameter<edm::InputTag>("glbmuSrc");

  _dRtruthToRpc =iConfig.getUntrackedParameter<double>("dRtruthToRpc" ,1.);
  _dRrpcToDttf  =iConfig.getUntrackedParameter<double>("dRrpcToDttf"  ,1.);
  _dRdttfToHcal =iConfig.getUntrackedParameter<double>("dRdttfToHcal" ,1.);
  _dRhcalToStdMu=iConfig.getUntrackedParameter<double>("dRhcalToStdMu",1.);
  _dRdttfToStdMu=iConfig.getUntrackedParameter<double>("dRdttfToStdMu",1.);

  _counters = _fileService->make<TH1F>("counter","counters",10,-0.5,9.5);

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

  // Raise flags to indicate matching
  bool foundTruth = false;
  bool foundRpc   = false;
  bool foundDttf  = false;
  bool foundHcal  = false;
  bool foundStdMu = false;

  // Here we open all collections!
  edm::Handle<reco::GenParticleCollection> truthParticles;
   // I will get it only if needed, later on!
  //iEvent.getByLabel(_genInput,truthParticles);

  edm::Handle<InternalTrackCollection> rpcTriggerPrimitives;
  iEvent.getByLabel(_rpcInput,rpcTriggerPrimitives);

  edm::Handle<InternalTrackCollection> dttfTriggerPrimitives;
  iEvent.getByLabel(_dttfInput,dttfTriggerPrimitives);

  edm::Handle<TriggerPrimitiveCollection> hcalTriggerPrimitives;
  iEvent.getByLabel(_hcalInput,hcalTriggerPrimitives);

  edm::Handle<reco::TrackCollection> standaloneMuons;
  iEvent.getByLabel(_stdmuInput,standaloneMuons);

  edm::Handle<reco::TrackCollection> globalMuons;
  iEvent.getByLabel(_glbmuInput,globalMuons);

  // Some extra collection, always interesting
  edm::Handle<CaloTowerCollection> caloTowers;
  iEvent.getByLabel("towerMaker",caloTowers);

  edm::Handle<HORecHitCollection> hoRecoHits;
  iEvent.getByLabel("horeco",hoRecoHits);

  // Weird idea: if running on data, let us use the global muons
  // instead of truth muons in the outer loop
  // May have to use good old unsigned int loop on collections
  // instead of nice loop with iterators
  //auto btruth = _doGen ? truthParticles->cbegin() : globalMuons->cbegin();
  //auto etruth = _doGen ? truthParticles->cend() : globalMuons->cend();
  //if (_doGen) {
  iEvent.getByLabel(_genInput,truthParticles);
  auto btruth = truthParticles->cbegin();
  auto etruth = truthParticles->cend();
  //}

  for( ; btruth != etruth; ++btruth ) {
    // Initial quality cuts
    if (_doGen) {
      if (std::abs(btruth->pdgId()) != 13 ||
	  btruth->pt()<16.)
	continue;
    }
    else {
      if (btruth->pt()<16.)
	continue;
    }
    foundTruth = true;
    // end of quality cuts, let's start with RPC
    auto brpc = rpcTriggerPrimitives->cbegin();
    auto erpc = rpcTriggerPrimitives->cend();
    for( ; brpc != erpc; ++brpc ) {
      double rpcEta=0.,rpcPhi=0.;
      int rpcStat=0; // number of RPC stations with a trigger primitive
      TriggerPrimitiveStationMap stubs = brpc->getStubs();
      // Chris' function will return configuration of RPC trigger
      // primitives: how many and which stations
      //int rpctype = getType(stubs);
 
      // Loop on RPC stations, for each of which you can get a TP
      unsigned station;
      for( station = 1; station <= 4; ++station ) {
	const unsigned idx = 4*1+station-1; // RPCb=1
	if( !stubs.count(idx) ) continue;
	TriggerPrimitiveList tpRpcB = stubs[idx];
	TriggerPrimitiveRef bestRpcB = getBestTriggerPrimitive(tpRpcB,1);
	rpcEta+=bestRpcB->getCMSGlobalEta();
	rpcPhi+=bestRpcB->getCMSGlobalPhi();
	++rpcStat;
      }
      if (rpcStat>0) {
	rpcEta/=rpcStat;
	rpcPhi/=rpcStat;
      }
      // Let us fill some histograms!
      fillDeltaEtaPhiHistograms(btruth->eta(),btruth->phi(),
				rpcEta,rpcPhi,
				"truth-rpc");
      ///////////////////////////////////////
      
      // Continue with matching only if we did found a match
      if (sqrt(reco::deltaR2(btruth->eta(),btruth->phi(),
			     rpcEta,rpcPhi))>_dRtruthToRpc)
	continue;
      foundRpc = true;

      // Now loop on DTTF tracks...
      auto bdttf = dttfTriggerPrimitives->cbegin();
      auto edttf = dttfTriggerPrimitives->cend();
      for( ; bdttf != edttf; ++bdttf ) {
	double dttfEta=0.,dttfPhi=0.;
	int dttfStat=0; // number of DTTF stations with a trigger primitive
	TriggerPrimitiveStationMap stubs = bdttf->getStubs();
	// Chris' function will return configuration of DTTF trigger
	// primitives: how many and which stations
	//int dttftype = getType(stubs);
	
	// Loop on DTTF stations, for each of which you can get a TP
	unsigned station;
	for( station = 1; station <= 4; ++station ) {
	  const unsigned idx = 4*0+station-1; // DTTF=0
	  if( !stubs.count(idx) ) continue;
	  TriggerPrimitiveList tpDttf = stubs[idx];
	  TriggerPrimitiveRef bestDttf = getBestTriggerPrimitive(tpDttf,0);
	  dttfEta+=bestDttf->getCMSGlobalEta();
	  dttfPhi+=bestDttf->getCMSGlobalPhi();
	  ++dttfStat;
	}
	if (dttfStat>0) {
	  dttfEta/=dttfStat;
	  dttfPhi/=dttfStat;
	}
	// Let us fill some histograms here too
	// Carefull that we are inside a double-loop
	// I want to fill the RPC-DTTF plot only once per truth muon
	// and, similarly, fill the TRUTH-DTTF only once per RPC stub 
	if (brpc == rpcTriggerPrimitives->cbegin()) {
	  fillDeltaEtaPhiHistograms(btruth->eta(),btruth->phi(),
				    dttfEta,dttfPhi,
				    "truth-dttf");
	}
	if (btruth == truthParticles->cbegin()) {
	  fillDeltaEtaPhiHistograms(rpcEta,rpcPhi,
				    dttfEta,dttfPhi,
				    "rpc-dttf");
	}
	// Check: what is the eta of the internal track vs. the
	// one of the trigger primitive?
	//if (btruth == truthParticles->cbegin() &&
	//    brpc == rpcTriggerPrimitives->cbegin()) {
	//  fillDeltaEtaPhiHistograms(bdttf->parent()->etaValue(),
	//			    bdttf->parent()->phiValue(),
	//			    dttfEta,dttfPhi,
	//			    "dttfTK-dttfTP");
	//}
	///////////////////////////////////////

	// Continue with matching only if we did found a match
	if (sqrt(reco::deltaR2(rpcEta,rpcPhi,
			       dttfEta,dttfPhi))>_dRrpcToDttf)
	  continue;
	foundDttf = true;

	// One other layer of complication: loop on HCAL TP
	auto bhcal = hcalTriggerPrimitives->cbegin();
	auto ehcal = hcalTriggerPrimitives->cend();
	for( ; bhcal != ehcal; ++bhcal ) {
	  //TriggerPrimitiveStationMap stubs = bhcal->getStubs();
	  //const unsigned idx = 4*4+1-1; // HCAL=4, station=1
	  //if( !stubs.count(idx) ) continue;
	  //TriggerPrimitiveList tpHcal = stubs[idx];
	  //TriggerPrimitiveRef bestHcal = 
	  //  getBestTriggerPrimitive(tpHcal,4);
	  if (brpc == rpcTriggerPrimitives->cbegin() &&
	      bdttf == dttfTriggerPrimitives->cbegin() ) {
	    fillDeltaEtaPhiHistograms(btruth->eta(),btruth->phi(),
				      bhcal->getCMSGlobalEta(),
				      bhcal->getCMSGlobalPhi(),
				      "truth-hcal");
	  }
	  if (btruth == truthParticles->cbegin() &&
	      bdttf == dttfTriggerPrimitives->cbegin() ) {
	    fillDeltaEtaPhiHistograms(rpcEta,rpcPhi,
				      bhcal->getCMSGlobalEta(),
				      bhcal->getCMSGlobalPhi(),
				      "rpc-hcal");
	  }
	  if (btruth == truthParticles->cbegin() &&
	      brpc == rpcTriggerPrimitives->cbegin() ) {
	    fillDeltaEtaPhiHistograms(dttfEta,dttfPhi,
				      bhcal->getCMSGlobalEta(),
				      bhcal->getCMSGlobalPhi(),
				      "dttf-hcal");
	  }

	  // Notice that here I have a truth muon, an RPC TP,
	  // a DTTF track and an HCAL TP, not necessarily matching

	  // Continue with matching only if we did found a match
	  if (sqrt(reco::deltaR2(dttfEta,dttfPhi,
				 bhcal->getCMSGlobalEta(),
				 bhcal->getCMSGlobalPhi()))>_dRdttfToHcal)
	    continue;
	  foundHcal = true;
	  
	  // Let's loop on muons
	  // I will be sneaky and write this code only once
	  // then use it for standalone and global muons by just
	  // modifying the python file...
	  auto bstdmu = standaloneMuons->cbegin();
	  auto estdmu = standaloneMuons->cend();
	  for( ; bstdmu != estdmu; ++bstdmu ) {
	    if (brpc == rpcTriggerPrimitives->cbegin() &&
		bdttf == dttfTriggerPrimitives->cbegin() &&
		bhcal == hcalTriggerPrimitives->cbegin() ) {
	      fillDeltaEtaPhiHistograms(btruth->eta(),btruth->phi(),
					bstdmu->eta(),bstdmu->phi(),
					"truth-standalone");
	    }

	    // Continue with matching only if we did found a match
	    if (sqrt(reco::deltaR2(bhcal->getCMSGlobalEta(),
				   bhcal->getCMSGlobalPhi(),
				   bstdmu->eta(),
				   bstdmu->phi()))>_dRhcalToStdMu)
	      continue;
	    foundStdMu = true;
  
	    // Technically, here I have a truth muon matched to
	    // an RPC TP, matched to a DTTF TP, matched to an HCAL TP,
	    // matched to a standalone (or global, if I change the
	    // collection name) muon

	    // Let's loop on Calo Towers
	    // I want to check if there is an HORecHit in front of the
	    // global muon, and check its energy. How does that
	    // hit match with HcalTrigPrimitives? And RPC and DTTF?
	    // Notice that there will be tons of noisy towers
	    auto bcalo = caloTowers->begin();
	    auto ecalo = caloTowers->end();
	    for( ; bcalo != ecalo; ++bcalo ) {
	      if (btruth == truthParticles->cbegin() &&
		  brpc == rpcTriggerPrimitives->cbegin() &&
		  bdttf == dttfTriggerPrimitives->cbegin() &&
		  bhcal == hcalTriggerPrimitives->cbegin() ) {
		fillDeltaEtaPhiHistograms(bstdmu->eta(),bstdmu->phi(),
					  bcalo->hadPosition().eta(),
					  bcalo->hadPosition().phi(),
					  "standalone-calotower");
	      }
	      if (brpc == rpcTriggerPrimitives->cbegin() &&
		  bdttf == dttfTriggerPrimitives->cbegin() &&
		  bhcal == hcalTriggerPrimitives->cbegin() &&
		  bstdmu == standaloneMuons->cbegin() ) {
		fillDeltaEtaPhiHistograms(btruth->eta(),btruth->phi(),
					  bcalo->hadPosition().eta(),
					  bcalo->hadPosition().phi(),
					  "truth-calotower");
	      }
	      if (btruth == truthParticles->cbegin() &&
		  bdttf == dttfTriggerPrimitives->cbegin() &&
		  bhcal == hcalTriggerPrimitives->cbegin() &&
		  bstdmu == standaloneMuons->cbegin() ) {
		fillDeltaEtaPhiHistograms(rpcEta,rpcPhi,
					  bcalo->hadPosition().eta(),
					  bcalo->hadPosition().phi(),
					  "rpc-calotower");
	      }
	      if (btruth == truthParticles->cbegin() &&
		  brpc == rpcTriggerPrimitives->cbegin() &&
		  bhcal == hcalTriggerPrimitives->cbegin() &&
		  bstdmu == standaloneMuons->cbegin() ) {
		fillDeltaEtaPhiHistograms(dttfEta,dttfPhi,
					  bcalo->hadPosition().eta(),
					  bcalo->hadPosition().phi(),
					  "dttf-calotower");
	      }
	      if (btruth == truthParticles->cbegin() &&
		  brpc == rpcTriggerPrimitives->cbegin() &&
		  bdttf == dttfTriggerPrimitives->cbegin() &&
		  bstdmu == standaloneMuons->cbegin() ) {
		fillDeltaEtaPhiHistograms(bhcal->getCMSGlobalEta(),
					  bhcal->getCMSGlobalPhi(),
					  bcalo->hadPosition().eta(),
					  bcalo->hadPosition().phi(),
					  "hcal-calotower");
	      }
	    } // end loop on calo towers
	  } // end loop on standalone muons
	} // end loop on HCAL TP
      } // end loop on DTTF
    } // end loop on RPC
  } // end loop on truth or global

  // Here we fill counters - notice that I have to avoid leaving
  // the function analyze before I get here, otherwise the counts
  // will not be correct
  if (true)       _counters->Fill(ALL);
  if (foundTruth) _counters->Fill(TRUTH);
  if (foundRpc)   _counters->Fill(RPC);
  if (foundDttf)  _counters->Fill(DTTF);
  if (foundHcal)  _counters->Fill(HCAL);
  if (foundStdMu) _counters->Fill(STDMU);

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

void L1TMuonCaloInspector::fillDeltaEtaPhiHistograms(float eta1, float phi1,
						     float eta2, float phi2,
						     std::string key) {
  
  if(!_h1dDeltaEta.count(key))
    _h1dDeltaEta[key] = 
      _fileService->make<TH1F>(Form("deta_%s",key.c_str()),
			       Form("#Delta#eta %s",key.c_str()),
			       500,-0.5,0.5);
  _h1dDeltaEta[key]->Fill(eta1-eta2);

  if(!_h1dDeltaPhi.count(key))
    _h1dDeltaPhi[key] = 
      _fileService->make<TH1F>(Form("dphi_%s",key.c_str()),
			       Form("#Delta#phi %s",key.c_str()),
			       500,-M_PI/10.,M_PI/10.);
  _h1dDeltaPhi[key]->Fill(phi1-phi2);
  
  if(!_h1dDeltaR.count(key))
    _h1dDeltaR[key] = 
      _fileService->make<TH1F>(Form("dR_%s",key.c_str()),
			       Form("#Delta R %s",key.c_str()),
			       500,0,1.0);
  _h1dDeltaR[key]->Fill(sqrt(reco::deltaR2(eta1,phi1,eta2,phi2)));

  if(!_h2dDeltaEtaPhi.count(key))
    _h2dDeltaEtaPhi[key] = 
      _fileService->make<TH2F>(Form("detaphi_%s",key.c_str()),
			       Form("#Delta#phi vs. #Delta#eta %s",
				    key.c_str()),
			       500,-0.5,0.5,
			       500,-M_PI/10.,M_PI/10.);
  _h2dDeltaEtaPhi[key]->Fill(eta1-eta2,
			     phi1-phi2);
  
  return;
}




//define this as a plug-in
DEFINE_FWK_MODULE(L1TMuonCaloInspector);
