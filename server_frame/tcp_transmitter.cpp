#include "tcp_transmitter.h"
#include "socketop.h"
#include "reactor.h"

#define HEAD_LEN 4

namespace comm
{

	tcp_transmitter::tcp_transmitter():_rcv_buf(1500), _snd_buf(1500)
	{

	}

	tcp_transmitter::~tcp_transmitter()
	{

	}

	int tcp_transmitter::open(int fd)
	{
		_fd = fd;
		on_read();
		return 0;
	}

	int tcp_transmitter::open(const string& remote_ip, ushort remote_port, int timeout_ms)
	{
		//todo
		return 0;
	}

	void tcp_transmitter::close()
	{

	}

	int tcp_transmitter::send(pcchar data, int size, int delay)
	{
		if (_snd_buf.data_size() > 0)
		{
			if (_snd_buf.space_size() < size)
				_snd_buf.resize((_snd_buf.data_size() + size)*2);

			memcpy(_snd_buf.space(), data, size);	
			_snd_buf.push(size);
		}
		else
		{
			int len = write(_fd, data, size);
			if (len < size)
			{
				if (_snd_buf.space_size() < (size - len))
					_snd_buf.resize(size * 2);
				
				memcpy(_snd_buf.space(), data + len, size - len);	
				_snd_buf.push(size - len);
				_reactor->add_handler(this, reactor::event::READ);
			}
		}
		return 0;
	}
	
	void tcp_transmitter::on_read()
	{
		int len = read(_fd, _rcv_buf.space(), _rcv_buf.space_size());
		LOG_DBG << "len:" << len<< endl;
		if (len > 0)
		{
			_rcv_buf.push(len);
			while (_rcv_buf.data_size() >= HEAD_LEN)
			{
				int pkg_len = *(int*)_rcv_buf.data();
				LOG_DBG << "pkg len:" << pkg_len<< endl;
				if (_rcv_buf.data_size() >= pkg_len + HEAD_LEN)
				{
					handle_data(_rcv_buf.data() + HEAD_LEN, pkg_len);
					_rcv_buf.pop(pkg_len + HEAD_LEN);
				}
				else
				{
					if (_rcv_buf.space_size() < pkg_len - _rcv_buf.data_size() + HEAD_LEN)
						_rcv_buf.resize((pkg_len + HEAD_LEN)
 * 2);
					break;	
				}
				_rcv_buf.adjust();
			}
		}
		else if (len == 0)
		{
			LOG_MSG << "len == 0" << endl;
			//on_disconnect();
			on_close();
		}
		else
		{
			if (errno != EAGAIN && errno != EINTR)
			{
				LOG_ERR << get_errno_str() << endl;
			}
		}
	}

	void tcp_transmitter::on_write()
	{

	}

	void tcp_transmitter::on_error()
	{

	}



}
