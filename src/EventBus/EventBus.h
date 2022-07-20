#ifndef EVENTBUS_H
#define EVENTBUS_H

#include <map>
#include <typeindex>
#include <list>

#include "../Logger/Logger.h"
#include "Event.h"

class IEventCallback {
private:
	virtual void call(Event& event) = 0;


public:
	virtual ~IEventCallback() = default;

	void execute(Event& event) {
		call(event);
	}

};

template <typename TOwner, typename TEvent> class EventCallback : public IEventCallback {
	// Callback function pointer that needs to be invoked
	private:
		typedef void (TOwner::*CallbackFunction)(TEvent&);

		TOwner* ownerInstance;
		CallbackFunction callbackFunction;

		virtual void call(Event& event) override {
			std::invoke(callbackFunction, ownerInstance, static_cast<TEvent&>(event));
		}

	public:
		EventCallback(TOwner* ownerInstance, CallbackFunction callbackFunction) {
			this->ownerInstance = ownerInstance;
			this->callbackFunction = callbackFunction;
		}

		virtual ~EventCallback() override = default;

};

typedef std::list<std::unique_ptr<IEventCallback>> HandlerList;

class EventBus {

private:
	std::map<std::type_index, std::unique_ptr<HandlerList>> subscribers;

public:
	EventBus() {
		Logger::info("EventBus constructor called!");
	}

	~EventBus() {
		Logger::info("EventBus destructor called!");
	}

	template<typename TEvent, typename TOwner>
	void subscribeToEvent(TOwner* ownerInstance, void (TOwner::*callbackFunction)(TEvent&)) {
		if (!subscribers[typeid(TEvent)].get()) {
			subscribers[typeid(TEvent)] = std::make_unique<HandlerList>();
		}

		auto subscriber = std::make_unique<EventCallback<TOwner, TEvent>>(ownerInstance, callbackFunction);
		subscribers[typeid(TEvent)]->push_back(std::move(subscriber));
	}

	template <typename TEvent, typename ...TArgs>
	void emit(TArgs&& ...args) {
		auto handlers = subscribers[typeid(TEvent)].get();
		if (handlers) {
			for (auto it = handlers->begin(); it != handlers->end(); it++) {
				auto handler = it->get();
				TEvent event(std::forward<TArgs>(args)...);
				handler->execute(event);
			}
		}
	}

	void reset() {
		subscribers.clear();
	}

};

#endif