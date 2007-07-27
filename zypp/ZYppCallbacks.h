/*---------------------------------------------------------------------\
|                          ____ _   __ __ ___                          |
|                         |__  / \ / / . \ . \                         |
|                           / / \ V /|  _/  _/                         |
|                          / /__ | | | | | |                           |
|                         /_____||_| |_| |_|                           |
|                                                                      |
\---------------------------------------------------------------------*/
/** \file zypp/ZYppCallbacks.h
 *
*/
#ifndef ZYPP_ZYPPCALLBACKS_H
#define ZYPP_ZYPPCALLBACKS_H

#include "zypp/Callback.h"
#include "zypp/Resolvable.h"
#include "zypp/Repository.h"
#include "zypp/Pathname.h"
#include "zypp/Message.h"
#include "zypp/Url.h"
#include "zypp/ProgressData.h"
#include "zypp/media/MediaUserAuth.h"

///////////////////////////////////////////////////////////////////
namespace zypp
{ /////////////////////////////////////////////////////////////////
  
  struct ProgressReport : public callback::ReportBase
  {
    virtual void start( const ProgressData &/*task*/ )
    {}
    
    virtual bool progress( const ProgressData &/*task*/ )
    { return true; }

//     virtual Action problem(
//         Repository /*source*/
//         , Error /*error*/
//         , const std::string &/*description*/ )
//     { return ABORT; }

    virtual void finish( const ProgressData &/*task*/ )
    {}

  };

  struct ProgressReportAdaptor
  {
    
    ProgressReportAdaptor( const ProgressData::ReceiverFnc &fnc,
                           callback::SendReport<ProgressReport> &report )
      : _fnc(fnc)
      , _report(report)
      , _first(true)
    {
    }
    
    bool operator()( const ProgressData &progress )
    {
      if ( _first )
      {
        _report->start(progress);
        _first = false;
      }
      
      _report->progress(progress);
      bool value = true;
      if ( _fnc )
        value = _fnc(progress);
      
      
      if ( progress.finalReport() )
      {
        _report->finish(progress);
      }
      return value;
    }
    
    ProgressData::ReceiverFnc _fnc;
    callback::SendReport<ProgressReport> &_report;
    bool _first;
  };
  
  ////////////////////////////////////////////////////////////////////////////
  
  namespace repo
  {
    // progress for downloading a resolvable
    struct DownloadResolvableReport : public callback::ReportBase
    {
      enum Action {
        ABORT,  // abort and return error
        RETRY,	// retry
        IGNORE, // ignore this resolvable but continue
      };

      enum Error {
        NO_ERROR,
        NOT_FOUND, 	// the requested Url was not found
        IO,		// IO error
        INVALID		// the downloaded file is invalid
      };

      virtual void start(
        Resolvable::constPtr /*resolvable_ptr*/
        , const Url &/*url*/
      ) {}


      // Dowmload delta rpm:
      // - path below url reported on start()
      // - expected download size (0 if unknown)
      // - download is interruptable
      // - problems are just informal
      virtual void startDeltaDownload( const Pathname & /*filename*/, const ByteCount & /*downloadsize*/ )
      {}

      virtual bool progressDeltaDownload( int /*value*/ )
      { return true; }

      virtual void problemDeltaDownload( const std::string &/*description*/ )
      {}

      virtual void finishDeltaDownload()
      {}

      // Apply delta rpm:
      // - local path of downloaded delta
      // - aplpy is not interruptable
      // - problems are just informal
      virtual void startDeltaApply( const Pathname & /*filename*/ )
      {}

      virtual void progressDeltaApply( int /*value*/ )
      {}

      virtual void problemDeltaApply( const std::string &/*description*/ )
      {}

      virtual void finishDeltaApply()
      {}

      // Dowmload patch rpm:
      // - path below url reported on start()
      // - expected download size (0 if unknown)
      // - download is interruptable
      virtual void startPatchDownload( const Pathname & /*filename*/, const ByteCount & /*downloadsize*/ )
      {}

