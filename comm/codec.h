#pragma once
#include "comm.h"

namespace comm
{
class ICodec
{
public:
	virtual ~ICodec()=0;
	virtual void encode(const char* data, int len) = 0;
	virtual bool decode(const char* data, int len) = 0;
};
}
