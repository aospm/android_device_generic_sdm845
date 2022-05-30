
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
#include "qrild_qrtr.h"
#include "qmi_qrild_svc.h"

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
uint8_t *qmi_qmux_add_header(struct qmi_service_info *service,
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
	struct qmi_service_info *uim_service =
	    qmi_service_get(&state->services, QMI_SERVICE_UIM);
	struct qrild_svc_ctl_allocate_cid_req *req =
	    qrild_svc_ctl_allocate_cid_req_alloc(state->txn);
	qrild_svc_ctl_allocate_cid_req_set_service(req, QMI_SERVICE_CTL);

	if (!uim_service) {
		LOGW("Can't find UIM service!\n");
		return -1;
	}

	uint8_t *buf =
	    (uint8_t *)qrild_svc_ctl_allocate_cid_req_encode(req, &len);
	//buf = qmi_qmux_add_header(uim_service, buf, &len);
	print_hex_dump("allocate CID", buf, len);

	rc = qrild_qrtr_send_to_service(state, QMI_SERVICE_CTL, (void *)buf,
					len);
	if (rc < 0) {
		uim_service->client_id = 1;
	}

	return rc;
}

int qmi_req_uim_get_card_status(struct rild_state *state) {
	size_t len;
	int rc;
	struct qmi_service_info *uim_service =
	    qmi_service_get(&state->services, QMI_SERVICE_UIM);
	struct qrild_svc_uim_get_card_status_req *req =
	    qrild_svc_uim_get_card_status_req_alloc(state->txn);
	
	// HACK
	uim_service->client_id = 1;

	uint8_t *buf =
	    (uint8_t *)qrild_svc_uim_get_card_status_req_encode(req, &len);
	//buf = qmi_qmux_add_header(uim_service, buf, &len);
	print_hex_dump("Get card status", buf, len);

	rc = qrild_qrtr_send_to_service(state, QMI_SERVICE_UIM, (void *)buf,
					len);

	qrild_svc_uim_get_card_status_req_free(req);

	return 0;
}

void dump_card_status(struct qrild_svc_uim_card_status *cs) {
	int i, ii, iii;
	printf("index_gw_primary: %u, ", cs->index_gw_primary);
	printf("index_1x_primary: %u, ", cs->index_1x_primary);
	printf("index_gw_secondary: %u, ", cs->index_gw_secondary);
	printf("index_1x_secondary: %u, ", cs->index_1x_secondary);
	printf("cards_n: %u", cs->cards_n);

	for (i = 0; i < cs->cards_n; i++) {
		printf("\ncard %d:\n\t", i);
		printf("card_state: %u, ", cs->cards[i].card_state);
		printf("upin_state: %u, ", cs->cards[i].upin_state);
		printf("upin_retries: %u, ", cs->cards[i].upin_retries);
		printf("upuk_retries: %u, ", cs->cards[i].upuk_retries);
		printf("error_code: %u, ", cs->cards[i].error_code);
		printf("applications_n: %u, ", cs->cards[i].applications_n);
		for (ii = 0; ii < cs->cards[i].applications_n; ii++) {
			printf("\n\tapplication %d:\n\t\t", ii);
			printf("type: %u, ",
			       cs->cards[i].applications[ii].type);
			printf("state: %u, ",
			       cs->cards[i].applications[ii].state);
			printf("personalization_state: %u, ",
			       cs->cards[i]
				   .applications[ii]
				   .personalization_state);
			printf("personalization_feature: %u, ",
			       cs->cards[i]
				   .applications[ii]
				   .personalization_feature);
			printf("personalization_retries: %u, ",
			       cs->cards[i]
				   .applications[ii]
				   .personalization_retries);
			printf("application_identifier_value_n: %u, ",
			       cs->cards[i]
				   .applications[ii]
				   .application_identifier_value_n);
			printf("\n\t\tapplication identifier\n\t\t\t");
			for (iii = 0; iii < cs->cards[i]
						.applications[ii]
						.application_identifier_value_n;
			     iii++) {
				printf("[%u], ",
				       cs->cards[i]
					   .applications[ii]
					   .application_identifier_value[iii]);
			}
			printf("\n\t\t");
			printf("pin2_state: %u, ",
			       cs->cards[i].applications[ii].pin2_state);
			printf("pin2_retries: %u, ",
			       cs->cards[i].applications[ii].pin2_retries);
			printf("puk2_retries: %u, ",
			       cs->cards[i].applications[ii].puk2_retries);
		}
	}
	printf("\n");
}

int qmi_resp_uim_get_card_status(struct rild_state *state, uint8_t *buf, size_t sz) {
	size_t buf_sz;
	struct qrild_svc_uim_get_card_status_resp *resp =
		qrild_svc_uim_get_card_status_resp_parse(buf, sz, &state->txn);
	
	struct qrild_svc_qmi_result *res = qrild_svc_uim_get_card_status_resp_get_result(resp);
	printf("%s: result: %u, error: %u\n", __func__, res->result, res->error);

	uint8_t *ptr = qmi_tlv_get((struct qmi_tlv*)resp, 16, &buf_sz);
	print_hex_dump("UIM card status resp", ptr, buf_sz);

	struct qrild_svc_uim_card_status *status =
		qrild_svc_uim_get_card_status_resp_get_status(resp);
	
	dump_card_status(status);

	exit(0);
	
	return 0;
}

const char* qmi_service_to_string(enum qmi_service service, bool short_name) {
	const struct enum_value* v = &qmi_service_values[0];
	while (v->value_str) {
		if (v->value == service)
			return short_name ? v->name : v->value_str;
		v++;
	}

	return NULL;
}
