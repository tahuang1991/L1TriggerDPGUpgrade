#include "L1TTreeMaker.h"

L1TTreeMaker::L1TTreeMaker(const edm::ParameterSet& iConfig)
{
  //now do what ever initialization is needed
  //  runSRLUTs = new csctf_analysis::RunSRLUTs();
  lctsTag= iConfig.getParameter<edm::InputTag>("lctsTag");
  vertexColTag= iConfig.getParameter<edm::InputTag>("vertexColTag");
  outTreeFileName= iConfig.getUntrackedParameter<string>("outTreeFileName");
  haveRECO = iConfig.getUntrackedParameter<bool>("haveRECO");
  singleSectorNum = iConfig.getUntrackedParameter<int>("singleSectorNum");
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

  float min_pt = 2;
  float max_pt = 100;
  float min_aEta = 0.9;
  float max_aEta = 2.4;
  //float max_aEta = 1.8;
  float etaFwd = 0;
  float etaBwd = 0;
  float ptFwd = 0;
  float ptBwd = 0;
  float phiFwd = 0;
  float phiBwd = 0;

  //  cout << "BaseSimTracks->size() = "<< BaseSimTracks->size() << endl;
  int nSimMu = 0;
  mc_n = 0;
  for(BaseSimTrk=BaseSimTracks->begin(); BaseSimTrk != BaseSimTracks->end(); BaseSimTrk++){
    if ((fabs(BaseSimTrk->type()) == 13) and
	(BaseSimTrk->momentum().pt() >= min_pt) and
	(BaseSimTrk->momentum().pt() <= max_pt) and
	(fabs(BaseSimTrk->momentum().eta()) >= min_aEta) and 
	(fabs(BaseSimTrk->momentum().eta()) <= max_aEta) ){
      mc_n++;
      mc_pt.push_back(BaseSimTrk->momentum().pt());
      mc_eta.push_back(BaseSimTrk->momentum().eta());
      mc_phi.push_back(BaseSimTrk->momentum().phi());
      mc_type.push_back(BaseSimTrk->momentum().type());
    }
  }

  // if (nSimMu > 2){
  //   cout << "nSimMu = "<< nSimMu << endl;
  //   for(BaseSimTrk=BaseSimTracks->begin(); BaseSimTrk != BaseSimTracks->end(); BaseSimTrk++){
  //     if ((fabs(BaseSimTrk->type()) == 13) ){
  // 	std::cout << "BaseSimTrk = "<< BaseSimTrk->type()
  // 		  << " pt " << BaseSimTrk->momentum().pt()
  // 		  << " eta " << BaseSimTrk->momentum().eta()
  // 		  << std::endl;
  // 	// if (!BaseSimTrk->noVertex())
  // 	//   std::cout << "BaseSimTrk = "<< BaseSimTrk->type()
  // 	// 	    << " pos( " << BaseSimTrk->trackerSurfaceMomentum().x()
  // 	// 	    << ", " << BaseSimTrk->trackerSurfaceMomentum().y()
  // 	// 	    << ", " << BaseSimTrk->trackerSurfaceMomentum().z()
  // 	// 	    << ") " 
  // 	// 	    << std::endl;
  //     }
  //   }
  // }
 
  // // lcts used in tracks
  // L1CSCTrackCollection::const_iterator tmp_trk = l1csctracks->begin();
  // for(; tmp_trk != l1csctracks->end(); tmp_trk++){
  //   auto l1track = tmp_trk->first;
  //   unsigned int quality_packed;
  //   unsigned int rank=l1track.rank();
  //   unsigned int pt_packed;
  //   unsigned m_ptAddress = l1track.ptLUTAddress();
  //   l1track.decodeRank(rank,pt_packed,quality_packed); //get the pt and gaulity packed
  //   // cout << "l1track.decodeRank = "
  //   //  	 << ", rank " << rank
  //   //  	 << ", pt_packed " << pt_packed
  //   //  	 << ", quality_packed " << quality_packed
  //   //  	 << endl;
  //   int nstubs=0;
  //   bool hasGEM = false;
  //   float GEMdPhi = -99;
  //   for(CSCCorrelatedLCTDigiCollection::DigiRangeIterator csc=tmp_trk->second.begin(); csc!=tmp_trk->second.end(); csc++){
  //     auto lctdigi = (*csc).second.first;
  //     h_GEMDPhi->Fill((*csc).second.first->getGEMDPhi());

  //     if ((*csc).first.endcap() == 1){
  // 	h_nStation->Fill((*csc).first.station());
  // 	hasTrkFwd = true;
  // 	nTrkStubsFwd++;
  // 	if (quality_packed >= 3) hasQ3TrkFwd = true;
  // 	if ((*csc).first.station() == 1) hasTrkFwdME1 = true;
  // 	if (ptscale[pt_packed] >= 20) hasTrkFwd20 = true;
  //     }
  //     if ((*csc).first.endcap() == 2){
  // 	h_nStation->Fill(-((*csc).first.station()));
  // 	hasTrkBwd = true;
  // 	nTrkStubsBwd++;
  // 	if (quality_packed >= 3) hasQ3TrkBwd = true;
  // 	if ((*csc).first.station() == 1) hasTrkBwdME1 = true;
  // 	if (ptscale[pt_packed] >= 20) hasTrkBwd20 = true;
  //     }

  //     if ((*csc).first.station()==1 and l1track.me1ID() == 0)
  // 	cout << "(*csc).first.station() " << (*csc).first.station() 
  // 	     << " l1track.me1ID() "<< l1track.me1ID() <<endl;
  //     if ((*csc).first.station()==2 and l1track.me2ID() == 0) 
  // 	cout << "(*csc).first.station() " << (*csc).first.station() 
  // 	     << " l1track.me2ID() "<< l1track.me2ID() <<endl;
  //     if ((*csc).first.station()==3 and l1track.me3ID() == 0)
  // 	cout << "(*csc).first.station() " << (*csc).first.station() 
  // 	     << " l1track.me3ID() "<< l1track.me3ID() <<endl;
  //     if ((*csc).first.station()==4 and l1track.me4ID() == 0)
  // 	cout << "(*csc).first.station() " << (*csc).first.station() 
  // 	     << " l1track.me4ID() "<< l1track.me4ID() <<endl;

  //     // if ((*csc).first.station() == 1){
  //     // 	// cout << "Station = " << (*csc).first.station()
  //     // 	//      << ", getGEMDPhi " << lctdigi->getGEMDPhi()
  //     // 	//      << ", getQuality " << lctdigi->getQuality()
  //     // 	//      << ", getKeyWG " << lctdigi->getKeyWG()
  //     // 	//      << ", getStrip " << lctdigi->getStrip()
  //     // 	//      << ", getPattern " << lctdigi->getPattern()
  //     // 	//      << ", getBend " << lctdigi->getBend()
  //     // 	//      << ", getCLCTPattern " << lctdigi->getCLCTPattern()
  //     // 	//      << ", getMPCLink " << lctdigi->getMPCLink()
  //     // 	//      << endl;
  //     // }
  //     nstubs++;
  //     if ((*csc).second.first->getGEMDPhi() > -99){
  // 	hasGEM = true;
  // 	GEMdPhi = (*csc).second.first->getGEMDPhi();
  // 	h_TFStubQualityGEM->Fill(lctdigi->getQuality());
  //     }
  //     h_TFStubQuality->Fill(lctdigi->getQuality());
  //   }
  // }
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
  // /// upgrade L1T muon track finder
  // auto itData    = L1TMtracks->cbegin();
  // auto itendData = L1TMtracks->cend(); 
  // for( ; itData != itendData ; ++itData){      
  //   TriggerPrimitiveStationMap tpsmData = itData->getStubs();
  //   // Getting the unique station ID number for ME1
  //   const unsigned id = 4*L1TMuon::InternalTrack::kCSC;
  //   // Looping over all four stations
  //   for(unsigned meNum=id; meNum<(id+4); meNum++){
  //     // Getting the trig prim lists for this station
  //     TriggerPrimitiveList tplData = tpsmData[meNum];
  //     //      cout << "ME " << meNum-id+1 << " -  # Trig Prims = " << tplData.size() << endl;
  //     for(unsigned tpNum = 0; tpNum < tplData.size() ; tpNum++){
  // 	//	cout << " ----- tp #" << tpNum << endl; 
  // 	// Creating references to the trig prim info
  // 	TriggerPrimitiveRef tprData = tplData.at(tpNum);
  // 	if ((*tprData).getCMSGlobalEta() > 0){
  // 	  hasL1TrkFwd = true;
  // 	  nL1TrkStubsFwd++;
  // 	  if ((meNum-id+1) == 1) hasL1TrkFwdME1 = true;
  // 	}
  // 	if ((*tprData).getCMSGlobalEta() < 0){
  // 	  hasL1TrkBwd = true;
  // 	  nL1TrkStubsBwd++;
  // 	  if ((meNum-id+1) == 1) hasL1TrkBwdME1 = true;
  // 	}
	
  //     }
  //   }
  // }

}


