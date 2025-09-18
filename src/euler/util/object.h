/* SPDX-License-Identifier: ISC */

#ifndef EULER_UTIL_OBJECT_H
#define EULER_UTIL_OBJECT_H

#include <atomic>
#include <cstddef>
#include <cstring>

#include "mruby.h"
#include "mruby/data.h"

namespace euler::util {
class State;
class Object;
class Logger;
template <typename T> class WeakReference;
template <typename T> class Reference;

template <typename To, typename From>
constexpr To
unsafe_cast(const From &from)
{
	To dst;
	std::memcpy(&dst, &from, sizeof(To));
	return dst;
}

/* Can't use shared_ptr since we need to fit this in a void *, otherwise things
 * get messy */

template <typename T> class WeakReference {
	friend class Reference<T>;

public:
	WeakReference() = default;
	WeakReference(std::nullptr_t)
	    : _object(nullptr)
	{
	}
	~WeakReference() = default;
	explicit WeakReference(T *object)
	    : _object(object)
	{
	}
	WeakReference(const WeakReference &other)
	    : _object(other._object)
	{
	}
	WeakReference &
	operator=(const WeakReference &other)
	{
		if (_object != nullptr) _object = nullptr;
		_object = other._object;
		return *this;
	}

	Reference<T>
	strengthen() const
	{
		return Reference<T>(_object);
	}

	explicit
	operator Reference<T>() const
	{
		return strengthen();
	}

	bool
	operator==(const WeakReference &other) const
	{
		return _object == other._object;
	}

	bool
	operator==(std::nullptr_t) const
	{
		return _object == nullptr;
	}

private:
	T *_object = nullptr;
};

class Object {
	template <typename T> friend class Reference;
	template <typename T> friend class WeakReference;
	friend class State;
	friend class Logger;

public:
	Object() = default;
	virtual ~Object() = default;

	[[nodiscard]] uint32_t
	reference_count() const
	{
		return _count;
	}

protected:
	Object(Object *parent);

private:
	/* We're in a weird position because State is also an object, but
	 * all Objects need a State reference. All objects except for State
	 * must be passed a State reference in their constructor. This private
	 * constructor allows us to create an Object without a State.
	 */
	// struct StateArg {};
	// explicit Object(StateArg)
	//     : _count(1)
	// {
	// }

	// WeakReference<State> _state;
	std::atomic<uint32_t> _count;
};

template <typename T> class Reference {
	friend class WeakReference<T>;
	template <typename U> friend class Reference;

private:
	static void
	decrement(T *obj)
	{
		if (obj == nullptr) return;
		if (obj->_count.fetch_sub(1, std::memory_order_release) > 1)
			return;
		std::atomic_thread_fence(std::memory_order_acquire);
		delete obj;
	}

	static void
	increment(T *obj)
	{
		if (obj == nullptr) return;
		obj->_count.fetch_add(1, std::memory_order_relaxed);
	}

public:
	void
	increment() const
	{
		increment(_object);
	}

	void
	decrement() const
	{
		decrement(_object);
	}

	Reference() = default;
	Reference(std::nullptr_t)
	    : _object(nullptr)
	{
	}
	~Reference() { decrement(_object); }
	explicit Reference(T *object)
	    : _object(object)
	{
		increment(_object);
	}

	Reference(const Reference &other)
	{
		_object = other._object;
		increment(_object);
	}

	Reference &
	operator=(const Reference &other)
	{
		if (_object != nullptr) decrement(_object);
		_object = other._object;
		increment(_object);
		return *this;
	}

	WeakReference<T>
	weaken() const
	{
		return WeakReference<T>(_object);
	}

	explicit
	operator WeakReference<T>() const
	{
		return WeakReference<T>(_object);
	}

	T *
	operator->() const
	{
		return _object;
	}

	const T *
	get() const
	{
		return _object;
	}

	T *
	get()
	{
		return _object;
	}

	void *
	wrap()
	{
		/* We're returning an instance of ourselves. Since we're the
		 * same size as a void *, we can be passed around as one. */
		increment();
		return unsafe_cast<void *>(*this);
	}

	static Reference
	unwrap(const void *ptr)
	{
		auto self = unsafe_cast<Reference>(ptr);
		self.increment();
		return self;
	}

	static Reference
	unwrap(const mrb_value value)
	{
		if (mrb_nil_p(value)) return Reference(nullptr);
		const auto ptr = DATA_PTR(value);
		if (ptr == nullptr) return Reference(nullptr);
		return unwrap(ptr);
	}

	bool
	operator==(std::nullptr_t) const
	{
		return _object == nullptr;
	}

	bool
	operator!=(std::nullptr_t) const
	{
		return _object != nullptr;
	}

	template <typename U> Reference(Reference<U> other)
	{
		static_assert(std::is_convertible_v<U *, T *>);
		_object = other._object;
		other._object = nullptr;
	}

private:
	T *_object = nullptr;
};

namespace detail {
static constexpr size_t
sizeof_reference()
{
	class Dummy : public Object { };
	return sizeof(Reference<Dummy>);
}
}

static_assert(sizeof(void *) == detail::sizeof_reference(),
    "Size of Reference<T> must be the same size as void *");

template <typename T, typename... Args>
Reference<T>
make_reference(Args &&...args)
{
	static_assert(std::is_base_of_v<Object, T>);
	auto ptr = new T(std::forward<Args>(args)...);
	return Reference<T>(ptr);
}

template <typename T>
Reference<T>
make_reference(T *ptr)
{
	static_assert(std::is_base_of_v<Object, T>);
	return Reference<T>(ptr);
}

template <typename T>
Reference<T>
make_reference(const T *ptr)
{
	static_assert(std::is_base_of_v<Object, T>);
	return Reference<T>(const_cast<T *>(ptr));
}
} /* namespace Euler::MRuby */

#endif /* EULER_UTIL_OBJECT_H */
