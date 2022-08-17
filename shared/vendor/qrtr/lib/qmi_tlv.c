#include <ctype.h>
#include <errno.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#include "libqrtr.h"

struct qmi_tlv *qmi_tlv_init(uint16_t txn, uint32_t msg_id, uint32_t msg_type)
{
	struct qmi_header *pkt;
	struct qmi_tlv *tlv;

	tlv = malloc(sizeof(struct qmi_tlv));
	memset(tlv, 0, sizeof(struct qmi_tlv));

	tlv->size = sizeof(struct qmi_header);
	tlv->allocated = malloc(tlv->size);
	tlv->buf = tlv->allocated;

	pkt = tlv->buf;
	pkt->type = msg_type;
	pkt->txn_id = txn;
	pkt->msg_id = msg_id;
	pkt->msg_len = 0;

	return tlv;
}

struct qmi_tlv *qmi_tlv_decode(void *buf, size_t len)
{
	struct qmi_tlv *tlv;

	tlv = malloc(sizeof(struct qmi_tlv));
	memset(tlv, 0, sizeof(struct qmi_tlv));

	tlv->buf = buf;
	tlv->size = len;

	return tlv;
}

void *qmi_tlv_encode(struct qmi_tlv *tlv, size_t *len)
{
	struct qmi_header *pkt;

	if (!tlv || tlv->error)
		return NULL;

	pkt = tlv->buf;
	pkt->msg_len = tlv->size - sizeof(struct qmi_header);

	*len = tlv->size;
	return tlv->buf;
}

void qmi_tlv_free(struct qmi_tlv *tlv)
{
	free(tlv->allocated);
	free(tlv);
}

static struct qmi_tlv_item *qmi_tlv_get_item(struct qmi_tlv *tlv, unsigned id)
{
	struct qmi_tlv_item *item;
	struct qmi_header *pkt;
	unsigned offset = 0;
	void *pkt_data;

	pkt = tlv->buf;
	pkt_data = &pkt[1];

	while (offset < tlv->size) {
		item = pkt_data + offset;
		if (item->key == id)
			return pkt_data + offset;

		offset += sizeof(struct qmi_tlv_item) + item->len;
	}
	return NULL;
}

void *qmi_tlv_get(struct qmi_tlv *tlv, uint8_t id, size_t *len)
{
	struct qmi_tlv_item *item;

	item = qmi_tlv_get_item(tlv, id);
	if (!item)
		return NULL;

	if (len)
		*len = item->len;
	return item->data;
}

void *qmi_tlv_get_array(struct qmi_tlv *tlv, uint8_t id, size_t len_size,
			size_t *len, size_t *size)
{
	struct qmi_tlv_item *item;
	unsigned count;
	void *ptr;

	item = qmi_tlv_get_item(tlv, id);
	if (!item)
		return NULL;

	ptr = item->data;
	switch (len_size) {
	case 4:
		count = *(uint32_t *)ptr++;
		break;
	case 2:
		count = *(uint16_t *)ptr++;
		break;
	case 1:
		count = *(uint8_t *)ptr++;
		break;
	default:
		return NULL;
	}

	*len = count;
	*size = (item->len - len_size) / count;

	return ptr;
}

static struct qmi_tlv_item *qmi_tlv_alloc_item(struct qmi_tlv *tlv, unsigned id,
					       size_t len)
{
	struct qmi_tlv_item *item;
	size_t new_size;
	bool migrate;
	void *newp;

	/* If using user provided buffer, migrate data */
	migrate = !tlv->allocated;

	new_size = tlv->size + sizeof(struct qmi_tlv_item) + len;
	newp = realloc(tlv->allocated, new_size);
	if (!newp)
		return NULL;

	if (migrate)
		memcpy(newp, tlv->buf, tlv->size);

	item = newp + tlv->size;
	item->key = id;
	item->len = len;

	tlv->buf = tlv->allocated = newp;
	tlv->size = new_size;

	return item;
}

int qmi_tlv_set(struct qmi_tlv *tlv, uint8_t id, void *buf, size_t len)
{
	struct qmi_tlv_item *item;

	if (!tlv)
		return -EINVAL;

	item = qmi_tlv_alloc_item(tlv, id, len);
	if (!item) {
		tlv->error = ENOMEM;
		return -ENOMEM;
	}

	memcpy(item->data, buf, len);

	return 0;
}

int qmi_tlv_set_array(struct qmi_tlv *tlv, uint8_t id, size_t len_size,
		      void *buf, size_t len, size_t size)
{
	struct qmi_tlv_item *item;
	size_t array_size;
	void *ptr;

	if (!tlv)
		return -EINVAL;

	array_size = len * size;
	item = qmi_tlv_alloc_item(tlv, id, len_size + array_size);
	if (!item) {
		tlv->error = ENOMEM;
		return -ENOMEM;
	}

	ptr = item->data;

	switch (len_size) {
	case 4:
		*(uint32_t *)ptr++ = len;
		break;
	case 2:
		*(uint16_t *)ptr++ = len;
		break;
	case 1:
		*(uint8_t *)ptr++ = len;
		break;
	}
	memcpy(ptr, buf, array_size);

	return 0;
}

struct qmi_response_type_v01 *qmi_tlv_get_result(struct qmi_tlv *tlv)
{
	return qmi_tlv_get(tlv, 2, NULL);
}

#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define LINE_LENGTH 40

static inline uint8_t to_hex(uint8_t ch)
{
	ch &= 0xf;
	return ch <= 9 ? '0' + ch : 'a' + ch - 10;
}


