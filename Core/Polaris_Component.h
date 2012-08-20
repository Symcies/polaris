#pragma once
#include "Execution_Engine.h"
#include "Data_Engine.h"
#include "Generic.h"
#include "State_Checking.h"

///============================================================================
/// Polaris_Component - adds interface
///============================================================================

template<typename InterfaceType=NULLTYPE,typename BaseType=NULLTYPE,typename ParentType=NULLTYPE,typename EntityType=NULLTYPE,typename ObjectType=Data_Object>
class Polaris_Component:
	public Polaris_Component<NULLTYPE,BaseType,ParentType,EntityType,ObjectType>,
	public InterfaceType
{
public:
#if STATE_CHECKS
	virtual void State_Check(){};
#endif
	friend InterfaceType;
	friend BaseType;

	typedef false_type Dispatched;
	typedef InterfaceType Interface_Type;
	typedef Polaris_Component This_Type;
};

///============================================================================
/// Polaris_Component - adds object and entity tag, without parent
///============================================================================

template<typename EntityType,typename ObjectType>
class Polaris_Component<NULLTYPE,NULLTYPE,NULLTYPE,EntityType,ObjectType>:
	public ObjectType
{
public:
	typedef NULLTYPE Parent_Type;
	typedef ObjectType Object_Type;
	typedef EntityType Entity_Tag;
};

///============================================================================
/// Polaris_Component - adds object and entity tag, with parent
///============================================================================

template<typename ParentType,typename EntityType,typename ObjectType>
class Polaris_Component<NULLTYPE,NULLTYPE,ParentType,EntityType,ObjectType>:
	public ObjectType
{
public:
	typedef ParentType Parent_Type;
	typedef ObjectType Object_Type;
	typedef EntityType Entity_Tag;
	Parent_Type* _parent;
};

///============================================================================
/// Polaris_Component - adds base
///============================================================================

template<typename BaseType,typename ParentType,typename EntityType,typename ObjectType>
class Polaris_Component<NULLTYPE,BaseType,ParentType,EntityType,ObjectType>:
	public Polaris_Component<NULLTYPE,NULLTYPE,ParentType,EntityType,ObjectType>,
	public BaseType
{
public:
	typedef BaseType Base_Type;
};


///============================================================================
/// Polaris_Data_Component - adds interface
///============================================================================

template<typename InterfaceType=NULLTYPE,typename BaseType=NULLTYPE,typename ParentType=NULLTYPE,typename EntityType=NULLTYPE>
class Polaris_Data_Component:
	public Polaris_Data_Component<NULLTYPE,BaseType,ParentType,EntityType>,
	public InterfaceType
{
public:
#if STATE_CHECKS
	virtual void State_Check(){};
#endif
	friend InterfaceType;
	friend BaseType;

	typedef false_type Dispatched;
	typedef InterfaceType Interface_Type;
	typedef Polaris_Data_Component This_Type;
};

///============================================================================
/// Polaris_Data_Component - adds object and entity tag, without parent
///============================================================================

template<typename EntityType>
class Polaris_Data_Component<NULLTYPE,NULLTYPE,NULLTYPE,EntityType>:
	public Data_Object
{
public:
	typedef NULLTYPE Parent_Type;
	typedef Data_Object Object_Type;
	typedef EntityType Entity_Tag;
};

///============================================================================
/// Polaris_Data_Component - adds object and entity tag, with parent
///============================================================================

template<typename ParentType,typename EntityType>
class Polaris_Data_Component<NULLTYPE,NULLTYPE,ParentType,EntityType>:
	public Data_Object
{
public:
	typedef ParentType Parent_Type;
	typedef Data_Object Object_Type;
	typedef EntityType Entity_Tag;
	Parent_Type* _parent;
};

///============================================================================
/// Polaris_Data_Component - adds base
///============================================================================

template<typename BaseType,typename ParentType,typename EntityType>
class Polaris_Data_Component<NULLTYPE,BaseType,ParentType,EntityType>:
	public Polaris_Data_Component<NULLTYPE,NULLTYPE,ParentType,EntityType>,
	public BaseType
{
public:
	typedef BaseType Base_Type;
};


