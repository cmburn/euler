/* SPDX-License-Identifier: ISC OR Apache-2.0 WITH LLVM-exception */

#include "euler/util/storage.h"

#include <physfs.h>
#include <physfssdl3.h>
#include <SDL3/SDL_storage.h>

euler::util::Storage::Storage() { _storage = PHYSFSSDL3_makeStorage(); }

euler::util::Storage::~Storage() { SDL_CloseStorage(_storage); }

bool
euler::util::Storage::ready() const
{
	return SDL_StorageReady(_storage);
}

uint64_t
euler::util::Storage::file_size(const char *path) const
{
	uint64_t size = 0;
	if (!SDL_GetStorageFileSize(_storage, path, &size))
		throw std::runtime_error(SDL_GetError());
	return size;
}

std::string
euler::util::Storage::read_file(const char *path) const
{
	const auto size = file_size(path);
	std::string content;
	content.resize(size);
	if (!SDL_ReadStorageFile(_storage, path, content.data(), size))
		throw std::runtime_error(SDL_GetError());
	return content;
}

void
euler::util::Storage::write_file(const char *path,
    const std::string_view content)
{
	if (const auto data = reinterpret_cast<const void *>(content.data());
	    !SDL_WriteStorageFile(_storage, path, data, content.size())) {
		throw std::runtime_error(SDL_GetError());
	}
}

void
euler::util::Storage::create_directory(const char *path)
{
	if (!SDL_CreateStorageDirectory(_storage, path))
		throw std::runtime_error(SDL_GetError());
}

struct UserData {
	using EnumerateCallback = euler::util::Storage::EnumerateCallback;
	const EnumerateCallback &callback;
	std::string error;
	void *
	to_ptr()
	{
		return this;
	}
	static UserData *
	from_ptr(void *ptr)
	{
		return static_cast<UserData *>(ptr);
	}
	SDL_EnumerationResult execute(std::string_view directory,
	    std::string_view file);
	static SDL_EnumerationResult
	execute(void *userdata, const char *dir, const char *file)
	{
		return from_ptr(userdata)->execute(dir, file);
	}
};

SDL_EnumerationResult
UserData::execute(const std::string_view directory, const std::string_view file)
{
	try {
		if (callback(directory, file)) return SDL_ENUM_CONTINUE;
		return SDL_ENUM_SUCCESS;
	} catch (const std::exception &e) {
		error = e.what();
		return SDL_ENUM_FAILURE;
	} catch (...) {
		error = "Unknown error in enumerate callback";
		return SDL_ENUM_FAILURE;
	}
}

void
euler::util::Storage::enumerate_directory(const char *directory,
    const EnumerateCallback &callback) const
{
	UserData user_data = {
		.callback = callback,
		.error = "",
	};

	if (!SDL_EnumerateStorageDirectory(_storage, directory,
		UserData::execute, &user_data)) {
		throw std::runtime_error(user_data.error);
	}
}
void
euler::util::Storage::remove_path(const char *path)
{
	if (!SDL_RemoveStoragePath(_storage, path))
		throw std::runtime_error(SDL_GetError());
}
void
euler::util::Storage::rename_path(const char *old_path, const char *new_path)
{
	if (!SDL_RenameStoragePath(_storage, old_path, new_path))
		throw std::runtime_error(SDL_GetError());
}

void
euler::util::Storage::copy(const char *old_path, const char *new_path)
{
	if (!SDL_CopyStorageFile(_storage, old_path, new_path))
		throw std::runtime_error(SDL_GetError());
}

using PathInfo = euler::util::Storage::PathInfo;

static PathInfo::Type
from_sdl(const SDL_PathType type)
{
	switch (type) {
	case SDL_PATHTYPE_NONE: return PathInfo::Type::None;
	case SDL_PATHTYPE_FILE: return PathInfo::Type::File;
	case SDL_PATHTYPE_DIRECTORY: return PathInfo::Type::Directory;
	default: return PathInfo::Type::Other;
	}
}

euler::util::Storage::PathInfo
euler::util::Storage::path_info(const char *path) const
{
	SDL_PathInfo info;
	if (!SDL_GetStoragePathInfo(_storage, path, &info))
		throw std::runtime_error(SDL_GetError());
	return PathInfo {
		.size = info.size,
		.create_time = info.create_time,
		.modify_time = info.modify_time,
		.access_time = info.access_time,
		.type = from_sdl(info.type),
	};
}

uint64_t
euler::util::Storage::space_remaining() const
{
	return SDL_GetStorageSpaceRemaining(_storage);
}

std::vector<std::string>
euler::util::Storage::glob_directory(const char *directory, const char *pattern,
    bool case_insensitive) const
{
	const SDL_GlobFlags flags
	    = case_insensitive ? SDL_GLOB_CASEINSENSITIVE : 0;
	int count = 0;
	const auto files = SDL_GlobStorageDirectory(_storage, directory,
	    pattern, flags, &count);
	if (files == nullptr) throw std::runtime_error(SDL_GetError());
	std::vector<std::string> result;
	result.reserve(count);
	for (int i = 0; i < count; ++i) result.emplace_back(files[i]);
	SDL_free(files);
	return result;
}
