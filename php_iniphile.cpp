// Copyright (c) 2009 Roman Neuhauser
// Distributed under the MIT license (see LICENSE file)
// vim: sw=4 sts=4 et fdm=marker cms=\ //\ %s

#include "php_iniphile.hpp"
#include "iniphile.hpp"

#include "zend_exceptions.h"

zend_class_entry *iniphile_ce;
zend_object_handlers iniphile_object_handlers;

struct phpini
// {{{
{
    zend_object std;
    iniphile_bridge *impl;
}; // }}}

#define PHPTHIS() \
    static_cast<phpini *>( \
        zend_object_store_get_object(getThis() TSRMLS_CC) \
    )

#define PHPINI_THROW(m) \
    zend_throw_exception( \
        zend_exception_get_default(TSRMLS_C) \
      , m \
      , 0 TSRMLS_CC \
    );

#if ZEND_MODULE_API_NO >= 20090626

#define PHPINI_EH_DECL zend_error_handling error_handling
#define PHPINI_EH_THROWING \
    zend_replace_error_handling( \
        EH_THROW \
      , NULL \
      , &error_handling TSRMLS_CC \
    )
#define PHPINI_EH_NORMAL zend_restore_error_handling(&error_handling TSRMLS_CC)

#else

#define PHPINI_EH_DECL
#define PHPINI_EH_THROWING \
    php_set_error_handling( \
        EH_THROW \
      , zend_exception_get_default(TSRMLS_C) TSRMLS_CC \
    )
#define PHPINI_EH_NORMAL php_std_error_handling()

#endif

void
iniphile_free_storage(void *object TSRMLS_DC) // {{{
{
    phpini *obj = static_cast<phpini *>(object);
    delete obj->impl;

    zend_hash_destroy(obj->std.properties);
    FREE_HASHTABLE(obj->std.properties);

    efree(obj);
} // }}}

static
void
init(zend_object *zob, zend_class_entry *type TSRMLS_DC) // {{{
{
    zob->ce = type;

    ALLOC_HASHTABLE(zob->properties);
    zend_hash_init(
        zob->properties
      , 0
      , 0
      , ZVAL_PTR_DTOR
      , 0
    );

    zval *tmp;

    zend_hash_copy(
        zob->properties
      , &(zob->ce)->default_properties
      , reinterpret_cast<copy_ctor_func_t>(zval_add_ref)
      , static_cast<void *>(&tmp)
      , sizeof(zval *)
    );

} // }}}

zend_object_value
iniphile_create_handler(zend_class_entry *type TSRMLS_DC) // {{{
{
    phpini *obj = static_cast<phpini *>(
        emalloc(sizeof(phpini))
    );
    memset(obj, 0, sizeof(phpini));

    zend_object *zob(&obj->std);

    init(zob, type TSRMLS_CC);
    zend_object_value retval;

    retval.handle = zend_objects_store_put(
        obj
      , 0
      , iniphile_free_storage
      , 0 TSRMLS_CC
    );
    retval.handlers = &iniphile_object_handlers;

    return retval;
} // }}}

static
void
get_strings(zval *dst, zval const *src, phpini *obj, char const *path TSRMLS_DC) // {{{
{
    typedef std::vector<std::string> Strings;
    zval **elm;
    HashPosition i;
    HashTable *hash = Z_ARRVAL_P(src);
    Strings dv;
    dv.reserve(zend_hash_num_elements(hash));
    for (
        zend_hash_internal_pointer_reset_ex(hash, &i);
        SUCCESS == zend_hash_get_current_data_ex(hash, (void**) &elm, &i);
        zend_hash_move_forward_ex(hash, &i)
    ) {
        zval tmp;
        int use_copy;
        zend_make_printable_zval(*elm, &tmp, &use_copy);
        dv.push_back(Z_STRVAL(use_copy ? tmp : **elm));
        if (use_copy) {
            zval_dtor(&tmp);
        }
    }
    Strings rv(obj->impl->get(path, dv));
    array_init(dst);
    for (int i = 0; i < rv.size(); ++i) {
        add_next_index_string(dst, estrdup(rv[i].c_str()), 0);
    }
} // }}}

