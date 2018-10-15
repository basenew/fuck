#include "socketop.h"
int open_tcp(){
	int fd = socket(AF_INET,SOCK_STREAM, 0);
	if (fd == -1)
	{
		LOG_SYS << "create socket for listen err:" << get_errno_str() << endl;
		return -1;
	}
	return fd;	
}

int open_udp(){
	return -1;
}

int open_acceptor(const string& ip, ushort port){
	

}
		
bool set_reuse(int fd){
        int reuse = 1;
        if (0 != setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int)))
	{
		LOG_SYS << "bind socket err:" << get_errno_str() << endl;
		return false;	
	}
}

bool set_nonblock(int fd){
	return true;
}

bool set_rcv_buf_size(int fd, int size){
	return true;
}

bool set_snd_buf_size(int fd, int size){
	return true;
}

bool bind(int fd, const string& ip, ushort port){
	if (set_reuse(fd))
	{
		sockaddr_in addr;
        	bzero(&addr,sizeof(addr));
        	addr.sin_family = AF_INET;
        	addr.sin_addr.s_addr = inet_addr(ip.c_str());
        	addr.sin_port = htons(port);
		if (0 != bind(fd, (sockaddr*)&addr, sizeof(addr)))
		{
			close(fd);
			LOG_SYS << "bind socket err:" << get_errno_str() << endl;
			return false;
		}
		
		return true;
	}
	return false;
}

bool listen(int fd, int que_size){

}

