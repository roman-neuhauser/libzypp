/*---------------------------------------------------------------------\
|                          ____ _   __ __ ___                          |
|                         |__  / \ / / . \ . \                         |
|                           / / \ V /|  _/  _/                         |
|                          / /__ | | | | | |                           |
|                         /_____||_| |_| |_|                           |
|                                                                      |
\---------------------------------------------------------------------*/
/** \file	zypp/KeyRing.h
 *
*/
#ifndef ZYPP_KEYRING_H
#define ZYPP_KEYRING_H

#include <iosfwd>
#include <map>
#include <list>
#include <set>
#include <string>

#include "zypp/base/ReferenceCounted.h"
#include "zypp/Callback.h"
#include "zypp/base/PtrTypes.h"
#include "zypp/Locale.h"

///////////////////////////////////////////////////////////////////
namespace zypp
{ /////////////////////////////////////////////////////////////////

  DEFINE_PTR_TYPE(KeyRing);

  struct KeyRingReport : public callback::ReportBase
  {
    virtual bool askUserToAcceptUnsignedFile( const Pathname &file )
    { return true; }
    virtual bool askUserToAcceptUnknownKey( const Pathname &file, const std::string &keyid, const std::string &keyname )
    { return true; }
    virtual bool askUserToTrustKey( const std::string &keyid, const std::string &keyname, const std::string &keydetails )
    { return true; }
    virtual bool askUserToAcceptVerificationFailed( const Pathname &file, const std::string &keyid, const std::string &keyname )
    { return true; }
  };
  
  struct KeyRingSignals : public callback::ReportBase
  {
    virtual void trustedKeyAdded( const KeyRing &keyring, const std::string &keyid, const std::string &keyname )
    {}
    virtual void trustedKeyRemoved( const KeyRing &keyring, const std::string &keyid, const std::string &keyname )
    {}
  };
  
  struct PublicKey
  {
    bool operator==(PublicKey b)
    { return b.id == id; }
    
    bool operator==(std::string sid)
    { return sid == id; }
    
    std::string id;
    std::string name;
    std::string fingerprint;
  };

  ///////////////////////////////////////////////////////////////////
  //
  //	CLASS NAME : KeyRing
  //
  /** Class that represent a text and multiple translations.
  */
  class KeyRing  : public base::ReferenceCounted, private base::NonCopyable
  {
    friend std::ostream & operator<<( std::ostream & str, const KeyRing & obj );

  public:
    /** Implementation  */
    class Impl;

  public:
    /** Default ctor */
    KeyRing();
    //explicit
    //KeyRing(const Pathname &general_kr, const Pathname &trusted_kr);

    /**
     * imports a key from a file.
     * throw if key was not imported
     */
    void importKey( const Pathname &keyfile, bool trusted = false);
    PublicKey readPublicKey( const Pathname &keyfile );
    
    void dumpTrustedPublicKey( const std::string &id, std::ostream &stream )
    { dumpPublicKey(id, true, stream); }
    
    void dumpUntrustedPublicKey( const std::string &id, std::ostream &stream )
    { dumpPublicKey(id, false, stream); }
    
    void dumpPublicKey( const std::string &id, bool trusted, std::ostream &stream );
    
    /**
     * reads the public key id used to sign file with keyfile
     */
    std::string readSignatureKeyId( const Pathname &data, const Pathname &keyfile );
    
    /**
     * removes a key from the keyring.
     * If trusted is true, Remove it from trusted keyring too.
     */
    void deleteKey( const std::string &id, bool trusted =  false);

    std::list<PublicKey> publicKeys();
    std::list<PublicKey> trustedPublicKeys();

    /**
     * Follows a signature verification interacting with the user.
     * The boolr eturned depends on user desicion to trust or not.
     */
    bool verifyFileSignatureWorkflow( const Pathname &file, const Pathname &signature);
    
    bool verifyFileSignature( const Pathname &file, const Pathname &signature);
    bool verifyFileTrustedSignature( const Pathname &file, const Pathname &signature);

/** Dtor */
    ~KeyRing();

  public:

    /** Synonym for \ref text */
    //std::string asString() const
    //{}

  private:
    /** Pointer to implementation */
    RWCOW_pointer<Impl> _pimpl;
  };
  ///////////////////////////////////////////////////////////////////

  /** \relates KeyRing Stream output */
  inline std::ostream & operator<<( std::ostream & str, const KeyRing & obj )
  {
    //return str << obj.asString();
    return str;
  }

  /////////////////////////////////////////////////////////////////
} // namespace zypp
///////////////////////////////////////////////////////////////////
#endif // ZYPP_KEYRING_H
