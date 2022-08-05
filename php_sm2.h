/* sm2 extension for PHP */

#ifndef PHP_SM2_H
# define PHP_SM2_H

extern zend_module_entry sm2_module_entry;
# define phpext_sm2_ptr &sm2_module_entry

# define PHP_SM2_VERSION "0.1.0"
//验证私钥长度
# define PHP_SM2_PRI_KEY_LEN 0x100d
//验证公钥长度
# define PHP_SM2_PUB_KEY_LEN 0x100e

# if defined(ZTS) && defined(COMPILE_DL_SM2)
ZEND_TSRMLS_CACHE_EXTERN()
# endif

#endif	/* PHP_SM2_H */

