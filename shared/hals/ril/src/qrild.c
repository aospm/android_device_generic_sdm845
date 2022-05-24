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

#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>

#include "libqrtr.h"
#include "qrild.h"
#include "util.h"

#include "qmi_qrild_svc.h"

struct rild_state {
	// The QRTR socket
	int sock;
	int txn;
};

struct rild_state state;

/**
 * qmux_add_header() - add a qmux header to a QMI packet
 * 
 * @service: the QMI service to send the data to
 * @client: the client ID
 * @data: the encoded QMI packet
 * @len: length of data
 * 
 * @return: a pointer to the QMI packet with the QMUX header prepended
 * 
 * This function frees data and replaces it with
 * a new pointer with the QMUX header prepended.
 * len is updated to the new length of the buffer.
 */
uint8_t* qmux_add_header(enum qmi_service service, uint8_t client, uint8_t* data, size_t *len) {
	uint8_t* buf = (uint8_t*)malloc(*len + sizeof(struct qmux_header));
	struct qmux_header* header = (struct qmux_header*)buf;

	memset(buf, 0, sizeof(struct qmux_header));
	memcpy(buf + sizeof(struct qmux_header), data, *len);

	*len += sizeof(struct qmux_header);

	free(data);

	header->tf = 1;
	header->len = *len - 1;
	header->ctrl_flag = 0;
	header->service = service;
	header->client = client;

	return buf;
}

int qmi_ctl_allocate_cid() {
	size_t len;
	struct qrild_svc_ctl_allocate_cid_req* req = qrild_svc_ctl_allocate_cid_req_alloc(state.txn);
	qrild_svc_ctl_allocate_cid_req_set_service(req, QMI_SERVICE_UIM);

	uint8_t* buf = (uint8_t*)qrild_svc_ctl_allocate_cid_req_encode(req, &len);
	buf = qmux_add_header(QMI_SERVICE_CTL, 0, buf, &len);
	print_hex_dump("allocate CID", buf, len);

	return 0;
}

int main(int argc, char **argv)
{
	(void)argc;
	(void)argv;
	int rc = 0;

	printf("qmux hdr size: %lu\n", sizeof(struct qmux_header));
	
	state.txn = 1;

	qmi_ctl_allocate_cid();
	return 0;

	// state->sock = qrtr_open(0);

	

	// close(state->sock);

	// return 0;
}
