/* SPDX-License-Identifier: ISC OR Apache-2.0 WITH LLVM-exception */

#ifndef EULER_UTIL_STORAGE_H
#define EULER_UTIL_STORAGE_H

#include <SDL3/SDL_storage.h>
#include <string>
#include <vector>

#include "euler/util/object.h"
#include "euler/util/state.h"

namespace euler::util {

class Storage final : public Object {
public:
	Storage(Reference<State> state);
	~Storage() override;

	std::string load_text_file(std::string_view path) const;
	std::vector<uint8_t> load_binary_file(std::string_view path) const;

private:
	SDL_Storage *_storage;
};
} /* namespace euler::util */


#endif /* EULER_UTIL_STORAGE_H */

