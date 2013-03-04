// This file was generated by ODB, object-relational mapping (ORM)
// compiler for C++.
//

namespace odb
{
  // Selection
  //

  inline
  access::object_traits< ::polaris::io::Selection >::id_type
  access::object_traits< ::polaris::io::Selection >::
  id (const object_type& o)
  {
    return o.auto_id;
  }

  inline
  void access::object_traits< ::polaris::io::Selection >::
  callback (database& db, object_type& x, callback_event e)
  {
    ODB_POTENTIALLY_UNUSED (db);
    ODB_POTENTIALLY_UNUSED (x);
    ODB_POTENTIALLY_UNUSED (e);
  }

  inline
  void access::object_traits< ::polaris::io::Selection >::
  callback (database& db, const object_type& x, callback_event e)
  {
    ODB_POTENTIALLY_UNUSED (db);
    ODB_POTENTIALLY_UNUSED (x);
    ODB_POTENTIALLY_UNUSED (e);
  }

  // Household
  //

  inline
  access::object_traits< ::polaris::io::Household >::id_type
  access::object_traits< ::polaris::io::Household >::
  id (const object_type& o)
  {
    return o.auto_id;
  }

  inline
  void access::object_traits< ::polaris::io::Household >::
  callback (database& db, object_type& x, callback_event e)
  {
    ODB_POTENTIALLY_UNUSED (db);
    ODB_POTENTIALLY_UNUSED (x);
    ODB_POTENTIALLY_UNUSED (e);
  }

  inline
  void access::object_traits< ::polaris::io::Household >::
  callback (database& db, const object_type& x, callback_event e)
  {
    ODB_POTENTIALLY_UNUSED (db);
    ODB_POTENTIALLY_UNUSED (x);
    ODB_POTENTIALLY_UNUSED (e);
  }

  // Vehicle
  //

  inline
  access::object_traits< ::polaris::io::Vehicle >::id_type
  access::object_traits< ::polaris::io::Vehicle >::
  id (const object_type& o)
  {
    return o.auto_id;
  }

  inline
  void access::object_traits< ::polaris::io::Vehicle >::
  callback (database& db, object_type& x, callback_event e)
  {
    ODB_POTENTIALLY_UNUSED (db);
    ODB_POTENTIALLY_UNUSED (x);
    ODB_POTENTIALLY_UNUSED (e);
  }

  inline
  void access::object_traits< ::polaris::io::Vehicle >::
  callback (database& db, const object_type& x, callback_event e)
  {
    ODB_POTENTIALLY_UNUSED (db);
    ODB_POTENTIALLY_UNUSED (x);
    ODB_POTENTIALLY_UNUSED (e);
  }

  // Trip
  //

  inline
  access::object_traits< ::polaris::io::Trip >::id_type
  access::object_traits< ::polaris::io::Trip >::
  id (const object_type& o)
  {
    return o.auto_id;
  }

  inline
  void access::object_traits< ::polaris::io::Trip >::
  callback (database& db, object_type& x, callback_event e)
  {
    ODB_POTENTIALLY_UNUSED (db);
    ODB_POTENTIALLY_UNUSED (x);
    ODB_POTENTIALLY_UNUSED (e);
  }

  inline
  void access::object_traits< ::polaris::io::Trip >::
  callback (database& db, const object_type& x, callback_event e)
  {
    ODB_POTENTIALLY_UNUSED (db);
    ODB_POTENTIALLY_UNUSED (x);
    ODB_POTENTIALLY_UNUSED (e);
  }

  // Plan
  //

  inline
  access::object_traits< ::polaris::io::Plan >::id_type
  access::object_traits< ::polaris::io::Plan >::
  id (const object_type& o)
  {
    return o.auto_id;
  }

  inline
  void access::object_traits< ::polaris::io::Plan >::
  callback (database& db, object_type& x, callback_event e)
  {
    ODB_POTENTIALLY_UNUSED (db);
    ODB_POTENTIALLY_UNUSED (x);
    ODB_POTENTIALLY_UNUSED (e);
  }

