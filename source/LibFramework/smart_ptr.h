#pragma once

#include <memory>

#define PTR(name) \
	class name; \
	typedef std::shared_ptr<name>       name##Ptr; \
	typedef std::shared_ptr<const name> name##ConstPtr;\
	typedef std::weak_ptr<name>			name##WeakPtr;\
	typedef std::unique_ptr<name>       name##UniquePtr;