// ------------ method called once each job just before starting event loop  ------------
void L1TTreeMaker::beginJob()
{
  l1tmuon = fs->make<TTree>("l1tmuon", "l1tmuon");
  l1tmuon->Branch("event",&event);

  l1tmuon->Branch("mc_n",&mc_n);
  l1tmuon->Branch("mc_pt",&mc_pt);
  l1tmuon->Branch("mc_eta",&mc_eta);
  l1tmuon->Branch("mc_phi",&mc_phi);
  l1tmuon->Branch("mc_type",&mc_type);

  // l1tmuon->Branch("pt",&pt);
  // l1tmuon->Branch("eta",&eta);
  // l1tmuon->Branch("phi",&phi);

  // l1tmuon->Branch("bx",&bx);
  // l1tmuon->Branch("endcap",&endcap);
  // l1tmuon->Branch("station",&station);
  // l1tmuon->Branch("ring",&ring);
  // l1tmuon->Branch("chamber",&chamber);

}

void L1TTreeMaker::intializeTree()
{
  mc_pt.clear(); mc_eta.clear(); mc_phi.clear(); mc_type.clear();
}
// ------------ method called once each job just after ending the event loop  ------------
void L1TTreeMaker::endJob() 
{
}

//define this as a plug-in
DEFINE_FWK_MODULE(L1TTreeMaker);