  inline
  void access::object_traits< ::polaris::io::Plan >::
  callback (database& db, const object_type& x, callback_event e)
  {
    ODB_POTENTIALLY_UNUSED (db);
    ODB_POTENTIALLY_UNUSED (x);
    ODB_POTENTIALLY_UNUSED (e);
  }

  // Traveler
  //

  inline
  access::object_traits< ::polaris::io::Traveler >::id_type
  access::object_traits< ::polaris::io::Traveler >::
  id (const object_type& o)
  {
    return o.auto_id;
  }

  inline
  void access::object_traits< ::polaris::io::Traveler >::
  callback (database& db, object_type& x, callback_event e)
  {
    ODB_POTENTIALLY_UNUSED (db);
    ODB_POTENTIALLY_UNUSED (x);
    ODB_POTENTIALLY_UNUSED (e);
  }

  inline
  void access::object_traits< ::polaris::io::Traveler >::
  callback (database& db, const object_type& x, callback_event e)
  {
    ODB_POTENTIALLY_UNUSED (db);
    ODB_POTENTIALLY_UNUSED (x);
    ODB_POTENTIALLY_UNUSED (e);
  }
}

namespace odb
{
  // Selection
  //

  inline
  void access::object_traits_impl< ::polaris::io::Selection, id_sqlite >::
  erase (database& db, const object_type& obj)
  {
    callback (db, obj, callback_event::pre_erase);
    erase (db, id (obj));
    callback (db, obj, callback_event::post_erase);
  }

  inline
  void access::object_traits_impl< ::polaris::io::Selection, id_sqlite >::
  load_ (statements_type&, object_type&)
  {
  }

  // Household
  //

  inline
  void access::object_traits_impl< ::polaris::io::Household, id_sqlite >::
  erase (database& db, const object_type& obj)
  {
    callback (db, obj, callback_event::pre_erase);
    erase (db, id (obj));
    callback (db, obj, callback_event::post_erase);
  }

  inline
  void access::object_traits_impl< ::polaris::io::Household, id_sqlite >::
  load_ (statements_type&, object_type&)
  {
  }

  // Vehicle
  //

  inline
  void access::object_traits_impl< ::polaris::io::Vehicle, id_sqlite >::
  erase (database& db, const object_type& obj)
  {
    callback (db, obj, callback_event::pre_erase);
    erase (db, id (obj));
    callback (db, obj, callback_event::post_erase);
  }

  inline
  void access::object_traits_impl< ::polaris::io::Vehicle, id_sqlite >::
  load_ (statements_type&, object_type&)
  {
  }

  // Trip
  //

  inline
  void access::object_traits_impl< ::polaris::io::Trip, id_sqlite >::
  erase (database& db, const object_type& obj)
  {
    callback (db, obj, callback_event::pre_erase);
    erase (db, id (obj));
    callback (db, obj, callback_event::post_erase);
  }

  inline
  void access::object_traits_impl< ::polaris::io::Trip, id_sqlite >::
  load_ (statements_type&, object_type&)
  {
  }

  // Plan
  //

  inline
  void access::object_traits_impl< ::polaris::io::Plan, id_sqlite >::
  erase (database& db, const object_type& obj)
  {
    callback (db, obj, callback_event::pre_erase);
    erase (db, id (obj));
    callback (db, obj, callback_event::post_erase);
  }

  inline
  void access::object_traits_impl< ::polaris::io::Plan, id_sqlite >::
  load_ (statements_type&, object_type&)
  {
  }

  // Traveler
  //

  inline
  void access::object_traits_impl< ::polaris::io::Traveler, id_sqlite >::
  erase (database& db, const object_type& obj)
  {
    callback (db, obj, callback_event::pre_erase);
    erase (db, id (obj));
    callback (db, obj, callback_event::post_erase);
  }

  inline
  void access::object_traits_impl< ::polaris::io::Traveler, id_sqlite >::
  load_ (statements_type&, object_type&)
  {
  }
}

