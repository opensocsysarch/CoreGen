//
// _CoreGenPluginTypes_h_
//
// Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// CoreGen can not be copied and/or distributed without the
// express permission of Tactical Computing Laboratories, LLC
//

/**
 * \class CoreGenPluginTypes
 *
 * \ingroup CoreGen
 *
 * \brief CoreGen Plugin Types
 *
 */

#ifndef _COREGENPLUGINTYPES_H_
#define _COREGENPLUGINTYPES_H_

#include <string>
#include "CoreGen/CoreGenPlugin/CoreGenPluginImpl.h"

/*! \typedef create_t
 *  \brief Function pointer to CoreGenPluginImpl create function
 *  Function pointer type defintion for the CoreGenPluginImpl
 *  create function.
 */
typedef CoreGenPluginImpl* create_t(CoreGenErrno *E);

/*! \typedef destroy_t
 *  \brief Function pointer to CoreGenPluginImpl destroy function
 *  Function pointer type defintion for the CoreGenPluginImpl
 *  destroy function.
 */
typedef void destroy_t(CoreGenPluginImpl*);

/*! \typedef getname_t
 *  \brief Function pointer to CoreGenPluginImpl getname function
 *  Function pointer type defintion for the CoreGenPluginImpl
 *  getname function.
 */
typedef char *getname_t();

/*! \struct CGPluginFunc
 *  \brief Struct containing plugin function pointers
 *
 *  Contains the required set of function pointers to load
 *  and utilizes plugins
 *
 */
typedef struct{
  create_t*   create;   ///< Create function pointer (constructor)
  destroy_t*  destroy;  ///< Destroy function pointer (destructor)
  getname_t*  getname;  ///< Getname function pointer (name resolution)
}CGPluginFunc;

#endif

// EOF
