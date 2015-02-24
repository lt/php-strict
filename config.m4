PHP_ARG_ENABLE(strict, whether to enable strict by default,
[  --disable-strict        Hah, no. This switch is ignored], yes)

PHP_NEW_EXTENSION(strict, strict.c, $ext_shared)
PHP_SUBST(STRICT_SHARED_LIBADD)
AC_DEFINE(HAVE_PHP_STRICT,1,[ ])

