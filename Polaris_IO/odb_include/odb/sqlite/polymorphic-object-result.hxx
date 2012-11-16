// file      : odb/sqlite/polymorphic-object-result.hxx
// copyright : Copyright (c) 2009-2012 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#ifndef ODB_SQLITE_POLYMORPHIC_OBJECT_RESULT_HXX
#define ODB_SQLITE_POLYMORPHIC_OBJECT_RESULT_HXX

#include <odb/pre.hxx>

#include <cstddef> // std::size_t

#include <odb/polymorphic-object-result.hxx>

#include <odb/details/shared-ptr.hxx>

#include <odb/sqlite/version.hxx>
#include <odb/sqlite/forward.hxx> // query
#include <odb/sqlite/result.hxx>
#include <odb/sqlite/statement.hxx>

namespace odb
{
  namespace sqlite
  {
    template <typename T>
    class polymorphic_object_result_impl:
      public odb::polymorphic_object_result_impl<T>,
      public result_impl_base
    {
    public:
      typedef odb::polymorphic_object_result_impl<T> base_type;

      typedef typename base_type::object_type object_type;
      typedef typename base_type::object_traits object_traits;
      typedef typename base_type::id_type id_type;

      typedef typename base_type::pointer_type pointer_type;
      typedef typename base_type::pointer_traits pointer_traits;

      typedef typename base_type::root_type root_type;
      typedef typename base_type::root_traits root_traits;
      typedef typename base_type::discriminator_type discriminator_type;

      typedef typename object_traits::statements_type statements_type;

      virtual
      ~polymorphic_object_result_impl ();

      polymorphic_object_result_impl (const query&,
                                      details::shared_ptr<select_statement>,
                                      statements_type&);

      virtual void
      load (object_type*, bool fetch);

      virtual id_type
      load_id ();

      virtual discriminator_type
      load_discriminator ();

      virtual void
      next ();

      virtual void
      cache ();

      virtual std::size_t
      size ();

      using base_type::current;

    private:
      void
      load_image ();

    private:
      statements_type& statements_;
    };
  }
}

#include <odb/sqlite/polymorphic-object-result.txx>

#include <odb/post.hxx>

#endif // ODB_SQLITE_POLYMORPHIC_OBJECT_RESULT_HXX
