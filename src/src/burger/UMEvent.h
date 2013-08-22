/**
 * @file UMEvent.h
 * a event
 *
 * @author tori31001 at gmail.com
 *
 * Copyright (C) 2013 Kazuma Hatta
 * Licensed  under the MIT license. 
 *
 */
#pragma once

#include "UMMacro.h"
#include "UMListener.h"
#include "UMAny.h"
#include "UMEventType.h"
#include <vector>

namespace burger
{

class UMEvent;
typedef std::shared_ptr<UMEvent> UMEventPtr;

typedef std::vector<UMEventPtr> UMEventList;

class UMEvent 
{
	DISALLOW_COPY_AND_ASSIGN(UMEvent);

public:
	typedef UMAny Parameter;
	
	UMEvent(UMEventType event_type)
		: key_(event_type),
		parameter_(0) {}

	UMEvent(UMEventType event_type, UMAny& umany)
		: key_(event_type),
		parameter_(umany) {}

	virtual ~UMEvent() {}

	void add_listener(UMListenerWeakPtr listener);
	void delete_listener(UMListenerWeakPtr listener);
	void notify();

	void set_parameter(UMAny& parameter) {
		parameter_ = parameter;
	}

private:
	typedef std::vector<UMListenerWeakPtr> ListenerList;
	
	const UMEventType key_;
	ListenerList listeners_;
	UMAny parameter_;
};

} // burger