      virtual bool progressPatchDownload( int /*value*/ )
      { return true; }

      virtual void problemPatchDownload( const std::string &/*description*/ )
      {}

      virtual void finishPatchDownload()
      {}


      // return false if the download should be aborted right now
      virtual bool progress(int /*value*/, Resolvable::constPtr /*resolvable_ptr*/)
      { return true; }

      virtual Action problem(
        Resolvable::constPtr /*resolvable_ptr*/
	, Error /*error*/
	, const std::string &/*description*/
      ) { return ABORT; }

      virtual void finish(Resolvable::constPtr /*resolvable_ptr*/
        , Error /*error*/
        , const std::string &/*reason*/
      ) {}
    };

    // progress for probing a source
    struct ProbeRepoReport : public callback::ReportBase
    {
      enum Action {
        ABORT,  // abort and return error
        RETRY	// retry
      };

      enum Error {
        NO_ERROR,
        NOT_FOUND, 	// the requested Url was not found
        IO,		// IO error
        INVALID,		// th source is invalid
        UNKNOWN
      };

      virtual void start(const Url &/*url*/) {}
      virtual void failedProbe( const Url &/*url*/, const std::string &/*type*/ ) {}
      virtual void successProbe( const Url &/*url*/, const std::string &/*type*/ ) {}
      virtual void finish(const Url &/*url*/, Error /*error*/, const std::string &/*reason*/ ) {}

      virtual bool progress(const Url &/*url*/, int /*value*/)
      { return true; }

      virtual Action problem( const Url &/*url*/, Error /*error*/, const std::string &/*description*/ ) { return ABORT; }
    };

    struct RepoCreateReport : public callback::ReportBase
    {
      enum Action {
        ABORT,  // abort and return error
        RETRY,	// retry
        IGNORE  // skip refresh, ignore failed refresh
      };

      enum Error {
        NO_ERROR,
        NOT_FOUND, 	// the requested Url was not found
        IO,		// IO error
        REJECTED,
        INVALID, // th source is invali
        UNKNOWN
      };

      virtual void start( const zypp::Url &/*url*/ ) {}
      virtual bool progress( int /*value*/ )
      { return true; }

      virtual Action problem(
          const zypp::Url &/*url*/
          , Error /*error*/
          , const std::string &/*description*/ )
      { return ABORT; }

      virtual void finish(
          const zypp::Url &/*url*/
          , Error /*error*/
          , const std::string &/*reason*/ )
      {}
    };

    struct RepoReport : public callback::ReportBase
    {
      enum Action {
        ABORT,  // abort and return error
        RETRY,	// retry
        IGNORE  // skip refresh, ignore failed refresh
      };

      enum Error {
        NO_ERROR,
        NOT_FOUND, 	// the requested Url was not found
        IO,		// IO error
        INVALID		// th source is invalid
      };

      virtual void start( const ProgressData &/*task*/, const RepoInfo /*repo*/  ) {}
      virtual bool progress( const ProgressData &/*task*/ )
      { return true; }

      virtual Action problem(
          Repository /*source*/
          , Error /*error*/
          , const std::string &/*description*/ )
      { return ABORT; }

      virtual void finish(
          Repository /*source*/
          , const std::string &/*task*/
          , Error /*error*/
          , const std::string &/*reason*/ )
      {}
    };


    /////////////////////////////////////////////////////////////////
  } // namespace source
  ///////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////
  namespace media
  {
    // media change request callback
    struct MediaChangeReport : public callback::ReportBase
    {
      enum Action {
        ABORT,  // abort and return error
        RETRY,	// retry
        IGNORE, // ignore this media in future, not available anymore
        IGNORE_ID,	// ignore wrong medium id
        CHANGE_URL,	// change media URL
        EJECT		// eject the medium
      };

