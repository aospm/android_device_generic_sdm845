
#include <errno.h>
#include <linux/qrtr.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "libqrtr.h"
#include "logging.h"

#include "list.h"
#include "util.h"
#include "qrild.h"
#include "qrild_qmi.h"
#include "qmi_qrild_svc.h"

struct qmi_service_version *qrild_qmi_find_service(struct rild_state *state,
					   enum qmi_service svc) {
	struct list_item *node;
	struct qmi_service_version *service;
	list_for_each(&state->services, node) {
		service = list_entry(node, struct qmi_service_version, li);
		if (service->type == svc)
			return service;
	}

	return NULL;
}

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
uint8_t *qmi_qmux_add_header(struct qmi_service_version *service,
			     uint8_t *data, size_t *len) {
	uint8_t *buf = (uint8_t *)malloc(*len + sizeof(struct qmux_header));
	struct qmux_header *header = (struct qmux_header *)buf;

	memset(buf, 0, sizeof(struct qmux_header));
	memcpy(buf + sizeof(struct qmux_header), data, *len);

	*len += sizeof(struct qmux_header);

	free(data);

	header->tf = 1;
	header->len = *len - 1;
	header->ctrl_flag = 0;
	header->service = service->type;
	header->client = service->client_id;

	return buf;
}

int qmi_ctl_allocate_cid(struct rild_state *state) {
	size_t len;
	int rc;
	struct qmi_service_version *uim_service =
	    qrild_qmi_find_service(state, QMI_SERVICE_UIM);
	struct qrild_svc_ctl_allocate_cid_req *req =
	    qrild_svc_ctl_allocate_cid_req_alloc(state->txn);
	qrild_svc_ctl_allocate_cid_req_set_service(req, QMI_SERVICE_CTL);

	uint8_t *buf =
	    (uint8_t *)qrild_svc_ctl_allocate_cid_req_encode(req, &len);
	buf = qmi_qmux_add_header(uim_service, buf, &len);
	print_hex_dump("allocate CID", buf, len);

	rc = qrild_qrtr_send_to_service(state, QMI_SERVICE_CTL, (void *)buf,
					len);
	if (rc < 0) {
		uim_service->client_id = 1;
	}

	return rc;
}

int qmi_uim_get_card_status(struct rild_state *state) {
	size_t len;
	int rc;
	struct qmi_service_version *uim_service =
	    qrild_qmi_find_service(state, QMI_SERVICE_UIM);
	struct qrild_svc_uim_get_card_status_req *req =
	    qrild_svc_uim_get_card_status_req_alloc(state->txn);
	
	// HACK
	uim_service->client_id = 1;

	uint8_t *buf =
	    (uint8_t *)qrild_svc_uim_get_card_status_req_encode(req, &len);
	buf = qmi_qmux_add_header(uim_service, buf, &len);
	print_hex_dump("Get card status", buf, len);

	rc = qrild_qrtr_send_to_service(state, QMI_SERVICE_UIM, (void *)buf,
					len);
	
	if (qrtr_poll(state->sock, 1000) < 0) {
		PLOGE_AND_EXIT("%s", __func__);
	}

	free(buf);
	buf = malloc(4096);

	len = qrtr_recv(state->sock, (void*)buf, 4096);
	print_hex_dump("card_status response", buf, len);

	return 0;
}
