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
#include <boost/python/dict.hpp>
#include <Pegasus/Common/CIMProperty.h>
#include <Pegasus/Common/CIMQualifier.h>
#include <Pegasus/Common/CIMMethod.h>
#include "obj/lmiwbem_nocasedict.h"
#include "obj/cim/lmiwbem_class.h"
#include "obj/cim/lmiwbem_method.h"
#include "obj/cim/lmiwbem_property.h"
#include "obj/cim/lmiwbem_qualifier.h"
#include "util/lmiwbem_convert.h"
#include "util/lmiwbem_util.h"

CIMClass::CIMClass()
    : m_classname()
    , m_super_classname()
    , m_properties()
    , m_qualifiers()
    , m_methods()
    , m_rc_class_properties()
    , m_rc_class_qualifiers()
    , m_rc_class_methods()
{
}

CIMClass::CIMClass(
    const bp::object &classname,
    const bp::object &properties,
    const bp::object &qualifiers,
    const bp::object &methods,
    const bp::object &superclass)
{
    m_classname = StringConv::asStdString(classname, "classname");
    m_super_classname = StringConv::asStdString(superclass, "superclass");
    m_properties = Conv::get<NocaseDict, bp::dict>(properties, "properties");
    m_qualifiers = Conv::get<NocaseDict, bp::dict>(qualifiers, "qualifiers");
    m_methods = Conv::get<NocaseDict, bp::dict>(methods, "methods");
}

void CIMClass::init_type()
{
    CIMBase<CIMClass>::init_type(bp::class_<CIMClass>("CIMClass", bp::init<>())
        .def(bp::init<
            const bp::object &,
            const bp::object &,
            const bp::object &,
            const bp::object &,
            const bp::object &>((
                bp::arg("classname"),
                bp::arg("properties") = NocaseDict::create(),
                bp::arg("qualifiers") = NocaseDict::create(),
                bp::arg("methods") = NocaseDict::create(),
                bp::arg("superclass") = std::string()),
                "Constructs a :py:class:`.CIMClass`.\n\n"
                ":param str classname: String containing class name\n"
                ":param NocaseDict properties: Dictionary containing properties\n"
                "\tof the class\n"
                ":param NocaseDict qualifiers: Dictionary containing qualifiers\n"
                "\tof the class\n"
                ":param NocaseDict methods: Dictionary containing methods of the class\n"
                ":param str superclass: String containing the name of super-class"))
#  if PY_MAJOR_VERSION < 3
        .def("__cmp__", &CIMClass::cmp)
#  else
        .def("__eq__", &CIMClass::eq)
        .def("__gt__", &CIMClass::gt)
        .def("__lt__", &CIMClass::lt)
        .def("__ge__", &CIMClass::ge)
        .def("__le__", &CIMClass::le)
#  endif // PY_MAJOR_VERSION
        .def("__repr__", &CIMClass::repr,
            ":returns: pretty string of the object")
        .def("copy", &CIMClass::copy,
            "copy()\n\n"
            ":returns: copy of the object itself\n"
            ":rtype: :py:class:`.CIMClass`")
        .add_property("classname",
            &CIMClass::getPyClassname,
            &CIMClass::setPyClassname,
            "Property storing class name.\n\n"
            ":rtype: unicode")
        .add_property("superclass",
            &CIMClass::getPySuperClassname,
            &CIMClass::setPySuperClassname,
            "Property storing super-class name.\n\n"
            ":rtype: unicode")
        .add_property("properties",
            &CIMClass::getPyProperties,
            &CIMClass::setPyProperties,
            "Property storing properties.\n\n"
            ":rtype: :py:class:`.NocaseDict`")
        .add_property("qualifiers",
            &CIMClass::getPyQualifiers,
            &CIMClass::setPyQualifiers,
            "Property storing qualifiers.\n\n"
            ":rtype: :py:class:`.NocaseDict`")
        .add_property("methods",
            &CIMClass::getPyMethods,
            &CIMClass::setPyMethods,
            "Property storing methods.\n\n"
            ":rtype: :py:class:`.NocaseDict`"));
}

