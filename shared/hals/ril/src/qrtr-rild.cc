/*
 * Copyright (C) 2022 Caleb Connolly <caleb.connolly@linaro.org>
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
#define LOG_TAG "QRTR-RILD"
#include <log/log.h>

#include <utils/Errors.h>

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>

#include <cstring>

#include "libqrtr.h"

int main(int argc, char **argv)
{
	struct qrtr_ctrl_pkt pkt;
	struct sockaddr_qrtr sq;
	unsigned int instance;
	unsigned int service;
	unsigned int version;
	unsigned int node;
	unsigned int port;
	socklen_t sl = sizeof(sq);
	struct timeval tv;
	int sock;
	int len;
	int rc;

	rc = 0;
	memset(&pkt, 0, sizeof(pkt));

	sock = qrtr_open(QRTR_TYPE_NEW_LOOKUP);

	pkt.cmd = QRTR_TYPE_NEW_LOOKUP;

	rc = qrtr_sendto(sock, 1, QRTR_PORT_CTRL, &pkt, sizeof(pkt));

	RLOGD("  Service Version Instance Node  Port\n");

	while ((len = recv(sock, &pkt, sizeof(pkt), 0)) > 0) {
		unsigned int type = pkt.cmd;
		const char *name = NULL;
		unsigned int i;

		if (len < sizeof(pkt) || type != QRTR_TYPE_NEW_SERVER) {
			RLOGW("invalid/short packet");
			continue;
		}

		if (!pkt.server.service && !pkt.server.instance &&
		    !pkt.server.node && !pkt.server.port)
			break;

		service = pkt.server.service;
		version = pkt.server.instance & 0xff;
		instance = pkt.server.instance >> 8;
		node = pkt.server.node;
		port = pkt.server.port;

		name = "--";

		RLOGI("%9d %7d %8d %4d %5d %s\n",
			service, version, instance, node, port, name);
	}

	if (len < 0) {
		RLOGE("recv()");
		return -1;
	}

	close(sock);

	return 0;
}
