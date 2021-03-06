/****************************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/
/****************************************************************************
 * include/sys/sock_internal.h
 *
 *   Copyright (C) 2007, 2009, 2011 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name NuttX nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

#ifndef __INCLUDE_SYS_SOCK_INTERNAL_H
#define __INCLUDE_SYS_SOCK_INTERNAL_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <sys/types.h>

#include <uio.h>

struct msghdr {
	void *msg_name;				/* Socket name      */
	int msg_namelen;			/* Length of name   */
	struct iovec *msg_iov;		/* Data blocks      */
	__kernel_size_t msg_iovlen;	/* Number of blocks   */
	void *msg_control;			/* Per protocol magic (eg BSD file descriptor passing) */
	__kernel_size_t msg_controllen;	/* Length of cmsg list */
	unsigned int msg_flags;
};

/*
 *  POSIX 1003.1g - ancillary data object information
 *  Ancillary data consits of a sequence of pairs of
 *  (cmsghdr, cmsg_data[])
 */

struct cmsghdr {
	__kernel_size_t cmsg_len;	/* data byte count, including hdr */
	int cmsg_level;				/* originating protocol */
	int cmsg_type;				/* protocol-specific type */
};

#define CMSG_NXTHDR(mhdr, cmsg) cmsg_nxthdr((mhdr), (cmsg))

#define CMSG_ALIGN(len) (((len)+sizeof(long)-1) & ~(sizeof(long)-1))

#define CMSG_DATA(cmsg) ((void *)((char *)(cmsg) + CMSG_ALIGN(sizeof(struct cmsghdr))))
#define CMSG_SPACE(len) (CMSG_ALIGN(sizeof(struct cmsghdr)) + CMSG_ALIGN(len))
#define CMSG_LEN(len) (CMSG_ALIGN(sizeof(struct cmsghdr)) + (len))

#define __CMSG_FIRSTHDR(ctl, len) ((len) >= sizeof(struct cmsghdr) ? \
								  (struct cmsghdr *)(ctl) : \
								  (struct cmsghdr *)NULL)
#define CMSG_FIRSTHDR(msg)  __CMSG_FIRSTHDR((msg)->msg_control, (msg)->msg_controllen)

static inline struct cmsghdr *__cmsg_nxthdr(void *__ctl, __kernel_size_t __size, struct cmsghdr *__cmsg)
{
	struct cmsghdr *__ptr;

	__ptr = (struct cmsghdr *)(((unsigned char *)__cmsg) + CMSG_ALIGN(__cmsg->cmsg_len));
	if ((unsigned long)((char *)(__ptr + 1) - (char *)__ctl) > __size) {
		return (struct cmsghdr *)0;
	}

	return __ptr;
}

static inline struct cmsghdr *cmsg_nxthdr(struct msghdr *__msg, struct cmsghdr *__cmsg)
{
	return __cmsg_nxthdr(__msg->msg_control, __msg->msg_controllen, __cmsg);
}

ssize_t recvmsg(int sockfd, struct msghdr *msg, int flags);
ssize_t sendmsg(int sockfd, struct msghdr *msg, int flags);


/****************************************************************************
 * Definitions
 ****************************************************************************/

/* The socket()domain parameter specifies a communication domain; this selects
 * the protocol family which will be used for communication.
 */

/* Protocol families */

#define PF_UNSPEC       0		/* Protocol family unspecified */
#define PF_UNIX         1		/* Local communication */
#define PF_LOCAL        1		/* Local communication */
#define PF_INET         2		/* IPv4 Internet protocols */
#define PF_INET6        3		/* IPv6 Internet protocols */
#define PF_IPX          4		/* IPX - Novell protocols */
#define PF_NETLINK      5		/* Kernel user interface device */
#define PF_X25          6		/* ITU-T X.25 / ISO-8208 protocol */
#define PF_AX25         7		/* Amateur radio AX.25 protocol */
#define PF_ATMPVC       8		/* Access to raw ATM PVCs */
#define PF_APPLETALK    9		/* Appletalk */
#define PF_PACKET      10		/* Low level packet interface */

/* Address families */

#define AF_UNSPEC      PF_UNSPEC
#define AF_UNIX        PF_UNIX
#define AF_LOCAL       PF_LOCAL
#define AF_INET        PF_INET
#define AF_INET6       PF_INET6
#define AF_IPX         PF_IPX
#define AF_NETLINK     PF_NETLINK
#define AF_X25         PF_X25
#define AF_AX25        PF_AX25
#define AF_ATMPVC      PF_ATMPVC
#define AF_APPLETALK   PF_APPLETALK
#define AF_PACKET      PF_PACKET

/* The socket created by socket() has the indicated type, which specifies
 * the communication semantics.
 */

#define SOCK_STREAM    0		/* Provides sequenced, reliable, two-way, connection-based byte streams.
								 * An  out-of-band data transmission mechanism may be supported. */
#define SOCK_DGRAM     1		/* Supports  datagrams (connectionless, unreliable messages of a fixed
								 * maximum length). */
#define SOCK_SEQPACKET 2		/* Provides a sequenced, reliable, two-way connection-based data
								 * transmission path for datagrams of fixed maximum length; a consumer
								 * is required to read an entire packet with each read system call. */
#define SOCK_RAW       3		/* Provides raw network protocol access. */
#define SOCK_RDM       4		/* Provides a reliable datagram layer that does not guarantee ordering. */
#define SOCK_PACKET    5		/* Obsolete and should not be used in new programs */

