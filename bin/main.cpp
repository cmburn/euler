/* SPDX-License-Identifier: ISC */

#include "euler/game/state.h"

int
main(const int argc, char **argv)
{
	try {
		const auto state = euler::game::make_state(argc, argv);
		if (!state->initialize()) {
			state->log()->error("Failed to initialize state");
			return EXIT_FAILURE;
		}
		int exit_code = 0;
		while (state->loop(exit_code)) { }
		state->log()->info("Exiting with code {}", exit_code);
		return exit_code;
	} catch (const std::exception &e) {
		fprintf(stderr, "Unhandled exception: %s\n", e.what());
		return EXIT_FAILURE;
	}
}
