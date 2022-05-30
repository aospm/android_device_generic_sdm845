#include <errno.h>
#include <string.h>
#include <stddef.h>
#include <stdio.h>
#include "qmi_qrild_svc.h"

void print_hex_dump(const char *prefix, const void *buf, size_t len);

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

	memset(data, 0, 2048);
	memcpy(data, val, amt);
	len += amt;

	size_t cards_sz = sizeof(struct qrild_svc_uim_card_status_cards);
	printf("ptr to cards[1] - cards[0]: %p\n", &val->cards[1] - &val->cards[0]);
	for(i = 0; i < val->cards_n; i++) {
		size_t ii;
		amt = offsetof(struct qrild_svc_uim_card_status_cards, applications);
		printf("card %zu, len %2zu, copying %2zu bytes\n", i, len, amt);
		memcpy(data+len, &val->cards[i], amt);
		print_hex_dump("serialised card", data + len, 9);
		len += amt;
		for(ii = 0; ii < val->cards[i].applications_n; ii++) {
			size_t applications_sz = sizeof(struct qrild_svc_uim_card_status_cards_applications);
			size_t iii;
			amt = offsetof(struct qrild_svc_uim_card_status_cards_applications, application_identifier_value);
			printf("application %zu, len %2zu, copying %2zu bytes\n", ii, len, amt);
			memcpy(data + len, &val->cards[i].applications[ii], amt);
			len += amt;
			amt = sizeof(uint8_t);
			size_t appn_id_sz = val->cards[i].applications[ii].application_identifier_value_n
				* amt;
			printf("application %zu, copying %zu id bytes\n", ii, appn_id_sz);
			for(iii = 0; iii < val->cards[i].applications[ii].application_identifier_value_n; iii++) {
				memcpy(data + len, val->cards[i].applications[ii].application_identifier_value + iii * amt, amt);
				len += amt;
			}
			amt = offsetof(struct qrild_svc_uim_card_status_cards_applications, puk2_retries) + sizeof(uint8_t) - offsetof(struct qrild_svc_uim_card_status_cards_applications, upin_replaces_pin1);
			printf("application %zu, len %2zu, copying %2zu bytes (upin_replaces_pin1)\n", ii, len, amt);
			printf("application offset of upin_replaces_pin1: %zu\n", offsetof(struct qrild_svc_uim_card_status_cards_applications, upin_replaces_pin1));
			uint8_t *ptr2 = (uint8_t*)((size_t)(&val->cards[i].applications[ii]) + offsetof(struct qrild_svc_uim_card_status_cards_applications, upin_replaces_pin1));
			memcpy(data + len, ptr2, amt);
			len += amt;
		}
	}

	return qmi_tlv_set((struct qmi_tlv*)uim_get_card_status_resp, 16, data, len);
}

struct qrild_svc_uim_card_status *qrild_svc_uim_get_card_status_resp_get_status(struct qrild_svc_uim_get_card_status_resp *uim_get_card_status_resp)
{
	size_t buf_sz, len = 0, amt, i;
	uint8_t *ptr;
	struct qrild_svc_uim_card_status *cs;

	ptr = qmi_tlv_get((struct qmi_tlv*)uim_get_card_status_resp, 16, &buf_sz);
	if (!ptr)
		return NULL;

	printf("resp_get_status: Read %zu bytes\n", buf_sz);
	printf("first 4 bytes: %2x %2x %2x %2x\n", ptr[0], ptr[1], ptr[2], ptr[3]);

	cs = malloc(sizeof(struct qrild_svc_uim_card_status));
	amt = offsetof(struct qrild_svc_uim_card_status, cards);
	memcpy(cs, ptr, amt);
	len += amt;
	size_t card_sz = sizeof(struct qrild_svc_uim_card_status_cards);
	cs->cards = malloc(card_sz * cs->cards_n);
	for(i = 0; i < cs->cards_n; i++) {
		size_t ii;
		size_t len_start_c = len;
		size_t appn_sizes = 0;
		amt = offsetof(struct qrild_svc_uim_card_status_cards, applications);
		printf("card %zu, len %2zu, copying %2zu bytes\n", i, len, amt);
		memcpy(&cs->cards[i], ptr + len, amt);
		len += amt;
		print_hex_dump("card", cs->cards + card_sz * i, 9);
		size_t application_sz = sizeof(struct qrild_svc_uim_card_status_cards_applications);
		cs->cards[i].applications = malloc(application_sz * cs->cards[i].applications_n);
		printf("Card %zu has %u applications\n", i, cs->cards[i].applications_n);
		for(ii = 0; ii < cs->cards[i].applications_n; ii++) {
			size_t len_start_a = len;
			size_t iii;
			amt = offsetof(struct qrild_svc_uim_card_status_cards_applications, application_identifier_value);
			printf("application %zu, len %2zu, copying %2zu bytes\n", ii, len, amt);
			memcpy(&cs->cards[i].applications[ii], ptr + len, amt);
			len += amt;
			amt = sizeof(uint8_t);
			size_t appn_id_sz = cs->cards[i].applications[ii].application_identifier_value_n
				* amt;
			cs->cards[i].applications[ii].application_identifier_value = malloc(appn_id_sz);
			printf("application %zu, copying %zu id bytes\n", ii, appn_id_sz);
			for(iii = 0; iii < cs->cards[i].applications[ii].application_identifier_value_n; iii++) {
				memcpy(cs->cards[i].applications[ii].application_identifier_value + iii * amt,
					ptr + len, amt);
				len += amt;
			}
			print_hex_dump("appn_id", cs->cards[i].applications[ii].application_identifier_value, appn_id_sz);
			amt = offsetof(struct qrild_svc_uim_card_status_cards_applications, puk2_retries) + sizeof(uint8_t) - offsetof(struct qrild_svc_uim_card_status_cards_applications, upin_replaces_pin1);
			printf("application %zu, len %2zu, copying %2zu bytes (upin_replaces_pin1)\n", ii, len, amt);
			uint8_t *ptr2 = (uint8_t*)((size_t)(&cs->cards[i].applications[ii]) + offsetof(struct qrild_svc_uim_card_status_cards_applications, upin_replaces_pin1));
			memcpy(ptr2, ptr + len, amt);
			len += amt;
			size_t appn_sz = cs->cards[i].applications[ii].application_identifier_value_n
			  * sizeof(uint8_t) + sizeof(struct qrild_svc_uim_card_status_cards_applications) - sizeof(uint8_t*);
			appn_sizes += appn_sz;
			if (len - len_start_a != appn_sz)
				printf("application %zu should be %zu bytes but was %zu bytes\n", ii, appn_sz, len - len_start_a);
		}
		size_t card_sz = appn_sizes + sizeof(struct qrild_svc_uim_card_status_cards) - sizeof(void*);
		if (card_sz != len - len_start_c)
			printf("card %zu should be %zu bytes, was %zu bytes\n", i, card_sz, len - len_start_c);
	}

	if (len != buf_sz) {
		printf("ERROR: buffer was %zu bytes but took %zu to fill structure!\n",
			buf_sz, len);
		exit(1);
	}

	return cs;
}