bp::object CIMClass::create(const Pegasus::CIMClass &cls)
{
    bp::object inst = CIMBase<CIMClass>::create();
    CIMClass &fake_this = CIMClass::asNative(inst);

    // Store list of properties for lazy evaluation
    fake_this.m_rc_class_properties.set(std::list<Pegasus::CIMConstProperty>());
    Pegasus::Uint32 cnt = cls.getPropertyCount();
    for (Pegasus::Uint32 i = 0; i < cnt; ++i)
        fake_this.m_rc_class_properties.get()->push_back(cls.getProperty(i));

    // Store list of qualifiersr for lazy evaluation
    fake_this.m_rc_class_qualifiers.set(std::list<Pegasus::CIMConstQualifier>());
    cnt = cls.getQualifierCount();
    for (Pegasus::Uint32 i = 0; i < cnt; ++i)
        fake_this.m_rc_class_qualifiers.get()->push_back(cls.getQualifier(i));

    // Store list of methods for lazy evaluation
    fake_this.m_rc_class_methods.set(std::list<Pegasus::CIMConstMethod>());
    cnt = cls.getMethodCount();
    for (Pegasus::Uint32 i = 0; i < cnt; ++i)
        fake_this.m_rc_class_methods.get()->push_back(cls.getMethod(i));

    fake_this.m_classname = cls.getClassName().getString().getCString();
    fake_this.m_super_classname = cls.getSuperClassName().getString().getCString();

    return inst;
}

bp::object CIMClass::create(const Pegasus::CIMObject &object)
{
    return create(Pegasus::CIMClass(object));
}

Pegasus::CIMClass CIMClass::asPegasusCIMClass()
{
    Pegasus::CIMClass cls(
        Pegasus::CIMName(m_classname.c_str()),
        Pegasus::CIMName(m_super_classname.c_str()));

    // Add all the properties
    const NocaseDict &properties = NocaseDict::asNative(getPyProperties());
    nocase_map_t::const_iterator it;
    for (it = properties.begin(); it != properties.end(); ++it) {
        CIMProperty &property = CIMProperty::asNative(it->second);
        cls.addProperty(property.asPegasusCIMProperty());
    }

    // Add all the qualifiers
    const NocaseDict &qualifiers = NocaseDict::asNative(getPyQualifiers());
    for (it = qualifiers.begin(); it != qualifiers.end(); ++it) {
        CIMQualifier &qualifier = CIMQualifier::asNative(it->second);
        cls.addQualifier(qualifier.asPegasusCIMQualifier());
    }

    // Add all the methods
    const NocaseDict &methods = NocaseDict::asNative(getPyMethods());
    for (it = methods.begin(); it != methods.end(); ++it) {
        CIMMethod &method = CIMMethod::asNative(it->second);
        cls.addMethod(method.asPegasusCIMMethod());
    }

    return cls;
}

#  if PY_MAJOR_VERSION < 3
int CIMClass::cmp(const bp::object &other)
{
    if (!isinstance(other, CIMClass::type()))
        return 1;

    CIMClass &other_class = CIMClass::asNative(other);

    int rval;
    if ((rval = m_classname.compare(other_class.m_classname)) != 0 ||
        (rval = m_super_classname.compare(other_class.m_super_classname)) != 0 ||
        (rval = compare(getPyProperties(), other_class.getPyProperties())) != 0 ||
        (rval = compare(getPyQualifiers(), other_class.getPyQualifiers())) != 0 ||
        (rval = compare(getPyMethods(), other_class.getPyMethods())) != 0)
    {
        return rval;
    }

    return 0;
}
#  else
bool CIMClass::eq(const bp::object &other)
{
    if (!isinstance(other, CIMClass::type()))
        return false;

    CIMClass &other_class = CIMClass::asNative(other);

    return m_classname == other_class.m_classname &&
        m_super_classname == other_class.m_super_classname &&
        compare(getPyProperties(), other_class.getPyProperties(), Py_EQ) &&
        compare(getPyQualifiers(), other_class.getPyQualifiers(), Py_EQ) &&
        compare(getPyMethods(), other_class.getPyMethods(), Py_EQ);
}

bool CIMClass::gt(const bp::object &other)
{
    if (!isinstance(other, CIMClass::type()))
        return false;

    CIMClass &other_class = CIMClass::asNative(other);

    return m_classname > other_class.m_classname ||
        m_super_classname > other_class.m_super_classname ||
        compare(getPyProperties(), other_class.getPyProperties(), Py_GT) ||
        compare(getPyQualifiers(), other_class.getPyQualifiers(), Py_GT) ||
        compare(getPyMethods(), other_class.getPyMethods(), Py_GT);
}

bool CIMClass::lt(const bp::object &other)
{
    if (!isinstance(other, CIMClass::type()))
        return false;

    CIMClass &other_class = CIMClass::asNative(other);

    return m_classname < other_class.m_classname ||
        m_super_classname < other_class.m_super_classname ||
        compare(getPyProperties(), other_class.getPyProperties(), Py_LT) ||
        compare(getPyQualifiers(), other_class.getPyQualifiers(), Py_LT) ||
        compare(getPyMethods(), other_class.getPyMethods(), Py_LT);
}

