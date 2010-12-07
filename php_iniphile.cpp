// Copyright (c) 2009-2010 Roman Neuhauser
// Distributed under the MIT license (see LICENSE file)
// vim: sw=4 sts=4 et fdm=marker cms=\ //\ %s

#include <sstream>
#include <fstream>

extern "C" {
#include "php_iniphile.h"
}
#include "iniphile.hpp"

#include "zend_exceptions.h"

iniphile_bridge*
parse_string(std::string const &syn)
{
    std::istringstream input(syn);
    return new iniphile_bridge(input, "(input)");
}

iniphile_bridge*
parse_file(std::string const &path)
{
    std::ifstream input(path.c_str(), std::ios_base::binary);
    return new iniphile_bridge(input, path);
}

zend_class_entry *iniphile_ce;
zend_class_entry *iniphile_error_ce;
zend_object_handlers iniphile_object_handlers;

struct phpini
// {{{
{
    zend_object std;
    iniphile_bridge *impl;
}; // }}}

#define PHPINI(o) \
    static_cast<phpini *>( \
        zend_object_store_get_object(o TSRMLS_CC) \
    )

#define PHPTHIS() PHPINI(getThis())

#define PHPINI_THROW(m) \
    zend_throw_exception( \
        iniphile_error_ce \
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

static
void
instantiate(zval *o) // {{{
{
    Z_TYPE_P(o) = IS_OBJECT;
    object_init_ex(o, iniphile_ce);
    Z_SET_REFCOUNT_P(o, 1);
    Z_UNSET_ISREF_P(o);
} // }}}
static
void
initialize(phpini *obj, char const *src, bool is_path TSRMLS_DC) // {{{
{
    try {
        obj->impl = is_path ? parse_file(src) : parse_string(src);
    } catch (iniphile_errors::stream_error) {
        zend_throw_exception_ex(
            iniphile_error_ce
          , 0 TSRMLS_CC
          , "'%s' could not be open"
          , src
        );
    } catch (iniphile_errors::syntax_error &e) {
        zend_throw_exception_ex(
            iniphile_error_ce
          , 0 TSRMLS_CC
          , "Syntax error in '%s': %s"
          , src
          , e.what()
        );
    }
} // }}}
static
char *
get_arg(int argc TSRMLS_DC) // {{{
{
    char *src;
    int srclen;
    PHPINI_EH_DECL;

    PHPINI_EH_THROWING;
    if (FAILURE == zend_parse_parameters(
        argc TSRMLS_CC
      , "s"
      , &src
      , &srclen
    )) {
        PHPINI_EH_NORMAL;
        return 0;
    }
    PHPINI_EH_NORMAL;
    return src;
} // }}}
#define CREATE_INIPHILE(o, ctor, is_path) \
    char *src = get_arg(ZEND_NUM_ARGS() TSRMLS_CC); \
    if (!src) RETURN_NULL(); \
    if (!ctor) instantiate(o TSRMLS_CC); \
    initialize(PHPINI(o), src, is_path TSRMLS_CC);

PHP_METHOD(iniphile, __construct) // {{{
{
    CREATE_INIPHILE(getThis(), true, true)
} // }}}
PHP_METHOD(iniphile, from_file) // {{{
{
    CREATE_INIPHILE(return_value, false, true)
} // }}}
PHP_METHOD(iniphile, from_string) // {{{
{
    CREATE_INIPHILE(return_value, false, false)
} // }}}
PHP_METHOD(iniphile, get) // {{{
{
    phpini *obj = PHPINI(getThis());
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

#define INIPHILE_BEGIN_ARG_INFO(method, argc) \
    ZEND_BEGIN_ARG_INFO_EX(arginfo_iniphile##method, 0, 0, argc)

INIPHILE_BEGIN_ARG_INFO(__construct, 1)
    ZEND_ARG_INFO(0, input)
ZEND_END_ARG_INFO()

INIPHILE_BEGIN_ARG_INFO(from_file, 1)
    ZEND_ARG_INFO(0, path)
ZEND_END_ARG_INFO()

INIPHILE_BEGIN_ARG_INFO(from_string, 1)
    ZEND_ARG_INFO(0, syntax)
ZEND_END_ARG_INFO()

INIPHILE_BEGIN_ARG_INFO(get, 2)
    ZEND_ARG_INFO(0, path)
    ZEND_ARG_INFO(0, dflt)
ZEND_END_ARG_INFO()

#define INIPHILE_ME(method, acc) \
    PHP_ME(iniphile, method, arginfo_iniphile##method, acc)

function_entry iniphile_methods[] = // {{{
{
    INIPHILE_ME(__construct, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
    INIPHILE_ME(get, ZEND_ACC_PUBLIC)
    INIPHILE_ME(from_file, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
    INIPHILE_ME(from_string, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
    {0, 0, 0}
}; // }}}

function_entry iniphile_exception_methods[] = // {{{
{
    {0, 0, 0}
}; // }}}

PHP_MINIT_FUNCTION(iniphile) // {{{
{
    zend_class_entry ce;

    INIT_CLASS_ENTRY(ce, "iniphile_error", iniphile_exception_methods);
    iniphile_error_ce = zend_register_internal_class_ex(
        &ce
      , zend_exception_get_default(TSRMLS_C)
      , NULL
      TSRMLS_CC
    );

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
