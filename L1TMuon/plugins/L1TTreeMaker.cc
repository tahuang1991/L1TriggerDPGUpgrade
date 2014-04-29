#include "CondFormats/L1TObjects/interface/L1MuTriggerScales.h"
#include "CondFormats/DataRecord/interface/L1MuTriggerScalesRcd.h"
#include "CondFormats/DataRecord/interface/L1MuTriggerPtScaleRcd.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "L1TTreeMaker.h"

L1TTreeMaker::L1TTreeMaker(const edm::ParameterSet& iConfig)
{
  //now do what ever initialization is needed
  //  runSRLUTs = new csctf_analysis::RunSRLUTs();
  lctsTag= iConfig.getParameter<edm::InputTag>("lctsTag");
  vertexColTag= iConfig.getParameter<edm::InputTag>("vertexColTag");
  //  outTreeFileName= iConfig.getUntrackedParameter<string>("outTreeFileName");
  //  haveRECO = iConfig.getUntrackedParameter<bool>("haveRECO");
}
L1TTreeMaker::~L1TTreeMaker()
{
}
void L1TTreeMaker::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  intializeTree();
  const float ptscale[33] = { 
    -1.,   0.0,   1.5,   2.0,   2.5,   3.0,   3.5,   4.0,
    4.5,   5.0,   6.0,   7.0,   8.0,  10.0,  12.0,  14.0,  
    16.0,  18.0,  20.0,  25.0,  30.0,  35.0,  40.0,  45.0, 
    50.0,  60.0,  70.0,  80.0,  90.0, 100.0, 120.0, 140.0, 1.E6 };

  edm::Handle<L1CSCTrackCollection> l1csctracks;
  iEvent.getByLabel("simCsctfTrackDigis",l1csctracks);
  edm::Handle<TriggerPrimitiveCollection> trigPrims;
  iEvent.getByLabel("L1TMuonTriggerPrimitives",trigPrims);
  edm::Handle<CSCCorrelatedLCTDigiCollection> lcts;
  iEvent.getByLabel("simCscTriggerPrimitiveDigis","MPCSORTED", lcts);
  edm::Handle<InternalTrackCollection> L1TMtracks;
  iEvent.getByLabel( "L1TMuonText" , "DataITC"  , L1TMtracks );
  edm::Handle<edm::SimTrackContainer> BaseSimTracks;
  iEvent.getByLabel("g4SimHits",BaseSimTracks);
  edm::SimTrackContainer::const_iterator BaseSimTrk;

  mc_n = 0;
  for(BaseSimTrk=BaseSimTracks->begin(); BaseSimTrk != BaseSimTracks->end(); BaseSimTrk++){
    if (fabs(BaseSimTrk->type()) == 13){
      mc_n++;
      mc_pt.push_back(BaseSimTrk->momentum().pt());
      mc_eta.push_back(BaseSimTrk->momentum().eta());
      mc_phi.push_back(BaseSimTrk->momentum().phi());
      mc_type.push_back(BaseSimTrk->type());
    }
  }
  
  // lcts used in tracks
  l1csctrk_n = 0;
  L1CSCTrackCollection::const_iterator tmp_trk = l1csctracks->begin();
  for(; tmp_trk != l1csctracks->end(); tmp_trk++){
    l1csctrk_n++;
    float pt=0, eta=-9, phi=-9, GEMdPhi = -99;
    unsigned int quality_packed=0, rank=0, ptLUTAddress=0;
    unsigned int pt_packed=0,phi_packed=0,eta_packed=0;
    int nstubs = 0;
    bool hasGEM = false, hasME1 = false, hasME2 = false, hasME3 = false, hasME4 = false;

    auto l1track = tmp_trk->first;
    rank=l1track.rank();
    ptLUTAddress = l1track.ptLUTAddress();
    l1track.decodeRank(rank,pt_packed,quality_packed); //get the pt and gaulity packed
    pt = ptscale[pt_packed];
    
    unsigned int sector = l1track.sector();// get sector

    edm::ESHandle< L1MuTriggerScales > scales;//get structures for scales (phi and eta
    iSetup.get< L1MuTriggerScalesRcd >().get(scales); // get scales from EventSetup
    const L1MuTriggerScales  *ts;// the trigger scales 
    ts = scales.product();
    unsigned gbl_phi = l1track.localPhi() + ((sector - 1)*24) + 6;
    if(gbl_phi > 143) gbl_phi -= 143;
    phi = ts->getPhiScale()->getLowEdge( gbl_phi&0xff );
    unsigned eta_sign = (l1track.endcap() == 1 ? 0 : 1);
    eta = ts->getRegionalEtaScale(2)->
	getCenter( ((l1track.eta_packed()) | (eta_sign<<5)) & 0x3f );
    phi_packed = l1track.localPhi();
    eta_packed = l1track.eta_packed();

    
    for(CSCCorrelatedLCTDigiCollection::DigiRangeIterator csc=tmp_trk->second.begin(); csc!=tmp_trk->second.end(); csc++){
      auto lctdigi = (*csc).second.first;
      if ((*csc).first.station()==1){
	GEMdPhi = lctdigi->getGEMDPhi();
	if (fabs(GEMdPhi) > 99) hasGEM = true;
	hasME1 = true;
      }
      if ((*csc).first.station()==2) hasME2 = true;
      if ((*csc).first.station()==3) hasME3 = true;
      if ((*csc).first.station()==4) hasME4 = true;

      // if ((*csc).first.endcap() == 1){
      // 	h_nStation->Fill((*csc).first.station());
      // 	hasTrkFwd = true;
      // 	nTrkStubsFwd++;
      // 	if (quality_packed >= 3) hasQ3TrkFwd = true;
      // 	if ((*csc).first.station() == 1) hasTrkFwdME1 = true;
      // 	if (ptscale[pt_packed] >= 20) hasTrkFwd20 = true;
      // }
      // if ((*csc).first.endcap() == 2){
      // 	h_nStation->Fill(-((*csc).first.station()));
      // 	hasTrkBwd = true;
      // 	nTrkStubsBwd++;
      // 	if (quality_packed >= 3) hasQ3TrkBwd = true;
      // 	if ((*csc).first.station() == 1) hasTrkBwdME1 = true;
      // 	if (ptscale[pt_packed] >= 20) hasTrkBwd20 = true;
      // }

      // if ((*csc).first.station() == 1){
      // 	// cout << "Station = " << (*csc).first.station()
      // 	//      << ", getGEMDPhi " << lctdigi->getGEMDPhi()
      // 	//      << ", getQuality " << lctdigi->getQuality()
      // 	//      << ", getKeyWG " << lctdigi->getKeyWG()
      // 	//      << ", getStrip " << lctdigi->getStrip()
      // 	//      << ", getPattern " << lctdigi->getPattern()
      // 	//      << ", getBend " << lctdigi->getBend()
      // 	//      << ", getCLCTPattern " << lctdigi->getCLCTPattern()
      // 	//      << ", getMPCLink " << lctdigi->getMPCLink()
      // 	//      << endl;
      // }
      nstubs++;
    }
    
    l1csctrk_pt.push_back(pt); l1csctrk_eta.push_back(eta); l1csctrk_phi.push_back(phi); l1csctrk_GEMdPhi.push_back(GEMdPhi);
    l1csctrk_quality_packed.push_back(quality_packed); l1csctrk_rank.push_back(rank); l1csctrk_ptLUTAddress.push_back(ptLUTAddress);
    l1csctrk_pt_packed.push_back(pt_packed); l1csctrk_eta_packed.push_back(eta_packed); l1csctrk_phi_packed.push_back(phi_packed);
    l1csctrk_nstubs.push_back(nstubs);
    l1csctrk_hasGEM.push_back(hasGEM); l1csctrk_hasME1.push_back(hasME1); l1csctrk_hasME2.push_back(hasME2); l1csctrk_hasME3.push_back(hasME3); l1csctrk_hasME4.push_back(hasME4);
  }


  // CSCCorrelatedLCTDigiCollection::DigiRangeIterator Citer;
  // for(Citer = lcts->begin(); Citer != lcts->end(); Citer++){
  //   CSCCorrelatedLCTDigiCollection::const_iterator Diter = (*Citer).second.first;
  //   CSCCorrelatedLCTDigiCollection::const_iterator Dend = (*Citer).second.second;
  //   for(; Diter != Dend; Diter++){
  //     if ((*Citer).first.endcap() == 1){
  // 	nFwdStubs++;
  // 	if (Diter->getGEMDPhi() > -99) hasFwdGE11 = true;
  //     }
  //     if ((*Citer).first.endcap() == 2){
  // 	nBwdStubs++;
  // 	if (Diter->getGEMDPhi() > -99) hasBwdGE11 = true;
  //     }
  //     if (Diter->getGEMDPhi() > -99){
  // 	h_StubQualityGEM->Fill(Diter->getQuality());
  //     }
  //     h_StubQuality->Fill(Diter->getQuality());
  //   }
  // }


  /// upgrade L1T muon track finder
  l1tm_n = 0;
  auto iL1Tmuons    = L1TMtracks->cbegin();
  for( ; iL1Tmuons != L1TMtracks->cend() ; ++iL1Tmuons){
    l1tm_n++;
    float pt=0, eta=-9, phi=-9, GEMdPhi = -99;
    int nstubs = 0, rank;
    bool hasGEM = false, hasME1 = false, hasME2 = false, hasME3 = false, hasME4 = false;

    // pt = iL1Tmuons->ptValue();
    // eta = iL1Tmuons->etaValue();
    // phi = iL1Tmuons->phiValue();
    eta = iL1Tmuons->theta;
    phi = iL1Tmuons->phi;
    rank = iL1Tmuons->rank;
    TriggerPrimitiveStationMap tpsmData = iL1Tmuons->getStubs();
    // Getting the unique station ID number for ME1
    const unsigned id = 4*L1TMuon::InternalTrack::kCSC;
    // Looping over all four stations
    for(unsigned meNum=id; meNum<(id+4); meNum++){
      // Getting the trig prim lists for this station
      TriggerPrimitiveList tplData = tpsmData[meNum];
      //      cout << "ME " << meNum-id+1 << " -  # Trig Prims = " << tplData.size() << endl;
      if (tplData.size()){
	nstubs++;
	if ((meNum-id+1) == 1) hasME1 = true;

	if ((meNum-id+1) == 2) hasME2 = true;
	if ((meNum-id+1) == 3) hasME3 = true;
	if ((meNum-id+1) == 4) hasME4 = true;

	for(unsigned tpNum = 0; tpNum < tplData.size() ; tpNum++){
	  //	cout << " ----- tp #" << tpNum << endl; 
	  // Creating references to the trig prim info
	  TriggerPrimitiveRef tprData = tplData.at(tpNum);
	  GEMdPhi = (*tprData).getCSCData().gemDPhi;
	  
	  // if ((*tprData).getCMSGlobalEta() > 0){
	  //   hasL1TrkFwd = true;
	  //   nL1TrkStubsFwd++;
	  //   if ((meNum-id+1) == 1) hasL1TrkFwdME1 = true;
	  // }
	  // if ((*tprData).getCMSGlobalEta() < 0){
	  //   hasL1TrkBwd = true;
	  //   nL1TrkStubsBwd++;
	  //   if ((meNum-id+1) == 1) hasL1TrkBwdME1 = true;
	  // }
	
	}
      }
    }
    l1tm_pt.push_back(pt); l1tm_eta.push_back(eta); l1tm_phi.push_back(phi); l1tm_GEMdPhi.push_back(GEMdPhi);
    l1tm_nstubs.push_back(nstubs); l1tm_rank.push_back(rank);
    l1tm_hasGEM.push_back(hasGEM); l1tm_hasME1.push_back(hasME1); l1tm_hasME2.push_back(hasME2); l1tm_hasME3.push_back(hasME3); l1tm_hasME4.push_back(hasME4);

  }
  l1muon->Fill();
}


