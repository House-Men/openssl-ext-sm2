/* sm2 extension for PHP */

#ifdef HAVE_CONFIG_H

# include "config.h"

#endif

#include "php.h"
#include "ext/standard/info.h"
#include "php_sm2.h"
#include "openssl_sm2/sm2_create_key_pair.h"
#include "openssl_sm2/sm2_sign_and_verify.h"
#include "openssl_sm2/sm2_encrypt_and_decrypt.h"

/* For compatibility with older PHP versions */
#ifndef ZEND_PARSE_PARAMETERS_NONE
#define ZEND_PARSE_PARAMETERS_NONE() \
    ZEND_PARSE_PARAMETERS_START(0, 0) \
    ZEND_PARSE_PARAMETERS_END()
#endif

/* {{{ void sm2_test1()
 */
PHP_FUNCTION (sm2_test1) {
    ZEND_PARSE_PARAMETERS_NONE();

    php_printf("The extension %s is loaded and working!\r\n", "sm2");
}

/* }}} */
void outPrintf(unsigned char *c1, char *name) {
    php_printf("\r\n");
    php_printf("%s:\r\n  ", name);
    int i;
    for (i = 0; i < strlen((char *) c1); i++) {
        php_printf("%02x", c1[i]);
        if (((i + 1) % 4) == 0) php_printf(" ");
    }
    php_printf("\r\n");
}
/* {{{ string sm2_test2( [ string $var ] )
 */
PHP_FUNCTION (sm2_test2) {
    char *var = "World";
    size_t var_len = sizeof("World") - 1;
    zend_string *retval;

    ZEND_PARSE_PARAMETERS_START(0, 1)
            Z_PARAM_OPTIONAL
            Z_PARAM_STRING(var, var_len)
    ZEND_PARSE_PARAMETERS_END();

    retval = strpprintf(0, "Hello %s", var);

    RETURN_STR(retval);
}
/* }}}*/

/* {{{bool sm2_key_pair( [ string $pub_key,sting $pri_key ] )
 */
PHP_FUNCTION (sm2_key_pair) {

    zval *pub_key, *pri_key;
    zend_string *pub_key_buff, *pri_key_buff = NULL;
    SM2_KEY_PAIR key_pair;
    int error;
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "z/z/", &pub_key, &pri_key) == FAILURE) {
        return;
    }

    RETVAL_FALSE;
    int pubKey_len, privekey_len;

    if ((error = sm2_create_key_pair(&key_pair))) {
        goto clean_up;
    }

    pubKey_len = sizeof(key_pair.pub_key);
    privekey_len = sizeof(key_pair.pri_key);
    //结果转化
    pub_key_buff = zend_string_alloc(pubKey_len, 0);
    pri_key_buff = zend_string_alloc(privekey_len, 0);
    memcpy(ZSTR_VAL(pub_key_buff), key_pair.pub_key, pubKey_len);
    memcpy(ZSTR_VAL(pri_key_buff), key_pair.pri_key, privekey_len);


    zval_ptr_dtor(pub_key);
    ZSTR_VAL(pub_key_buff)[pubKey_len] = '\0';
    ZVAL_NEW_STR(pub_key, pub_key_buff);

    zval_ptr_dtor(pri_key);
    ZSTR_VAL(pri_key_buff)[privekey_len] = '\0';
    ZVAL_NEW_STR(pri_key, pri_key_buff);
    pub_key_buff = NULL;
    pri_key_buff = NULL;
    error = 0;
    clean_up:

    RETVAL_LONG(error);
}
/* }}}*/

PHP_FUNCTION (sm2_sign) {

    zval *signature;
    zend_string *signature_buff = NULL;

    unsigned char *msg, *pri_key, *iv;
    size_t msg_len, pri_key_len, signature_len, iv_len, mode = 0;
    iv = NULL;
    unsigned char sign[64];
    int error;
    SM2_SIGNATURE_STRUCT sm2_sig;
    SM2_KEY_PAIR key_pair;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "sz/ss|l", &msg, &msg_len, &signature, &pri_key, &pri_key_len,
                              &iv, &iv_len, &mode) == FAILURE) {
        return;
    }
    RETVAL_FALSE;
    if (pri_key_len != 32) {
        error = PHP_SM2_PRI_KEY_LEN;
        goto clean_up;
    }
    memcpy(key_pair.pri_key, pri_key, pri_key_len);
    generate_pubic_key(&key_pair);

    if ((error = sm2_sign_data(msg,
                               (int) msg_len,
                               iv,
                               (int) iv_len,
                               key_pair.pub_key,
                               key_pair.pri_key,
                               &sm2_sig,(int)mode))) {
        goto clean_up;
    }

    int i;

    for (i = 0; i < 32; i++) {
        sign[i] = sm2_sig.r_coordinate[i];
    }
    for (i = 0; i < 32; i++) {
        sign[i + 32] = sm2_sig.s_coordinate[i];
    }
    signature_len = 64;
    signature_buff = zend_string_alloc(signature_len, 0);
    memcpy(ZSTR_VAL(signature_buff), sign, signature_len);

    zval_ptr_dtor(signature);
    ZSTR_VAL(signature_buff)[signature_len] = '\0';
    ZVAL_NEW_STR(signature, signature_buff);

    signature_buff = NULL;
    error = 0;
    clean_up:
    RETVAL_LONG(error);
}

