#pragma once
#include "engine-core.h"

#include "Handle.h"

class COREDLL NotificationDelegate {
public:
	virtual void newObject(Handle, int type) = 0;
	virtual void updatedObject(Handle, int type) = 0;
};