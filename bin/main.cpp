/* SPDX-License-Identifier: ISC */

#include "euler/app/state.h"

int
main(const int argc, char **argv)
{
	try {
		const auto state = euler::app::make_state(argc, argv);
		if (!state->initialize()) {
			state->log()->error("Failed to initialize state");
			return EXIT_FAILURE;
		}
		state->log()->info("Initialization complete");
		int exit_code = 0;
		while (state->loop(exit_code)) {
			/* ReSharper disable once CppRedundantControlFlowJump */
			continue; /* NOLINT(*-redundant-control-flow) */
		}
		state->log()->info("Exiting with code {}", exit_code);
		return exit_code;
	} catch (const std::exception &e) {
		fprintf(stderr, "Unhandled exception: %s\n", e.what());
		return EXIT_FAILURE;
	}
}
