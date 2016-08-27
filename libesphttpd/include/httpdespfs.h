#ifdef __cplusplus
extern "C" {
#endif
#ifndef HTTPDESPFS_H
#define HTTPDESPFS_H

#include "httpd.h"

int cgiEspFsHook(HttpdConnData *connData);
int ICACHE_FLASH_ATTR cgiEspFsTemplate(HttpdConnData *connData);

#endif
#ifdef __cplusplus
}
#endif
