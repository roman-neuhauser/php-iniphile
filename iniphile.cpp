// Copyright (c) 2009 Roman Neuhauser
// Distributed under the MIT license (see LICENSE file)
// vim: sw=4 sts=4 et fdm=marker cms=\ //\ %s

#include <fstream>

#include "iniphile.hpp"
#include "iniphile/input.hpp"
#include "iniphile/ast.hpp"
#include "iniphile/output.hpp"

namespace iniphile_errors
{

stream_error::stream_error(std::string const &msg)
: msg(msg)
{}

char const *
stream_error::what() const throw()
{
    return msg.c_str();
}

stream_error::~stream_error() throw() {}

syntax_error::syntax_error(std::string const &msg)
: msg(msg)
{}

char const *
syntax_error::what() const throw()
{
    return msg.c_str();
}

syntax_error::~syntax_error() throw() {}

} // namespace iniphile_errors

iniphile_bridge::iniphile_bridge(std::string const path)
: src(path)
{
    std::ifstream input(path.c_str(), std::ios_base::binary);
    if (!input)
        throw iniphile_errors::stream_error(path);
    input.unsetf(std::ios::skipws);

    std::ostringstream erros;
    iniphile::parse_result cfg(
        iniphile::parse(input, erros)
    );
    if (!cfg)
        throw iniphile_errors::syntax_error(erros.str());

    afg = new iniphile::ast::node(iniphile::normalize(*cfg));
}

iniphile_bridge::~iniphile_bridge()
{
    if (afg)
        delete afg;
}

std::string const
iniphile_bridge::path()
{
    return src;
}

template<class T>
T
iniphile_bridge::get(std::string const query, T dflt)
{
    return iniphile::get(*afg, query, dflt);
}

template
std::vector<std::string>
iniphile_bridge::get< std::vector<std::string> >(std::string const query, std::vector<std::string> dflt);

template
std::string
iniphile_bridge::get<std::string>(std::string const query, std::string dflt);

template
bool
iniphile_bridge::get<bool>(std::string const query, bool dflt);

template
long
iniphile_bridge::get<long>(std::string const query, long dflt);

template
double
iniphile_bridge::get<double>(std::string const query, double dflt);

