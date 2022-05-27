#ifndef __UTIL_H__
#define __UTIL_H__

#include <stdlib.h>
#include <string.h>

#include "qrild_qrtr.h"

#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define MAX(x, y) ((x) > (y) ? (x) : (y))

void print_hex_dump(const char *prefix, const void *buf, size_t len);
const char* qmi_service_to_string(enum qmi_service service, bool short_name);

inline void* zalloc(size_t size) {
        void* buf = malloc(size);
        memset(buf, 0, size);
        return buf;
}

#endif
