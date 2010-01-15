// Copyright (c) 2009 Roman Neuhauser
// Distributed under the MIT license (see LICENSE file)
// vim: sw=4 sts=4 et fdm=marker cms=\ //\ %s

#ifndef INIPHILE_HPP
#define INIPHILE_HPP

#include <string>
#include <vector>
#include <exception>

#include "iniphile/astfwd.hpp"

namespace iniphile_errors
{

struct
stream_error
: public virtual std::exception
{
    stream_error(std::string const &msg);
    virtual ~stream_error() throw();
    virtual char const *what() const throw();
private:
    std::string const msg;
};

struct
syntax_error
: public virtual std::exception
{
    syntax_error(std::string const &msg);
    virtual ~syntax_error() throw();
    virtual char const *what() const throw();
private:
    std::string const msg;
};

} // namespace iniphile_errors

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
    bool open_;
};

#endif // INIPHILE_HPP
