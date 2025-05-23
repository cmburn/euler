/* SPDX-License-Identifier: ISC */

#ifndef EULER_UTIL_OBJECT_H
#define EULER_UTIL_OBJECT_H

#include <atomic>
#include <cstddef>
#include <cstring>

#include "mruby.h"

namespace Euler::Util {

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
template <typename T> class Reference {
	friend class WeakReference<T>;

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

	// static_assert(std::is_base_of_v<Object, T>);
	Reference() = default;
	Reference(std::nullptr_t)
	    : _object(nullptr)
	{
	}
	~Reference() { decrement(_object); }
	explicit Reference(T *object)
	    : _object(object)
	{
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

	WeakReference<T> weaken() const
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
	unwrap(void *ptr)
	{
		auto self = unsafe_cast<Reference>(ptr);
		self.increment();
		return self;
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

private:
	T *_object = nullptr;
};

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

	Reference<T>
	to_ref() const
	{
		return Reference<T>(_object);
	}

	Reference<T>
	operator->() const
	{
		return to_ref();
	}

	explicit operator Reference<T>() const { return to_ref(); }

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

public:
	Object()
	    : _count(1)
	{
	}

	virtual ~Object() = default;

	mrb_state *mrb() const;

	[[nodiscard]] uint32_t
	reference_count() const
	{
		return _count;
	}

private:
	std::atomic<uint32_t> _count;
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
    "Size of Reference<T> must be the same as a void *");

template <typename T, typename... Args>
Reference<T>
make_reference(Args &&...args)
{
	static_assert(std::is_base_of_v<Object, T>);
	return Reference<T>(new T(std::forward<Args>(args)...));
}

} /* namespace Euler::MRuby */

#endif /* EULER_UTIL_OBJECT_H */
