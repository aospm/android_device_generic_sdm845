#ifndef __QRILD_QRTR__
#define __QRILD_QRTR__

#include "qrild.h"
#include <stdbool.h>

int qrild_qrtr_send_to_service(struct rild_state *state,
			       enum qmi_service svc_id, const void *data,
			       size_t sz);
bool qrild_qrtr_do_lookup(struct rild_state *state);
void qrild_qrtr_recv(struct rild_state *state);

#endif
