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
		Type type;
		uint64_t size;
		int64_t create_time;
		int64_t modify_time;
		int64_t access_time;
	};

	std::string load_text_file(std::string_view path) const;
	std::vector<uint8_t> load_binary_file(std::string_view path) const;

	bool ready() const;
	uint64_t file_size(const std::string &) const;
	std::vector<uint8_t> read_file(const std::filesystem::path &) const;
	void write_file(const std::filesystem::path &,
	    std::span<const uint8_t>);
	void create_directory(const std::filesystem::path &);
	using EnumerateCallback = std::function<bool(const std::filesystem::path &,
	    const std::filesystem::path &)>;
	void enumerate_directory(const std::filesystem::path &directory,
	    const EnumerateCallback &callback) const;
	void remove_path(const std::filesystem::path &);
	void rename_path(const std::filesystem::path &old_path,
	    const std::filesystem::path &new_path);
	void copy(const std::filesystem::path &old_path,
	    const std::filesystem::path &new_path);
	PathInfo path_info(const std::filesystem::path &) const;
	uint64_t space_remaining() const;
	std::vector<std::filesystem::path> glob_directory(
	    const std::filesystem::path &directory,
	    const std::string &pattern,
	    bool case_insensitive = false) const;

private:
	SDL_Storage *_storage;
};
} /* namespace euler::util */

#endif /* EULER_UTIL_STORAGE_H */
