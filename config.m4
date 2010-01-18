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
    for sub in "" include; do
      if test -e $PHP_BOOST/$sub/boost/spirit.hpp; then
        INIPHILE_BOOST_INCLUDE=$PHP_BOOST/$sub
        break;
      fi
    done
  else
    for path in /usr/local /usr; do
      for sub in "" include; do
        if test -e $path/$sub/boost/spirit.hpp; then
          INIPHILE_BOOST_INCLUDE=$path/$sub
          break;
        fi
      done
    done
  fi
  if test -d "$INIPHILE_BOOST_INCLUDE"; then
    :
  else
    AC_MSG_ERROR([boost/spirit.hpp not found, try again --with-boost=DIR])
  fi
  PHP_ADD_INCLUDE($INIPHILE_BOOST_INCLUDE)
  PHP_NEW_EXTENSION(iniphile, php_iniphile.cpp iniphile.cpp, $ext_shared)
fi

dnl vim: sw=2 sts=2 et fdm=marker cms=\ #\ %s
