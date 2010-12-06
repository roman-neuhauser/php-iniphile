// Copyright (c) 2009-2010 Roman Neuhauser
// Distributed under the MIT license (see LICENSE file)
// vim: sw=4 sts=4 et fdm=marker cms=\ //\ %s

#ifndef INIPHILE_HPP
#define INIPHILE_HPP

#include <string>
#include <vector>
#include <exception>
#include <istream>

#include "iniphile/astfwd.hpp"
#include "errors.hpp"

struct iniphile_bridge
{
    explicit
    iniphile_bridge(std::istream& input, std::string const& desc);
    ~iniphile_bridge();

    template<class T>
    T
    get(std::string const query, T dflt);
private:
    iniphile::ast::node *afg;
};

iniphile_bridge*
parse_string(std::string const &input);

iniphile_bridge*
parse_file(std::string const &path);

#endif // INIPHILE_HPP
