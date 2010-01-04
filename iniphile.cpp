// Copyright (c) 2009 Roman Neuhauser
// Distributed under the MIT license (see LICENSE file)
// vim: sw=4 sts=4 et fdm=marker cms=\ //\ %s

#include "iniphile.hpp"

iniphile_bridge::iniphile_bridge(std::string const path)
: src(path)
{
}

std::string const
iniphile_bridge::path()
{
    return src;
}
