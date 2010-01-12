// Copyright (c) 2009 Roman Neuhauser
// Distributed under the MIT license (see LICENSE file)
// vim: sw=4 sts=4 et fdm=marker cms=\ //\ %s

#ifndef INIPHILE_HPP
#define INIPHILE_HPP

#include <string>
#include "iniphile/astfwd.hpp"

struct iniphile_bridge
{
    explicit
    iniphile_bridge(std::string const path);
    ~iniphile_bridge();
    std::string const
    path();
    std::string const
    get_string(std::string const query);
    bool
    get_bool(std::string const query);
    int
    get_int(std::string const query);
    bool is_open();
private:
    std::string const src;
    iniphile::ast::node *afg;
    bool open_;
};

#endif // INIPHILE_HPP