// ------------ method called once each job just before starting event loop  ------------
void L1TTreeMaker::beginJob()
{
  l1muon = fs->make<TTree>("l1muon", "l1muon");
  l1muon->Branch("event",&event);

  l1muon->Branch("mc_n",&mc_n);
  l1muon->Branch("mc_pt",&mc_pt);
  l1muon->Branch("mc_eta",&mc_eta);
  l1muon->Branch("mc_phi",&mc_phi);
  l1muon->Branch("mc_type",&mc_type);

  l1muon->Branch("l1csctrk_n", &l1csctrk_n);
  l1muon->Branch("l1csctrk_pt", &l1csctrk_pt); 
  l1muon->Branch("l1csctrk_eta", &l1csctrk_eta);
  l1muon->Branch("l1csctrk_phi", &l1csctrk_phi);
  l1muon->Branch("l1csctrk_GEMdPhi", &l1csctrk_GEMdPhi);
  l1muon->Branch("l1csctrk_quality_packed", &l1csctrk_quality_packed);
  l1muon->Branch("l1csctrk_rank", &l1csctrk_rank);
  l1muon->Branch("l1csctrk_pt_packed", &l1csctrk_pt_packed);
  l1muon->Branch("l1csctrk_eta_packed", &l1csctrk_eta_packed);
  l1muon->Branch("l1csctrk_phi_packed", &l1csctrk_phi_packed);
  l1muon->Branch("l1csctrk_ptLUTAddress", &l1csctrk_ptLUTAddress);
  l1muon->Branch("l1csctrk_nstubs", &l1csctrk_nstubs);
  l1muon->Branch("l1csctrk_hasGEM", &l1csctrk_hasGEM);
  l1muon->Branch("l1csctrk_hasME1", &l1csctrk_hasME1);
  l1muon->Branch("l1csctrk_hasME2", &l1csctrk_hasME2);
  l1muon->Branch("l1csctrk_hasME3", &l1csctrk_hasME3);
  l1muon->Branch("l1csctrk_hasME4", &l1csctrk_hasME4);

  l1muon->Branch("l1tm_n", &l1tm_n);
  l1muon->Branch("l1tm_pt", &l1tm_pt); 
  l1muon->Branch("l1tm_eta", &l1tm_eta);
  l1muon->Branch("l1tm_phi", &l1tm_phi);
  l1muon->Branch("l1tm_rank", &l1tm_rank);
  l1muon->Branch("l1tm_GEMdPhi", &l1tm_GEMdPhi);
  l1muon->Branch("l1tm_nstubs", &l1tm_nstubs);
  l1muon->Branch("l1tm_hasGEM", &l1tm_hasGEM);
  l1muon->Branch("l1tm_hasME1", &l1tm_hasME1);
  l1muon->Branch("l1tm_hasME2", &l1tm_hasME2);
  l1muon->Branch("l1tm_hasME3", &l1tm_hasME3);
  l1muon->Branch("l1tm_hasME4", &l1tm_hasME4);

}

