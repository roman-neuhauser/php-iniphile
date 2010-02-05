PHP_ARG_WITH(
  boost-dir,
  [for Boost C++ libraries],
  [  --with-boost-dir=DIR    INIPHILE: Boost install prefix],
  [$prefix]
)
PHP_ARG_WITH(
  iniphile,
  [for iniphile support],
  [  --with-iniphile[=DIR]   Include iniphile support]
)

if test $PHP_INIPHILE != no; then
  PHP_REQUIRE_CXX()
  PHP_SUBST(INIPHILE_SHARED_LIBADD)
  PHP_ADD_LIBRARY(stdc++, 1, INIPHILE_SHARED_LIBADD)

  iniphile_canary=include/iniphile/astfwd.hpp
  if test -r $PHP_INIPHILE/$iniphile_canary; then
    INIPHILE_DIR=$PHP_INIPHILE
  else
    AC_MSG_CHECKING(for Iniphile in default path)
    for i in /usr/local /usr; do
      if test -r $i/$iniphile_canary; then
        INIPHILE_DIR=$i
        AC_MSG_RESULT(found in $i)
        break
      fi
    done
  fi
  if test -z "$INIPHILE_DIR"; then
    AC_MSG_RESULT(not found)
    AC_MSG_ERROR(Please reinstall the Iniphile C++ library)
  else
    PHP_ADD_INCLUDE($INIPHILE_DIR/include)
    PHP_ADD_LIBRARY_WITH_PATH(iniphile, $INIPHILE_DIR/$PHP_LIBDIR, INIPHILE_SHARED_LIBADD)
  fi

  if test "x$PHP_BOOST_DIR" != "xno"; then
    for sub in "" include; do
      if test -e $PHP_BOOST_DIR/$sub/boost/spirit.hpp; then
        INIPHILE_BOOST_INCLUDE=$PHP_BOOST_DIR/$sub
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
    AC_MSG_ERROR([boost/spirit.hpp not found, try again --with-boost-dir=DIR])
  fi
  PHP_ADD_INCLUDE($INIPHILE_BOOST_INCLUDE)

  PHP_NEW_EXTENSION(iniphile, php_iniphile.cpp iniphile.cpp errors.cpp, $ext_shared)
fi

dnl vim: sw=2 sts=2 et fdm=marker cms=\ #\ %s
