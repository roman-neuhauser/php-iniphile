// Copyright (c) 2009-2010 Roman Neuhauser
// Distributed under the MIT license (see LICENSE file)
// vim: sw=4 sts=4 et fdm=marker cms=\ //\ %s

#include <istream>
#include <sstream>

#include "iniphile.hpp"
#include "iniphile/input.hpp"
#include "iniphile/ast.hpp"
#include "iniphile/output.hpp"

#include "errors.hpp"

iniphile_bridge::iniphile_bridge(
    std::istream& input
  , std::string const &desc
)
{
    if (!input)
        throw iniphile_errors::stream_error(desc);
    input.unsetf(std::ios::skipws);

    std::ostringstream erros;
    iniphile::parse_result cfg(
        iniphile::parse(desc, input, erros)
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

template<class T>
T
iniphile_bridge::get(std::string const query, T dflt)
{
    return iniphile::get(*afg, query, dflt);
}

#define INIPHILE_BRIDGE_GET_SPEC(T) \
    template T iniphile_bridge::get< T >(std::string const query, T dflt)

INIPHILE_BRIDGE_GET_SPEC(std::vector<std::string>);
INIPHILE_BRIDGE_GET_SPEC(std::string);
INIPHILE_BRIDGE_GET_SPEC(bool);
INIPHILE_BRIDGE_GET_SPEC(long);
INIPHILE_BRIDGE_GET_SPEC(double);