void L1TTreeMaker::intializeTree()
{
  mc_pt.clear(); mc_eta.clear(); mc_phi.clear(); mc_type.clear();

  l1csctrk_pt.clear(); l1csctrk_eta.clear(); l1csctrk_phi.clear(); l1csctrk_GEMdPhi.clear();
  l1csctrk_quality_packed.clear(); l1csctrk_rank.clear(); l1csctrk_ptLUTAddress.clear();
  l1csctrk_pt_packed.clear(); l1csctrk_eta_packed.clear(); l1csctrk_phi_packed.clear();
  l1csctrk_nstubs.clear();
  l1csctrk_hasGEM.clear(); l1csctrk_hasME1.clear(); l1csctrk_hasME2.clear(); l1csctrk_hasME3.clear(); l1csctrk_hasME4.clear();

  l1tm_pt.clear(); l1tm_eta.clear(); l1tm_phi.clear(); l1tm_GEMdPhi.clear();
  l1tm_nstubs.clear();
  l1tm_hasGEM.clear(); l1tm_hasME1.clear(); l1tm_hasME2.clear(); l1tm_hasME3.clear(); l1tm_hasME4.clear();

}
// ------------ method called once each job just after ending the event loop  ------------
void L1TTreeMaker::endJob() 
{
}

//define this as a plug-in
DEFINE_FWK_MODULE(L1TTreeMaker);
