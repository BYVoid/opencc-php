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

#ifndef PHP_OPENCC_H
#define PHP_OPENCC_H

#include <opencc.h>

extern zend_module_entry opencc_module_entry;
#define phpext_opencc_ptr &opencc_module_entry

#ifdef PHP_WIN32
#	define PHP_OPENCC_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#	define PHP_OPENCC_API __attribute__ ((visibility("default")))
#else
#	define PHP_OPENCC_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

PHP_MINIT_FUNCTION(opencc);
PHP_MSHUTDOWN_FUNCTION(opencc);
PHP_RINIT_FUNCTION(opencc);
PHP_RSHUTDOWN_FUNCTION(opencc);
PHP_MINFO_FUNCTION(opencc);

PHP_FUNCTION(opencc_open);
PHP_FUNCTION(opencc_close);
PHP_FUNCTION(opencc_convert);
PHP_FUNCTION(opencc_set_conversion_mode);


ZEND_BEGIN_MODULE_GLOBALS(opencc)
	char * default_config;
ZEND_END_MODULE_GLOBALS(opencc)

/* In every utility function you add that needs to use variables 
   in php_opencc_globals, call TSRMLS_FETCH(); after declaring other 
   variables used by that function, or better yet, pass in TSRMLS_CC
   after the last function argument and declare your utility function
   with TSRMLS_DC after the last declared argument.  Always refer to
   the globals in your function as OPENCC_G(variable).  You are 
   encouraged to rename these macros something shorter, see
   examples in any other php module directory.
*/

#ifdef ZTS
#define OPENCC_G(v) TSRMG(opencc_globals_id, zend_opencc_globals *, v)
#else
#define OPENCC_G(v) (opencc_globals.v)
#endif

#endif	/* PHP_OPENCC_H */
