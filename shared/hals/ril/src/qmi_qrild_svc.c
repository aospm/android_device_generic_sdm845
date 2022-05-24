#include <errno.h>
#include <string.h>
#include "qmi_qrild_svc.h"

struct qrild_svc_ctl_allocate_cid_req *qrild_svc_ctl_allocate_cid_req_alloc(unsigned txn)
{
	return (struct qrild_svc_ctl_allocate_cid_req*)qmi_tlv_init_ctl(txn, 34, 0);
}

struct qrild_svc_ctl_allocate_cid_req *qrild_svc_ctl_allocate_cid_req_parse(void *buf, size_t len, unsigned *txn)
{
	return (struct qrild_svc_ctl_allocate_cid_req*)qmi_tlv_decode_ctl(buf, len, txn, 0);
}

void *qrild_svc_ctl_allocate_cid_req_encode(struct qrild_svc_ctl_allocate_cid_req *ctl_allocate_cid_req, size_t *len)
{
	return qmi_tlv_encode_ctl((struct qmi_tlv*)ctl_allocate_cid_req, len);
}

void qrild_svc_ctl_allocate_cid_req_free(struct qrild_svc_ctl_allocate_cid_req *ctl_allocate_cid_req)
{
	qmi_tlv_free((struct qmi_tlv*)ctl_allocate_cid_req);
}

int qrild_svc_ctl_allocate_cid_req_set_service(struct qrild_svc_ctl_allocate_cid_req *ctl_allocate_cid_req, uint8_t val)
{
	return qmi_tlv_set((struct qmi_tlv*)ctl_allocate_cid_req, 1, &val, sizeof(uint8_t));
}

int qrild_svc_ctl_allocate_cid_req_get_service(struct qrild_svc_ctl_allocate_cid_req *ctl_allocate_cid_req, uint8_t *val)
{
	uint8_t *ptr;
	size_t len;

	ptr = qmi_tlv_get((struct qmi_tlv*)ctl_allocate_cid_req, 1, &len);
	if (!ptr)
		return -ENOENT;

	if (len != sizeof(uint8_t))
		return -EINVAL;

	*val = *(uint8_t*)ptr;
	return 0;
}

struct qrild_svc_ctl_allocate_cid_resp *qrild_svc_ctl_allocate_cid_resp_alloc(unsigned txn)
{
	return (struct qrild_svc_ctl_allocate_cid_resp*)qmi_tlv_init_ctl(txn, 34, 2);
}

struct qrild_svc_ctl_allocate_cid_resp *qrild_svc_ctl_allocate_cid_resp_parse(void *buf, size_t len, unsigned *txn)
{
	return (struct qrild_svc_ctl_allocate_cid_resp*)qmi_tlv_decode_ctl(buf, len, txn, 2);
}

void *qrild_svc_ctl_allocate_cid_resp_encode(struct qrild_svc_ctl_allocate_cid_resp *ctl_allocate_cid_resp, size_t *len)
{
	return qmi_tlv_encode_ctl((struct qmi_tlv*)ctl_allocate_cid_resp, len);
}

void qrild_svc_ctl_allocate_cid_resp_free(struct qrild_svc_ctl_allocate_cid_resp *ctl_allocate_cid_resp)
{
	qmi_tlv_free((struct qmi_tlv*)ctl_allocate_cid_resp);
}

int qrild_svc_ctl_allocate_cid_resp_set_result(struct qrild_svc_ctl_allocate_cid_resp *ctl_allocate_cid_resp, struct qrild_svc_qmi_result *val)
{
	return qmi_tlv_set((struct qmi_tlv*)ctl_allocate_cid_resp, 2, val, sizeof(struct qrild_svc_qmi_result));
}

struct qrild_svc_qmi_result *qrild_svc_ctl_allocate_cid_resp_get_result(struct qrild_svc_ctl_allocate_cid_resp *ctl_allocate_cid_resp)
{
	size_t len;
	void *ptr;

	ptr = qmi_tlv_get((struct qmi_tlv*)ctl_allocate_cid_resp, 2, &len);
	if (!ptr)
		return NULL;

	if (len != sizeof(struct qrild_svc_qmi_result))
		return NULL;

	return ptr;
}

int qrild_svc_ctl_allocate_cid_resp_set_client_id(struct qrild_svc_ctl_allocate_cid_resp *ctl_allocate_cid_resp, uint16_t val)
{
	return qmi_tlv_set((struct qmi_tlv*)ctl_allocate_cid_resp, 1, &val, sizeof(uint16_t));
}

int qrild_svc_ctl_allocate_cid_resp_get_client_id(struct qrild_svc_ctl_allocate_cid_resp *ctl_allocate_cid_resp, uint16_t *val)
{
	uint16_t *ptr;
	size_t len;

	ptr = qmi_tlv_get((struct qmi_tlv*)ctl_allocate_cid_resp, 1, &len);
	if (!ptr)
		return -ENOENT;

	if (len != sizeof(uint16_t))
		return -EINVAL;

	*val = *(uint16_t*)ptr;
	return 0;
}

