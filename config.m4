PHP_ARG_WITH(
  boost,
  [for Boost C++ libraries],
  [  --with-boost       Boost C++ libraries],
  [$prefix]
)
PHP_ARG_ENABLE(
  iniphile,
  [whether to enable the iniphile extension],
  [  --enable-iniphile       Enable "iniphile" extension support]
)

if test $PHP_INIPHILE != no; then
  PHP_REQUIRE_CXX()
  PHP_SUBST(INIPHILE_SHARED_LIBADD)
  PHP_ADD_LIBRARY(stdc++, 1, INIPHILE_SHARED_LIBADD)
  if test "x$PHP_BOOST" != "xno"; then
    if test -e $PHP_BOOST/include/boost/spirit.hpp; then
      INIPHILE_BOOST_INCLUDE=$PHP_BOOST/include
    fi
  else
    for path in /usr/local /usr; do
      if test -e $path/include/boost/spirit.hpp; then
        INIPHILE_BOOST_INCLUDE=$path/include
        break
      fi
    done
  fi
  if test -z "$INIPHILE_BOOST_INCLUDE"; then
    AC_MSG_ERROR([$PHP_BOOST/include/boost/spirit.hpp not found, try again --with-boost=DIR])
  fi
  PHP_ADD_INCLUDE($INIPHILE_BOOST_INCLUDE)
  PHP_NEW_EXTENSION(iniphile, php_iniphile.cpp iniphile.cpp, $ext_shared)
fi

dnl vim: sw=2 sts=2 et fdm=marker cms=\ #\ %s
