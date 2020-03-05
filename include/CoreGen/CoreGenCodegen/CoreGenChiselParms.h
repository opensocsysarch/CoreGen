//
// _CoreGenChiselParms_h_
//
// Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

/**
 * \class CoreGenChiselParms
 *
 * \ingroup CoreGenCodegen
 *
 * \brief The CoreGen Chisel Parms class - contains a map that acts as a dict for parameters 
 *
 */

#ifndef _COREGENCHISELPARMS_H_
#define _COREGENCHISELPARMS_H_

// Standard Headers
#include <iostream>
#include <fstream>
#include <string>
#include <sys/stat.h>
#include <cerrno>
#include <cstdlib>
#include <map>

#include "CoreGen/CoreGenBackend/CoreGenUtil.h"
#include "CoreGen/CoreGenBackend/CoreGenErrno.h"

//DDD:  We'll try a map of strings, I have feeling a better container is going to be more useful.
//      For now, string appears to be the most common denominator 
//      Assumed: Key and Val must have overloaded copy constructors as well as << operator
template<typename KeyT, typename ValT>
class CoreGenChiselParms {
  private:
    std::map<KeyT, ValT>  CGChiselParms;       ///< Map containing Parameters 
    CoreGenErrno                *Errno;              ///< CoreGen Errno Structure

    CoreGenChiselParms()  {};                       ///< Default constructor not supported, must supply CoreGenErrno

  public:
            CoreGenChiselParms(CoreGenErrno *E);

    bool    KeyExists(KeyT key){  return (CGChiselParms.count(key) > 0);};

    ValT    GetVal(KeyT key);

    bool    Set(KeyT key, ValT val);  

    bool    Replace(KeyT curKey, ValT newVal);

    bool    Delete(KeyT key); 
      
};

template<typename KeyT, typename ValT>
CoreGenChiselParms<KeyT, ValT>::CoreGenChiselParms(CoreGenErrno *E) :
  Errno(E),
  CGChiselParms (){
}

template<typename KeyT, typename ValT>
bool CoreGenChiselParms<KeyT, ValT>::Set(KeyT key, ValT val){
  if(KeyExists(key)){
    std::ostringstream stream;
    stream << key;
    Errno->SetError(CGERR_ERROR,
          "Key already exists in CoreGenChiselParms::SetKey - use Replace to update a Key value. Key: " + stream.str());
    return false;
  }
  std::pair<typename std::map<KeyT, ValT>::iterator,bool> retVal;
  retVal = CGChiselParms.insert(std::make_pair(key, val));
  return retVal.second;
  
}

template<typename KeyT, typename ValT>
ValT CoreGenChiselParms<KeyT, ValT>::GetVal(KeyT key){
  if(!KeyExists(key)){
    std::ostringstream stream;
    stream << key;
    Errno->SetError(CGERR_ERROR,
          "Key does not exist in CoreGenChiselParms::GetVal Key: " + stream.str());
  }
  return CGChiselParms.find(key)->second;
}

template<typename KeyT, typename ValT>
bool CoreGenChiselParms<KeyT, ValT>::Replace(KeyT curKey, ValT newVal){
  if(!KeyExists(curKey)){
    std::ostringstream stream;
    stream << curKey;
    Errno->SetError(CGERR_ERROR,
          "Key does not exist in CoreGenChiselParms::Replace Key: " + stream.str());
  }
  typename std::map<KeyT,ValT>::iterator it;

  it = CGChiselParms.find(curKey);
  if (it != CGChiselParms.end()){
    CGChiselParms.erase(it);
  }
  it = CGChiselParms.insert(std::make_pair(curKey, newVal));
  return it.second;
}
      
template<typename KeyT, typename ValT>
bool CoreGenChiselParms<KeyT, ValT>::Delete(KeyT key){
  if(!KeyExists(key)){
    std::ostringstream stream;
    stream << key;
    Errno->SetError(CGERR_ERROR,
          "Key does not exist in CoreGenChiselParms::Delete Key: " + stream.str());
  }
  typename std::map<KeyT,ValT>::iterator it;

  it = CGChiselParms.find(key);
  bool retVal = it->second;
  if (it != CGChiselParms.end()){
    CGChiselParms.erase(it);
  }
  return retVal;
}



#endif

// EOF
