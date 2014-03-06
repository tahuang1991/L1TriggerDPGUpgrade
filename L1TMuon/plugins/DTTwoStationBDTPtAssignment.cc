#include "L1TriggerDPGUpgrade/L1TMuon/interface/DTTwoStationBDTPtAssignment.h"
#include "L1TriggerDPGUpgrade/DataFormats/interface/L1TMuonInternalTrack.h"

#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/EventSetup.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/FileInPath.h"

#include "CommonTools/Utils/interface/TMVAZipReader.h"

#include "DataFormats/MuonDetId/interface/DTChamberId.h"

#include "TString.h"
#include "TVector2.h"
#include "TMath.h"

using namespace L1TMuon;

DTTwoStationBDTPtAssignment::
DTTwoStationBDTPtAssignment( const edm::ParameterSet& ps ):
  PtAssignmentUnit(ps) {
  using namespace reco::details;

  for( int sta1 = 0; sta1 < 3; ++sta1 ) {
    for( int sta2 = sta1+1; sta2 < 4; ++sta2) {
      edm::FileInPath weightsfile = 
	ps.getParameter<edm::FileInPath>(Form("weights_MB%i_MB%i",
					      sta1+1,sta2+1));
      _mvanames[sta1][sta2] = 
	ps.getParameter<std::string>(Form("mvaname_MB%i_MB%i",sta1+1,sta2+1));
      _bdt_readers[sta1][sta2].reset(new TMVA::Reader("!Color:!Silent"));
      _bdt_readers[sta1][sta2]->AddVariable("DTeta",
					    &_DTeta[sta1][sta2]);
      _bdt_readers[sta1][sta2]->AddVariable(Form("dPhi%i%i",sta1+1,sta2+1),
					    &_dPhi[sta1][sta2]);
      if( sta1 != 2 ) {
	_bdt_readers[sta1][sta2]->AddVariable(Form("Phib%i",sta1+1),
					      &_phiB_one[sta1]);
      }
      if( sta2 != 2 ) {
	_bdt_readers[sta1][sta2]->AddVariable(Form("Phib%i",sta2+1),
					      &_phiB_two[sta2]);
      }
      loadTMVAWeights(_bdt_readers[sta1][sta2].get(), 
		      _mvanames[sta1][sta2].c_str(),
		      weightsfile.fullPath().c_str());
    }
  }
  bx_window = ps.getParameter<unsigned>("bx_match_window");  
}

void DTTwoStationBDTPtAssignment::
updateEventSetup(const edm::EventSetup& es) {
  PtAssignmentUnit::updateEventSetup(es);
}

