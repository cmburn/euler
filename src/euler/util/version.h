/* SPDX-License-Identifier: ISC */

#ifndef EULER_UTIL_VERSION_H
#define EULER_UTIL_VERSION_H
#include <cassert>
#include <cstdint>

#include <vulkan/vulkan_core.h>

namespace euler::util {

class Version {
public:
	constexpr Version(const uint16_t major = 0, const uint16_t minor = 0,
	    const uint16_t patch = 0)
	    : _major(major)
	    , _minor(minor)
	    , _patch(patch)
	{
		assert(major < (1 << 10));
		assert(minor < (1 << 10));
		assert(patch < (1 << 12));
	}

	[[nodiscard]] constexpr uint32_t
	to_vulkan() const
	{
		return VK_MAKE_VERSION(_major, _minor, _patch);
	}

	[[nodiscard]] constexpr
	operator uint32_t() const
	{
		return to_vulkan();
	}

	constexpr uint32_t
	major() const
	{
		return _major;
	}

	constexpr uint32_t
	minor() const
	{
		return _minor;
	}
	constexpr uint32_t
	patch() const
	{
		return _patch;
	}

	void
	set_major(const uint32_t major)
	{
		assert(major < (1 << 10));
		_major = major;
	}

	void
	set_minor(const uint32_t minor)
	{
		assert(minor < (1 << 10));
		_minor = minor;
	}

	void
	set_patch(const uint32_t patch)
	{
		assert(patch < (1 << 12));
		_patch = patch;
	}

private:
	uint16_t _major : 10;
	uint16_t _minor : 10;
	uint16_t _patch : 12;
};

extern const Version VERSION;

}

#endif /* EULER_UTIL_VERSION_H */
