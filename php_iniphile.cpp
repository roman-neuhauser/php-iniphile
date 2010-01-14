// Copyright (c) 2009 Roman Neuhauser
// Distributed under the MIT license (see LICENSE file)
// vim: sw=4 sts=4 et fdm=marker cms=\ //\ %s

#include "php_iniphile.hpp"
#include "iniphile.hpp"

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

PHP_METHOD(iniphile, __construct) // {{{
{
    char *path;
    int pathlen;

    if (FAILURE == zend_parse_parameters(
        ZEND_NUM_ARGS() TSRMLS_CC
      , "s"
      , &path
      , &pathlen
    )) {
        return;
    }
    phpini *obj = PHPTHIS();
    obj->impl = new iniphile_bridge(path);
} // }}}

PHP_METHOD(iniphile, is_open) // {{{
{
    phpini *obj = PHPTHIS();
    RETURN_BOOL(obj->impl->is_open());
} // }}}
PHP_METHOD(iniphile, path) // {{{
{
    phpini *obj = PHPTHIS();
    RETURN_STRING(estrdup(obj->impl->path().c_str()), 0);
} // }}}
PHP_METHOD(iniphile, get_strings) // {{{
{
    phpini *obj = PHPTHIS();
    char *path;
    int path_len;
    zval *arr;
    if (FAILURE == zend_parse_parameters(
        ZEND_NUM_ARGS() TSRMLS_CC
      , "sa"
      , &path
      , &path_len
      , &arr
    )) {
        RETURN_NULL();
    }
    typedef std::vector<std::string> Strings;
    zval **elm;
    HashPosition i;
    HashTable *hash = Z_ARRVAL_P(arr);
    Strings dv(zend_hash_num_elements(hash));
    for (
        zend_hash_internal_pointer_reset_ex(hash, &i);
        SUCCESS == zend_hash_get_current_data_ex(hash, (void**) &elm, &i);
        zend_hash_move_forward_ex(hash, &i)
    ) {
        dv.push_back(Z_STRVAL_PP(elm));
    }
    Strings rv(obj->impl->get(path, dv));
    array_init(return_value);
    for (int i = 0; i < rv.size(); ++i) {
        add_next_index_string(return_value, estrdup(rv[i].c_str()), 0);
    }
} // }}}
PHP_METHOD(iniphile, get_string) // {{{
{
    phpini *obj = PHPTHIS();
    char *path, *dflt;
    int path_len, dflt_len;
    if (FAILURE == zend_parse_parameters(
        ZEND_NUM_ARGS() TSRMLS_CC
      , "ss"
      , &path
      , &path_len
      , &dflt
      , &dflt_len
    )) {
        RETURN_NULL();
    }
    RETURN_STRING(estrdup(obj->impl->get(path, std::string(dflt)).c_str()), 0);
} // }}}
PHP_METHOD(iniphile, get_bool) // {{{
{
    phpini *obj = PHPTHIS();
    char *path;
    int path_len;
    zend_bool dflt;
    if (FAILURE == zend_parse_parameters(
        ZEND_NUM_ARGS() TSRMLS_CC
      , "sb"
      , &path
      , &path_len
      , &dflt
    )) {
        RETURN_NULL();
    }
    RETURN_BOOL(obj->impl->get(path, !!dflt));
} // }}}
PHP_METHOD(iniphile, get_long) // {{{
{
    phpini *obj = PHPTHIS();
    char *path;
    int path_len;
    long dflt;
    if (FAILURE == zend_parse_parameters(
        ZEND_NUM_ARGS() TSRMLS_CC
      , "sl"
      , &path
      , &path_len
      , &dflt
    )) {
        RETURN_NULL();
    }
    RETURN_LONG(obj->impl->get(path, dflt));
} // }}}
PHP_METHOD(iniphile, get_double) // {{{
{
    phpini *obj = PHPTHIS();
    char *path;
    int path_len;
    double dflt;
    if (FAILURE == zend_parse_parameters(
        ZEND_NUM_ARGS() TSRMLS_CC
      , "sd"
      , &path
      , &path_len
      , &dflt
    )) {
        RETURN_NULL();
    }
    RETURN_DOUBLE(obj->impl->get(path, dflt));
} // }}}

function_entry iniphile_methods[] = // {{{
{
    PHP_ME(iniphile, __construct, 0, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
    PHP_ME(iniphile, is_open, 0, ZEND_ACC_PUBLIC)
    PHP_ME(iniphile, path, 0, ZEND_ACC_PUBLIC)
    PHP_ME(iniphile, get_strings, 0, ZEND_ACC_PUBLIC)
    PHP_ME(iniphile, get_string, 0, ZEND_ACC_PUBLIC)
    PHP_ME(iniphile, get_bool, 0, ZEND_ACC_PUBLIC)
    PHP_ME(iniphile, get_long, 0, ZEND_ACC_PUBLIC)
    PHP_ME(iniphile, get_double, 0, ZEND_ACC_PUBLIC)
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