/* Bits in the FLAGS argument to `send', `recv', et al. These are the bits
 * recognized by Linus, not all are supported by TinyAra.
 */

#define MSG_OOB        0x0001	/* Process out-of-band data.  */
#define MSG_PEEK       0x0002	/* Peek at incoming messages.  */
#define MSG_DONTROUTE  0x0004	/* Don't use local routing.  */
#define MSG_CTRUNC     0x0008	/* Control data lost before delivery.  */
#define MSG_PROXY      0x0010	/* Supply or ask second address.  */
#define MSG_TRUNC      0x0020
#define MSG_DONTWAIT   0x0040	/* Enable nonblocking IO.  */
#define MSG_EOR        0x0080	/* End of record.  */
#define MSG_WAITALL    0x0100	/* Wait for a full request.  */
#define MSG_FIN        0x0200
#define MSG_SYN        0x0400
#define MSG_CONFIRM    0x0800	/* Confirm path validity.  */
#define MSG_RST        0x1000
#define MSG_ERRQUEUE   0x2000	/* Fetch message from error queue.  */
#define MSG_NOSIGNAL   0x4000	/* Do not generate SIGPIPE.  */
#define MSG_MORE       0x8000	/* Sender will send more.  */

/* Socket options */

#define SO_DEBUG        0		/* Enables recording of debugging information (get/set).
								 * arg: pointer to integer containing a boolean value */
#define SO_ACCEPTCONN   1		/* Reports whether socket listening is enabled (get only).
								 * arg: pointer to integer containing a boolean value */
#define SO_BROADCAST    2		/* Permits sending of broadcast messages (get/set).
								 * arg: pointer to integer containing a boolean value */
#define SO_REUSEADDR    3		/* Allow reuse of local addresses (get/set)
								 * arg: pointer to integer containing a boolean value */
#define SO_KEEPALIVE    4		/* Keeps connections active by enabling the periodic transmission
								 * of messages (get/set).
								 * arg: pointer to integer containing a boolean value */
#define SO_LINGER       5		/* Lingers on a close() if data is present (get/set)
								 * arg: struct linger */
#define SO_OOBINLINE    6		/* Leaves received out-of-band data (data marked urgent) inline
								 * (get/set) arg: pointer to integer containing a boolean value */
#define SO_SNDBUF       7		/* Sets send buffer size. arg: integer value (get/set). */
#define SO_RCVBUF       8		/* Sets receive buffer size. arg: integer value (get/set). */
#define SO_ERROR        9		/* Reports and clears error status (get only).  arg: returns
								 * an integer value */
#define SO_TYPE        10		/* Reports the socket type (get only). return: int */
#define SO_DONTROUTE   11		/* Requests that outgoing messages bypass standard routing (get/set)
								 * arg: pointer to integer containing a boolean value */
#define SO_RCVLOWAT    12		/* Sets the minimum number of bytes to process for socket input
								 * (get/set). arg: integer value */
#define SO_RCVTIMEO    13		/* Sets the timeout value that specifies the maximum amount of time
								 * an input function waits until it completes (get/set).
								 * arg: struct timeval */
#define SO_SNDLOWAT    14		/* Sets the minimum number of bytes to process for socket output
								 * (get/set). arg: integer value */
#define SO_SNDTIMEO    15		/* Sets the timeout value specifying the amount of time that an
								 * output function blocks because flow control prevents data from
								 * being sent(get/set). arg: struct timeval */

/* Protocol levels supported by get/setsockopt(): */

#define SOL_SOCKET     0xfff	/* Only socket-level options supported */

/* Values for the 'how' argument of shutdown() */

#define SHUT_RD        1		/* Bit 0: Disables further receive operations */
#define SHUT_WR        2		/* Bit 1: Disables further send operations */
#define SHUT_RDWR      3		/* Bits 0+1: Disables further send and receive operations */

/****************************************************************************
 * Type Definitions
 ****************************************************************************/

/* sockaddr_storage structure. This structure must be (1) large enough to
 * accommodate all supported protocol-specific address structures, and (2)
 * aligned at an appropriate boundary so that pointers to it can be cast
 * as pointers to protocol-specific address structures and used to access
 * the fields of those structures without alignment problems
 */

#ifdef CONFIG_NET_IPv6
struct sockaddr_storage {
#ifdef CONFIG_NET_LWIP
	uint8_t ss_len;
	uint8_t ss_family;			/* Address family */
#else
	sa_family_t ss_family;		/* Address family: See AF_* definitions */
#endif
	char ss_data[18];			/* 18-bytes of address data */
};
#else
struct sockaddr_storage {
#ifdef CONFIG_NET_LWIP
	uint8_t ss_len;
	uint8_t ss_family;			/* Address family */
#else
	sa_family_t ss_family;		/* Address family: See AF_* definitions */
#endif
	char ss_data[14];			/* 14-bytes of address data */
};
#endif

/* The sockaddr structure is used to define a socket address which is used
 * in the bind(), connect(), getpeername(), getsockname(), recvfrom(), and
 * sendto() functions.
 */

struct sockaddr {
#ifdef CONFIG_NET_LWIP
	uint8_t sa_len;
	uint8_t sa_family;			/* Address family */
#else
	sa_family_t sa_family;		/* Address family: See AF_* definitions */
#endif
	char sa_data[14];			/* 14-bytes of address data */
};

/* Used with the SO_LINGER socket option */

struct linger {
	int l_onoff;				/* Indicates whether linger option is enabled. */
	int l_linger;				/* Linger time, in seconds. */
};

#endif							/* __INCLUDE_SYS_SOCK_INTERNAL_H */
