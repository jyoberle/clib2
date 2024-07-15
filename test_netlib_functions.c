// Checks the netlib support functions
#include <stdarg.h>
#include <stdio.h>
#include <stdio_headers.h>
#include <exec/types.h>
#include <proto/exec.h>
#include <exec/tasks.h>
#include <proto/dos.h>
#include <devices/input.h>
#include <clib/alib_protos.h>
#include <locale_headers.h>
#include <stdlib_constructor.h>
#include <stdlib_headers.h>
#include <dirent.h>
#include <macros.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <ftw.h>
#include <inttypes.h>
#include <sys/mount.h>
#include <setjmp.h>
#include <sys/uio.h>
#include <ulimit.h>
#include <errno.h>
#include <sys/utsname.h>
#include <libgen.h>
#include <getopt.h>
#include <sys/resource.h>
#include <sys/systeminfo.h>
#include <termios.h>
#include <sys/timeb.h>
#include <socket_headers.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <grp.h>
#include <pwd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <getaddrinfo.h>

#define WEB_SITE "www.perdu.com"
#define USER_AGENT "Amiga"
#define HTTP_PORT 80
#define URL "/index.html"
#define NET_TEST_FILE "ram:bidule.bin"
#define HOST_EXAMPLE "44.135.86.12"
#define LNAOF_HOST_EXAMPLE "0.135.86.12"
#define NETOF_HOST_EXAMPLE "0.0.0.44"
#define LOCAL_HOST "127.0.0.1"
#define BUFFER_SIZE 256
#define MESSAGE_BUFFER_SIZE 2048
#define CHAT_PORT 8090
#define BSDSOCKET_LIBRARY_VERSION 3
#define CLIENT_STRING_TO_SEND "JOB loves Amiga\n"
#define SECOND_CLIENT_STRING_TO_SEND "And what about you?\n"
#define AMITCP_PASSWD_FILE "RAM:db/passwd"
#define AMITCP_PASSWD_FILE_FILL "amiga_user|*|1000|1000|Amiga User|RAM:|cli"
#define AMIGA_USER "amiga_user"
#define AMITCP_ASSIGN "AmiTCP"
#define RAM_DIR "RAM:"
#define GETPASS_PROMPT "password"
#define GROUP_NAME "group"
#define OPTION_TRACE_NETLIB 0

extern struct ExecBase *SysBase;
extern struct DosLibrary *DOSBase;

static char buffer1[BUFFER_SIZE],buffer2[BUFFER_SIZE];
static WORD errCnt;
struct Library *SocketBaseClient;

// We redefine some socket calls because the bsdsocket.library base cannot be shared between processes
#define socketClient(domain, type, protocol) ({ \
  LONG _socket_domain = (domain); \
  LONG _socket_type = (type); \
  LONG _socket_protocol = (protocol); \
  LONG _socket__re = \
  ({ \
  register struct Library * const __socket__bn __asm("a6") = (struct Library *) (SocketBaseClient); register int _d1 __asm("d1"); register int _a0 __asm("a0"); register int _a1 __asm("a1");\
  register LONG __socket__re __asm("d0"); \
  register LONG __socket_domain __asm("d0") = (_socket_domain); \
  register LONG __socket_type __asm("d1") = (_socket_type); \
  register LONG __socket_protocol __asm("d2") = (_socket_protocol); \
  __asm volatile ("jsr %%a6@(-30:W)" \
  : "=r" (_d1), "=r" (_a0), "=r" (_a1),  "=r"(__socket__re) \
  : "r"(__socket__bn), "r"(__socket_domain), "r"(__socket_type), "r"(__socket_protocol) \
  : "fp0", "fp1", "cc", "memory"); \
  __socket__re; \
  }); \
  _socket__re; \
})

