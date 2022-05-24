#ifndef __QRILD_H__
#define __QRILD_H__

#include <stdint.h>
#include <stdlib.h>

struct qmi_service_info {
	uint8_t num_services;
	struct {
		uint8_t service;
		uint16_t major_version;
		uint16_t minor_version;
	} *services;
};

struct enum_value {
	int value;
	const char* value_str;
	const char* name;
};

/**
 * QmiService:
 * @QMI_SERVICE_UNKNOWN: Unknown service.
 * @QMI_SERVICE_CTL: Control service.
 * @QMI_SERVICE_WDS: Wireless Data Service.
 * @QMI_SERVICE_DMS: Device Management Service.
 * @QMI_SERVICE_NAS: Network Access Service.
 * @QMI_SERVICE_QOS: Quality Of Service service.
 * @QMI_SERVICE_WMS: Wireless Messaging Service.
 * @QMI_SERVICE_PDS: Position Determination Service.
 * @QMI_SERVICE_AUTH: Authentication service.
 * @QMI_SERVICE_AT: AT service.
 * @QMI_SERVICE_VOICE: Voice service.
 * @QMI_SERVICE_CAT2: Card Application Toolkit service (v2).
 * @QMI_SERVICE_UIM: User Identity Module service.
 * @QMI_SERVICE_PBM: Phonebook Management service.
 * @QMI_SERVICE_QCHAT: QCHAT service. Since: 1.8.
 * @QMI_SERVICE_RMTFS: Remote file system service.
 * @QMI_SERVICE_TEST: Test service. Since: 1.8.
 * @QMI_SERVICE_LOC: Location service (~ PDS v2).
 * @QMI_SERVICE_SAR: Service access proxy service.
 * @QMI_SERVICE_IMS: IMS settings service. Since: 1.8.
 * @QMI_SERVICE_ADC: Analog to digital converter driver service. Since: 1.8.
 * @QMI_SERVICE_CSD: Core sound driver service. Since: 1.8.
 * @QMI_SERVICE_MFS: Modem embedded file system service. Since: 1.8.
 * @QMI_SERVICE_TIME: Time service. Since: 1.8.
 * @QMI_SERVICE_TS: Thermal sensors service. Since: 1.8.
 * @QMI_SERVICE_TMD: Thermal mitigation device service. Since: 1.8.
 * @QMI_SERVICE_SAP: Service access proxy service. Since: 1.8.
 * @QMI_SERVICE_WDA: Wireless data administrative service. Since: 1.8.
 * @QMI_SERVICE_TSYNC: TSYNC control service. Since: 1.8.
 * @QMI_SERVICE_RFSA: Remote file system access service. Since: 1.8.
 * @QMI_SERVICE_CSVT: Circuit switched videotelephony service. Since: 1.8.
 * @QMI_SERVICE_QCMAP: Qualcomm mobile access point service. Since: 1.8.
 * @QMI_SERVICE_IMSP: IMS presence service. Since: 1.8.
 * @QMI_SERVICE_IMSVT: IMS videotelephony service. Since: 1.8.
 * @QMI_SERVICE_IMSA: IMS application service. Since: 1.8.
 * @QMI_SERVICE_COEX: Coexistence service. Since: 1.8.
 * @QMI_SERVICE_PDC: Persistent device configuration service. Since: 1.8.
 * @QMI_SERVICE_STX: Simultaneous transmit service. Since: 1.8.
 * @QMI_SERVICE_BIT: Bearer independent transport service. Since: 1.8.
 * @QMI_SERVICE_IMSRTP: IMS RTP service. Since: 1.8.
 * @QMI_SERVICE_RFRPE: RF radiated performance enhancement service. Since: 1.8.
 * @QMI_SERVICE_DSD: Data system determination service. Since: 1.8.
 * @QMI_SERVICE_SSCTL: Subsystem control service. Since: 1.8.
 * @QMI_SERVICE_DPM: Data Port Mapper service. Since: 1.30.
 * @QMI_SERVICE_CAT: Card Application Toolkit service (v1).
 * @QMI_SERVICE_RMS: Remote Management Service.
 * @QMI_SERVICE_OMA: Open Mobile Alliance device management service.
 * @QMI_SERVICE_FOTA: Firmware Over The Air service. Since: 1.24.
 * @QMI_SERVICE_GMS: Telit General Modem Service. Since: 1.24.
 * @QMI_SERVICE_GAS: Telit General Application Service. Since: 1.24.
 *
 * QMI services.
 *
 * Since: 1.0
 */
