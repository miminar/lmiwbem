/* ***** BEGIN LICENSE BLOCK *****
 *
 *   Copyright (C) 2014, Peter Hatina <phatina@redhat.com>
 *
 *   This library is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU Lesser General Public License as
 *   published by the Free Software Foundation, either version 2.1 of the
 *   License, or (at your option) any later version.
 *
 *   This library is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *   GNU Lesser General Public License for more details.
 *
 *   You should have received a copy of the GNU Lesser General Public
 *   License along with this program; if not, write to the Free Software
 *   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 *   MA 02110-1301 USA
 *
 * ***** END LICENSE BLOCK ***** */

#include <config.h>
#include <sstream>
#include <boost/python/class.hpp>
#include <boost/python/dict.hpp>
#include "obj/cim/lmiwbem_class_name.h"
#include "util/lmiwbem_convert.h"
#include "util/lmiwbem_util.h"

CIMClassName::CIMClassName()
    : m_classname()
    , m_namespace()
    , m_hostname()
{
}

CIMClassName::CIMClassName(
    const bp::object &classname,
    const bp::object &namespace_,
    const bp::object &hostname)
{
    m_classname = StringConv::asStdString(classname, "classname");
    m_namespace = StringConv::asStdString(namespace_, "namespace");
    m_hostname  = StringConv::asStdString(hostname, "hostname");
}

void CIMClassName::init_type()
{
    CIMBase<CIMClassName>::init_type(
        bp::class_<CIMClassName>("CIMClassName", bp::init<>())
        .def(bp::init<
            const bp::object &,
            const bp::object &,
            const bp::object &>((
                bp::arg("classname"),
                bp::arg("host") = std::string(),
                bp::arg("namespace") = std::string()),
                "Constructs a :py:class:`.CIMClassName`.\n\n"
                ":param str classname: String containing class name\n"
                ":param str host: String containing host name\n"
                ":param str namespace: String containing namespace name"))
#  if PY_MAJOR_VERSION < 3
        .def("__cmp__", &CIMClassName::cmp)
#  else
        .def("__eq__", &CIMClassName::eq)
        .def("__gt__", &CIMClassName::gt)
        .def("__lt__", &CIMClassName::lt)
        .def("__ge__", &CIMClassName::ge)
        .def("__le__", &CIMClassName::le)
#  endif // PY_MAJOR_VERSION
        .def("__repr__", &CIMClassName::repr,
            ":returns: pretty string of the object")
        .def("copy", &CIMClassName::copy,
            "copy()\n\n"
            ":returns: copy of the object itself\n"
            ":rtype: :py:class:`.CIMClassName`")
        .add_property("classname",
            &CIMClassName::getPyClassname,
            &CIMClassName::setPyClassname,
            "Property storing class name.\n\n"
            ":rtype: unicode")
        .add_property("namespace",
            &CIMClassName::getPyNamespace,
            &CIMClassName::setPyNamespace,
            "Property storing namespace name.\n\n"
            ":rtype: unicode")
        .add_property("host",
            &CIMClassName::getPyHostname,
            &CIMClassName::setPyHostname,
            "Property storing host name.\n\n"
            ":rtype: unicode"));
}

bp::object CIMClassName::create(
    const std::string &classname_,
    const std::string &namespace_,
    const std::string &hostname)
{
    bp::object inst = CIMBase<CIMClassName>::create();
    CIMClassName &classname = CIMClassName::asNative(inst);

    classname.m_classname = classname_;
    classname.m_namespace = namespace_;
    classname.m_hostname  = hostname;

    return inst;
}

#  if PY_MAJOR_VERSION < 3
int CIMClassName::cmp(const bp::object &other)
{
    if (!isinstance(other, CIMClassName::type()))
        return 1;

    CIMClassName &other_classname = CIMClassName::asNative(other);

    int rval;
    if ((rval = m_classname.compare(other_classname.m_classname)) != 0 ||
        (rval = m_namespace.compare(other_classname.m_namespace)) != 0 ||
        (rval = m_hostname.compare(other_classname.m_hostname)) != 0)
    {
        return rval;
    }

    return 0;
}
#  else
bool CIMClassName::eq(const bp::object &other)
{
    if (!isinstance(other, CIMClassName::type()))
        return false;

    CIMClassName &other_classname = CIMClassName::asNative(other);

    return m_classname == other_classname.m_classname &&
        m_namespace == other_classname.m_namespace &&
        m_hostname  == other_classname.m_hostname;
}

bool CIMClassName::gt(const bp::object &other)
{
    if (!isinstance(other, CIMClassName::type()))
        return false;

    CIMClassName &other_classname = CIMClassName::asNative(other);

    return m_classname > other_classname.m_classname ||
        m_namespace > other_classname.m_namespace ||
        m_hostname  > other_classname.m_hostname;
}

bool CIMClassName::lt(const bp::object &other)
{
    if (!isinstance(other, CIMClassName::type()))
        return false;

    CIMClassName &other_classname = CIMClassName::asNative(other);

    return m_classname < other_classname.m_classname ||
        m_namespace < other_classname.m_namespace ||
        m_hostname  < other_classname.m_hostname;
}

bool CIMClassName::ge(const bp::object &other)
{
    return gt(other) || eq(other);
}

bool CIMClassName::le(const bp::object &other)
{
    return lt(other) || eq(other);
}
#  endif // PY_MAJOR_VERSION

bp::object CIMClassName::repr()
{
    std::stringstream ss;
    ss << "CIMClassName(classname='" << m_classname << '\'';
    if (!m_hostname.empty())
        ss << ", host='" << m_hostname << '\'';
    if (!m_namespace.empty())
        ss << ", namespace='" << m_namespace << '\'';
    ss << ')';
    return StringConv::asPyUnicode(ss.str());
}

bp::object CIMClassName::copy()
{
    bp::object obj = CIMBase<CIMClassName>::create();
    CIMClassName &classname = CIMClassName::asNative(obj);

    classname.m_classname = m_classname;
    classname.m_namespace = m_namespace;
    classname.m_hostname  = m_hostname;

    return obj;
}

std::string CIMClassName::getClassname() const
{
    return m_classname;
}

std::string CIMClassName::getNamespace() const
{
    return m_namespace;
}

std::string CIMClassName::getHostname() const
{
    return m_hostname;
}

bp::object CIMClassName::getPyClassname() const
{
    return StringConv::asPyUnicode(m_classname);
}

bp::object CIMClassName::getPyNamespace() const
{
    return StringConv::asPyUnicode(m_namespace);
}

bp::object CIMClassName::getPyHostname() const
{
    return StringConv::asPyUnicode(m_hostname);
}

void CIMClassName::setClassname(const std::string &classname)
{
    m_classname = classname;
}

void CIMClassName::setNamespace(const std::string &namespace_)
{
    m_namespace = namespace_;
}

void CIMClassName::setHostname(const std::string &hostname)
{
    m_hostname = hostname;
}

void CIMClassName::setPyClassname(const bp::object &classname)
{
    m_classname = StringConv::asStdString(classname, "classname");
}

void CIMClassName::setPyNamespace(const bp::object &namespace_)
{
    m_namespace = StringConv::asStdString(namespace_, "namespace");
}

void CIMClassName::setPyHostname(const bp::object &hostname)
{
    m_hostname = StringConv::asStdString(hostname, "hostname");
}
