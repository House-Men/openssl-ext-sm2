PHP_ARG_WITH([sm2],
  [for sm2 support],
  [AS_HELP_STRING([--with-sm2],
    [Include sm2 support (requires OpenSSL >= 1.0.1)])])

PHP_ARG_WITH([kerberos],
  [for Kerberos support],
  [AS_HELP_STRING([--with-kerberos],
    [OPENSSL: Include Kerberos support])],
  [no],
  [no])

PHP_ARG_WITH([system-ciphers],
  [whether to use system default cipher list instead of hardcoded value],
  [AS_HELP_STRING([--with-system-ciphers],
    [OPENSSL: Use system default cipher list instead of hardcoded value])],
  [no],
  [no])

if test "$PHP_SM2" != "no"; then
  PHP_NEW_EXTENSION(sm2, sm2.c openssl_sm2/sm2_java.c openssl_sm2/sm2_create_key_pair.c openssl_sm2/sm2_sign_and_verify.c openssl_sm2/sm3_with_preprocess.c openssl_sm2/sm2_encrypt_and_decrypt.c, $ext_shared)
  PHP_SUBST(SM2L_SHARED_LIBADD)

  if test "$PHP_KERBEROS" != "no"; then
    PKG_CHECK_MODULES([KERBEROS], [krb5-gssapi krb5])

    PHP_EVAL_INCLINE($KERBEROS_CFLAGS)
    PHP_EVAL_LIBLINE($KERBEROS_LIBS, SM2_SHARED_LIBADD)
  fi

  PHP_SETUP_OPENSSL(SM2_SHARED_LIBADD,
  [
    AC_DEFINE(HAVE_OPENSSL_EXT,1,[ ])
  ], [
    AC_MSG_ERROR([OpenSSL check failed. Please check config.log for more information.])
  ])
  if test "$PHP_SYSTEM_CIPHERS" != "no"; then
    AC_DEFINE(USE_OPENSSL_SYSTEM_CIPHERS,1,[ Use system default cipher list instead of hardcoded value ])
  fi
  AC_CHECK_FUNCS([RAND_egd])
fi
