/**
 * @file UMListener.h
 * a listener
 *
 * @author tori31001 at gmail.com
 *
 * Copyright (C) 2013 Kazuma Hatta
 * Licensed  under the MIT license. 
 *
 */
#pragma once
#include <memory>
#include <vector>
#include "UMEventType.h"

namespace burger
{

class UMListener;
typedef std::shared_ptr<UMListener> UMListenerPtr;
typedef std::weak_ptr<UMListener> UMListenerWeakPtr;

class UMAny;

/**
 * listener (observer)
 */
class UMListener
{
public:
	virtual void update(UMEventType event_type, UMAny& parameter) = 0;
};

} // burger
