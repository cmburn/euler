/* SPDX-License-Identifier: ISC OR Apache-2.0 WITH LLVM-exception */

#include "euler/util/storage.h"

#include <physfs.h>
#include <physfssdl3.h>

euler::util::Storage::Storage()
{
	_storage = PHYSFSSDL3_makeStorage();
}

euler::util::Storage::~Storage()
{
	SDL_CloseStorage(_storage);
}

std::string
euler::util::Storage::load_text_file(std::string_view path) const
{
	throw std::runtime_error("Not implemented");
}
std::vector<uint8_t>
euler::util::Storage::load_binary_file(std::string_view path) const
{
	throw std::runtime_error("Not implemented");
}