bool CIMClass::ge(const bp::object &other)
{
    return gt(other) || eq(other);
}

bool CIMClass::le(const bp::object &other)
{
    return lt(other) || eq(other);
}
#  endif // PY_MAJOR_VERSION

bp::object CIMClass::repr()
{
    std::stringstream ss;
    ss << "CIMClass(classname='" << m_classname << "', ...)";
    return StringConv::asPyUnicode(ss.str());
}

bp::object CIMClass::copy()
{
    bp::object obj = CIMBase<CIMClass>::create();
    CIMClass &cls = CIMClass::asNative(obj);
    NocaseDict &properties = NocaseDict::asNative(getPyProperties());
    NocaseDict &qualifiers = NocaseDict::asNative(getPyQualifiers());
    NocaseDict &methods    = NocaseDict::asNative(getPyMethods());

    cls.m_classname = m_classname;
    cls.m_super_classname = m_super_classname;
    cls.m_properties = properties.copy();
    cls.m_qualifiers = qualifiers.copy();
    cls.m_methods    = methods.copy();

    return obj;
}

std::string CIMClass::getClassname() const
{
    return m_classname;
}

std::string CIMClass::getSuperClassname() const
{
    return m_super_classname;
}

bp::object CIMClass::getPyClassname() const
{
    return StringConv::asPyUnicode(m_classname);
}

bp::object CIMClass::getPySuperClassname() const
{
    return StringConv::asPyUnicode(m_super_classname);
}

bp::object CIMClass::getPyProperties()
{
    if (!m_rc_class_properties.empty()) {
        m_properties = NocaseDict::create();
        std::list<Pegasus::CIMConstProperty>::const_iterator it;
        std::list<Pegasus::CIMConstProperty> &properties = *m_rc_class_properties.get();

        for (it = properties.begin(); it != properties.end(); ++it)
            m_properties[bp::object(it->getName())] = CIMProperty::create(*it);

        m_rc_class_properties.release();
    }

    return m_properties;
}

bp::object CIMClass::getPyQualifiers()
{
    if (!m_rc_class_qualifiers.empty()) {
        m_qualifiers = NocaseDict::create();
        std::list<Pegasus::CIMConstQualifier>::const_iterator it;
        std::list<Pegasus::CIMConstQualifier> &qualifiers = *m_rc_class_qualifiers.get();

        for (it = qualifiers.begin(); it != qualifiers.end(); ++it)
            m_qualifiers[bp::object(it->getName())] = CIMQualifier::create(*it);

        m_rc_class_qualifiers.release();
    }

    return m_qualifiers;
}

bp::object CIMClass::getPyMethods()
{
    if (!m_rc_class_methods.empty()) {
        m_methods = NocaseDict::create();
        std::list<Pegasus::CIMConstMethod>::const_iterator it;
        std::list<Pegasus::CIMConstMethod> &methods = *m_rc_class_methods.get();

        for (it = methods.begin(); it != methods.end(); ++it)
            m_methods[bp::object(it->getName())] = CIMMethod::create(*it);

        m_rc_class_methods.release();
    }

    return m_methods;
}

void CIMClass::setClassname(const std::string &classname)
{
    m_classname = classname;
}

void CIMClass::setSuperClassname(const std::string &super_classname)
{
    m_super_classname = super_classname;
}

void CIMClass::setPyClassname(const bp::object &classname)
{
    m_classname = StringConv::asStdString(classname, "classname");
}

void CIMClass::setPySuperClassname(const bp::object &super_classname)
{
    m_super_classname = StringConv::asStdString(super_classname, "superclass");
}

void CIMClass::setPyProperties(const bp::object &properties)
{
    m_properties = Conv::get<NocaseDict, bp::dict>(properties, "properties");

    // Unref cached resource, it will never be used
    m_rc_class_properties.release();
}

void CIMClass::setPyQualifiers(const bp::object &qualifiers)
{
    m_qualifiers = Conv::get<NocaseDict, bp::dict>(qualifiers, "qualifiers");

    // Unref cached resource, it will never be used
    m_rc_class_qualifiers.release();
}

void CIMClass::setPyMethods(const bp::object &methods)
{
    m_methods = Conv::get<NocaseDict, bp::dict>(methods, "methods");

    // Unref cached resource, it will never be used
    m_rc_class_methods.release();
}