PHP_FUNCTION (sm2_sign_verify) {

    unsigned char *msg, *signature, *pub_key, *iv;
    size_t msg_len, pub_key_len, signature_len, iv_len, mode = 0;
    int error;
    SM2_SIGNATURE_STRUCT sm2_sig;
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "ssss|l", &msg, &msg_len, &signature, &signature_len, &pub_key,
                              &pub_key_len,
                              &iv, &iv_len, &mode) == FAILURE) {
        return;
    }
    memcpy(sm2_sig.r_coordinate, signature, sizeof(sm2_sig.r_coordinate));
    memcpy(sm2_sig.s_coordinate, (signature + sizeof(sm2_sig.r_coordinate)), sizeof(sm2_sig.s_coordinate));

    error = sm2_verify_sig(msg,
                           (int)msg_len,
                           iv,
                           (int)iv_len,
                           pub_key,
                           &sm2_sig,
						   (int)mode);

    RETVAL_LONG(error);
}

PHP_FUNCTION (sm2_encrypt) {

    zval *encrypt;
    zend_string *encrypt_buff = NULL;
    unsigned char *msg, *pub_key, c1[65], c3[32];
    size_t msg_len, pub_key_len, mode = 0;
    unsigned char *c2, *plaintext = NULL;
    int plaintext_len, error,c1_len,c2_len,c3_len;
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "sz/s|l", &msg, &msg_len, &encrypt, &pub_key,
                              &pub_key_len, &mode) == FAILURE) {
        return;
    }
//
    if (!(c2 = (unsigned char *) malloc(msg_len))) {
        error = 0x203;
        goto clean_up;
    }
   ;

    SM2_KEY_PAIR key_pair;
    memcpy(key_pair.pub_key, pub_key, sizeof(key_pair.pub_key));
    if ((error = php_sm2_encrypt(msg, (int) msg_len, key_pair.pub_key, c1, c3, c2, (int)mode))) {
        goto clean_up;
    }
    c1_len=sizeof(c1);
    c2_len=(int)msg_len;
    c3_len=sizeof(c3);
    plaintext_len=c1_len+c3_len+c2_len;
    //动态分配的字节数
    if (!(plaintext = (unsigned char *) malloc(plaintext_len))) {
        error = 0x204;
        goto clean_up;
    }

	if(mode < 1) {
		memcpy(plaintext, c1, c1_len);
		memcpy((plaintext + c1_len), c3, c3_len);
		memcpy((plaintext + c1_len + c3_len), c2, c2_len);
	}else{
		memcpy(plaintext, c1, c1_len);
		memcpy((plaintext + c1_len), c2, c2_len);
		memcpy((plaintext + c1_len + c2_len), c3, c3_len);
	}
//    //结果转化
    encrypt_buff = zend_string_alloc(plaintext_len, 0);
    memcpy(ZSTR_VAL(encrypt_buff), plaintext, plaintext_len);

    zval_ptr_dtor(encrypt);
    ZSTR_VAL(encrypt_buff)[plaintext_len] = '\0';
    ZVAL_NEW_STR(encrypt, encrypt_buff);
    error = 0;
    clean_up:
    if (plaintext) {
        free(plaintext);
    }
    if (c2) {
        free(c2);
    }
    encrypt_buff = NULL;
    RETVAL_LONG(error);
}


