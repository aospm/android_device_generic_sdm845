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

struct qrild_svc_uim_get_card_status_req *qrild_svc_uim_get_card_status_req_alloc(unsigned txn)
{
	return (struct qrild_svc_uim_get_card_status_req*)qmi_tlv_init(txn, 47, 0);
}

struct qrild_svc_uim_get_card_status_req *qrild_svc_uim_get_card_status_req_parse(void *buf, size_t len, unsigned *txn)
{
	return (struct qrild_svc_uim_get_card_status_req*)qmi_tlv_decode(buf, len, txn, 0);
}

void *qrild_svc_uim_get_card_status_req_encode(struct qrild_svc_uim_get_card_status_req *uim_get_card_status_req, size_t *len)
{
	return qmi_tlv_encode((struct qmi_tlv*)uim_get_card_status_req, len);
}

void qrild_svc_uim_get_card_status_req_free(struct qrild_svc_uim_get_card_status_req *uim_get_card_status_req)
{
	qmi_tlv_free((struct qmi_tlv*)uim_get_card_status_req);
}

struct qrild_svc_uim_get_card_status_resp *qrild_svc_uim_get_card_status_resp_alloc(unsigned txn)
{
	return (struct qrild_svc_uim_get_card_status_resp*)qmi_tlv_init(txn, 47, 2);
}

struct qrild_svc_uim_get_card_status_resp *qrild_svc_uim_get_card_status_resp_parse(void *buf, size_t len, unsigned *txn)
{
	return (struct qrild_svc_uim_get_card_status_resp*)qmi_tlv_decode(buf, len, txn, 2);
}

void *qrild_svc_uim_get_card_status_resp_encode(struct qrild_svc_uim_get_card_status_resp *uim_get_card_status_resp, size_t *len)
{
	return qmi_tlv_encode((struct qmi_tlv*)uim_get_card_status_resp, len);
}

void qrild_svc_uim_get_card_status_resp_free(struct qrild_svc_uim_get_card_status_resp *uim_get_card_status_resp)
{
	qmi_tlv_free((struct qmi_tlv*)uim_get_card_status_resp);
}

int qrild_svc_uim_get_card_status_resp_set_result(struct qrild_svc_uim_get_card_status_resp *uim_get_card_status_resp, struct qrild_svc_qmi_result *val)
{
	return qmi_tlv_set((struct qmi_tlv*)uim_get_card_status_resp, 2, val, sizeof(struct qrild_svc_qmi_result));
}

struct qrild_svc_qmi_result *qrild_svc_uim_get_card_status_resp_get_result(struct qrild_svc_uim_get_card_status_resp *uim_get_card_status_resp)
{
	size_t len;
	void *ptr;

	ptr = qmi_tlv_get((struct qmi_tlv*)uim_get_card_status_resp, 2, &len);
	if (!ptr)
		return NULL;

	if (len != sizeof(struct qrild_svc_qmi_result))
		return NULL;

	return ptr;
}

int qrild_svc_uim_get_card_status_resp_set_status(struct qrild_svc_uim_get_card_status_resp *uim_get_card_status_resp, struct qrild_svc_uim_card_status *val)
{
	uint8_t* data = malloc(2048);
	size_t amt = offsetof(struct qrild_svc_uim_card_status, cards);
	size_t len = 0;
	size_t i;

	memcpy(data, val, amt);
	len += amt;


	for(i = 0; i < val->cards_n; i++) {
		size_t cards_sz = sizeof(val->cards[0]);
		size_t ii;
		amt = offsetof(struct qrild_svc_uim_card_status_cards, applications);
		printf("card %zu, len %2zu, copying %2zu bytes", i, len, amt);
		memcpy(data+len, val->cards + cards_sz * i, amt);
		len += amt;
		for(ii = 0; ii < val->cards[i].applications_n; ii++) {
			size_t applications_sz = sizeof(val->cards[0].applications[0]);
			size_t iii;
			amt = offsetof(struct qrild_svc_uim_card_status_cards_applications, application_identifier_value);
			printf("application %zu, len %2zu, copying %2zu bytes", ii, len, amt);
			memcpy(data + len, val->cards[i].applications + applications_sz * ii, amt);
			len += amt;
			for(iii = 0; iii < val->cards[i].applications[ii].application_identifier_value_n; iii++) {
				amt = sizeof(val->cards[0].applications[0].application_identifier_value[0]);
				printf("application_identifier_value %zu, len %2zu, copying %2zu bytes", iii, len, amt);
				memcpy(data + len, val->cards[i].applications[i].application_identifier_value, amt);
				len += amt;
			}
			amt = offsetof(struct qrild_svc_uim_card_status_cards_applications, puk2_retries) - offsetof(struct qrild_svc_uim_card_status_cards_applications, upin_replaces_pin1);
			printf("application %zu, len %2zu, copying %2zu bytes (upin_replaces_pin1)", ii, len, amt);
			memcpy(data + len, val->cards[i].applications + sizeof(struct qrild_svc_uim_card_status_cards_applications) * ii + offsetof(struct qrild_svc_uim_card_status_cards_applications, upin_replaces_pin1), amt);
			len += amt;
		}
	}

	return qmi_tlv_set((struct qmi_tlv*)uim_get_card_status_resp, 16, data, len);
}

struct qrild_svc_uim_card_status *qrild_svc_uim_get_card_status_resp_get_status(struct qrild_svc_uim_get_card_status_resp *uim_get_card_status_resp)
{
	size_t len;
	void *ptr;

	ptr = qmi_tlv_get((struct qmi_tlv*)uim_get_card_status_resp, 16, &len);
	if (!ptr)
		return NULL;

	if (len != sizeof(struct qrild_svc_uim_card_status))
		return NULL;

	return ptr;
}

