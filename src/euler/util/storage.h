/* SPDX-License-Identifier: ISC OR Apache-2.0 WITH LLVM-exception */

#ifndef EULER_UTIL_STORAGE_H
#define EULER_UTIL_STORAGE_H

#include <functional>
#include <string>
#include <vector>

#include <SDL3/SDL_storage.h>

#include "euler/util/object.h"
#include "euler/util/state.h"

namespace euler::util {

class Storage final : public Object {
public:
	Storage();
	~Storage() override;
	using Data = std::vector<uint8_t>;
	using DataView = std::span<const uint8_t>;

	struct PathInfo {
		enum class Type {
			None,
			File,
			Directory,
			Other,
		};
		uint64_t size;
		int64_t create_time;
		int64_t modify_time;
		int64_t access_time;
		Type type;
	};

	bool ready() const;
	uint64_t file_size(const char *path) const;

	uint64_t file_size(const std::string &path) const
	{
		return file_size(path.c_str());
	}

	std::string read_file(const char *path) const;

	std::string
	read_file(const std::string &path) const
	{
		return read_file(path.c_str());
	}

	void write_file(const char *path, std::string_view content);

	void
	write_file(const std::string &str, const std::string_view content)
	{
		write_file(str.c_str(), content);
	}

	void
	write_file(const char *path, const DataView &content)
	{
		const auto view = std::string_view(
		    reinterpret_cast<const char *>(content.data()),
		    content.size());
		write_file(path, view);
	}

	void
	write_file(const std::string &str, const DataView &content)
	{
		write_file(str.c_str(), content);
	}

	void create_directory(const char *);

	void
	create_directory(const std::string &path)
	{
		create_directory(path.c_str());
	}

	using EnumerateCallback = std::function<bool(std::string_view directory,
	    std::string_view file)>;
	void enumerate_directory(const char *directory,
	    const EnumerateCallback &callback) const;

	void
	enumerate_directory(const std::string &directory,
	    const EnumerateCallback &callback) const
	{
		enumerate_directory(directory.c_str(), callback);
	}

	void remove_path(const char *path);
	void
	remove_path(const std::string &path)
	{
		remove_path(path.c_str());
	}

	void rename_path(const char *old_path, const char *new_path);
	void
	rename_path(const std::string &old_path, const std::string &new_path)
	{
		rename_path(old_path.c_str(), new_path.c_str());
	}

	void copy(const char *old_path, const char *new_path);
	PathInfo path_info(const char *) const;
	uint64_t space_remaining() const;
	std::vector<std::string> glob_directory(const char *directory,
	    const char *pattern, bool case_insensitive = false) const;

private:
	SDL_Storage *_storage;
};
} /* namespace euler::util */

#endif /* EULER_UTIL_STORAGE_H */
