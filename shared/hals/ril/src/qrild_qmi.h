#ifndef __QRILD_QMI_H__
#define __QRILD_QMI_H__

uint8_t *qmi_qmux_add_header(struct qmi_service_info *service,
			 uint8_t *data, size_t *len);
int qmi_ctl_allocate_cid(struct rild_state *state);
int qmi_req_uim_get_card_status(struct rild_state *state);
int qmi_resp_uim_get_card_status(struct rild_state *state, uint8_t *buf, size_t sz);
const char* qmi_service_to_string(enum qmi_service service, bool short_name);

#endif