///============================================================================
/// Polaris_Execution_Component - adds interface
///============================================================================

template<typename InterfaceType=NULLTYPE,typename BaseType=NULLTYPE,typename ParentType=NULLTYPE,typename EntityType=NULLTYPE>
class Polaris_Execution_Component:
	public Polaris_Execution_Component<NULLTYPE,BaseType,ParentType,EntityType>,
	public InterfaceType
{
public:
#if STATE_CHECKS
	virtual void State_Check(){};
#endif
	friend InterfaceType;
	friend BaseType;

	typedef false_type Dispatched;
	typedef InterfaceType Interface_Type;
	typedef Polaris_Execution_Component This_Type;
};

///============================================================================
/// Polaris_Execution_Component - adds object and entity tag, without parent
///============================================================================

template<typename EntityType>
class Polaris_Execution_Component<NULLTYPE,NULLTYPE,NULLTYPE,EntityType>:
	public Execution_Object
{
public:
	typedef NULLTYPE Parent_Type;
	typedef Execution_Object Object_Type;
	typedef EntityType Entity_Tag;
};

///============================================================================
/// Polaris_Execution_Component - adds object and entity tag, with parent
///============================================================================

template<typename ParentType,typename EntityType>
class Polaris_Execution_Component<NULLTYPE,NULLTYPE,ParentType,EntityType>:
	public Execution_Object
{
public:
	typedef ParentType Parent_Type;
	typedef Execution_Object Object_Type;
	typedef EntityType Entity_Tag;
	Parent_Type* _parent;
};

///============================================================================
/// Polaris_Execution_Component - adds base
///============================================================================

template<typename BaseType,typename ParentType,typename EntityType>
class Polaris_Execution_Component<NULLTYPE,BaseType,ParentType,EntityType>:
	public Polaris_Execution_Component<NULLTYPE,NULLTYPE,ParentType,EntityType>,
	public BaseType
{
public:
	typedef BaseType Base_Type;
};




///============================================================================
/// basic schedule macro
///============================================================================

#define schedule_local_event(SELF_AWARE_TYPE,CONDITIONAL_FUNCTION,LOCAL_EVENT_FUNCTION,FIRST_ITERATION,TARGET_TYPES,...) \
       ((SELF_AWARE_TYPE::This_Type*)this)->Load_Register<SELF_AWARE_TYPE::This_Type>((Conditional)(&CONDITIONAL_FUNCTION<SELF_AWARE_TYPE::This_Type,SELF_AWARE_TYPE::This_Type,NULLTYPE>),(Event)(&SELF_AWARE_TYPE::This_Type::LOCAL_EVENT_FUNCTION<SELF_AWARE_TYPE::This_Type,SELF_AWARE_TYPE::This_Type,TARGET_TYPES,__VA_ARGS__>),FIRST_ITERATION)

#define schedule_plocal_event(SELF_AWARE_TYPE,CONDITIONAL_FUNCTION,LOCAL_EVENT_FUNCTION,FIRST_ITERATION,TARGET_TYPES,...) \
       ((SELF_AWARE_TYPE::This_Type*)pthis)->Load_Register<SELF_AWARE_TYPE::This_Type>((Conditional)(&CONDITIONAL_FUNCTION<SELF_AWARE_TYPE::This_Type,SELF_AWARE_TYPE::This_Type,NULLTYPE>),(Event)(&SELF_AWARE_TYPE::This_Type::LOCAL_EVENT_FUNCTION<SELF_AWARE_TYPE::This_Type,SELF_AWARE_TYPE::This_Type,TARGET_TYPES,__VA_ARGS__>),FIRST_ITERATION)

///============================================================================
/// pthis macro
///============================================================================

#define PTHIS(SELF_AWARE_TYPE) ((SELF_AWARE_TYPE::This_Type*)this)
#define _pthis ((ThisType*)this)