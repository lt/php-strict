#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_strict.h"

ZEND_API zend_op_array *strict_compile_file(zend_file_handle *fd, int type)
{
	CG(declarables).strict_types = 1;
	return orig_compile_file(fd, type);
}

ZEND_API zend_op_array *strict_compile_string(zval *src, char *filename)
{
	CG(declarables).strict_types = 1;
	return orig_compile_string(src, filename);
}

void handle_declare(zend_ast **ast_ptr)
{
	zend_ast *ast = *ast_ptr;

	zend_ast_list *declares = zend_ast_get_list(ast->child[0]);
	zend_ast *declare_ast;
	zend_ast *name_ast;
	zend_ast *value_ast;

	zend_string *name;
	uint32_t i;

	for (i = 0; i < declares->children; i++) {
		declare_ast = declares->child[i];
		name_ast = declare_ast->child[0];
		value_ast = declare_ast->child[1];

		name = zend_ast_get_str(name_ast);

		if (zend_string_equals_literal_ci(name, "strict_types")) {
			/* if nothing left after custom value.... 

			zend_ast_destroy(ast);
			*ast_ptr = NULL;
			*/
		}
	}
}

void find_declares(zend_ast **ast_ptr)
{
	zend_ast *ast = *ast_ptr;
	
	if (!ast) {
		return;
	}

	if (ast->kind == ZEND_AST_DECLARE) {
		handle_declare(ast_ptr);
	}
	else {
		zend_ast_apply(ast, find_declares);
	}
}

ZEND_API void strict_ast_process(zend_ast *ast)
{
	zend_ast_apply(ast, find_declares);

	if (orig_ast_process) {
		orig_ast_process(ast);
	}
}

/* {{{ PHP_MINIT_FUNCTION */
PHP_MINIT_FUNCTION(strict)
{
	STRICT_HOOK(ast_process);
	STRICT_HOOK(compile_file);
	STRICT_HOOK(compile_string);

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
	"0.1.0",
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
 */
