#ifndef __QMI_QRILD_SVC_H__
#define __QMI_QRILD_SVC_H__

#include <stdint.h>
#include <stdlib.h>

struct qmi_tlv;

struct qmi_tlv *qmi_tlv_init(unsigned txn, unsigned msg_id, unsigned type);
struct qmi_tlv *qmi_tlv_init_ctl(unsigned txn, unsigned msg_id, unsigned type);
struct qmi_tlv *qmi_tlv_decode(void *buf, size_t len, unsigned *txn, unsigned type);
struct qmi_tlv *qmi_tlv_decode_ctl(void *buf, size_t len, unsigned *txn, unsigned type);
void *qmi_tlv_encode(struct qmi_tlv *tlv, size_t *len);
void *qmi_tlv_encode_ctl(struct qmi_tlv *tlv, size_t *len);
void qmi_tlv_free(struct qmi_tlv *tlv);

void *qmi_tlv_get(struct qmi_tlv *tlv, unsigned id, size_t *len);
void *qmi_tlv_get_array(struct qmi_tlv *tlv, unsigned id, unsigned len_size, size_t *len, size_t *size);
int qmi_tlv_set(struct qmi_tlv *tlv, unsigned id, void *buf, size_t len);
int qmi_tlv_set_array(struct qmi_tlv *tlv, unsigned id, unsigned len_size, void *buf, size_t len, size_t size);

#define QMI_QRILD_RESULT_SUCCESS 0
#define QMI_QRILD_RESULT_FAILURE 1
#define QMI_UIM_GET_CARD_STATUS 0x2F

struct qrild_svc_qmi_result {
	uint16_t result;
	uint16_t error;
};

struct qrild_svc_uim_card_status {
	uint16_t index_gw_primary;
	uint16_t index_1x_primary;
	uint16_t index_gw_secondary;
	uint16_t index_1x_secondary;
	uint8_t cards_n;
	struct qrild_svc_uim_card_status_cards {
		uint8_t card_state;
		uint8_t upin_state;
		uint8_t upin_retries;
		uint8_t upuk_retries;
		uint8_t error_code;
		uint8_t applications_n;
		struct qrild_svc_uim_card_status_cards_applications {
			uint8_t type;
			uint8_t state;
			uint8_t personalization_state;
			uint8_t personalization_feature;
			uint8_t personalization_retries;
			uint8_t personalization_unblock_retries;
			uint8_t application_identifier_value_n;
			uint8_t *application_identifier_value;
			uint8_t upin_replaces_pin1;
			uint8_t pin1_state;
			uint8_t pin1_retries;
			uint8_t puk1_retries;
			uint8_t pin2_state;
			uint8_t pin2_retries;
			uint8_t puk2_retries;
		} __attribute__((__packed__))  *applications;
	} __attribute__((__packed__)) *cards;
} __attribute__((__packed__));

struct qrild_svc_ctl_allocate_cid_req;
struct qrild_svc_ctl_allocate_cid_resp;
struct qrild_svc_uim_get_card_status_req;
struct qrild_svc_uim_get_card_status_resp;

/*
 * qrild_svc_ctl_allocate_cid_req message
 */
struct qrild_svc_ctl_allocate_cid_req *qrild_svc_ctl_allocate_cid_req_alloc(unsigned txn);
struct qrild_svc_ctl_allocate_cid_req *qrild_svc_ctl_allocate_cid_req_parse(void *buf, size_t len, unsigned *txn);
void *qrild_svc_ctl_allocate_cid_req_encode(struct qrild_svc_ctl_allocate_cid_req *ctl_allocate_cid_req, size_t *len);
void qrild_svc_ctl_allocate_cid_req_free(struct qrild_svc_ctl_allocate_cid_req *ctl_allocate_cid_req);

int qrild_svc_ctl_allocate_cid_req_set_service(struct qrild_svc_ctl_allocate_cid_req *ctl_allocate_cid_req, uint8_t val);
int qrild_svc_ctl_allocate_cid_req_get_service(struct qrild_svc_ctl_allocate_cid_req *ctl_allocate_cid_req, uint8_t *val);

/*
 * qrild_svc_ctl_allocate_cid_resp message
 */
struct qrild_svc_ctl_allocate_cid_resp *qrild_svc_ctl_allocate_cid_resp_alloc(unsigned txn);
struct qrild_svc_ctl_allocate_cid_resp *qrild_svc_ctl_allocate_cid_resp_parse(void *buf, size_t len, unsigned *txn);
void *qrild_svc_ctl_allocate_cid_resp_encode(struct qrild_svc_ctl_allocate_cid_resp *ctl_allocate_cid_resp, size_t *len);
void qrild_svc_ctl_allocate_cid_resp_free(struct qrild_svc_ctl_allocate_cid_resp *ctl_allocate_cid_resp);

int qrild_svc_ctl_allocate_cid_resp_set_result(struct qrild_svc_ctl_allocate_cid_resp *ctl_allocate_cid_resp, struct qrild_svc_qmi_result *val);
struct qrild_svc_qmi_result *qrild_svc_ctl_allocate_cid_resp_get_result(struct qrild_svc_ctl_allocate_cid_resp *ctl_allocate_cid_resp);

int qrild_svc_ctl_allocate_cid_resp_set_client_id(struct qrild_svc_ctl_allocate_cid_resp *ctl_allocate_cid_resp, uint16_t val);
int qrild_svc_ctl_allocate_cid_resp_get_client_id(struct qrild_svc_ctl_allocate_cid_resp *ctl_allocate_cid_resp, uint16_t *val);

/*
 * qrild_svc_uim_get_card_status_req message
 */
struct qrild_svc_uim_get_card_status_req *qrild_svc_uim_get_card_status_req_alloc(unsigned txn);
struct qrild_svc_uim_get_card_status_req *qrild_svc_uim_get_card_status_req_parse(void *buf, size_t len, unsigned *txn);
void *qrild_svc_uim_get_card_status_req_encode(struct qrild_svc_uim_get_card_status_req *uim_get_card_status_req, size_t *len);
void qrild_svc_uim_get_card_status_req_free(struct qrild_svc_uim_get_card_status_req *uim_get_card_status_req);

/*
 * qrild_svc_uim_get_card_status_resp message
 */
struct qrild_svc_uim_get_card_status_resp *qrild_svc_uim_get_card_status_resp_alloc(unsigned txn);
struct qrild_svc_uim_get_card_status_resp *qrild_svc_uim_get_card_status_resp_parse(void *buf, size_t len, unsigned *txn);
void *qrild_svc_uim_get_card_status_resp_encode(struct qrild_svc_uim_get_card_status_resp *uim_get_card_status_resp, size_t *len);
void qrild_svc_uim_get_card_status_resp_free(struct qrild_svc_uim_get_card_status_resp *uim_get_card_status_resp);

int qrild_svc_uim_get_card_status_resp_set_result(struct qrild_svc_uim_get_card_status_resp *uim_get_card_status_resp, struct qrild_svc_qmi_result *val);
struct qrild_svc_qmi_result *qrild_svc_uim_get_card_status_resp_get_result(struct qrild_svc_uim_get_card_status_resp *uim_get_card_status_resp);

int qrild_svc_uim_get_card_status_resp_set_status(struct qrild_svc_uim_get_card_status_resp *uim_get_card_status_resp, struct qrild_svc_uim_card_status *val);
struct qrild_svc_uim_card_status *qrild_svc_uim_get_card_status_resp_get_status(struct qrild_svc_uim_get_card_status_resp *uim_get_card_status_resp);

#endif
