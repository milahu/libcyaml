/*
 * SPDX-License-Identifier: ISC
 *
 * Copyright (C) 2017-2018 Michael Drake <tlsa@netsurf-browser.org>
 */

#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <cyaml/cyaml.h>

#include "ttest.h"
#include "test.h"

/**
 * Print program usage
 *
 * \param[in]  prog_name  Name of program.
 */
static void usage(const char *prog_name)
{
	fprintf(stderr, "Usage: %s [-q|-v|-d]\n", prog_name);
}

/**
 * Main entry point from OS.
 *
 * \param[in]  argc  Argument count.
 * \param[in]  argv  Vector of string arguments.
 * \return Program return code.
 */
int main(int argc, char *argv[])
{
	bool pass = true;
	bool quiet = false;
	ttest_report_ctx_t rc;
	cyaml_log_fn_t log_fn = cyaml_log;
	cyaml_log_t log_level = CYAML_LOG_ERROR;
	enum {
		ARG_PROG_NAME,
		ARG_VERBOSE,
		ARG__COUNT,
	};

	if (argc > ARG__COUNT) {
		usage(argv[ARG_PROG_NAME]);
		return EXIT_FAILURE;

	} else if (argc > ARG_VERBOSE) {
		if ((strcmp(argv[ARG_VERBOSE], "-q") == 0) ||
		    (strcmp(argv[ARG_VERBOSE], "--quiet") == 0)) {
			quiet = true;
			log_fn = NULL;
		} else if ((strcmp(argv[ARG_VERBOSE], "-v") == 0) ||
		           (strcmp(argv[ARG_VERBOSE], "--verbose") == 0)) {
			log_level = CYAML_LOG_INFO;
		} else if ((strcmp(argv[ARG_VERBOSE], "-d") == 0) ||
		           (strcmp(argv[ARG_VERBOSE], "--debug") == 0)) {
			log_level = CYAML_LOG_DEBUG;
		}
	}

	rc = ttest_init(quiet);

	pass &= utf8_tests(&rc, log_level, log_fn);
	pass &= util_tests(&rc, log_level, log_fn);
	pass &= free_tests(&rc, log_level, log_fn);
	pass &= load_tests(&rc, log_level, log_fn);
	pass &= errs_tests(&rc, log_level, log_fn);
	pass &= file_tests(&rc, log_level, log_fn);
	pass &= save_tests(&rc, log_level, log_fn);

	ttest_report(&rc);

	return (pass) ? EXIT_SUCCESS : EXIT_FAILURE;
}
