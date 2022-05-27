
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

static void print_service(struct qmi_service_version *pkt) {
	if (!pkt)
		return;

	LOGD("PKT: | %4d | %4d | %5d | %4d  | %4d  | %s", pkt->type, pkt->node,
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
	struct qmi_service_version *service = qrild_qmi_find_service(state, svc_id);
	if (!service) {
		LOGE("Failed to find service %d (%s)", svc_id,
		     qmi_service_to_string(svc_id, false));
		return -1;
	}

	return qrtr_sendto(state->sock, service->node, service->port, data, sz);
}

/* QRTR doesn't implement the QMI CTL service */
bool qrild_qrtr_do_lookup(struct rild_state *state) {
	struct qrtr_ctrl_pkt pkt;
	struct qmi_service_version *service;
	int rc, len;

	if (state->sock < 0)
		return false;

	memset(&pkt, 0, sizeof(pkt));

	pkt.cmd = QRTR_TYPE_NEW_LOOKUP;

	rc = qrtr_sendto(state->sock, 1, QRTR_PORT_CTRL, &pkt, sizeof(pkt));
	if (rc < 0)
		PLOGE_AND_EXIT("Couldn't send lookup");

	LOGD("PKT: | type | node | port  | major | minor | name");

	while ((len = recv(state->sock, &pkt, sizeof(pkt), 0)) >= 0) {
		uint32_t type = pkt.cmd;

		if (len < sizeof(pkt) || type != QRTR_TYPE_NEW_SERVER) {
			PLOGW("invalid/short packet");
			continue;
		}

		if (!pkt.server.service && !pkt.server.instance &&
		    !pkt.server.node && !pkt.server.port)
			break;

		service = zalloc(sizeof(*service));

		service->type = pkt.server.service;
		service->node = pkt.server.node;
		service->port = pkt.server.port;
		service->major = pkt.server.instance & 0xff;
		service->minor = pkt.server.instance >> 8;
		service->name = qmi_service_to_string(service->type, false);

		print_service(service);

		list_append(&state->services, &service->li);
	}

	return true;
}