PHP_FUNCTION (sm2_decrypt) {

    zval *msg;
    zend_string *msg_buff = NULL;
    unsigned char *encrypt, *pri_key, *c2, *plaintext, c1[65], c3[32];
    size_t encrypt_len, pri_key_len, mode = 0;
    int c1_len,c2_len,c3_len,error;
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "sz/s|l", &encrypt, &encrypt_len, &msg, &pri_key,
                              &pri_key_len, &mode) == FAILURE) {
        return;
    }
    RETVAL_FALSE;
    c1_len=65;
    c3_len=32;
    c2_len= (int) encrypt_len - c1_len - 32;
    if(!(c2 = (unsigned char *) malloc(c2_len))){
        error=0x301;
        goto  clean_up;
    }

    if(!( plaintext = (unsigned char *) malloc(c2_len))){
        error=0x302;
        goto  clean_up;
    }
	if(mode < 1) {
		memcpy(c1, encrypt, 65);
		memcpy(c3, (encrypt + 65), 32);
		memcpy(c2, (encrypt + 65 + 32), c2_len);
	}else {
		memcpy(c1, encrypt, 65);
		memcpy(c2, (encrypt + 65), c2_len);
		memcpy(c3, (encrypt + 65 + c2_len), 32);
	}
    if((error=php_sm2_decrypt(c1, c3, c2, c2_len, pri_key, plaintext, (int)mode))){
        goto  clean_up;
    }
    msg_buff = zend_string_alloc(c2_len, 0);
    memcpy(ZSTR_VAL(msg_buff), plaintext, c2_len);
    zval_ptr_dtor(msg);
    ZSTR_VAL(msg_buff)[c2_len] = '\0';
    ZVAL_NEW_STR(msg, msg_buff);
    error=0;

    clean_up:
    if (plaintext) {
        free(plaintext);
    }
    if (c2) {
        free(c2);
    }
    msg_buff = NULL;
    RETVAL_LONG(error);
}

/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION (sm2) {
#if defined(ZTS) && defined(COMPILE_DL_SM2)
    ZEND_TSRMLS_CACHE_UPDATE();
#endif

    return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION (sm2) {
    php_info_print_table_start();
    php_info_print_table_header(2, "sm2 support", "enabled");
    php_info_print_table_end();
}
/* }}} */

/* {{{ arginfo
 */
ZEND_BEGIN_ARG_INFO(arginfo_sm2_test1, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_sm2_test2, 0)
                ZEND_ARG_INFO(0, str)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_sm2_key_pair, 0, 0, 2)
                ZEND_ARG_INFO(1, pub_key)
                ZEND_ARG_INFO(1, pri_key)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_sm2_sign, 0, 0, 3)
                ZEND_ARG_INFO(0, msg)
                ZEND_ARG_INFO(1, signature)
                ZEND_ARG_INFO(0, pri_key)
                ZEND_ARG_INFO(0, iv)
				ZEND_ARG_INFO(0, mode)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(arginfo_sm2_sign_verify, 0, 0, 3)
                ZEND_ARG_INFO(0, msg)
                ZEND_ARG_INFO(0, signature)
                ZEND_ARG_INFO(0, pub_key)
                ZEND_ARG_INFO(0, iv)
				ZEND_ARG_INFO(0, mode)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_sm2_encrypt, 0, 0, 3)
                ZEND_ARG_INFO(0, msg)
                ZEND_ARG_INFO(1, encrypt)
                ZEND_ARG_INFO(0, pub_key)
				ZEND_ARG_INFO(0, mode)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_sm2_decrypt, 0, 0, 3)
                ZEND_ARG_INFO(0, encrypt)
                ZEND_ARG_INFO(1, msg)
                ZEND_ARG_INFO(0, pri_key)
				ZEND_ARG_INFO(0, mode)
ZEND_END_ARG_INFO()
/* }}} */

/* {{{ sm2_functions[]
 */
static const zend_function_entry sm2_functions[] = {
        PHP_FE(sm2_test1, arginfo_sm2_test1)
        PHP_FE(sm2_test2, arginfo_sm2_test2)
        PHP_FE(sm2_key_pair, arginfo_sm2_key_pair)
        PHP_FE(sm2_sign, arginfo_sm2_sign)
        PHP_FE(sm2_sign_verify, arginfo_sm2_sign_verify)
        PHP_FE(sm2_encrypt, arginfo_sm2_encrypt)
        PHP_FE(sm2_decrypt, arginfo_sm2_decrypt)
        PHP_FE_END
};
/* }}} */

/* {{{ sm2_module_entry
 */
zend_module_entry sm2_module_entry = {
        STANDARD_MODULE_HEADER,
        "sm2",                    /* Extension name */
        sm2_functions,            /* zend_function_entry */
        NULL,                            /* PHP_MINIT - Module initialization */
        NULL,                            /* PHP_MSHUTDOWN - Module shutdown */
        PHP_RINIT(sm2),            /* PHP_RINIT - Request initialization */
        NULL,                            /* PHP_RSHUTDOWN - Request shutdown */
        PHP_MINFO(sm2),            /* PHP_MINFO - Module info */
        PHP_SM2_VERSION,        /* Version */
        STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_SM2
# ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
# endif

ZEND_GET_MODULE(sm2)

#endif

