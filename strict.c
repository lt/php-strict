#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_strict.h"

ZEND_DECLARE_MODULE_GLOBALS(strict);

ZEND_API zend_op_array *strict_compile_file(zend_file_handle *fd, int type)
{
	CG(declarables).strict_types = strict_default;
	return orig_compile_file(fd, type);
}

ZEND_API zend_op_array *strict_compile_string(zval *src, char *filename)
{
	CG(declarables).strict_types = strict_default;
	return orig_compile_string(src, filename);
}

void parse_declare(zend_ast **ast_ptr)
{
	zend_ast *ast = *ast_ptr;
	zend_string *arg;

	int want_default = 0;
	int strict_mode = 0;

	while (IS_DECLARE_EX(ast)) {
		arg = GET_DECLARE_EX(ast);

		if (strcmp(arg->val, "DEFAULT\0") == 0) {
			want_default = 1;
		}

		ast = ast->child[0];
	}

	if (IS_DECLARE_VAL(ast)) {
		strict_mode = GET_DECLARE_VAL(ast);

		if (want_default == 1) {
			strict_default = strict_mode;
		}
	}
}

void handle_declare(zend_ast *ast)
{
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
			parse_declare(&value_ast);
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
		handle_declare(ast);
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
