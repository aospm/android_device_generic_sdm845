/*
 * Copyright (C) 2022, Linaro Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <telephony/ril.h>

#include <dirent.h>
#include <fcntl.h>
#include <libgen.h>
#include <linux/qrtr.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <syslog.h>
#include <errno.h>

#include "libqrtr.h"
#include "logging.h"

#include "qmi_qrild_svc.h"
#include "qrild.h"
#include "qrild_qrtr.h"
#include "qrild_qmi.h"
#include "util.h"

int main(int argc, char **argv) {
	struct rild_state state;
	int len, rc;
	uint8_t buf[32];
	const char *progname = basename(argv[0]);

	(void)argc;

	qlog_setup(progname, false);
	qlog_set_min_priority(LOG_DEBUG);

	memset(&state, 0, sizeof(state));
	memset(buf, 0, 32);

	state.sock = -1;
	state.txn = 1;
	list_init(&state.services);

	state.sock = qrtr_open(0);
	if (state.sock < 0) {
		LOGE("Failed to open QRTR socket: %d", state.sock);
		return EXIT_FAILURE;
	}

	// Find all QRTR services
	qrild_qrtr_do_lookup(&state);

	//qmi_ctl_allocate_cid(&state);

	while (true) {
		rc = qrtr_poll(state.sock, -1);
		if (rc < 0)
			PLOGE_AND_EXIT("Failed to poll");
		
		qrild_qrtr_recv(&state);

		if (qmi_service_get(&state.services, QMI_SERVICE_UIM))
			qmi_req_uim_get_card_status(&state);
	}

	// len = recv(state.sock, buf, 32, 0);
	// if (len < 0)
	// 	PLOGE_AND_EXIT("recv() %d", len);
	// printf("Got %d bytes\n", len);
	// print_hex_dump("RECV:", buf, len);

	qrtr_close(state.sock);

	return 0;
}
