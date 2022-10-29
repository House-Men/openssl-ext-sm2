dnl config.m4 for extension sm2
PHP_ARG_WITH(openssl, for OpenSSL support,
[  --with-openssl[=DIR]      Include OpenSSL support (requires OpenSSL >= 1.0.1)])

PHP_ARG_WITH(kerberos, for Kerberos support,
[  --with-kerberos[=DIR]     OPENSSL: Include Kerberos support], no, no)

PHP_ARG_WITH(system-ciphers, whether to use system default cipher list instead of hardcoded value,
[  --with-system-ciphers   OPENSSL: Use system default cipher list instead of hardcoded value], no, no)

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary.

dnl If your extension references something external, use with:

dnl PHP_ARG_WITH(sm2, for sm2 support,
dnl Make sure that the comment is aligned:
dnl [  --with-sm2             Include sm2 support])

dnl Otherwise use enable:

PHP_ARG_ENABLE(sm2, whether to enable sm2 support,
dnl Make sure that the comment is aligned:
[  --enable-sm2          Enable sm2 support], no)

if test "$PHP_SM2" != "no"; then
  dnl Write more examples of tests here...

  dnl # get library FOO build options from pkg-config output
  dnl AC_PATH_PROG(PKG_CONFIG, pkg-config, no)
  dnl AC_MSG_CHECKING(for libfoo)
  dnl if test -x "$PKG_CONFIG" && $PKG_CONFIG --exists foo; then
  dnl   if $PKG_CONFIG foo --atleast-version 1.2.3; then
  dnl     LIBFOO_CFLAGS=\`$PKG_CONFIG foo --cflags\`
  dnl     LIBFOO_LIBDIR=\`$PKG_CONFIG foo --libs\`
  dnl     LIBFOO_VERSON=\`$PKG_CONFIG foo --modversion\`
  dnl     AC_MSG_RESULT(from pkgconfig: version $LIBFOO_VERSON)
  dnl   else
  dnl     AC_MSG_ERROR(system libfoo is too old: version 1.2.3 required)
  dnl   fi
  dnl else
  dnl   AC_MSG_ERROR(pkg-config not found)
  dnl fi
  dnl PHP_EVAL_LIBLINE($LIBFOO_LIBDIR, SM2_SHARED_LIBADD)
  dnl PHP_EVAL_INCLINE($LIBFOO_CFLAGS)

  dnl # --with-sm2 -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/sm2.h"  # you most likely want to change this
  dnl if test -r $PHP_SM2/$SEARCH_FOR; then # path given as parameter
  dnl   SM2_DIR=$PHP_SM2
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for sm2 files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       SM2_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$SM2_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the sm2 distribution])
  dnl fi

  dnl # --with-sm2 -> add include path
  dnl PHP_ADD_INCLUDE($SM2_DIR/include)

  dnl # --with-sm2 -> check for lib and symbol presence
  dnl LIBNAME=SM2 # you may want to change this
  dnl LIBSYMBOL=SM2 # you most likely want to change this

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $SM2_DIR/$PHP_LIBDIR, SM2_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_SM2LIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong sm2 lib version or lib not found])
  dnl ],[
  dnl   -L$SM2_DIR/$PHP_LIBDIR -lm
  dnl ])
  dnl
  dnl PHP_SUBST(SM2_SHARED_LIBADD)

  dnl # In case of no dependencies
  AC_DEFINE(HAVE_SM2, 1, [ Have sm2 support ])

  PHP_NEW_EXTENSION(sm2, sm2.c openssl_sm2/sm2_create_key_pair.c openssl_sm2/sm2_sign_and_verify.c openssl_sm2/sm3_with_preprocess.c openssl_sm2/sm2_encrypt_and_decrypt.c, $ext_shared)
  PHP_SUBST(SM2_SHARED_LIBADD)

  if test "$PHP_KERBEROS" != "no"; then
    PHP_SETUP_KERBEROS(SM2_SHARED_LIBADD)
  fi

  AC_CHECK_FUNCS([RAND_egd])

    PHP_SETUP_OPENSSL(SM2_SHARED_LIBADD,
    [
      AC_DEFINE(HAVE_OPENSSL_EXT,1,[ ])
    ], [
      AC_MSG_ERROR([OpenSSL check failed. Please check config.log for more information.])
    ])
  if test "$PHP_SYSTEM_CIPHERS" != "no"; then
    AC_DEFINE(USE_OPENSSL_SYSTEM_CIPHERS,1,[ Use system default cipher list instead of hardcoded value ])
  fi
fi