// this will modify the track in place
void DTTwoStationBDTPtAssignment::assignPt(InternalTrack& trk) {

  TriggerPrimitiveRef tp_one, tp_two;
  // get necessary information from the track
  const int trk_bx = trk.bx();
  const unsigned long dt_mode = trk.dtMode();
  const unsigned long csc_mode = trk.cscMode();
  if( !dt_mode || 
      (!csc_mode && (dt_mode == 0x1 || dt_mode == 0x2 || 
		     dt_mode == 0x4 || dt_mode == 0x8) )) {
    throw cms::Exception("TrackTypeException") 
      << "The track given to DTTwoStationBDTPtAssignment"
      << " contains fewer than two DT stubs!" << std::endl;
  }

  if(csc_mode) { 
    double pt_packed = trk.pt_packed();
    trk.setPtValue(pt_scale->getPtScale()->getCenter(pt_packed));  
    return;
  }

  TriggerPrimitiveStationMap the_tps = trk.getStubs();
  const unsigned max_station2 = ( csc_mode ? 5 : 4 );

  for( unsigned station1 = 1; station1 <= 3; ++station1 ) {
    if( dt_mode & (1 << (station1-1)) ) {
      const TriggerPrimitiveList& first_station = the_tps[station1-1];
      for( auto& tpr : first_station ) {
	if( std::abs(trk_bx - tpr->getDTData().bx) < bx_window &&
	    tpr->getDTData().qualityCode != -1) {
	  tp_one = tpr;
	}
      }
      for( unsigned station2 = station1+1; 
	   station2 <= max_station2; ++station2 ) {
	if( (dt_mode & (1 << (station2-1))) && station2 != 5 ) {
	  const TriggerPrimitiveList& second_station = the_tps[station2-1];
	  for( auto& tpr : second_station ) {
	    if( std::abs(trk_bx - tpr->getDTData().bx) < bx_window &&
		tpr->getDTData().qualityCode != -1 ) {
	      tp_two = tpr;
	    }
	  }
	  break; // no need to continue after first active station is found
	} else if( station2 == 5 ) {
	  const unsigned idx = 4*InternalTrack::kCSC; // CSC station one
	  const TriggerPrimitiveList& second_station = the_tps[idx];
	  for( auto& tpr : second_station ) {
	    if( std::abs(trk_bx - (tpr->getCSCData().bx - 6) ) < bx_window ) {
	      tp_two = tpr;
	    }
	  }
	  break; // no need to continue after first active station is found
	}	
      }
      break; // no need to continue after first active station is found
    }
  }
  
  if( tp_one.isNull() || tp_two.isNull() ) {
    throw cms::Exception("StubsNotPresent")
      << "The reported stubs for dt mode " 
      << std::hex << dt_mode << std::dec 
      << " and csc mode " << std::hex << csc_mode << std::dec 
      << " were not present on bx = " << trk_bx << std::endl;
  }
  
  // now that we have the two trigger primitives we can calculating the pT
  //double phi1 = tp_one->getCMSGlobalPhi();
  //double phi2 = tp_two->getCMSGlobalPhi();
  Int_t sector1 = tp_one->detId<DTChamberId>().sector();  
  Int_t sector2 = tp_two->detId<DTChamberId>().sector();
  Int_t sector_diff = (sector2 - sector1)%12;
  if( sector_diff >= 6 ) sector_diff -= 12;
  if( sector_diff < -6 ) sector_diff += 12;
  int offset = (2144 >> 0) * sector_diff;

  //Float_t delta_phi = TMath::RadToDeg()*TVector2::Phi_mpi_pi(phi2 - phi1);
  Int_t   dphi_int = (tp_two->getDTData().radialAngle - 
		      tp_one->getDTData().radialAngle + 
		      offset) ;//delta_phi*(4096.0/62.0);
  Int_t phibend_one = tp_one->getDTData().bendingAngle;
  Int_t phibend_two = tp_two->getDTData().bendingAngle;
  Int_t eta_int = trk.eta_packed();
  
  int dt_station1 = tp_one->detId<DTChamberId>().station();
  int dt_station2 = ( csc_mode ? 3 : tp_two->detId<DTChamberId>().station() );

  Float_t ptValue = getBDTPt(dt_station1,dt_station2,dphi_int,
			     phibend_one,phibend_two,eta_int);
  // rescale the pt by what 2-station track type it is
  switch( trk.quality() ) {
  case 7: // 4-station
  case 6: // 1-2 + 3 or 4
  case 5: // 1-3-4
  case 3: // 1-2, 1-3, 1-4
    ptValue *= 1.15;
    break;
  case 4: // 2-3-4
  case 2: // 2-3 , 2-4
    ptValue *= 1.3;
    break;
  case 1: // 3-4
    ptValue *= 1.7;
    break;
  case 0: // badness
    ptValue = -1;
  default:
    throw cms::Exception("Invalid DT quality") 
      << "Given quality: " << trk.quality() 
      << " matches no known DT code." << std::endl;
  }
  
  // finally set the pt value in the internal track
  // it will be put into the L1 pt binning in further processing
  trk.setPtValue(ptValue);  
  trk.setPtPacked(pt_scale->getPtScale()->getPacked(ptValue));
}

Float_t  DTTwoStationBDTPtAssignment::
getBDTPt(Int_t stA, Int_t stB, Float_t dPhi, 
	 Float_t PhibA, Float_t PhibB, 
	 Float_t DTeta) {
  // getBDTPt assigns PT based on only 2-stations.
  // stA = Usually the inner  station, stA=1..3
  // stB = Usually the outter  station, stB=2..4
  // dPhi = Deflection between stA and stB. 
  //        It's technically a signed integer between -511..511.
  // PhibA = Bend angle through stA. 
  //         It's technicaly a signed integer between -511..511.
  // PhibB = Bend angle through stB. 
  //         It's technicaly a signed integer between -511..511.
  // DTeta = DT track eta value. 
  //         It's technically an unsigned integer between 0..63. But,
  //   the BDTs were trained only for +Z side, 
  //   so the inputs should be shifted to always be in the range 32..63. 

  if( stA > stB || stA == 4 || stB == 1 ) return -999;

  --stA; --stB; // get the right indices for the array

  _DTeta[stA][stB] = DTeta + (DTeta < 32)*32;
  _dPhi[stA][stB]  = dPhi;
  _phiB_one[stA]   = PhibA;
  _phiB_two[stB]   = PhibB;
  
  const std::string& mvaname = _mvanames[stA][stB];
  Float_t val;
  val = _bdt_readers[stA][stB]->EvaluateRegression(mvaname.c_str())[0];
  if (std::abs(val)>0.0) val = 1.0/std::abs(val);  
  return val;
}

#include "L1TriggerDPGUpgrade/L1TMuon/interface/PtAssignmentUnitFactory.h"
DEFINE_EDM_PLUGIN( PtAssignmentUnitFactory, 
		   DTTwoStationBDTPtAssignment, 
		   "DTTwoStationBDTPtAssignment");