enum qmi_service { /*< since=1.0 >*/
    QMI_SERVICE_UNKNOWN = -1,
    QMI_SERVICE_CTL     = 0x00,
    QMI_SERVICE_WDS     = 0x01,
    QMI_SERVICE_DMS     = 0x02,
    QMI_SERVICE_NAS     = 0x03,
    QMI_SERVICE_QOS     = 0x04,
    QMI_SERVICE_WMS     = 0x05,
    QMI_SERVICE_PDS     = 0x06,
    QMI_SERVICE_AUTH    = 0x07,
    QMI_SERVICE_AT      = 0x08,
    QMI_SERVICE_VOICE   = 0x09,
    QMI_SERVICE_CAT2    = 0x0A,
    QMI_SERVICE_UIM     = 0x0B,
    QMI_SERVICE_PBM     = 0x0C,
    QMI_SERVICE_QCHAT   = 0x0D,
    QMI_SERVICE_RMTFS   = 0x0E,
    QMI_SERVICE_TEST    = 0x0F,
    QMI_SERVICE_LOC     = 0x10,
    QMI_SERVICE_SAR     = 0x11,
    QMI_SERVICE_IMS     = 0x12,
    QMI_SERVICE_ADC     = 0x13,
    QMI_SERVICE_CSD     = 0x14,
    QMI_SERVICE_MFS     = 0x15,
    QMI_SERVICE_TIME    = 0x16,
    QMI_SERVICE_TS      = 0x17,
    QMI_SERVICE_TMD     = 0x18,
    QMI_SERVICE_SAP     = 0x19,
    QMI_SERVICE_WDA     = 0x1A,
    QMI_SERVICE_TSYNC   = 0x1B,
    QMI_SERVICE_RFSA    = 0x1C,
    QMI_SERVICE_CSVT    = 0x1D,
    QMI_SERVICE_QCMAP   = 0x1E,
    QMI_SERVICE_IMSP    = 0x1F,
    QMI_SERVICE_IMSVT   = 0x20,
    QMI_SERVICE_IMSA    = 0x21,
    QMI_SERVICE_COEX    = 0x22,
    /* 0x23, reserved */
    QMI_SERVICE_PDC     = 0x24,
    /* 0x25, reserved */
    QMI_SERVICE_STX     = 0x26,
    QMI_SERVICE_BIT     = 0x27,
    QMI_SERVICE_IMSRTP  = 0x28,
    QMI_SERVICE_RFRPE   = 0x29,
    QMI_SERVICE_DSD     = 0x2A,
    QMI_SERVICE_SSCTL   = 0x2B,
    QMI_SERVICE_DPM     = 0x2F,
    QMI_SERVICE_CAT     = 0xE0,
    QMI_SERVICE_RMS     = 0xE1,
    QMI_SERVICE_OMA     = 0xE2,
    QMI_SERVICE_FOTA    = 0xE6,
    QMI_SERVICE_GMS     = 0xE7,
    QMI_SERVICE_GAS     = 0xE8,
};

