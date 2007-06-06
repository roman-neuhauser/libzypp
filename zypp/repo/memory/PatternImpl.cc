/*---------------------------------------------------------------------\
|                          ____ _   __ __ ___                          |
|                         |__  / \ / / . \ . \                         |
|                           / / \ V /|  _/  _/                         |
|                          / /__ | | | | | |                           |
|                         /_____||_| |_| |_|                           |
|                                                                      |
\---------------------------------------------------------------------*/
/** \file	zypp/repository/memory/PatternImpl.cc
 *
*/
#include "zypp/repo/memory/PatternImpl.h"

using namespace std;

///////////////////////////////////////////////////////////////////
namespace zypp
{ /////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////
namespace repo
{ /////////////////////////////////////////////////////////////////
namespace memory
{
///////////////////////////////////////////////////////////////////
//
//	METHOD NAME : PatternImpl::PatternImpl
//	METHOD TYPE : Ctor
//
PatternImpl::PatternImpl(data::Pattern_Ptr ptr)
{}

///////////////////////////////////////////////////////////////////
//
//	METHOD NAME : PatternImpl::~PatternImpl
//	METHOD TYPE : Dtor
//
PatternImpl::~PatternImpl()
{}

Source_Ref PatternImpl::source() const
{
  return Source_Ref::noSource;
}

TranslatedText PatternImpl::summary() const
{
  return _summary;
}

TranslatedText PatternImpl::description() const
{
  return _description;
}

TranslatedText PatternImpl::category() const
{
  return _category;
}

bool PatternImpl::userVisible() const
{
  return _visible;
}

Label PatternImpl::order() const
{
  return _order;
}

Pathname PatternImpl::icon() const
{
  return _icon;
}

/////////////////////////////////////////////////////////////////
} // namespace detail
///////////////////////////////////////////////////////////////////
}
/////////////////////////////////////////////////////////////////
} // namespace zypp
///////////////////////////////////////////////////////////////////
