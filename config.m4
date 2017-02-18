dnl $Id$
dnl config.m4 for extension phop

PHP_ARG_ENABLE(phop, whether to enable phop support,
[  --enable-phop           Enable phop support])

if test "$PHP_PHOP" != "no"; then
  PHP_NEW_EXTENSION(phop, phop.c, $ext_shared,, -DZEND_ENABLE_STATIC_TSRMLS_CACHE=1)
fi