#define connectClient(sock, name, namelen) ({ \
  LONG _connect_sock = (sock); \
  struct sockaddr * _connect_name = (name); \
  LONG _connect_namelen = (namelen); \
  LONG _connect__re = \
  ({ \
  register struct Library * const __connect__bn __asm("a6") = (struct Library *) (SocketBaseClient); register int _d1 __asm("d1"); register int _a0 __asm("a0"); register int _a1 __asm("a1");\
  register LONG __connect__re __asm("d0"); \
  register LONG __connect_sock __asm("d0") = (_connect_sock); \
  register struct sockaddr * __connect_name __asm("a0") = (_connect_name); \
  register LONG __connect_namelen __asm("d1") = (_connect_namelen); \
  __asm volatile ("jsr %%a6@(-54:W)" \
  : "=r" (_d1), "=r" (_a0), "=r" (_a1),  "=r"(__connect__re) \
  : "r"(__connect__bn), "r"(__connect_sock), "r"(__connect_name), "r"(__connect_namelen) \
  : "fp0", "fp1", "cc", "memory"); \
  __connect__re; \
  }); \
  _connect__re; \
})

#define sendClient(sock, buf, len, flags) ({ \
  LONG _send_sock = (sock); \
  APTR _send_buf = (buf); \
  LONG _send_len = (len); \
  LONG _send_flags = (flags); \
  LONG _send__re = \
  ({ \
  register struct Library * const __send__bn __asm("a6") = (struct Library *) (SocketBaseClient); register int _d1 __asm("d1"); register int _a0 __asm("a0"); register int _a1 __asm("a1");\
  register LONG __send__re __asm("d0"); \
  register LONG __send_sock __asm("d0") = (_send_sock); \
  register APTR __send_buf __asm("a0") = (_send_buf); \
  register LONG __send_len __asm("d1") = (_send_len); \
  register LONG __send_flags __asm("d2") = (_send_flags); \
  __asm volatile ("jsr %%a6@(-66:W)" \
  : "=r" (_d1), "=r" (_a0), "=r" (_a1),  "=r"(__send__re) \
  : "r"(__send__bn), "r"(__send_sock), "r"(__send_buf), "r"(__send_len), "r"(__send_flags) \
  : "fp0", "fp1", "cc", "memory"); \
  __send__re; \
  }); \
  _send__re; \
})

#define recvClient(sock, buf, len, flags) ({ \
  LONG _recv_sock = (sock); \
  APTR _recv_buf = (buf); \
  LONG _recv_len = (len); \
  LONG _recv_flags = (flags); \
  LONG _recv__re = \
  ({ \
  register struct Library * const __recv__bn __asm("a6") = (struct Library *) (SocketBaseClient); register int _d1 __asm("d1"); register int _a0 __asm("a0"); register int _a1 __asm("a1");\
  register LONG __recv__re __asm("d0"); \
  register LONG __recv_sock __asm("d0") = (_recv_sock); \
  register APTR __recv_buf __asm("a0") = (_recv_buf); \
  register LONG __recv_len __asm("d1") = (_recv_len); \
  register LONG __recv_flags __asm("d2") = (_recv_flags); \
  __asm volatile ("jsr %%a6@(-78:W)" \
  : "=r" (_d1), "=r" (_a0), "=r" (_a1),  "=r"(__recv__re) \
  : "r"(__recv__bn), "r"(__recv_sock), "r"(__recv_buf), "r"(__recv_len), "r"(__recv_flags) \
  : "fp0", "fp1", "cc", "memory"); \
  __recv__re; \
  }); \
  _recv__re; \
})

#define CloseSocketClient(sock) ({ \
  LONG _CloseSocket_sock = (sock); \
  LONG _CloseSocket__re = \
  ({ \
  register struct Library * const __CloseSocket__bn __asm("a6") = (struct Library *) (SocketBaseClient); register int _d1 __asm("d1"); register int _a0 __asm("a0"); register int _a1 __asm("a1");\
  register LONG __CloseSocket__re __asm("d0"); \
  register LONG __CloseSocket_sock __asm("d0") = (_CloseSocket_sock); \
  __asm volatile ("jsr %%a6@(-120:W)" \
  : "=r" (_d1), "=r" (_a0), "=r" (_a1),  "=r"(__CloseSocket__re) \
  : "r"(__CloseSocket__bn), "r"(__CloseSocket_sock) \
  : "fp0", "fp1", "cc", "memory"); \
  __CloseSocket__re; \
  }); \
  _CloseSocket__re; \
})

