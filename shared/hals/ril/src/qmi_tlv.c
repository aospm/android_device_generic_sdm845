#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

struct qmi_packet {
	uint8_t flags;
	uint16_t txn_id;
	uint16_t msg_id;
	uint16_t msg_len;
	uint8_t data[];
} __attribute__((__packed__));

struct qmi_ctl_packet {
	uint8_t flags;
	uint8_t txn_id;
	uint16_t msg_id;
	uint16_t msg_len;
	uint8_t data[];
} __attribute__((__packed__));

struct qmi_tlv {
	void *allocated;
	void *buf;
	size_t size;
	int error;
	bool is_ctl;
};

struct qmi_tlv_item {
	uint8_t key;
	uint16_t len;
	uint8_t data[];
} __attribute__((__packed__));

#define TLV_INIT(_name, _packet, _is_ctl)                                      \
	struct qmi_tlv *qmi_tlv_init##_name(unsigned txn, unsigned msg_id,     \
					    unsigned msg_type) {               \
		_packet *pkt;                                                  \
		struct qmi_tlv *tlv;                                           \
                                                                               \
		tlv = malloc(sizeof(struct qmi_tlv));                          \
		memset(tlv, 0, sizeof(struct qmi_tlv));                        \
                                                                               \
		tlv->size = sizeof(_packet);                                   \
		tlv->allocated = malloc(tlv->size);                            \
		tlv->buf = tlv->allocated;                                     \
		tlv->is_ctl = _is_ctl;                                         \
                                                                               \
		pkt = tlv->buf;                                                \
		pkt->flags = msg_type;                                         \
		pkt->txn_id = txn;                                             \
		pkt->msg_id = msg_id;                                          \
		pkt->msg_len = 0;                                              \
                                                                               \
		return tlv;                                                    \
	}

#define TLV_DECODE(_name, _packet)                                             \
	struct qmi_tlv *qmi_tlv_decode##_name(                                 \
	    void *buf, size_t len, unsigned *txn, unsigned msg_type) {         \
		_packet *pkt = buf;                                            \
		struct qmi_tlv *tlv;                                           \
                                                                               \
		if (pkt->flags != msg_type)                                    \
			return NULL;                                           \
                                                                               \
		tlv = malloc(sizeof(struct qmi_tlv));                          \
		memset(tlv, 0, sizeof(struct qmi_tlv));                        \
                                                                               \
		tlv->buf = buf;                                                \
		tlv->size = len;                                               \
                                                                               \
		if (txn)                                                       \
			*txn = pkt->txn_id;                                    \
                                                                               \
		return tlv;                                                    \
	}

#define TLV_ENCODE(_name, _packet)                                             \
	void *qmi_tlv_encode##_name(struct qmi_tlv *tlv, size_t *len) {        \
		_packet *pkt;                                                  \
                                                                               \
		if (!tlv || tlv->error)                                        \
			return NULL;                                           \
                                                                               \
		pkt = tlv->buf;                                                \
		pkt->msg_len = tlv->size - sizeof(_packet);                    \
                                                                               \
		*len = tlv->size;                                              \
		return tlv->buf;                                               \
	}

#define TLV_GET_ITEM(_name, _packet)                                           \
	static struct qmi_tlv_item *qmi_tlv_get_item##_name(                   \
	    struct qmi_tlv *tlv, unsigned id) {                                \
		struct qmi_tlv_item *item;                                     \
		_packet *pkt;                                                  \
		unsigned offset = 0;                                           \
		void *pkt_data;                                                \
                                                                               \
		pkt = tlv->buf;                                                \
		pkt_data = pkt->data;                                          \
                                                                               \
		while (offset < tlv->size) {                                   \
			item = pkt_data + offset;                              \
			if (item->key == id)                                   \
				return pkt_data + offset;                      \
                                                                               \
			offset += sizeof(struct qmi_tlv_item) + item->len;     \
		}                                                              \
		return NULL;                                                   \
	}

TLV_INIT(, struct qmi_packet, false)
TLV_INIT(_ctl, struct qmi_ctl_packet, true)
TLV_DECODE(, struct qmi_packet)
TLV_DECODE(_ctl, struct qmi_ctl_packet)
TLV_ENCODE(, struct qmi_packet)
TLV_ENCODE(_ctl, struct qmi_ctl_packet)
TLV_GET_ITEM(, struct qmi_packet)
TLV_GET_ITEM(_ctl, struct qmi_ctl_packet)

void qmi_tlv_free(struct qmi_tlv *tlv) {
	free(tlv->allocated);
	free(tlv);
}

void *qmi_tlv_get(struct qmi_tlv *tlv, unsigned id, size_t *len) {
	struct qmi_tlv_item *item;

	if (!tlv->is_ctl)
		item = qmi_tlv_get_item(tlv, id);
	else
		item = qmi_tlv_get_item_ctl(tlv, id);

	if (!item)
		return NULL;

	*len = item->len;
	return item->data;
}

void *qmi_tlv_get_array(struct qmi_tlv *tlv, unsigned id, unsigned len_size,
			size_t *len, size_t *size) {
	struct qmi_tlv_item *item;
	unsigned count;
	void *ptr;

	if (!tlv->is_ctl)
		item = qmi_tlv_get_item(tlv, id);
	else
		item = qmi_tlv_get_item_ctl(tlv, id);

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
	}

	*len = count;
	*size = (item->len - len_size) / count;

	return ptr;
}

static struct qmi_tlv_item *qmi_tlv_alloc_item(struct qmi_tlv *tlv, unsigned id,
					       size_t len) {
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

int qmi_tlv_set(struct qmi_tlv *tlv, unsigned id, void *buf, size_t len) {
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

int qmi_tlv_set_array(struct qmi_tlv *tlv, unsigned id, unsigned len_size,
		      void *buf, size_t len, size_t size) {
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
