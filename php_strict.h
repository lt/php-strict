#ifndef STRICT_H
#define STRICT_H

#define STRICT_HOOK(fn)    \
	orig_##fn = zend_##fn; \
	zend_##fn = strict_##fn;

ZEND_API zend_op_array *(*orig_compile_file)(zend_file_handle *fd, int type);
ZEND_API zend_op_array *(*orig_compile_string)(zval *src, char *filename);
ZEND_API zend_ast_process_t orig_ast_process;

ZEND_BEGIN_MODULE_GLOBALS(strict)
ZEND_END_MODULE_GLOBALS(strict)

#ifdef ZTS
# define STRICT_G(v) TSRMG(strict_globals_id, zend_strict_globals *, v)
#else
# define STRICT_G(v) strict_globals.v
#endif

#endif /* STRICT_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 */

