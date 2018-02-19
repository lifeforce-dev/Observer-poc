//---------------------------------------------------------------
//
// main.cpp
//

#include "Log.h"
#include "Observatory.h"

#include <memory>

class NetworkObserver
{
public:
	// Called when certain data has been updated so we can update any state we need to.
	virtual void OnUpdated() = 0;

	// Called when a message is received so we can handle it.
	virtual void OnMessageReceived(uint32_t type) = 0;

	// Called when NetworkController is about to be destroyed. Give us a chance to clean up any
	// state we might need to clean before the network controller is actually destroyed.
	virtual void OnNetControllerAboutToBeDestroyed() = 0;

	// Called when NetworkController has been destroyed. Maybe that means nothing to us because we've
	// already cleaned everything up, but still might be good to know.
	virtual void OnNetControllerDestroyed() = 0;
};

class NetController : public Observatory<NetworkObserver>
{
public:
	NetController::NetController()
	{
	}

	NetController::~NetController()
	{
		LOG_DEBUG("NetController destroyed.");
		NotifyObserversSafe([](NetworkObserver* o)
		{
			o->OnNetControllerDestroyed();
		});
	}

	void NetController::Update()
	{
		LOG_DEBUG("NetworkController data updated.");
		NotifyObservers([](NetworkObserver* o)
		{
			o->OnUpdated();
		});
	}

	void NetController::ReceiveMessage()
	{
		LOG_DEBUG("NetworkController message received.");
		uint32_t type = 0;
		NotifyObservers([type](NetworkObserver* o)
		{
			o->OnMessageReceived(type);
		});
	}

	void NetController::Shutdown()
	{
		LOG_DEBUG("NetController about to be destroyed.");
		NotifyObserversSafe([](NetworkObserver* o)
		{
			o->OnNetControllerAboutToBeDestroyed();
		});
	}
};

class ConfigProtoHandler : public NetworkObserver
{
public:
	ConfigProtoHandler(NetController* tester)
		: m_controller(tester)
	{
		m_controller->AddObserver(this);
	}

	virtual ~ConfigProtoHandler()
	{
	}

	virtual void OnUpdated() override
	{
		LOG_DEBUG("ConfigProtoHandler Updated.");
	}
	virtual void OnMessageReceived(uint32_t type) override
	{
		LOG_DEBUG("ConfigProtoHandler Message Received.");
	}
	virtual void OnNetControllerAboutToBeDestroyed() override
	{
		m_controller->RemoveObserver(this);
		m_controller = nullptr;
		LOG_DEBUG("ConfigProtoHandler Handling NetworkController About To Be Destroyed.");
	}
	virtual void OnNetControllerDestroyed() override
	{
		LOG_DEBUG("ConfigProtoHandler handling NetworkController being destroyed");
	}

private:
	NetController* m_controller;
};

class ActionProtoHandler : public NetworkObserver
{
public:
	ActionProtoHandler(NetController* tester)
		: m_controller(tester)
	{
		m_controller->AddObserver(this);
	}

	virtual ~ActionProtoHandler()
	{
	}

	virtual void OnUpdated() override
	{
		LOG_DEBUG("ActionProtoHandler Updated.");
	}
	virtual void OnMessageReceived(uint32_t type) override
	{
		
		LOG_DEBUG("ActionProtoHandler Message Received.");
	}
	virtual void OnNetControllerAboutToBeDestroyed() override
	{
		m_controller->RemoveObserver(this);
		m_controller = nullptr;
		LOG_DEBUG("ActionProtoHandler Handling NetworkController About To Be Destroyed.");
	}
	virtual void OnNetControllerDestroyed() override
	{
		LOG_DEBUG("ActionProtoHandler handling NetworkController being destroyed");
	}

private:
	NetController* m_controller;
};

class DataProtoHandler : public NetworkObserver
{
public:
	DataProtoHandler(NetController* tester)
		: m_controller(tester)
	{
		m_controller->AddObserver(this);
	}
	virtual ~DataProtoHandler()
	{
	}

	virtual void OnUpdated() override
	{
		LOG_DEBUG("DataProtoHandler Updated.");
	}
	virtual void OnMessageReceived(uint32_t type) override
	{
		LOG_DEBUG("DataProtoHandler Message Received.");
	}
	virtual void OnNetControllerAboutToBeDestroyed() override
	{
		m_controller->RemoveObserver(this);
		m_controller = nullptr;
		LOG_DEBUG("DataProtoHandler Handling NetworkController About To Be Destroyed.");
	}
	virtual void OnNetControllerDestroyed() override
	{
		LOG_DEBUG("DataProtoHandler handling NetworkController being destroyed");
	}

private:
	NetController* m_controller;
};

int main()
{
	{
		std::unique_ptr<NetController> controller{new NetController};
		DataProtoHandler datahandler(controller.get());
		ActionProtoHandler actionHandler(controller.get());
		ConfigProtoHandler configHandler(controller.get());

		controller->ReceiveMessage();
		controller->Update();
		controller->Shutdown();
	}

	return 0;
}
