// Copyright (c) 2009 Roman Neuhauser
// Distributed under the MIT license (see LICENSE file)
// vim: sw=4 sts=4 et fdm=marker cms=\ //\ %s

#include "iniphile.hpp"

iniphile::iniphile(std::string const path)
: src(path)
{
}

std::string const
iniphile::path()
{
    return src;
}
