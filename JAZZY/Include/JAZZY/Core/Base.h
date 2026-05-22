#pragma once

namespace jazzy
{
	class Base
	{
	public:
		Base();
		virtual ~Base();

	protected:
		//	These /**/ is causing issues with the code. I have to use the // instead >:[
		//	these "= delete" forbids the surface level copying of anything that inherits from the Base class
		//	Basically, you cant assign this class to anything and manipulate the copy, this will now cause an error
		//	You need to directly manipulate the instance
		Base(const Base&) = delete;
		Base(Base&) = delete;
		Base& operator = (const Base&) = delete;
		Base& operator = (Base&&) = delete;

	};
}

