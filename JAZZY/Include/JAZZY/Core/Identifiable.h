#pragma once

#include <JAZZY/Core/Base.h>
#include <JAZZY/Core/Common.h>
#include <type_traits> 

/*
 *		CPP Noob
 *		
 *		#define dx3d_typeid(Class) is similar to #define PI = 3.14
 *		Before compilation, the preprocessor textually replaces the keyword for the value you equated it to
 *		In this case 'dxd_typeid(Class)' is exchanged with the code defined below it
 *		
 *		not sure how 'static' works in this context
 *		size_t, an unsigned int equal to the size of an object within memory in bytes
 *		static const auto id, persists for the entirety of the program, read-only variable
 *		typeid(Class).hash_code(),	each type has a designated number of bytes then it is hashed (acts kinda like an id per type) this persists throughout the program execution
 *									when comparing objects of the exact same type, a match is guaranteed 
 */

#define dx3d_typeid(Class)\
public:\
using type_id_tag = Class;\
static size_t GetTypeId()\
{\
	static const auto id = typeid(Class).hash_code();\
	return id;\
}\
size_t getTypeId() const noexcept override\
{\
	return GetTypeId();\
}

namespace jazzy
{
	class Identifiable : public Base
	{
	public:
		explicit Identifiable(const BaseDesc& desc) :
			Base(desc)
		{
		}
		virtual size_t getTypeId() const noexcept = 0;
	};

	// template <typename T> is used to generalize functions to any type
	// concept, a named type requirement
	// Checks if the class type is the same as its type_id_tag
	template <typename T>
	concept HasTypeId = std::is_same_v<typename T::type_id_tag, T>;

	// checks if the first type is a base class of the second type
	template <typename Base, typename Derived>
	concept isRegistered = std::is_base_of_v<Base, Derived> && HasTypeId<Derived>;
}