static VOID fcntCreateClient(void)
{
	int sockfd,connfd;
	struct sockaddr_in servaddr;
    int n;

#if OPTION_TRACE_NETLIB
	printf("Client waiting server start...\n");
#endif

	Delay(200); // wait for server start

	// Open the library for the client, since its base cannot be shared accross processes
	if((SocketBaseClient = OpenLibrary("bsdsocket.library",BSDSOCKET_LIBRARY_VERSION)) == NULL)
	{
		errCnt++;
	}
	else
	{
#if OPTION_TRACE_NETLIB
		printf("Client opened bsdsocket.library\n");
#endif
		if((sockfd = socketClient(AF_INET,SOCK_STREAM,0)) < 0)
		{
			errCnt++;
		}
		else
		{
#if OPTION_TRACE_NETLIB
			printf("Client created socket\n");
#endif
			bzero(&servaddr,sizeof(servaddr));	
			servaddr.sin_family = AF_INET;
			servaddr.sin_addr.s_addr = inet_addr(LOCAL_HOST);
			servaddr.sin_port = htons(CHAT_PORT);
		
			// Connect the client socket to server socket
			if(connectClient(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr)) < 0) 
			{
				errCnt++;
			}
			else
			{
#if OPTION_TRACE_NETLIB
				printf("Client connected to the the server socket\n");
#endif
				if(sendClient(sockfd,CLIENT_STRING_TO_SEND,strlen(CLIENT_STRING_TO_SEND),0) < 0)
				{
					errCnt++;	
				}
				else
				{
#if OPTION_TRACE_NETLIB
					printf("Client sent message to the the server\n");
#endif

					if(recvClient(sockfd,buffer1,BUFFER_SIZE,0) < 0)
					{
						errCnt++;
					}
					else
					{
#if OPTION_TRACE_NETLIB
						printf("Client received message back from the server : %s\n",buffer1);
#endif

						if(sendClient(sockfd,SECOND_CLIENT_STRING_TO_SEND,strlen(SECOND_CLIENT_STRING_TO_SEND),0) < 0)
						{
							errCnt++;	
						}
						else
						{
#if OPTION_TRACE_NETLIB
							printf("Client sent second message to the the server\n");
#endif

							if(recvClient(sockfd,buffer1,BUFFER_SIZE,0) < 0)
							{
								errCnt++;
							}
							else
							{
#if OPTION_TRACE_NETLIB
								printf("Client received second message back from the server : %s\n",buffer1);
#endif
							}
						}
					}
				}
			}

			if(CloseSocketClient(sockfd) == -1)
			{
				errCnt++;
			}
			else
			{
#if OPTION_TRACE_NETLIB
				printf("Client closed socket\n");
#endif				
			}
		}

		CloseLibrary(SocketBaseClient);

#if OPTION_TRACE_NETLIB
		printf("Client closed bsdsocket.library\n");
#endif	
	}
}

