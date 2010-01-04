// Copyright (c) 2009 Roman Neuhauser
// Distributed under the MIT license (see LICENSE file)
// vim: sw=4 sts=4 et fdm=marker cms=\ //\ %s

#ifndef INIPHILE_HPP
#define INIPHILE_HPP

#include <string>

struct iniphile_bridge
{
    explicit
    iniphile_bridge(std::string const path);
    std::string const
    path();
private:
    std::string src;
};

#endif // INIPHILE_HPP
