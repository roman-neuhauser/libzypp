

#ifndef ZYPP_REPOSITORY_IMPL_h
#define ZYPP_REPOSITORY_IMPL_h

#include <iosfwd>
#include <string>

#include "zypp/base/ReferenceCounted.h"
#include "zypp/base/NonCopyable.h"
#include "zypp/base/PtrTypes.h"
#include "zypp/base/ProvideNumericId.h"
#include "zypp/ResStore.h"
#include "zypp/Repository.h"
#include "zypp/RepoInfo.h"

///////////////////////////////////////////////////////////////////
namespace zypp
{ /////////////////////////////////////////////////////////////////
  ///////////////////////////////////////////////////////////////////
  namespace repo
  { /////////////////////////////////////////////////////////////////

    DEFINE_PTR_TYPE(RepositoryImpl);

    class RepositoryImpl : public base::ReferenceCounted,
                           public base::ProvideNumericId<RepositoryImpl,Repository::NumericId>,
                           private base::NonCopyable
    {
      friend std::ostream & operator<<( std::ostream & str, const RepositoryImpl & obj );
    public:
      
      /**
       * \short Ctor.
       * \param info What is known about this source at construction time.
       *
       */
      RepositoryImpl( const RepoInfo &info = RepoInfo() );
      
      /**
       * \short Information about this repository
       * You can't change this information after creation.
       * Use \ref RepoManager for that.
       */
      const RepoInfo info() const;
      
      /**
       * \short Dtor
       */
      ~RepositoryImpl();

      const ResStore & resolvables() const;

      struct null {};
    public:
      /** Offer default Impl. */
      static RepositoryImpl_Ptr nullimpl()
      {
        static RepositoryImpl_Ptr _nullimpl( new RepositoryImpl( null() ) );
        return _nullimpl;
      }

      RepositoryImpl( const null & );

      Repository selfRepository()
      { return Repository( this ); }
    protected:
      RepoInfo _info;
      
      ResStore _store;
    };
  }
}

#endif