// Test setsockopt, bind, listen, accept, recvfrom, sendto, recvmsg, sendmsg, shutdown, getsockname, getsockopt
static void fcntCreateServer(void)
{
	int sockfd,connfd;
	LONG len;
    struct sockaddr_in servaddr,cli;
    int n;
	const int enable = 1;
	struct msghdr msg;
	struct iovec vec;
	struct sockaddr address;
	socklen_t address_len;
	int option_value;
	socklen_t option_len;

    if((sockfd = socket(AF_INET,SOCK_STREAM,0)) < 0)
	{
        errCnt++;
    }
    else
	{
#if OPTION_TRACE_NETLIB
        printf("Server socket successfully created\n");
#endif

		if(setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&enable,sizeof(int)) < 0)
		{
			errCnt++;
		}
		else
		{
#if OPTION_TRACE_NETLIB
        	printf("Server set options\n");
#endif
			option_len = sizeof(option_value);
			if(getsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,(void *)&option_value,&option_len) == ERROR) errCnt++;
			bzero(&servaddr, sizeof(servaddr));
	
			// assign IP, PORT
			servaddr.sin_family = AF_INET;
			servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
			servaddr.sin_port = htons(CHAT_PORT);
		
			// Binding newly created socket to given IP and verification
			if ((bind(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr))) < 0)
			{
				errCnt++;
			}
			else
			{
#if OPTION_TRACE_NETLIB
				printf("Server socket successfully binded\n");
#endif
				address_len = sizeof(address);
			
				if(getsockname(sockfd,&address,&address_len) == ERROR)
					errCnt++;

				if ((listen(sockfd,1)) < 0) {
					errCnt++;
				}
				else
				{
#if OPTION_TRACE_NETLIB
					printf("Server listening...\n");
#endif
					len = sizeof(cli);
					connfd = accept(sockfd,(struct sockaddr*)&cli,&len);

					if (connfd < 0)
					{
						errCnt++;
					}
					else
					{
#if OPTION_TRACE_NETLIB
						printf("Server accepted the client\n");
#endif

						bzero(buffer2,BUFFER_SIZE);
#if OPTION_TRACE_NETLIB
						printf("Server waiting message...\n");
#endif
					
						if(recvfrom(connfd,buffer2,sizeof(buffer2),0,NULL,NULL) < 0)
						{
							errCnt++;
						}
						else
						{
#if OPTION_TRACE_NETLIB
							printf("Message from client: %s",buffer2);
#endif

							if(sendto(connfd,buffer2,strlen(buffer2),0,NULL,0) == ERROR)
							{
								errCnt++;
							}
							else
							{
#if OPTION_TRACE_NETLIB
								printf("Server send message back to client: %s",buffer2);
#endif							
							}

							bzero(buffer2,BUFFER_SIZE);
							msg.msg_name = NULL;
							msg.msg_namelen = 0;
							msg.msg_iov = &vec;
							msg.msg_iovlen = 1;
							msg.msg_control = NULL;
							msg.msg_controllen = 0;
							msg.msg_flags = 0;
							vec.iov_base = buffer2;
							vec.iov_len = BUFFER_SIZE;

							if(recvmsg(connfd,&msg,0) < 0)
							{
								errCnt++;
							}
							else
							{
#if OPTION_TRACE_NETLIB
								printf("Second message from client: %s",buffer2);
#endif
								msg.msg_name = NULL;
								msg.msg_namelen = 0;
								msg.msg_iov = &vec;
								msg.msg_iovlen = 1;
								msg.msg_control = NULL;
								msg.msg_controllen = 0;
								msg.msg_flags = 0;
								vec.iov_base = buffer2;
								vec.iov_len = BUFFER_SIZE;
								
								if(sendmsg(connfd,&msg,0) < 0)
								{
									errCnt++;
								}
								else
								{
#if OPTION_TRACE_NETLIB
									printf("Server send second message back to client: %s",buffer2);
#endif										
									Delay(200); // wait for client socket close
									shutdown(sockfd,2); // 2 = SHUT_RDWR
#if OPTION_TRACE_NETLIB
									printf("Server socket shutdown\n");
#endif							
								}
							}
						}
					}
				}
			}
		}

    	if(close(sockfd) == ERROR)
		{
			errCnt++;
		}
		else
		{
#if OPTION_TRACE_NETLIB
			printf("Server socket closed\n");
#endif				
		}
	}
}

