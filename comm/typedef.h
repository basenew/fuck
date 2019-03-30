#ifndef __TYPEDEF_H__
#define __TYPEDEF_H__

namespace comm 
{

using ushort = unsigned short;
using byte   = unsigned char;
using uchar  = unsigned char;
using int8   = char;
using uint8  = unsigned char;
using int16  = short;
using uint16 = unsigned short;
using int32  = int;
using uint32 = unsigned int;
using int64  = long;
using uint64 = unsigned long;

using pbyte   = unsigned char*;
using pchar   = char*;
using puchar  = unsigned char*;
using pint8   = char*;
using puint8  = unsigned char*;
using pint16  = short*;
using puint16 = unsigned short*;
using pint32  = int*;
using puint32 = unsigned int*;
using pint64  = long*;
using puint64 = unsigned long*;

using pcchar   = const char*;
using pcbyte   = const unsigned char*;
using pcuchar  = const unsigned char*;
using pcint8   = const char*;
using pcuint8  = const unsigned char*;
using pcint16  = const short*;
using pcuint16 = const unsigned short*;
using pcint32  = const int*;
using pcuint32 = const unsigned int*;
using pcint64  = const long*;
using pcuint64 = const unsigned long*;

using fd = int;
using cid = uint32;

const int KB = 1024;
const int MB = KB*KB;
const int GB = KB*MB;
};

#endif
