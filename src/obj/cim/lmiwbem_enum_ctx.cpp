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
#include <boost/python/class.hpp>
#include "obj/cim/lmiwbem_enum_ctx.h"

CIMEnumerationContext::CIMEnumerationContext()
    : m_enum_ctx_ptr()
    , m_is_with_paths(true)
    , m_namespace()
{
}

void CIMEnumerationContext::init_type()
{
    CIMBase<CIMEnumerationContext>::init_type(
        bp::class_<CIMEnumerationContext, boost::noncopyable>("CIMEnumerationContext", bp::init<>())
        .def("__repr__", &CIMEnumerationContext::repr)
        .def("clear", &CIMEnumerationContext::clear));
}

bp::object CIMEnumerationContext::create(
    Pegasus::CIMEnumerationContext *ctx_ptr,
    const bool with_paths,
    const std::string &ns)
{
    bp::object inst(CIMBase<CIMEnumerationContext>::create());
    CIMEnumerationContext &fake_this = CIMEnumerationContext::asNative(inst);
    fake_this.m_enum_ctx_ptr.reset(ctx_ptr);
    fake_this.m_is_with_paths = with_paths;
    fake_this.m_namespace = ns;
    return inst;
}

bp::object CIMEnumerationContext::create(
    const boost::shared_ptr<Pegasus::CIMEnumerationContext> &ctx_ptr,
    const bool with_paths,
    const std::string &ns)
{
    bp::object inst(CIMBase<CIMEnumerationContext>::create());
    CIMEnumerationContext &fake_this = CIMEnumerationContext::asNative(inst);
    fake_this.m_enum_ctx_ptr = ctx_ptr;
    fake_this.m_is_with_paths = with_paths;
    fake_this.m_namespace = ns;
    return inst;
}

bp::object CIMEnumerationContext::repr()
{
    return StringConv::asPyUnicode(
        std::string("CIMEnumerationContext()"));
}

Pegasus::CIMEnumerationContext &CIMEnumerationContext::getPegasusContext()
{
    if (!m_enum_ctx_ptr) {
        std::cout << "We don't have a context!\n";

        // Just in case, we don't dereference a NULL pointer.
        m_enum_ctx_ptr.reset(new Pegasus::CIMEnumerationContext);
    }

    return *m_enum_ctx_ptr;
}

std::string CIMEnumerationContext::getNamespace() const
{
    return m_namespace;
}

bool CIMEnumerationContext::getIsWithPaths() const
{
    return m_is_with_paths;
}

void CIMEnumerationContext::setNamespace(const std::string &ns)
{
    m_namespace = ns;
}

void CIMEnumerationContext::setIsWithPaths(const bool is_with_paths)
{
    m_is_with_paths = is_with_paths;
}

void CIMEnumerationContext::clear()
{
    if (!m_enum_ctx_ptr)
        return;
    m_enum_ctx_ptr->clear();
}
