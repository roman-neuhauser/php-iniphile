// Copyright (c) 2009 Roman Neuhauser
// Distributed under the MIT license (see LICENSE file)
// vim: sw=4 sts=4 et fdm=marker cms=\ //\ %s

#include <fstream>

#include "iniphile.hpp"
#include "iniphile/input.hpp"
#include "iniphile/ast.hpp"
#include "iniphile/output.hpp"


iniphile_bridge::iniphile_bridge(std::string const path)
: src(path)
{
    std::ifstream input(path.c_str(), std::ios_base::binary);
    input.unsetf(std::ios::skipws);

    iniphile::parse_result cfg(
        iniphile::parse(input, std::cerr)
    );

    if (!cfg) {
        return;
    }
    afg = new iniphile::ast::node(iniphile::normalize(*cfg));
}

iniphile_bridge::~iniphile_bridge()
{
    if (afg)
        delete afg;
}

bool
iniphile_bridge::is_open()
{
    return !!afg;
}

std::string const
iniphile_bridge::path()
{
    return src;
}

std::string
iniphile_bridge::get_string(std::string const query, std::string dflt)
{
    if (!afg) return dflt;
    return iniphile::get(*afg, query, dflt);
}

bool
iniphile_bridge::get_bool(std::string const query, bool dflt)
{
    if (!afg) return dflt;
    return iniphile::get(*afg, query, dflt);
}

long
iniphile_bridge::get_long(std::string const query, long dflt)
{
    if (!afg) return dflt;
    return iniphile::get(*afg, query, dflt);
}
