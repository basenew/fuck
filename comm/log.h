#ifndef __COMM_LOG_H__
#define __COMM_LOG_H__

#include "typedef.h"
#include "sys.h"
#include <fstream>
#include <mutex>
#include <thread>
#include <iomanip>
#include <ctime>
#include <iostream>
#include <sstream>
#include <unistd.h>
#include <pthread.h>

using namespace std;

namespace comm
{

	using pcchar = const char*;

	class Log:public ofstream
	{
		public:
			enum ELVL
			{
				NON,
				SYS,
				ERR,
				MSG,
				DBG,
				ALL,
			};

			enum EOUT
			{
				STATIC = 0,
				FILE = 1,
				STD = 2,
				SYSLOG = 4,
				NET = 8,

			};

			enum ESPLIT
			{
				HOUR,
				MINU,
				LINE,
				SIZE,
			};

			bool if_log(ELVL lvl){return lvl <= _lvl;}

			/*
			   bool config(int out = STD, const string& path = string(""), 
			   const string& name = string(""), bool safe = true, ESPLIT split = HOUR);
			   int print(pcchar msg, pcchar file_name = nullptr, int line = -1, 
			   pcchar func_name = nullptr, ELVL lvl = DBG, int outs = FILE);
			   template<class T>
			   Log& operator<<(const T& v)
			   {
			   _out << v;
			   return *this;
			   }
			*/
		private:
			ofstream _out;
			bool _safe;	
			long _size;
			long _line;
			string _path;
			string _name;
			int _lvl;
			int _eout;
			ESPLIT _split;
		private:
			Log():
				_safe(false),
				_size(0),
				_line(0),
				_lvl(DBG),
				_eout(STD|FILE),
				_split(HOUR)
			{
				gen_file_name();
				open(_path+_name, ios::out|ios::trunc);
				//open("log", ios::out|ios::trunc);
				if (!is_open()) cout << "open " << _path << _name << " err:" << get_errno_str() << endl;
			}

			~Log()
			{
				_out.close();
			}

			void gen_file_name()
			{
				if (_name.empty())
				{
					string date_time = get_time_string("_");	
					stringstream ss;
					ss << date_time << "_" << getpid() << ".log";
					ss >> _name;
					cout << "new name:" << _name << endl;
//					_name = "test.log";
				}
			}

		public:
			static Log& ins()
			{
				static Log log;
				return log;
			}
	};

}
#define LOGIF(L) if (Log::ins().if_log(L))Log::ins()<<get_time_string()<<" "<<__FILE__<<":"<<__LINE__ <<" "<<__FUNCTION__<<" "<< pthread_self()<< " "
#define LOG_DBG LOGIF(Log::DBG)
#define LOG_MSG LOGIF(Log::MSG)
#define LOG_ERR LOGIF(Log::ERR)
#define LOG_SYS LOGIF(Log::SYS)

/*
#define LOG_MSG(msg, size) Log::GetLog.print(msg, size, __FILE__, __LINE__, nullptr, ELVL::MSG)
#define LOG_ERR(msg, size) Log::GetLog.print(msg, size, __FILE__, __LINE__, nullptr, ELVL::ERR);
#define LOG_SYS(msg, size) Log::GetLog.print(msg, size, __FILE__, __LINE__, nullptr, ELVL::SYS);

#define LOG_DBGS(msg) Log::GetLog.print(msg.c_str(), msg.size(), __FILE__, __LINE__, nullptr);
#define LOG_MSGS(msg) Log::GetLog.print(msg.c_str(), msg.size(), __FILE__, __LINE__, nullptr, ELVL::MSG);
#define LOG_ERRS(msg) Log::GetLog.print(msg.c_str(), msg.size(), __FILE__, __LINE__, nullptr, ELVL::ERR);
#define LOG_SYSS(msg) Log::GetLog.print(msg.c_str(), msg.size(), __FILE__, __LINE__, nullptr, ELVL::SYS);
*/

#endif