      enum Error {
        NO_ERROR,
        NOT_FOUND,  // the medie not found at all
        IO,	// error accessing the media
        INVALID, // media is broken
        WRONG	// wrong media, need a different one
      };

      virtual Action requestMedia(
        Repository /*source*/
        , unsigned /*mediumNr*/
        , Error /*error*/
        , const std::string &/*description*/
      ) { return ABORT; }
    };

    // progress for downloading a file
    struct DownloadProgressReport : public callback::ReportBase
    {
        enum Action {
          ABORT,  // abort and return error
          RETRY,	// retry
          IGNORE	// ignore the failure
        };

        enum Error {
          NO_ERROR,
          NOT_FOUND, 	// the requested Url was not found
          IO,		// IO error
          ACCESS_DENIED // user authent. failed while accessing restricted file
        };

        virtual void start( const Url &/*file*/, Pathname /*localfile*/ ) {}

        virtual bool progress(int /*value*/, const Url &/*file*/)
        { return true; }

        virtual Action problem(
          const Url &/*file*/
  	  , Error /*error*/
  	  , const std::string &/*description*/
        ) { return ABORT; }

        virtual void finish(
          const Url &/*file*/
          , Error /*error*/
	  , const std::string &/*reason*/
        ) {}
    };

    // authentication issues report
    struct AuthenticationReport : public callback::ReportBase
    {
      /**
       * Prompt for authentication data.
       * 
       * \param url       URL which required the authentication
       * \param msg       prompt text
       * \param auth_data input/output object for handling authentication
       *        data. As an input parameter auth_data can be prefilled with
       *        username (from previous try) or auth_type (available
       *        authentication methods aquired from server (only CurlAuthData)).
       *        As an output parameter it serves for sending username, pasword
       *        or other special data (derived AuthData objects).
       * \return true if user chooses to continue with authentication,
       *         false otherwise
       */
      virtual bool prompt(const Url & /* url */,
        const std::string & /* msg */,
        AuthData & /* auth_data */)
      {
        return false;
      }
    };

    /////////////////////////////////////////////////////////////////
  } // namespace media
  ///////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////
  namespace target
  {

    // resolvable Message
    struct MessageResolvableReport : public callback::ReportBase
    {
        virtual void show(
	  Message::constPtr /*message*/
        ) {}
    };

    // resolvable Script
    struct ScriptResolvableReport : public callback::ReportBase
    {
      enum Task   { DO, UNDO };
      enum Notify { OUTPUT, PING };

      /** Whether executing do_script on install or undo_script on delete. */
      virtual void start( const Resolvable::constPtr & /*script_r*/,
			  const Pathname & /*path_r*/,
                          Task )
      {}
      /** Progress provides the script output. If the script is quiet,
       * from time to time still-alive pings are sent to the ui. Returning \c FALSE
       * aborts script execution.
      */
      virtual bool progress( Notify , const std::string & = std::string() )
      { return true; }
      /** Report error. */
      virtual void problem( const std::string & /*description*/ )
      {}
      /** Report success. */
      virtual void finish()
      {}
    };

    ///////////////////////////////////////////////////////////////////
    namespace rpm
    {

      // progress for installing a resolvable
      struct InstallResolvableReport : public callback::ReportBase
      {
        enum Action {
          ABORT,  // abort and return error
          RETRY,	// retry
	  IGNORE	// ignore the failure
        };

        enum Error {
	  NO_ERROR,
          NOT_FOUND, 	// the requested Url was not found
	  IO,		// IO error
	  INVALID		// th resolvable is invalid
        };

        // the level of RPM pushing
        enum RpmLevel {
            RPM,
            RPM_NODEPS,
            RPM_NODEPS_FORCE
        };

        virtual void start(
	  Resolvable::constPtr /*resolvable*/
        ) {}

        virtual bool progress(int /*value*/, Resolvable::constPtr /*resolvable*/)
        { return true; }