PHP_METHOD(iniphile, __construct) // {{{
{
    char *path;
    int pathlen;
    PHPINI_EH_DECL;

    PHPINI_EH_THROWING;
    if (FAILURE == zend_parse_parameters(
        ZEND_NUM_ARGS() TSRMLS_CC
      , "s"
      , &path
      , &pathlen
    )) {
        PHPINI_EH_NORMAL;
        return;
    }
    PHPINI_EH_NORMAL;

    phpini *obj = PHPTHIS();
    try {
        obj->impl = new iniphile_bridge(path);
    } catch (iniphile_errors::stream_error &e) {
        zend_throw_exception_ex(
            zend_exception_get_default(TSRMLS_C)
          , 0 TSRMLS_CC
          , "'%s' could not be open"
          , path
        );
    } catch (iniphile_errors::syntax_error &e) {
        zend_throw_exception_ex(
            zend_exception_get_default(TSRMLS_C)
          , 0 TSRMLS_CC
          , "Syntax error in '%s': %s"
          , path
          , e.what()
        );
    }
} // }}}
PHP_METHOD(iniphile, path) // {{{
{
    phpini *obj = PHPTHIS();
    RETURN_STRING(estrdup(obj->impl->path().c_str()), 0);
} // }}}
PHP_METHOD(iniphile, get) // {{{
{
    phpini *obj = PHPTHIS();
    char *path;
    int path_len;
    zval *dflt;
    PHPINI_EH_DECL;

    PHPINI_EH_THROWING;
    if (FAILURE == zend_parse_parameters(
        ZEND_NUM_ARGS() TSRMLS_CC
      , "sz"
      , &path
      , &path_len
      , &dflt
    )) {
        PHPINI_EH_NORMAL;
        return;
    }
    PHPINI_EH_NORMAL;

    switch (Z_TYPE_P(dflt)) {
    case IS_BOOL:
        RETURN_BOOL(obj->impl->get(path, !!Z_LVAL_P(dflt)));
    case IS_LONG:
        RETURN_LONG(obj->impl->get(path, Z_LVAL_P(dflt)));
    case IS_DOUBLE:
        RETURN_DOUBLE(obj->impl->get(path, Z_DVAL_P(dflt)));
    case IS_STRING:
        RETURN_STRING(estrdup(obj->impl->get(path, std::string(Z_STRVAL_P(dflt))).c_str()), 0);
    case IS_OBJECT: {
        zval tmp;
        int use_copy;

        PHPINI_EH_THROWING;
        zend_make_printable_zval(dflt, &tmp, &use_copy);
        PHPINI_EH_NORMAL;
        zval* str(use_copy ? &tmp : dflt);
        RETVAL_STRING(estrdup(obj->impl->get(path, std::string(Z_STRVAL_P(str))).c_str()), 0);
        if (use_copy) {
            zval_dtor(str);
        }
        break;
    }
    case IS_ARRAY:
        PHPINI_EH_THROWING;
        get_strings(return_value, dflt, obj, path TSRMLS_CC);
        PHPINI_EH_NORMAL;
        break;
    default:
        PHPINI_THROW("Unsupported value type");
    }
} // }}}

function_entry iniphile_methods[] = // {{{
{
    PHP_ME(iniphile, __construct, 0, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
    PHP_ME(iniphile, path, 0, ZEND_ACC_PUBLIC)
    PHP_ME(iniphile, get, 0, ZEND_ACC_PUBLIC)
    {0, 0, 0}
}; // }}}

PHP_MINIT_FUNCTION(iniphile) // {{{
{
    zend_class_entry ce;
    INIT_CLASS_ENTRY(ce, "iniphile", iniphile_methods);
    iniphile_ce = zend_register_internal_class(&ce TSRMLS_CC);
    iniphile_ce->create_object = &iniphile_create_handler;
    memcpy(
        &iniphile_object_handlers
      , zend_get_std_object_handlers()
      , sizeof(zend_object_handlers)
    );
    iniphile_object_handlers.clone_obj = 0;

    return SUCCESS;
} // }}}

zend_module_entry iniphile_module_entry = // {{{
{
    STANDARD_MODULE_HEADER,
    PHP_INIPHILE_EXTNAME,
    0, // functions
    PHP_MINIT(iniphile),
    0, // MSHUTDOWN
    0, // RINIT
    0, // RSHUTDOWN
    0, // MINFO
    PHP_INIPHILE_EXTVER,
    STANDARD_MODULE_PROPERTIES
}; // }}}

#ifdef COMPILE_DL_INIPHILE
extern "C" {
ZEND_GET_MODULE(iniphile)
}
#endif
