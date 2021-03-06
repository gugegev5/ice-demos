// **********************************************************************
//
// Copyright (c) 2003-2015 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include <Ice/Ice.h>

using namespace std;

namespace
{

class LocatorRegistryI : public Ice::LocatorRegistry
{
public:

    LocatorRegistryI()
    {
    }

    virtual void
    setAdapterDirectProxy_async(string id, shared_ptr<Ice::ObjectPrx> proxy,
                                function<void ()> response,
                                function<void (exception_ptr)>,
                                const Ice::Current&)
    {
        if(!proxy)
        {
            _adapters.erase(id);
        }
        else
        {
            _adapters[id] = proxy;
        }
        response();
    }

    virtual void
    setReplicatedAdapterDirectProxy_async(string, string, shared_ptr<Ice::ObjectPrx>, function<void ()> response,
                                          function<void (exception_ptr)>, const Ice::Current&)
    {
        assert(false); // Not used by this demo
        response();
    }

    virtual void
    setServerProcessProxy_async(string, shared_ptr<Ice::ProcessPrx>, function<void ()> response,
                                function<void (exception_ptr)>, const Ice::Current&)
    {
        assert(false); // Not used by this demo
        response();
    }

    shared_ptr<Ice::ObjectPrx>
    getAdapter(const string& id)
    {
        auto p = _adapters.find(id);
        if(p == _adapters.end())
        {
            throw Ice::AdapterNotFoundException();
        }
        return p->second;
    }

private:

    map<string, shared_ptr<Ice::ObjectPrx>> _adapters;
};
typedef IceInternal::Handle<LocatorRegistryI> LocatorRegistryIPtr;

class LocatorI : public Ice::Locator
{
public:

    LocatorI(shared_ptr<LocatorRegistryI> registry,
             shared_ptr<Ice::LocatorRegistryPrx> registryPrx) :
        _registry(move(registry)),
        _registryPrx(move(registryPrx))
    {
    }

    virtual void
    findObjectById_async(Ice::Identity,
                         function<void (const shared_ptr<Ice::ObjectPrx>&)> response,
                         function<void (exception_ptr)>,
                         const Ice::Current&) const
    {
        response(nullptr);
    }


    virtual void
    findAdapterById_async(string id,
                          function<void (const shared_ptr<Ice::ObjectPrx>&)> response,
                          function<void (exception_ptr)>,
                          const Ice::Current&) const
    {
        response(_registry->getAdapter(id));
    }

    virtual shared_ptr<Ice::LocatorRegistryPrx>
    getRegistry(const Ice::Current&) const
    {
        return _registryPrx;
    }

private:

    const shared_ptr<LocatorRegistryI> _registry;
    const shared_ptr<Ice::LocatorRegistryPrx> _registryPrx;
};

}

class LocatorServer : public Ice::Application
{
public:

    virtual int run(int, char*[]);
};

int
main(int argc, char* argv[])
{
    LocatorServer app;
    return app.main(argc, argv, "config.locator");
}

int
LocatorServer::run(int argc, char*[])
{
    if(argc > 1)
    {
        cerr << appName() << ": too many arguments" << endl;
        return EXIT_FAILURE;
    }

    auto adapter = communicator()->createObjectAdapter("Locator");
    auto registry = make_shared<LocatorRegistryI>();

    auto registryPrx = Ice::uncheckedCast<Ice::LocatorRegistryPrx>(
        adapter->add(registry, communicator()->stringToIdentity("registry")));

    adapter->add(make_shared<LocatorI>(move(registry), move(registryPrx)),
                 communicator()->stringToIdentity("locator"));
    adapter->activate();
    communicator()->waitForShutdown();
    return EXIT_SUCCESS;
}
