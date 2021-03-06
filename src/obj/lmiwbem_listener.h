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

#ifndef   LMIWBEM_LISTENER_H
#  define LMIWBEM_LISTENER_H

#  include <map>
#  include <Pegasus/Consumer/CIMIndicationConsumer.h>
#  include <Pegasus/Listener/CIMListener.h>
#  include <boost/python/object.hpp>
#  include <boost/shared_ptr.hpp>
#  include "obj/lmiwbem_cimbase.h"
#  include "obj/cim/lmiwbem_instance.h"

namespace bp = boost::python;

class CallableWithParams;
class CIMIndicationListener;

typedef std::map<
    std::string,
    std::list<CallableWithParams>
> handler_map_t;

class CallableWithParams
{
public:
    CallableWithParams() { }
    CallableWithParams(
        const bp::object &callable,
        const bp::object &args,
        const bp::object &kwds);
    CallableWithParams(const CallableWithParams &copy);

    void call(const bp::object &indication) const;

    CallableWithParams &operator=(const CallableWithParams &rhs);

private:
    bp::object m_callable;
    bp::object m_args;
    bp::object m_kwds;
};

class CIMIndicationConsumer: public Pegasus::CIMIndicationConsumer
{
public:
    CIMIndicationConsumer(CIMIndicationListener *listener);
    virtual ~CIMIndicationConsumer();

    void consumeIndication(
        const Pegasus::OperationContext &context,
        const Pegasus::String &url,
        const Pegasus::CIMInstance &indication);

private:
    CIMIndicationListener *m_listener;
};

class CIMIndicationListener: public CIMBase<CIMIndicationListener>
{
public:
    CIMIndicationListener(
        const bp::object &hostname,
        const bp::object &port,
        const bp::object &certfile,
        const bp::object &keyfile,
        const bp::object &trust_store);

    static void init_type();

    bp::object repr();

    void start(const bp::object &retries);
    void stop();

    bool getIsAlive() const;
    bool getUsesSSL() const;

    std::string getListenAddress() const;
    int getPort() const;
    bp::object getPyListenAddress() const;
    bp::object getPyPort() const;

    bp::object addPyHandler(const bp::tuple &args, const bp::dict &kwds);
    void removePyHandler(const bp::object &name);
    bp::object getPyHandlers() const;

private:
    friend class CIMIndicationConsumer;
    void call(
        const std::string &name,
        const bp::object &indication) const;

    boost::shared_ptr<Pegasus::CIMListener> m_listener;
    CIMIndicationConsumer m_consumer;

    handler_map_t m_handlers;

    Pegasus::Uint32 m_port;
    std::string m_listen_address;
    std::string m_certfile;
    std::string m_keyfile;
    std::string m_trust_store;
    Mutex m_mutex;          // A guard for m_terminating flag.
    bool m_terminating;
};

#endif // LMIWBEM_LISTENER_H
