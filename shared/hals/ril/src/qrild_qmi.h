#ifndef __QRILD_QMI_H__
#define __QRILD_QMI_H__

struct qmi_service_version *qrild_qmi_find_service(struct rild_state *state,
					     enum qmi_service svc);
uint8_t *qmi_qmux_add_header(struct qmi_service_version *service,
			 uint8_t *data, size_t *len);
int qmi_ctl_allocate_cid(struct rild_state *state);
int qmi_uim_get_card_status(struct rild_state *state);

#endif