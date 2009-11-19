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

// vim: sw=4 sts=4 et fdm=marker cms=\ //\ %s
