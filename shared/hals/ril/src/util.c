#include <ctype.h>
#include <stdint.h>
#include <stdio.h>

#include "logging.h"
#include "libqrtr.h"

#include "util.h"
#include "qrild_qrtr.h"

static uint8_t to_hex(uint8_t ch)
{
	ch &= 0xf;
	return ch <= 9 ? '0' + ch : 'a' + ch - 10;
}

#define LINE_LENGTH 48

void print_hex_dump(const char *prefix, const void *buf, size_t len)
{
	const uint8_t *ptr = buf;
	size_t linelen;
	uint8_t ch;
	char line[LINE_LENGTH * 4 + 1];
	int li;
	int i;
	int j;

	if (len < 0) {
		LOGW("%s: len %d less than 0", __func__, len);
		return;
	}

	if (prefix)
		LOGD("<<< %s:", prefix);

	for (i = 0; i < len; i += LINE_LENGTH) {
		linelen = MIN(LINE_LENGTH, len - i);
		li = 0;

		for (j = 0; j < linelen; j++) {
			ch = ptr[i + j];
			line[li++] = to_hex(ch >> 4);
			line[li++] = to_hex(ch);
			line[li++] = ' ';
		}

		for (; j < LINE_LENGTH; j++) {
			line[li++] = ' ';
			line[li++] = ' ';
			line[li++] = ' ';
		}

		// for (j = 0; j < linelen; j++) {
		// 	ch = ptr[i + j];
		// 	line[li++] = isprint(ch) ? ch : '.';
		// }

		line[li] = '\0';

		LOGD("<<< %04x: %s\n", i, line);
	}
}

const char* qmi_service_to_string(enum qmi_service service, bool short_name) {
	const struct enum_value* v = &qmi_service_values[0];
	while (v->value_str) {
		if (v->value == service)
			return short_name ? v->name : v->value_str;
		v++;
	}

	return NULL;
}