        virtual Action problem(
          Resolvable::constPtr /*resolvable*/
  	  , Error /*error*/
   	  , const std::string &/*description*/
	  , RpmLevel /*level*/
        ) { return ABORT; }

        virtual void finish(
          Resolvable::constPtr /*resolvable*/
          , Error /*error*/
	  , const std::string &/*reason*/
	  , RpmLevel /*level*/
        ) {}
      };

      // progress for removing a resolvable
      struct RemoveResolvableReport : public callback::ReportBase
      {
        enum Action {
          ABORT,  // abort and return error
          RETRY,	// retry
	  IGNORE	// ignore the failure
        };

        enum Error {
	  NO_ERROR,
          NOT_FOUND, 	// the requested Url was not found
	  IO,		// IO error
	  INVALID		// th resolvable is invalid
        };

        virtual void start(
	  Resolvable::constPtr /*resolvable*/
        ) {}

        virtual bool progress(int /*value*/, Resolvable::constPtr /*resolvable*/)
        { return true; }

        virtual Action problem(
          Resolvable::constPtr /*resolvable*/
  	  , Error /*error*/
  	  , const std::string &/*description*/
        ) { return ABORT; }

        virtual void finish(
          Resolvable::constPtr /*resolvable*/
          , Error /*error*/
	  , const std::string &/*reason*/
        ) {}
      };

      // progress for rebuilding the database
      struct RebuildDBReport : public callback::ReportBase
      {
        enum Action {
          ABORT,  // abort and return error
          RETRY,	// retry
	  IGNORE	// ignore the failure
        };

        enum Error {
	  NO_ERROR,
	  FAILED		// failed to rebuild
        };

        virtual void start(Pathname /*path*/) {}

        virtual bool progress(int /*value*/, Pathname /*path*/)
        { return true; }

        virtual Action problem(
	  Pathname /*path*/
  	 , Error /*error*/
  	 , const std::string &/*description*/
        ) { return ABORT; }

        virtual void finish(
	  Pathname /*path*/
          , Error /*error*/
	  , const std::string &/*reason*/
        ) {}
      };

      // progress for converting the database
      struct ConvertDBReport : public callback::ReportBase
      {
        enum Action {
          ABORT,  // abort and return error
          RETRY,	// retry
	  IGNORE	// ignore the failure
        };

        enum Error {
	  NO_ERROR,
	  FAILED		// conversion failed
        };

        virtual void start(
	  Pathname /*path*/
        ) {}

        virtual bool progress(int /*value*/, Pathname /*path*/)
        { return true; }

        virtual Action problem(
	  Pathname /*path*/
  	  , Error /*error*/
  	 , const std::string &/*description*/
        ) { return ABORT; }

        virtual void finish(
	  Pathname /*path*/
          , Error /*error*/
          , const std::string &/*reason*/
        ) {}
      };

       // progress for scanning the database
      struct ScanDBReport : public callback::ReportBase
      {
        enum Action {
          ABORT,  // abort and return error
          RETRY,	// retry
	  IGNORE	// ignore the failure
        };

        enum Error {
	  NO_ERROR,
	  FAILED		// conversion failed
        };

        virtual void start(
        ) {}

        virtual bool progress(int /*value*/)
        { return true; }

        virtual Action problem(
  	  Error /*error*/
  	 , const std::string &/*description*/
        ) { return ABORT; }

        virtual void finish(
          Error /*error*/
	  , const std::string &/*reason*/
        ) {}
      };

      /////////////////////////////////////////////////////////////////
    } // namespace rpm
    ///////////////////////////////////////////////////////////////////

    /////////////////////////////////////////////////////////////////
  } // namespace target
  ///////////////////////////////////////////////////////////////////

  /////////////////////////////////////////////////////////////////
} // namespace zypp
///////////////////////////////////////////////////////////////////

#endif // ZYPP_ZYPPCALLBACKS_H
