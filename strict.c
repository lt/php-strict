#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"

ZEND_API zend_op_array *(*weak_compile_file)(zend_file_handle *fd, int type);
ZEND_API zend_op_array *(*weak_compile_string)(zval *src, char *filename);

ZEND_API zend_op_array *strict_compile_file(zend_file_handle *fd, int type)
{
	CG(declarables).strict_types = 1;
	return weak_compile_file(fd, type);
}

ZEND_API zend_op_array *strict_compile_string(zval *src, char *filename)
{
	CG(declarables).strict_types = 1;
	return weak_compile_string(src, filename);
}

/* {{{ PHP_MINIT_FUNCTION */
PHP_MINIT_FUNCTION(strict)
{
	weak_compile_file = zend_compile_file;
	weak_compile_string = zend_compile_string;

	zend_compile_file = strict_compile_file;
	zend_compile_string = strict_compile_string;

	return SUCCESS;
}
/* }}} */

/* {{{ strict_module_entry */
zend_module_entry strict_module_entry = {
	STANDARD_MODULE_HEADER,
	"strict",
	NULL,
	PHP_MINIT(strict),
	NULL,
	NULL,
	NULL,
	NULL,
	"0.0.0",
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_STRICT
ZEND_GET_MODULE(strict)
#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
