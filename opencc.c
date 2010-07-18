/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2008 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author: BYVoid <byvoid.kcp@gmail.com>                                |
  +----------------------------------------------------------------------+
*/

/* $Id: header 252479 2008-02-07 19:39:50Z iliaa $ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_opencc.h"

#define LIBOPENCC "libopencc.so"

ZEND_DECLARE_MODULE_GLOBALS(opencc)

/* True global resources - no need for thread safety here */
static int le_opencc;

/* Function Entry */
const zend_function_entry opencc_functions[] = {
	PHP_FE(opencc_open,	NULL)
	PHP_FE(opencc_close,	NULL)
	PHP_FE(opencc_convert,	NULL)
	{NULL, NULL, NULL}
};

/* Module Entry */
zend_module_entry opencc_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	"opencc",
	opencc_functions,
	PHP_MINIT(opencc),
	PHP_MSHUTDOWN(opencc),
	PHP_RINIT(opencc),
	PHP_RSHUTDOWN(opencc),
	PHP_MINFO(opencc),
#if ZEND_MODULE_API_NO >= 20010901
	PACKAGE_VERSION,
#endif
	STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_OPENCC
ZEND_GET_MODULE(opencc)
#endif

/* Remove comments and fill if you need to have entries in php.ini
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("opencc.global_value",      "42", PHP_INI_ALL, OnUpdateLong, global_value, zend_opencc_globals, opencc_globals)
    STD_PHP_INI_ENTRY("opencc.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_opencc_globals, opencc_globals)
PHP_INI_END()
*/

/* {{{ php_opencc_init_globals
 */
/* Uncomment this function if you have INI entries
static void php_opencc_init_globals(zend_opencc_globals *opencc_globals)
{
	opencc_globals->global_value = 0;
	opencc_globals->global_string = NULL;
}
*/
/* }}} */

/* Module Initialize */
PHP_MINIT_FUNCTION(opencc)
{
	/* If you have INI entries, uncomment these lines 
	REGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}

/* Module Shotdown */
PHP_MSHUTDOWN_FUNCTION(opencc)
{
	/* uncomment this line if you have INI entries
	UNREGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}

PHP_MINFO_FUNCTION(opencc)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "opencc support", "enabled");
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini
	DISPLAY_INI_ENTRIES();
	*/
}

void * load_symbol(void * dl_handle, const char * symbol)
{
	void * ret = dlsym(dl_handle, symbol);
	if (!ret)
	{
		fprintf(stderr, "DLERROR: %s\n", dlerror());
		exit(1);
	}
}

/* Initialize */
PHP_RINIT_FUNCTION(opencc)
{
	OPENCC_G(dl_handle) = dlopen(LIBOPENCC, RTLD_NOW);
	if (!OPENCC_G(dl_handle))
	{
		fprintf(stderr, "DLERROR: %s\n", dlerror());
		exit(1);
	}
	
	OPENCC_G(opencc_sym).opencc_open = load_symbol(OPENCC_G(dl_handle), "opencc_open");
	OPENCC_G(opencc_sym).opencc_close = load_symbol(OPENCC_G(dl_handle), "opencc_close");
	OPENCC_G(opencc_sym).opencc_convert_utf8 = load_symbol(OPENCC_G(dl_handle), "opencc_convert_utf8");
	OPENCC_G(opencc_sym).opencc_dict_load = load_symbol(OPENCC_G(dl_handle), "opencc_dict_load");
	OPENCC_G(opencc_sym).opencc_errno = load_symbol(OPENCC_G(dl_handle), "opencc_errno");
	OPENCC_G(opencc_sym).opencc_perror = load_symbol(OPENCC_G(dl_handle), "opencc_perror");
	
	return SUCCESS;
}

/* Module Shutdown */
PHP_RSHUTDOWN_FUNCTION(opencc)
{
	dlclose(OPENCC_G(dl_handle));
	return SUCCESS;
}

PHP_FUNCTION(opencc_open)
{
	char * config;
	int config_len;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &config, &config_len) == FAILURE) {
		RETURN_BOOL(0);
	}
	opencc_t od = OPENCC_G(opencc_sym).opencc_open(config);
	
	if (od == (opencc_t) -1)
	{
		RETURN_BOOL(0);
	}
	
	RETURN_RESOURCE((long) od);
}

PHP_FUNCTION(opencc_close)
{
	zval * zod;
	opencc_t od;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r", &zod) == FAILURE)
	{
		RETURN_BOOL(0);
	}
	
	od = (opencc_t) zod->value.lval;
	if (od == (opencc_t) -1 || od == NULL)
	{
		RETURN_BOOL(0);
	}
	
	int retval = OPENCC_G(opencc_sym).opencc_close(od);
	
	RETURN_LONG(retval);
}

PHP_FUNCTION(opencc_convert)
{
	zval * zod;
	char * inbuf = NULL;
	int inbuf_len;
	opencc_t od;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rs", &zod, &inbuf, &inbuf_len) == FAILURE)
	{
		RETURN_BOOL(0);
	}
	
	od = (opencc_t) zod->value.lval;
	if (od == (opencc_t) -1 || od == NULL)
	{
		RETURN_BOOL(0);
	}
	
	char * outbuf = OPENCC_G(opencc_sym).opencc_convert_utf8(od, inbuf, inbuf_len);
	int len = strlen(outbuf);
	
	char * rs = emalloc(sizeof(char) * (len + 1));
	strncpy(rs, outbuf, len + 1);
	free(outbuf);
	
	RETURN_STRINGL(rs, len, 0);
}
