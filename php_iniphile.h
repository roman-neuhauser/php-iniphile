// Copyright (c) 2009-2010 Roman Neuhauser
// Distributed under the MIT license (see LICENSE file)
// vim: sw=4 sts=4 et fdm=marker cms=\ //\ %s

#ifndef PHP_INIPHILE_HPP
#define PHP_INIPHILE_HPP

#define PHP_INIPHILE_EXTNAME "iniphile"
#define PHP_INIPHILE_EXTVER "0.0"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"

extern zend_module_entry iniphile_module_entry;
#define phpext_iniphile_ptr &iniphile_module_entry

#endif // PHP_INIPHILE_HPP
