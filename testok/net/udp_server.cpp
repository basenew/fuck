#include <sys/types.h>
#include <sys/socket.h>
#include <sys/error.h>

enum ERR_CODE
{
	ERR_OK,
	ERR_FAIL,
	ERR_INVALID_FD,
	ERR_INVALID_PORT,
};

class Task
{
public:
	Task();
	~Task();
	bool run();
	bool stop();
	virtual void thread_loop() = 0;

private:
	bool running_;
};

class UdpSvr:Task
{
public:
	UdpSvr(unsigned short port);
	~UdpSvr();

	virtual void thread_loop() = 0;
private:
	bool create_udp_server_fd_();

private:
	int fd_;
};

struct ScokAddr
{
	string ip;
	unsigned short port;
};


class SockUtils
{
public:
	static int create_tcp_socket(int& fd); 
	static int create_udp_socket(int& fd); 
	static int create_tcp_svr_socket(int& fd); 
	static int create_udp_svr_socket(int& fd); 
	static int bind(int fd, const string& ip, unsigned short port); 
	static int reuse(int fd); 
	static int listen(int fd); 
	static int accept(int fd, ScokAddr& client_addr); 
};

int SockUtils::create_udp_socket(int& fd)
{
	fd = socket(AF_INET, SOCK_DGRAM, 0);
	if (fd == -1)
	{
		perror("create udp socket fail");
		return err_no;
	}

	return ERR_OK;
}

int SockUtils::bind(int fd, const string& ip, unsigned short port); 
{
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.port = htons(port);
	addr.sin_addr.s_addr = ip.empty()?htonl(0):inet_addr(ip.c_str());
	int ret = bind(fd, sockaddr*(&addr), sizeof(addr));
	if (ret != 0)
	{
		perror("create udp socket fail");
		return err_no;
	}

	return ERR_OK;
}
int SockUtils::create_udp_server_fd(int& fd, unsigned short port)
{
	if (fd <= 0) return ERR_INVALID_FD;
	if (port == 0) return ERR_INVALID_PORT;

	int ret;
	ret = create_udp_socket(fd);
	if (retl != ERR_OK) return ret;
	
	ret = bind(fd, "", port);
	if (ret != ERR_OK) return ret;

	return ERR_OK;
}

int main(int argc, char** argv)
{
	return 0;
}
