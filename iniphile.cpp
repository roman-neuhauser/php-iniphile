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

template<class T>
T
iniphile_bridge::get(std::string const query, T dflt)
{
    if (!afg) return dflt;
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

