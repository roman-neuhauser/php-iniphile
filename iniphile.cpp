// Copyright (c) 2009 Roman Neuhauser
// Distributed under the MIT license (see LICENSE file)
// vim: sw=4 sts=4 et fdm=marker cms=\ //\ %s

#include <fstream>

#include "iniphile.hpp"
#include "iniphile/input.hpp"
#include "iniphile/ast.hpp"


iniphile_bridge::iniphile_bridge(std::string const path)
: src(path)
, open_(false)
{
    std::ifstream input(path, std::ios_base::binary);
    input.unsetf(std::ios::skipws);
    std::istreambuf_iterator<char> b(input), e;

    auto cfg = iniphile::parse(b, e, std::cerr);

    if (!cfg) {
        return;
    }
    afg = new iniphile::ast::node(iniphile::normalize(*cfg));
    open_ = !!input;
}

iniphile_bridge::~iniphile_bridge()
{
    delete afg;
}

bool
iniphile_bridge::is_open()
{
    return open_;
}

std::string const
iniphile_bridge::path()
{
    return src;
}

std::string const
iniphile_bridge::get_string(std::string const query)
{
    if (!open_) return "";
    return iniphile::get_string(*afg, query);
}
