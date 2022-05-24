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

struct qrild_svc_qmi_result {
	uint16_t result;
	uint16_t error;
};

struct qrild_svc_ctl_allocate_cid_req;
struct qrild_svc_ctl_allocate_cid_resp;

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

#endif
