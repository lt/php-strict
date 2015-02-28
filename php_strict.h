#ifndef STRICT_H
#define STRICT_H

#define STRICT_HOOK(fn)    \
	orig_##fn = zend_##fn; \
	zend_##fn = strict_##fn;

/*
ZEND_AST_DECLARE
 ZEND_AST_CONST_DECL
  ZEND_AST_CONST_ELEM
   ZEND_AST_ZVAL       // name
   ZEND_AST_BINARY_OP  // value
    ZEND_AST_ZVAL      // long
    ZEND_AST_ZVAL      // string
*/

#define IS_DECLARE_EX(ast) \
	ast->kind == ZEND_AST_BINARY_OP && \
	ast->attr == ZEND_ADD && \
	ast->child[1]->kind == ZEND_AST_ZVAL && \
	Z_TYPE(((zend_ast_zval *)ast->child[1])->val) == IS_STRING

#define GET_DECLARE_EX(ast) \
	Z_STR(((zend_ast_zval *)ast->child[1])->val)

ZEND_API zend_op_array *(*orig_compile_file)(zend_file_handle *fd, int type);
ZEND_API zend_op_array *(*orig_compile_string)(zval *src, char *filename);
ZEND_API zend_ast_process_t orig_ast_process;

ZEND_BEGIN_MODULE_GLOBALS(strict)
	int example;
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

