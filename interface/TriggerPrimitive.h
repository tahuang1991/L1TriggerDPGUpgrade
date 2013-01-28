
// 
// Class: L1ITMu::TriggerPrimitive
//
// Info: This class implements a unifying layer between DT, CSC and RPC
//       trigger primitives (TPs) such that TPs from different subsystems
//       can be queried for their position and orientation information
//       in a consistent way amongst all subsystems.
//
// Note: Not all input data types are persistable, so we make local
//       copies of all data from various digi types.
//
//       At the end of the day this should represent the output of some
//       common sector receiver module.
//
// Author: L. Gray (FNAL)
//

#include <boost/cstdint.hpp>

//DetId
#include "DataFormats/DetId/interface/DetId.h"

// DT digi types
class DTChamberId;
class L1MuDTChambPhDigi;
class L1MuDTChambThDigi;

// CSC digi types
class CSCCorrelatedLCTDigi;
class CSCDetId;

// RPC digi types
class RPCDigi;
class RPCDetId;

namespace L1ITMu {

  class TriggerPrimitive {
  public:
    // define the subsystems that we have available
    enum subsystem_type{kDT,kCSC,kRPC,kNSubsystems};
    
    // define the data we save locally from each subsystem type
    // variables in these structs keep their colloquial meaning
    // within a subsystem
    struct RPCData {
      unsigned sector;
      unsigned chamber;
      uint16_t strip;
      uint16_t bx;
    };

    struct CSCData {
      unsigned sector;
      unsigned subsector;
      uint16_t trknmb;
      uint16_t valid;
      uint16_t quality;
      uint16_t keywire;
      uint16_t strip;
      uint16_t pattern;
      uint16_t bend;
      uint16_t bx;
      uint16_t mpclink;
      uint16_t bx0; 
      uint16_t syncErr;
      uint16_t cscID;
    };

    struct DTData {
      // from ChambPhDigi
      int bx; // relative? bx number
      int wheel; // wheel number -2,-1,0,1,2
      int sector; // 1-12 in DT speak (these correspond to CSC sub-sectors)
      int station; // 1-4 radially outwards
      int radialAngle; // need to look this up
      int bendingAngle; // pitch of segment relative to chamber
      int qualityCode; // need to decode
      int Ts2TagCode; // ??
      int BxCntCode; // ????
      // from ChambThDigi
      unsigned char outPos[7]; // seems to be indexed by BX
      unsigned char outQual[7]; // again by BX
    };

    //Persistency
    TriggerPrimitive(): _subsystem(kNSubsystems) {}

    //DT
    TriggerPrimitive(const DTChamberId&,
		     const L1MuDTChambPhDigi&);
    TriggerPrimitive(const DTChamberId&,
		     const L1MuDTChambThDigi&);    
    TriggerPrimitive(const DTChamberId&,		     
		     const L1MuDTChambPhDigi&,
		     const L1MuDTChambThDigi&);    
    //CSC
    TriggerPrimitive(const CSCDetId&,
		     const CSCCorrelatedLCTDigi&);
    //RPC
    TriggerPrimitive(const RPCDetId&,
		     const RPCDigi&);
    
    // return the subsystem we belong to
    const subsystem_type subsystem() const { return _subsystem; }    

    const double getCMSGlobalEta() const { return _eta; }
    void   setCMSGlobalEta(const double eta) { _eta = eta; }
    const double getCMSGlobalPhi() const { return _phi; }    
    void   setCMSGlobalPhi(const double phi) { _phi = phi; }

    void setThetaBend(const double theta) { _theta = theta; }
    double getThetaBend() const { return _theta; }

    template<typename IDType>
      IDType detId() const { return IDType(_id); }

    // accessors to raw subsystem data
    const DTData  getDTData()  const { return _dt;  }
    const CSCData getCSCData() const { return _csc; }
    const RPCData getRPCData() const { return _rpc; }    

    // consistent accessors to common information    
    const int getBX() const;
    
    const unsigned getGlobalSector() const { return _globalsector; } 
    const unsigned getSubSector() const { return _subsector; } 

  private:
    // Translate to 'global' position information at the level of 60
    // degree sectors. Use CSC sectors as a template
    void calculateDTGlobalSector(const DTChamberId& chid, 
				 unsigned& global_sector, 
				 unsigned& subsector );
    void calculateCSCGlobalSector(const CSCDetId& chid, 
				  unsigned& global_sector, 
				  unsigned& subsector );
    void calculateRPCGlobalSector(const RPCDetId& chid, 
				  unsigned& global_sector, 
				  unsigned& subsector );
      
    DTData  _dt;
    CSCData _csc;
    RPCData _rpc;
    
    DetId _id;
    
    subsystem_type _subsystem;

    unsigned _globalsector; // [1,6] in 60 degree sectors
    unsigned _subsector; // [1,2] in 30 degree partitions of a sector 
    double _eta,_phi; // global pseudorapidity, phi
    double _theta; // bend angle with respect to ray from (0,0,0)
  };

}
