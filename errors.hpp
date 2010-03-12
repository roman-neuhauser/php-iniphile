// Copyright (c) 2009-2010 Roman Neuhauser
// Distributed under the MIT license (see LICENSE file)
// vim: sw=4 sts=4 et fdm=marker cms=\ //\ %s

#ifndef ERRORS_HPP
#define ERRORS_HPP

#include <string>
#include <exception>

namespace iniphile_errors
{

template<class T>
struct
error
: public virtual std::exception
{
    error(std::string const &msg);
    virtual ~error() throw();
    virtual char const *what() const throw();
private:
    std::string const msg;
};

namespace types
{
struct stream {};
struct syntax {};
}

typedef error<types::stream> stream_error;
typedef error<types::syntax> syntax_error;

} // namespace iniphile_errors

#endif // ERRORS_HPP
