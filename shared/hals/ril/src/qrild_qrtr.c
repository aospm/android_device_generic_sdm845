
#include <errno.h>
#include <linux/qrtr.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "libqrtr.h"
#include "logging.h"

#include "list.h"
#include "qrild.h"
#include "qrild_qrtr.h"
#include "qrild_qmi.h"
#include "util.h"

#include "qmi_qrild_svc.h"

static void print_service(struct qmi_service_info *pkt) {
	if (!pkt)
		return;

	LOGD("| %4d | %4d | %5d | %4d  | %4d  | %s", pkt->type, pkt->node,
	     pkt->port, pkt->major, pkt->minor,
	     pkt->name ? pkt->name : "<unknown>");
}

int qrild_qrtr_send_to_service(struct rild_state *state,
			       enum qmi_service svc_id, const void *data,
			       size_t sz) {
	if (svc_id == QMI_SERVICE_CTL) {
		LOGW("CTL service not supported by QRTR");
		return -1;
	}

	// The TX n is encoded in the QMI packet, it must be incremented for
	// every transmit
	state->txn++;

	struct qmi_service_info *service = qmi_service_get(&state->services, svc_id);
	if (!service) {
		LOGE("Failed to find service %d (%s)", svc_id,
		     qmi_service_to_string(svc_id, false));
		return -1;
	}

	printf("Sending to service:\n");
	print_service(service);
	print_hex_dump("QRTR TX", data, sz);

	return qrtr_sendto(state->sock, service->node, service->port, data, sz);
}

void qrild_qrtr_recv(struct rild_state *state) {
	struct qrtr_packet pkt;
	struct sockaddr_qrtr sq;
	socklen_t sl = sizeof(sq);
	char buf[1024];
	int ret, len;
	uint32_t msg_id;
	struct qmi_service_info *service;

	ret = recvfrom(state->sock, buf, sizeof(buf), 0, (void *)&sq, &sl);
	if (ret < 0) {
		if (errno == ENETRESET)
			LOGE("[QRTR] received ENETRESET, please retry\n");
		else
			LOGE("[QRTR] recvfrom failed\n");
		exit(1);
	}

	ret = qrtr_decode(&pkt, buf, ret, &sq);
	if (ret < 0) {
		LOGE("[QRTR] failed to decode qrtr packet\n");
		return;
	}
	len = ret;

	switch (pkt.type) {
	case QRTR_TYPE_NEW_SERVER:
		if (!pkt.service && !pkt.instance &&
		    !pkt.node && !pkt.port)
			break;

		service = zalloc(sizeof(*service));

		service->type = pkt.service;
		service->node = pkt.node;
		service->port = pkt.port;
		service->major = pkt.instance & 0xff;
		service->minor = pkt.instance >> 8;
		service->name = qmi_service_to_string(service->type, false);

		printf("Found new service:\n");
		print_service(service);

		list_append(&state->services, &service->li);
		break;
	case QRTR_TYPE_DATA:
		ret = qmi_decode_header(&pkt, &msg_id);
		if (ret < 0) {
			LOGE("[QRTR] failed to decode qmi header\n");
			return;
		}

		printf("[QRTR] data packet from port %d, msg_id %2x\n",
			sq.sq_port, msg_id);
		print_hex_dump("QRTR RX", pkt.data, pkt.data_len);

		switch (msg_id) {
		case (QMI_UIM_GET_CARD_STATUS):
			qmi_resp_uim_get_card_status(state, pkt.data, pkt.data_len);
			break;
		}
		break;
	};
}

/* QRTR doesn't implement the QMI CTL service */
bool qrild_qrtr_do_lookup(struct rild_state *state) {
	struct qrtr_ctrl_pkt pkt;
	struct qmi_service_info *service;
	int rc, len;

	if (state->sock < 0)
		return false;

	memset(&pkt, 0, sizeof(pkt));

	pkt.cmd = QRTR_TYPE_NEW_LOOKUP;

	rc = qrtr_sendto(state->sock, 1, QRTR_PORT_CTRL, &pkt, sizeof(pkt));
	if (rc < 0)
		PLOGE_AND_EXIT("Couldn't send lookup");

	//LOGD("| type | node | port  | major | minor | name");

	// while ((len = recv(state->sock, &pkt, sizeof(pkt), 0)) >= 0) {
	// 	uint32_t type = pkt.cmd;

	// 	if (len < sizeof(pkt) || type != QRTR_TYPE_NEW_SERVER) {
	// 		PLOGW("invalid/short packet");
	// 		continue;
	// 	}

	// 	if (!pkt.server.service && !pkt.server.instance &&
	// 	    !pkt.server.node && !pkt.server.port)
	// 		break;

	// 	service = zalloc(sizeof(*service));

	// 	service->type = pkt.server.service;
	// 	service->node = pkt.server.node;
	// 	service->port = pkt.server.port;
	// 	service->major = pkt.server.instance & 0xff;
	// 	service->minor = pkt.server.instance >> 8;
	// 	service->name = qmi_service_to_string(service->type, false);

	// 	print_service(service);

	// 	list_append(&state->services, &service->li);
	// }

	return true;
}
