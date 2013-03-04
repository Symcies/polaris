// This file was generated by ODB, object-relational mapping (ORM)
// compiler for C++.
//

namespace odb
{
  // Link_Delay
  //

  inline
  access::object_traits< ::polaris::io::Link_Delay >::id_type
  access::object_traits< ::polaris::io::Link_Delay >::
  id (const object_type& o)
  {
    return o.auto_id;
  }

  inline
  void access::object_traits< ::polaris::io::Link_Delay >::
  callback (database& db, object_type& x, callback_event e)
  {
    ODB_POTENTIALLY_UNUSED (db);
    ODB_POTENTIALLY_UNUSED (x);
    ODB_POTENTIALLY_UNUSED (e);
  }

  inline
  void access::object_traits< ::polaris::io::Link_Delay >::
  callback (database& db, const object_type& x, callback_event e)
  {
    ODB_POTENTIALLY_UNUSED (db);
    ODB_POTENTIALLY_UNUSED (x);
    ODB_POTENTIALLY_UNUSED (e);
  }

  // Performance
  //

  inline
  access::object_traits< ::polaris::io::Performance >::id_type
  access::object_traits< ::polaris::io::Performance >::
  id (const object_type& o)
  {
    return o.auto_id;
  }

  inline
  void access::object_traits< ::polaris::io::Performance >::
  callback (database& db, object_type& x, callback_event e)
  {
    ODB_POTENTIALLY_UNUSED (db);
    ODB_POTENTIALLY_UNUSED (x);
    ODB_POTENTIALLY_UNUSED (e);
  }

  inline
  void access::object_traits< ::polaris::io::Performance >::
  callback (database& db, const object_type& x, callback_event e)
  {
    ODB_POTENTIALLY_UNUSED (db);
    ODB_POTENTIALLY_UNUSED (x);
    ODB_POTENTIALLY_UNUSED (e);
  }

  // Problem
  //

  inline
  access::object_traits< ::polaris::io::Problem >::id_type
  access::object_traits< ::polaris::io::Problem >::
  id (const object_type& o)
  {
    return o.problem;
  }

  inline
  void access::object_traits< ::polaris::io::Problem >::
  callback (database& db, object_type& x, callback_event e)
  {
    ODB_POTENTIALLY_UNUSED (db);
    ODB_POTENTIALLY_UNUSED (x);
    ODB_POTENTIALLY_UNUSED (e);
  }

  inline
  void access::object_traits< ::polaris::io::Problem >::
  callback (database& db, const object_type& x, callback_event e)
  {
    ODB_POTENTIALLY_UNUSED (db);
    ODB_POTENTIALLY_UNUSED (x);
    ODB_POTENTIALLY_UNUSED (e);
  }

  // Skim
  //

  inline
  access::object_traits< ::polaris::io::Skim >::id_type
  access::object_traits< ::polaris::io::Skim >::
  id (const object_type& o)
  {
    return o.auto_id;
  }

  inline
  void access::object_traits< ::polaris::io::Skim >::
  callback (database& db, object_type& x, callback_event e)
  {
    ODB_POTENTIALLY_UNUSED (db);
    ODB_POTENTIALLY_UNUSED (x);
    ODB_POTENTIALLY_UNUSED (e);
  }

  inline
  void access::object_traits< ::polaris::io::Skim >::
  callback (database& db, const object_type& x, callback_event e)
  {
    ODB_POTENTIALLY_UNUSED (db);
    ODB_POTENTIALLY_UNUSED (x);
    ODB_POTENTIALLY_UNUSED (e);
  }

  // Event
  //

  inline
  access::object_traits< ::polaris::io::Event >::id_type
  access::object_traits< ::polaris::io::Event >::
  id (const object_type& o)
  {
    return o.auto_id;
  }

  inline
  void access::object_traits< ::polaris::io::Event >::
  callback (database& db, object_type& x, callback_event e)
  {
    ODB_POTENTIALLY_UNUSED (db);
    ODB_POTENTIALLY_UNUSED (x);
    ODB_POTENTIALLY_UNUSED (e);
  }

  inline
  void access::object_traits< ::polaris::io::Event >::
  callback (database& db, const object_type& x, callback_event e)
  {
    ODB_POTENTIALLY_UNUSED (db);
    ODB_POTENTIALLY_UNUSED (x);
    ODB_POTENTIALLY_UNUSED (e);
  }
}

namespace odb
{
  // Link_Delay
  //

  inline
  void access::object_traits_impl< ::polaris::io::Link_Delay, id_sqlite >::
  erase (database& db, const object_type& obj)
  {
    callback (db, obj, callback_event::pre_erase);
    erase (db, id (obj));
    callback (db, obj, callback_event::post_erase);
  }

  inline
  void access::object_traits_impl< ::polaris::io::Link_Delay, id_sqlite >::
  load_ (statements_type&, object_type&)
  {
  }

  // Performance
  //

  inline
  void access::object_traits_impl< ::polaris::io::Performance, id_sqlite >::
  erase (database& db, const object_type& obj)
  {
    callback (db, obj, callback_event::pre_erase);
    erase (db, id (obj));
    callback (db, obj, callback_event::post_erase);
  }

  inline
  void access::object_traits_impl< ::polaris::io::Performance, id_sqlite >::
  load_ (statements_type&, object_type&)
  {
  }

  // Problem
  //

  inline
  void access::object_traits_impl< ::polaris::io::Problem, id_sqlite >::
  erase (database& db, const object_type& obj)
  {
    callback (db, obj, callback_event::pre_erase);
    erase (db, id (obj));
    callback (db, obj, callback_event::post_erase);
  }

  inline
  void access::object_traits_impl< ::polaris::io::Problem, id_sqlite >::
  load_ (statements_type&, object_type&)
  {
  }

  // Skim
  //

  inline
  void access::object_traits_impl< ::polaris::io::Skim, id_sqlite >::
  erase (database& db, const object_type& obj)
  {
    callback (db, obj, callback_event::pre_erase);
    erase (db, id (obj));
    callback (db, obj, callback_event::post_erase);
  }

  inline
  void access::object_traits_impl< ::polaris::io::Skim, id_sqlite >::
  load_ (statements_type&, object_type&)
  {
  }

  // Event
  //

  inline
  void access::object_traits_impl< ::polaris::io::Event, id_sqlite >::
  erase (database& db, const object_type& obj)
  {
    callback (db, obj, callback_event::pre_erase);
    erase (db, id (obj));
    callback (db, obj, callback_event::post_erase);
  }

  inline
  void access::object_traits_impl< ::polaris::io::Event, id_sqlite >::
  load_ (statements_type&, object_type&)
  {
  }
}

