// Copyright (c) 2009 Roman Neuhauser
// Distributed under the MIT license (see LICENSE file)
// vim: sw=4 sts=4 et fdm=marker cms=\ //\ %s

#ifndef INIPHILE_HPP
#define INIPHILE_HPP

#include <string>
#include <vector>
#include <exception>

#include "iniphile/astfwd.hpp"
#include "errors.hpp"

struct iniphile_bridge
{
    explicit
    iniphile_bridge(std::string const path);
    ~iniphile_bridge();
    std::string const
    path();

    template<class T>
    T
    get(std::string const query, T dflt);
private:
    std::string const src;
    iniphile::ast::node *afg;
};

#endif // INIPHILE_HPP