static const struct enum_value qmi_service_values[] = {
    { QMI_SERVICE_UNKNOWN, "QMI_SERVICE_UNKNOWN", "unknown" },
    { QMI_SERVICE_CTL, "QMI_SERVICE_CTL", "ctl" },
    { QMI_SERVICE_WDS, "QMI_SERVICE_WDS", "wds" },
    { QMI_SERVICE_DMS, "QMI_SERVICE_DMS", "dms" },
    { QMI_SERVICE_NAS, "QMI_SERVICE_NAS", "nas" },
    { QMI_SERVICE_QOS, "QMI_SERVICE_QOS", "qos" },
    { QMI_SERVICE_WMS, "QMI_SERVICE_WMS", "wms" },
    { QMI_SERVICE_PDS, "QMI_SERVICE_PDS", "pds" },
    { QMI_SERVICE_AUTH, "QMI_SERVICE_AUTH", "auth" },
    { QMI_SERVICE_AT, "QMI_SERVICE_AT", "at" },
    { QMI_SERVICE_VOICE, "QMI_SERVICE_VOICE", "voice" },
    { QMI_SERVICE_CAT2, "QMI_SERVICE_CAT2", "cat2" },
    { QMI_SERVICE_UIM, "QMI_SERVICE_UIM", "uim" },
    { QMI_SERVICE_PBM, "QMI_SERVICE_PBM", "pbm" },
    { QMI_SERVICE_QCHAT, "QMI_SERVICE_QCHAT", "qchat" },
    { QMI_SERVICE_RMTFS, "QMI_SERVICE_RMTFS", "rmtfs" },
    { QMI_SERVICE_TEST, "QMI_SERVICE_TEST", "test" },
    { QMI_SERVICE_LOC, "QMI_SERVICE_LOC", "loc" },
    { QMI_SERVICE_SAR, "QMI_SERVICE_SAR", "sar" },
    { QMI_SERVICE_IMS, "QMI_SERVICE_IMS", "ims" },
    { QMI_SERVICE_ADC, "QMI_SERVICE_ADC", "adc" },
    { QMI_SERVICE_CSD, "QMI_SERVICE_CSD", "csd" },
    { QMI_SERVICE_MFS, "QMI_SERVICE_MFS", "mfs" },
    { QMI_SERVICE_TIME, "QMI_SERVICE_TIME", "time" },
    { QMI_SERVICE_TS, "QMI_SERVICE_TS", "ts" },
    { QMI_SERVICE_TMD, "QMI_SERVICE_TMD", "tmd" },
    { QMI_SERVICE_SAP, "QMI_SERVICE_SAP", "sap" },
    { QMI_SERVICE_WDA, "QMI_SERVICE_WDA", "wda" },
    { QMI_SERVICE_TSYNC, "QMI_SERVICE_TSYNC", "tsync" },
    { QMI_SERVICE_RFSA, "QMI_SERVICE_RFSA", "rfsa" },
    { QMI_SERVICE_CSVT, "QMI_SERVICE_CSVT", "csvt" },
    { QMI_SERVICE_QCMAP, "QMI_SERVICE_QCMAP", "qcmap" },
    { QMI_SERVICE_IMSP, "QMI_SERVICE_IMSP", "imsp" },
    { QMI_SERVICE_IMSVT, "QMI_SERVICE_IMSVT", "imsvt" },
    { QMI_SERVICE_IMSA, "QMI_SERVICE_IMSA", "imsa" },
    { QMI_SERVICE_COEX, "QMI_SERVICE_COEX", "coex" },
    { QMI_SERVICE_PDC, "QMI_SERVICE_PDC", "pdc" },
    { QMI_SERVICE_STX, "QMI_SERVICE_STX", "stx" },
    { QMI_SERVICE_BIT, "QMI_SERVICE_BIT", "bit" },
    { QMI_SERVICE_IMSRTP, "QMI_SERVICE_IMSRTP", "imsrtp" },
    { QMI_SERVICE_RFRPE, "QMI_SERVICE_RFRPE", "rfrpe" },
    { QMI_SERVICE_DSD, "QMI_SERVICE_DSD", "dsd" },
    { QMI_SERVICE_SSCTL, "QMI_SERVICE_SSCTL", "ssctl" },
    { QMI_SERVICE_DPM, "QMI_SERVICE_DPM", "dpm" },
    { QMI_SERVICE_CAT, "QMI_SERVICE_CAT", "cat" },
    { QMI_SERVICE_RMS, "QMI_SERVICE_RMS", "rms" },
    { QMI_SERVICE_OMA, "QMI_SERVICE_OMA", "oma" },
    { QMI_SERVICE_FOTA, "QMI_SERVICE_FOTA", "fota" },
    { QMI_SERVICE_GMS, "QMI_SERVICE_GMS", "gms" },
    { QMI_SERVICE_GAS, "QMI_SERVICE_GAS", "gas" },
    { 0, NULL, NULL }
};

// Largely from https://github.com/DiUS/sierra-gobi-drivers/blob/master/GobiNet/QMI.h

struct __attribute__((packed)) qmux_header {
	/* Always 1 */
	uint8_t tf;

	/* Length of message excluding tf byte */
	uint16_t len;

	/*
	 * Dxxxxxxx
	 * ||||||||
	 * |+++++++- Unused
	 * +-------- Direction (1 = from server (the modem), 0 = from control point (us))
	 */
	uint8_t ctrl_flag;

	/* Service type / id */
	/* enum qmi_service */uint8_t service;

	/* Client ID */
	uint8_t client;
};

#endif