WORD testNetlibFunctions(void)
{
	errCnt = 0;

	// Test inet_addr, inet_ntoa, inet_aton, inet_lnaof, inet_netof
	struct in_addr in;

	if((in.s_addr = inet_addr(HOST_EXAMPLE)) == INADDR_NONE)
	{
		errCnt++;
	}
	else
	{
		if(strcmp(inet_ntoa(in),HOST_EXAMPLE))
			errCnt++;
	}

	if((in.s_addr = inet_network(HOST_EXAMPLE)) == INADDR_NONE)
	{
		errCnt++;
	}
	else
	{
		if(strcmp(inet_ntoa(in),HOST_EXAMPLE))
			errCnt++;
	}

	if(inet_aton(HOST_EXAMPLE,&in) == 0)
	{
		errCnt++;
	}
	else
	{
		if(strcmp(inet_ntoa(in),HOST_EXAMPLE))
			errCnt++;
	}

	// Test hstrerror
	if(hstrerror(HOST_NOT_FOUND) == NULL) errCnt++;

#if 0 // Inet_MakeAddr is not supported
	in = inet_makeaddr(inet_addr(NETOF_HOST_EXAMPLE),inet_addr(LNAOF_HOST_EXAMPLE));

	if(strcmp(inet_ntoa(in),HOST_EXAMPLE))
			errCnt++;
#endif

#if 0 // Inet_LnaOf and Inet_NetOf are not supported
	if(strcmp(inet_lnaof(in),LNAOF_HOST_EXAMPLE))
		errCnt++;

	if(strcmp(inet_netof(in),NETOF_HOST_EXAMPLE))
		errCnt++;
#endif

	// Test getprotobyname, socket, gethostbyname, connect, close, ioctl, waitselect, select, send, recv, getpeername
	// From https://gilles-pelletier-vft.go.yj.fr/amiga/bsdsocket.htm, with adaptation
	BOOL connected = FALSE,sent = FALSE,received = FALSE,loopRecv = FALSE;
	struct sockaddr_in servaddr;
	struct hostent *hp;
	struct protoent *pe;
	struct stat file_stat;
	socklen_t option_len;
	int sock_id;
	char *message;
	int msglen,msgcur,msgtodolen;
	int err;
	int port = HTTP_PORT;
	int receivedBytes;
	long zero = 0,one = 1,selectRes;
	fd_set fd_read,fd_write;
	struct timeval tv;
	ULONG addr;
	struct sockaddr address;
	socklen_t address_len;

	if((message = malloc(MESSAGE_BUFFER_SIZE)) == NULL)
	{
		errCnt++;
	}
	else
	{
		if((pe = (struct protoent *)getprotobyname("tcp")) == NULL)
		{
			errCnt++;
		}
		else
		{
			if((sock_id = socket(AF_INET, SOCK_STREAM, pe->p_proto)) == ERROR)
			{
				errCnt++;
			}
			else
			{
				memset(&servaddr,0,sizeof(servaddr));

				if((hp = gethostbyname(WEB_SITE)) == NULL)
				{
					errCnt++;
				}
				else
				{
					UBYTE *p = (UBYTE *)hp->h_addr ;
					addr = p[0] << 24 | p[1] << 16 | p[2] << 8 | p[3] ;

					servaddr.sin_addr.s_addr = addr;
					servaddr.sin_port = htons(port);
					servaddr.sin_family = AF_INET;

					if(ioctl(sock_id,FIONBIO,(void *)&one) == ERROR) // set non-blocking mode
					{
						errCnt++;
					}
					else
					{
						if((connect(sock_id,(struct sockaddr *)&servaddr,sizeof(servaddr))) < 0)
						{
							// Set time out for connect
							FD_ZERO(&fd_write);
							FD_SET(sock_id,&fd_write);
							tv.tv_sec  = 10;
							tv.tv_usec = 0;

							selectRes = waitselect(sock_id + 1,NULL,&fd_write,NULL,&tv,NULL);

							switch(selectRes)
							{
								case 0:
									break;
								case -1:
									break;
								default:
									connected = TRUE;
									break;
							}
						}
						else
						{
							connected = TRUE; // direct connection
						}

						if(!connected)
						{
							errCnt++;
						} 
						else
						{
							address_len = sizeof(address);

							if(getpeername(sock_id,&address,&address_len) == ERROR) errCnt++;


							FILE *f = fopen(NET_TEST_FILE, "wb") ;

							sprintf( message,
									"GET %s HTTP/1.0\r\n"
									"Host: %s\r\n"
									"From: nowhere\r\n"
									"User-Agent: %s\r\n\r\n",
									URL,
									WEB_SITE,
									USER_AGENT) ;

									msglen     = 0 ;
									msgtodolen = strlen(message) ;
									msgcur     = 0 ;


							if(send(sock_id,message,strlen(message),0) != strlen(message))
							{
								// Set timeout for send
								FD_ZERO(&fd_write);
								FD_SET(sock_id,&fd_write);
								tv.tv_sec  = 4;
								tv.tv_usec = 0;

								selectRes = select(sock_id + 1,NULL,&fd_write,NULL,&tv);

								switch(selectRes)
								{
									case 0: // send timeout
										break;
									case -1: // send error
										break;
									default:
										sent = TRUE;
										break;
								}
							}
							else
							{
								sent = TRUE; // direct send
							}

							if(!sent)
							{
								errCnt++;
							} 
							else
							{
								// Prepare receive
								memset(message,0,MESSAGE_BUFFER_SIZE); // clear receive buffer
								FD_ZERO(&fd_read);
								FD_SET(sock_id,&fd_read);
								tv.tv_sec  = 10; // first receive may take longer
								tv.tv_usec = 0;
								loopRecv = TRUE;

								do
								{
									selectRes = waitselect(sock_id + 1,&fd_read,NULL,NULL,&tv,NULL);

									switch(selectRes)
									{
										case 0:
											loopRecv = FALSE; // recv timeout
											break;
										case -1:
											loopRecv = FALSE; // recv error
											break;
										default:
											receivedBytes = recv(sock_id,message,MESSAGE_BUFFER_SIZE,0);

											if(receivedBytes == -1 || receivedBytes == 0)
											{
												loopRecv = FALSE;
											}
											else
											{
												if (f != NULL)
												{
													fwrite(message,receivedBytes,1,f);
												}
											}
											break;
									}

									FD_ZERO(&fd_read);
									FD_SET(sock_id,&fd_read);
									tv.tv_sec  = 4; // next receive need less time
									tv.tv_usec = 0;
								} while(loopRecv);

								if (f == NULL)
								{
									errCnt++;
								}
								else
								{
									fclose(f) ;

									if(stat(NET_TEST_FILE,&file_stat) == ERROR)
									{
										errCnt++;
									}
									else
									{
										if(file_stat.st_size <= 0)
											errCnt++;
									}
								}
							}
						}
					}
				}

				close(sock_id);
			}
		}

		free(message);
	}

	// Test gethostid, gethostbyaddr
	struct sockaddr_in sin;
	struct hostent *hpi;
	sin.sin_addr.s_addr = gethostid();

	if((hpi = gethostbyaddr((char *)&sin.sin_addr.s_addr,sizeof(struct in_addr),AF_INET)) == NULL)
	{
		errCnt++;
	} else
	{
		if(strlen(hpi->h_name) == 0)
			errCnt++;
	}

	// Test gethostname
	if(gethostname(buffer1,BUFFER_SIZE) == ERROR) errCnt++;

	// Test getnetbyaddr
#if 0 // getnetbyaddr is not supported
	if(getnetbyaddr(in.s_addr,AF_INET) == NULL) errCnt++;
#endif

#if 0 // getnetbyname is not supported
	// Test getnetbyaddr
	if(getnetbyname(WEB_SITE) == NULL) errCnt++;
#endif

	// Test getprotobynumber (always returns NULL)
	struct protoent *protoententry;

	if((protoententry = getprotobynumber(IPPROTO_TCP)) != NULL)
	{
#if OPTION_TRACE_NETLIB
		printf("getprotobynumber: p_name = %s, p_proto = %d\n",protoententry->p_name,protoententry->p_proto);
#endif		
	}

	// Test getservbyname
	struct servent *sevrentry;

	if((sevrentry = getservbyname("HTTP","TCP")) == NULL)
	{
		errCnt++;
	}
	else
	{
#if OPTION_TRACE_NETLIB
		printf("getservbyname: s_name: %s, s_port: %u, s_proto: %s\n",sevrentry->s_name,sevrentry->s_port,sevrentry->s_proto);
#endif
	}

	// Test getservbyport
	if((sevrentry = getservbyport(514,"TCP")) == NULL)
	{
		errCnt++;
	}
	else
	{
#if OPTION_TRACE_NETLIB
		printf("getservbyport: s_name: %s, s_port: %u, s_proto: %s\n",sevrentry->s_name,sevrentry->s_port,sevrentry->s_proto);
#endif
	}

	// Test getaddrinfo
	struct addrinfo *result;

	if(getaddrinfo(WEB_SITE,NULL,NULL,&result) != 0)
	{
		errCnt++;
	}
	else
	{
		freeaddrinfo(result);
	}

	// Create a new process to launch a client
	struct TagItem stags[5];

	stags[0].ti_Tag = NP_Entry;
	stags[0].ti_Data = (ULONG)fcntCreateClient;
	stags[1].ti_Tag = NP_Output;
	stags[1].ti_Data = (ULONG)Output();
	stags[2].ti_Tag = NP_CloseOutput;
	stags[2].ti_Data = (ULONG)FALSE;
	stags[3].ti_Tag = NP_Input;
	stags[3].ti_Data = (ULONG)Input();
	stags[4].ti_Tag = NP_CloseInput;
	stags[4].ti_Data = (ULONG)FALSE;
	stags[5].ti_Tag = TAG_END;

	if(CreateNewProc(stags) == NULL)
	{
		errCnt++;
	}
	else
	{
		fcntCreateServer();
    }

	// Test umask
	mode_t initialMask = umask(S_IRWXU); // RWX mask for owner

	if(umask(initialMask) != S_IRWXU)
		errCnt++;

	// Test sysinfo
	buffer1[0] = '\0';

	if(sysinfo(SI_SYSNAME,buffer1,BUFFER_SIZE) == -1)
	{
		errCnt++;
	}
	else
	{
		if(strlen(buffer1) == 0)
			errCnt++;
	}

	// Test crypt
	int file_desc;
	BPTR lock;

	// We first create the RAM:db/passwd file, fill it with basic information and then assign AmiTCP: to RAM:
	if(mkdir("RAM:db",S_IRWXU) == ERROR)
	{
		errCnt++;
	}
	else
	{
		if((file_desc = open(AMITCP_PASSWD_FILE,O_CREAT | O_RDWR)) == ERROR)
		{
			errCnt++;
		}
		else
		{
			if((size_t)write(file_desc,AMITCP_PASSWD_FILE_FILL,strlen(AMITCP_PASSWD_FILE_FILL)) != strlen(AMITCP_PASSWD_FILE_FILL))
			{
				errCnt++;
			}
			else
			{
				if(close(file_desc) == ERROR)
				{
					errCnt++;
				}
				else
				{
					if((lock = Lock(RAM_DIR,SHARED_LOCK)) == (BPTR)NULL)
					{
						errCnt++;
					}
					else
					{
						if(AssignLock(AMITCP_ASSIGN,lock) == DOSFALSE)
						{
							errCnt++;
						}
						else
						{
							char *crypt_ret = crypt("alpha","Amiga User");
							if(crypt_ret == NULL) errCnt++;
						}
					}
				}
			}
		}
	}

	// Regarding the user group functions, since we do not have an AmiTCP installation, we just call these functions without checks
	// Test setuid, getuid, seteuid, geteuid, setgid, getgid, setegid, getegid
	setuid(0);
	getuid();
	seteuid(0);
	geteuid();
	setgid(0);
	getgid();
	setegid(0x1000);
	getegid();

	// Test setgrent, getgrent, endgrent
	setgrent();
	getgrent();
	endgrent();

	// Test getgrgid, getgrnam
	getgrgid(0);
	getgrnam(GROUP_NAME);

	// Test setgroups, getgroups
	gid_t gid_list[4];
	setgroups(0,NULL);
	getgroups(4,gid_list);

	// Test getpass
	printf("Please type a pseudo password followed by enter\n");
	getpass(GETPASS_PROMPT);

	// Test setpwent, getpwent, endpwent
	setpwent();
	getpwent();
	endpwent();

	// Test getpwnam, getpwuid
	getpwnam(AMIGA_USER);
	getpwuid(0);

	// Test initgroups
	initgroups(AMIGA_USER,0);

	// Test setreuid, setregid
	setreuid(0,0);
	setregid(0,0);

	// Test setsid
	setsid();

	// Test uname
	struct utsname utsname_buf;
	uname(&utsname_buf);

	return(errCnt);
}