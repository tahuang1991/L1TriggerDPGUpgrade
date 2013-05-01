#ifndef __L1TMUON_CORRIDORPTREFINEMENT_H__
#define __L1TMUON_CORRIDORPTREFINEMENT_H__
// 
// Class: L1TMuon::CorridorPtRefinement
//
// Info: Implements the 'corridor' (confidence interval)
//       based tail-clipping from B. Scurlock.
//
// Author: L. Gray (FNAL), B. Scurlock (UF)
//
#include <memory>
#include <vector>
#include <unordered_map>
#include "L1TriggerDPGUpgrade/L1TMuon/interface/PtRefinementUnit.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/FileInPath.h"
#include "TH1F.h"

class TGraph;
class TH1F;

namespace L1TMuon {
  
  class CorridorPtRefinement: public PtRefinementUnit {    
  public:
    typedef std::unique_ptr<TH1F>   ptBinning;
    typedef std::unique_ptr<TGraph> pTGraph;
    typedef std::unordered_map<unsigned, std::unordered_map<unsigned, pTGraph > > 
      corridor_2stn_map;

    CorridorPtRefinement(const edm::ParameterSet&);
    ~CorridorPtRefinement() {}

    virtual void refinePt(const edm::EventSetup&, 
			  InternalTrack&) const;
  private:
    void get_corridors_from_file();
    double solveCorridor(double rawPtHypothesis, 
			 double observable,
			 const pTGraph &corridor_belt) const;
    double calculateMaxAllowedPt(double ptHypothesis, 
				 int first_station, 
				 int second_station, 
				 double dphi, 
				 double phib_first, 
				 double phib_second, 
				 int clip_fraction);
    edm::FileInPath _fcorridors;
    ptBinning ptBins;
    corridor_2stn_map _dphi_corridors;
    std::unordered_map<unsigned, std::unique_ptr<TGraph> > _phib_corridors;    
  };
}

#endif
