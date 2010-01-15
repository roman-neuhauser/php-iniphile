PHP_ARG_ENABLE(
  iniphile,
  [whether to enable the iniphile extension],
  [  --enable-iniphile       Enable "iniphile" extension support]
)

if test $PHP_INIPHILE != no; then
  PHP_REQUIRE_CXX()
  PHP_SUBST(INIPHILE_SHARED_LIBADD)
  PHP_ADD_LIBRARY(stdc++, 1, INIPHILE_SHARED_LIBADD)
  PHP_NEW_EXTENSION(iniphile, php_iniphile.cpp iniphile.cpp, $ext_shared)
fi

dnl vim: sw=2 sts=2 et fdm=marker cms=\ #\ %s
