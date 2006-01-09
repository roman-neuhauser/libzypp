/*---------------------------------------------------------------------\
|                          ____ _   __ __ ___                          |
|                         |__  / \ / / . \ . \                         |
|                           / / \ V /|  _/  _/                         |
|                          / /__ | | | | | |                           |
|                         /_____||_| |_| |_|                           |
|                                                                      |
\---------------------------------------------------------------------*/
/** \file	zypp/NVRAD.h
 *
*/
#ifndef ZYPP_NVRAD_H
#define ZYPP_NVRAD_H

#include "zypp/NVRA.h"
#include "zypp/Dependencies.h"

///////////////////////////////////////////////////////////////////
namespace zypp
{ /////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////
  //
  //	CLASS NAME : NVRAD
  //
  /**  Helper storing Name, Edition, Arch and Dependencies. */
  struct NVRAD : public NVRA
  {
    /** Default ctor */
    NVRAD()
    {}

    /** Ctor */
    explicit
    NVRAD( const std::string & name_r,
           const Edition & edition_r = Edition(),
           const Arch & arch_r = Arch(),
           const Dependencies & deps_r = Dependencies() )
    : NVRA( name_r, edition_r, arch_r )
    , deps( deps_r )
    {}

    /** Ctor */
    explicit
    NVRAD( const NVRA & nvra_r,
           const Dependencies & deps_r = Dependencies() )
    : NVRA( nvra_r )
    , deps( deps_r )
    {}

    /** Ctor */
    explicit
    NVRAD( const NVR & nvr_r,
           const Arch & arch_r = Arch(),
           const Dependencies & deps_r = Dependencies() )
    : NVRA( nvr_r, arch_r )
    , deps( deps_r )
    {}

    /**  */
    Dependencies deps;
  };
  ///////////////////////////////////////////////////////////////////

  /////////////////////////////////////////////////////////////////
} // namespace zypp
///////////////////////////////////////////////////////////////////
#endif // ZYPP_NVRAD_H
