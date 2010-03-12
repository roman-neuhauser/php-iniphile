// Copyright (c) 2009-2010 Roman Neuhauser
// Distributed under the MIT license (see LICENSE file)
// vim: sw=4 sts=4 et fdm=marker cms=\ //\ %s

#include "errors.hpp"

namespace iniphile_errors
{

template<class T>
error<T>::error(std::string const &msg)
: msg(msg)
{}

template <class T>
error<T>::~error() throw()
{}

template <class T>
char const *
error<T>::what() const throw()
{
    return msg.c_str();
}

template struct error<types::stream>;
template struct error<types::syntax>;

} // namespace iniphile_errors

