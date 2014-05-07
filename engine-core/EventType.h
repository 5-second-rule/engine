#pragma once
#include "engine-core.h"
#include "Handle.h"

enum class COREDLL EventType{
	UPDATE,
	ACTION,
	REGISTER_PLAYER,
	REGISTER_PLAYER_RESPONSE,
	SPECIAL
};

enum class COREDLL Response{
	OK,
	FAIL
};

struct COREDLL RegistrationRequestHeader {
	unsigned int playerGuid;
	int responseTag;
};

struct COREDLL RegistrationResponseHeader {
	int responseTag;
	int response;
};