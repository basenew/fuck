#pragma once
#include "comm.h"

namespace comm
{
class io_if
{
	public:
		virtual ~io_if() = 0;
		virtual int read(char* buf) = 0; 
};
}

