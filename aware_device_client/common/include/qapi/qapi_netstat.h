/* Copyright (c) 2016-2021 Qualcomm Technologies, Inc.
   All rights reserved.
   Confidential and Proprietary - Qualcomm Technologies, Inc.
*/

/*
!! IMPORTANT NOTE: "DATA SERVICES" VERSION CATEGORY. 
*/

/**
  @file qapi_netstat.h
*/

#ifndef _QAPI_NETSTAT_H_
#define _QAPI_NETSTAT_H_

#include <stdint.h>
#include "qapi_data_txm_base.h"
#include "qapi_status.h"

#define  TXM_QAPI_SOCKET_NWSTATS TXM_QAPI_NET_BASE + 27

/** @addtogroup qapi_getnwstats
@{ */

/** Protocol type for network statistics.
 */
typedef enum {
	
  QAPI_NWSTATS_IPv4 = 1,   /**< Network statistics of IP */
  QAPI_NWSTATS_IPv6,       /**< Network statistics of IPv6 */
  QAPI_NWSTATS_TCP,        /**< Network statistics of TCP */
  QAPI_NWSTATS_UDP,        /**< Network statistics of UDP */
  QAPI_NWSTATS_ICMP,       /**< Network statistics of ICMP */
  QAPI_NWSTATS_ICMPv6,     /**< Network statistics of ICMP6 */
  QAPI_NWSTAT_MAX = 65535
  
}qapi_nwstats_prot_t ;

/** Operations for network statistics.
 */
typedef enum {

 QAPI_NWSTATS_GET = 1,       /**< Gets network statistics. */
 QAPI_NWSTATS_CLR,           /**< Clears network statistics. */
 QAPI_NWSTAT_OP_MAX = 65535  /**< Maximum value. Do not use */

}qapi_nwstats_op_t;

typedef unsigned long   u_long;

typedef struct qapi_ipv4_stats     {
   u_long   ipForwarding;     /**< 1= we are a gateway; 2 = host */
   u_long   ipDefaultTTL;     /**< IP time to live */
   u_long   ipInReceives;     /**< total received datagrams (bad too) */
   u_long   ipInHdrErrors;    /**< Header Err (xsum, ver, ttl, etc) */
   u_long   ipInAddrErrors;   /**< nonsense IP addresses */
   u_long   ipForwDatagrams;  /**< routed packets */
   u_long   ipUnknownProtos;  /**< unknown protocol types */
   u_long   ipInDiscards;     /**< dropped (ie no buffer space) */
   u_long   ipInDelivers;     /**< delivered receive packets */
   u_long   ipOutRequests;    /**< sends (not including routed) */
   u_long   ipOutDiscards;    /**< sends dropped (no buffer) */
   u_long   ipOutNoRoutes;    /**< dropped, can't route */
   u_long   ipReasmTimeout;   /**< fragment reassembly timeouts */
   u_long   ipReasmReqds;     /**< frags received */
   u_long   ipReasmOKs;       /**< packets successfully reassembled */
   u_long   ipReasmFails;     /**< packets reassemblys failed */
   u_long   ipFragOKs;        /**< packets we fragmented for send */
   u_long   ipFragFails;      /**< packets we wanted to frag and couldnt */
   u_long   ipFragCreates;    /**< fragments we made */
   u_long   ipRoutingDiscards; /**< Routing entries which were discarded */
   u_long   ipFilterDrop;     /**< Packets dropped in filtering */
}qapi_ipv4_stats_t;

/**
 * Network statistics for IPv6.
 */

typedef struct qapi_ipv6_stats_s
{
   uint32_t ipv6IfStatsInReceives;        /**< Input datagrams received */
   uint32_t ipv6IfStatsInHdrErrors;       /**< Input datagrams discarded because of errors in their IPv6 headers. */
   uint32_t ipv6IfStatsInTooBigErrors;    /**< Input datagrams that could not be forwarded because their size exceeded the limit. */
   uint32_t ipv6IfStatsInNoRoutes;        /**< Input datagrams discarded because no route was found. */
   uint32_t ipv6IfStatsInAddrErrors;      /**< Input datagrams discarded because of invalid IPv6 destination address. */
   uint32_t ipv6IfStatsInUnknownProtos;   /**< Input datagrams discarded because of unknown protocol. */
   uint32_t ipv6IfStatsInTruncatedPkts;   /**< Input datagrams discarded because datagram frame did not carry enough data. */
   uint32_t ipv6IfStatsInDiscards;        /**< Number of input datagrams discarded, where no problems were encountered to prevent their continued processing, e.g., lack of buffer space. */
   uint32_t ipv6IfStatsInDelivers;        /**< Number of datagrams successfully delivered. */
   uint32_t ipv6IfStatsOutNoRoutes;       /**< Outgoing datagrams discarded because no route was found. */
   uint32_t ipv6IfStatsOutForwDatagrams;  /**< Output datagrams that this entity received and forwarded. */
   uint32_t ipv6IfStatsOutRequests;       /**< IPv6 datagrams that local IPv6 user protocols supplied to IPv6 in transmission requests. */
   uint32_t ipv6IfStatsOutDiscards;       /**< Number of output datagrams discarded, where no problems were encountered to prevent their transmission to their destination, e.g., lack of buffer space. */
   uint32_t ipv6IfStatsOutFragOKs;        /**< Number of fragmented IPv6 datagrams. */
   uint32_t ipv6IfStatsOutFragFails;      /**< Input datagrams discarded because datagram fragmentation failed. */  
   uint32_t ipv6IfStatsOutFragCreates;    /**< Number of output datagram fragments generated. */
   uint32_t ipv6IfStatsReasmReqds;        /**< Received IPv6 fragments that must be reassembled. */
   uint32_t ipv6IfStatsReasmOKs;          /**< Number of IPv6 datagrams successfully reassembled. */
   uint32_t ipv6IfStatsReasmFails;        /**< Failures detected by the IPv6 reassembly algorithm. */
   uint32_t ipv6IfStatsInMcastPkts;       /**< Multicast packets received. */
   uint32_t ipv6IfStatsOutMcastPkts;      /**< Multicast packets transmitted. */
   uint32_t ipv6FilterDrop;               /**< Packets dropped in filtering. */
}qapi_ipv6_stats_t;

/**
 * Network statistics for Transmission Control Protocol (TCP).
 */
typedef struct qapi_tcp_stats
{
   long     tcpRtoAlgorithm;   /**< Algorithm that determines the timeout value. */
   long     tcpRtoMin;         /**< Minimum value for retransmission timeout (in ms). */
   long     tcpRtoMax;         /**< Maximum value for retransmission timeout (in ms). */
   long     tcpMaxConn;        /**< Maximum number of TCP connections allowed. */
   u_long   tcpActiveOpens;    /**< Number of active and open TCP connections. */
   u_long   tcpPassiveOpens;   /**< Number of passive and open TCP connections. */
   u_long   tcpAttemptFails;   /**< Connections that made a direct transition to the CLOSED state from either the SYN-SENT or SYN-RCVD states, and connections that made a direct transition to the LISTEN state from the SYN-RCVD state. */
   u_long   tcpEstabResets;    /**< Connections that were closed from ESTABLISHED or CLOSE-WAIT state. */
   u_long   tcpCurrEstab;      /**< Currently established connections. */
   u_long   tcpInSegs;         /**< Total number of segments received, including those received in error. */
   u_long   tcpOutSegs;        /**< Total number of segments sent. */
   u_long   tcpRetransSegs;    /**< Total number of segments retransmitted. */
   void *   tcpConnTable;  	   /**< Table containing TCP connection-specific information (32-bit pointer). */
   u_long   tcpInErrs;         /**< Total number of segments received in error. */
   u_long   tcpOutRsts;        /**< Number of TCP segments sent containing the RST flag. */
   u_long   tcpFilterDrop;     /**< Number of packets dropped in filtering.*/
} qapi_tcp_stats_t;

/**
 * Network statistics for User Datagram Protocol (UDP).
 */
typedef struct qapi_udp_stats
{
   u_long   udpInDatagrams;   /**< Total delivered datagrams. */
   u_long   udpNoPorts;       /**< Undelivered datagrams: unused port. */
   u_long   udpInErrors;      /**< Undelivered datagrams: other reasons. */
   u_long   udpOutDatagrams;  /**< Number of successfully sent datagrams. */
   u_long   udpOutErrors;     /**< Number of sending packet errors. */
   u_long   udpFilterDrop;    /**< Number of packets dropped in filtering. */
} qapi_udp_stats_t;

/**
 * Network statistics for Internet Control Message Protocol (ICMP).
 */

typedef struct qapi_icmp_stats {
   u_long   icmpInMsgs;          /**< Received ICMP packets, including errors. */
   u_long   icmpInErrors;        /**< Errors such as bad sums, bad len, etc. */
   u_long   icmpInDestUnreachs;  /**< Number of ICMP Destination Unreachable messages received. */
   u_long   icmpInTimeExcds;     /**< Number of ICMP Time Exceeded messages received. */
   u_long   icmpInParmProbs;     /**< Number of ICMP Parameter Problem messages received. */
   u_long   icmpInSrcQuenchs;    /**< Number of ICMP Source Quench messages received. */
   u_long   icmpInRedirects;     /**< Number of ICMP Redirect messages received. */
   u_long   icmpInEchos;         /**< Number of ICMP Echo (request) messages received. */
   u_long   icmpInEchoReps;      /**< Number of ICMP Echo Reply messages received. */
   u_long   icmpInTimestamps;    /**< Number of ICMP Timestamp (request) messages received. */
   u_long   icmpInTimestampReps; /**< Number of ICMP Timestamp Reply messages received. */
   u_long   icmpInAddrMasks;     /**< Number of ICMP Address Mask Request messages received. */
   u_long   icmpInAddrMaskReps;  /**< Number of ICMP Address Mask Reply messages received. */
   u_long   icmpOutMsgs;         /**< Total number of sent ICMP messages, including errors. */
   u_long   icmpOutErrors;       /**< ICMP Layer errors ONLY (see rfc #1156). */
   u_long   icmpOutDestUnreachs; /**< Number of ICMP Destination Unreachable messages sent. */
   u_long   icmpOutTimeExcds;    /**< Number of ICMP Time Exceeded messages sent. */
   u_long   icmpOutParmProbs;    /**< Number of ICMP Parameter Problem messages sent. */
   u_long   icmpOutSrcQuenchs;   /**< Number of ICMP Source Quench messages sent. */
   u_long   icmpOutRedirects;    /**< Number of ICMP Redirect messages sent. */
   u_long   icmpOutEchos;        /**< Number of ICMP Echo (request) messages sent. */
   u_long   icmpOutEchoReps;     /**< Number of ICMP Echo Reply messages sent. */
   u_long   icmpOutTimestamps;   /**< Number of ICMP Timestamp (request) messages sent. */
   u_long   icmpOutTimestampReps;/**< Number of ICMP Timestamp Reply messages sent. */
   u_long   icmpOutAddrMasks;    /**< Number of ICMP Address Mask Request messages sent. */
   u_long   icmpOutAddrMaskReps; /**< Number of ICMP Address Mask Reply messages sent. */
}qapi_icmp_stats_t;

/**
 * Network statistics for ICMPv6.
 */

typedef struct qapi_icmp6_stats
{
   uint32_t InMsgs;                      /**< Incoming ICMP packets. */
   uint32_t InErrors;                    /**< Errors such as bad sums, bad len, etc. */
   uint32_t InDestUnreachs;              /**< Number of ICMP Destination Unreachable messages received. */
   uint32_t InAdminProhibs;              /**< ICMP destination unreachable/communication administratively prohibited messages. */
   uint32_t InTimeExcds;                 /**< Number of ICMP Time Exceeded messages received. */
   uint32_t InParmProblems;              /**< Number of ICMP Parameter Problem messages received. */
   uint32_t InPktTooBigs;               /**< Number of ICMP Packet Too Big messages received. */
   uint32_t InEchos;                    /**< Number of ICMP Echo (request) messages received. */ 
   uint32_t InEchoReplies;              /**< Number of ICMP Echo Reply messages received. */
   uint32_t InRouterSolicits;           /**< Number of ICMP Router Solicit messages received. */
   uint32_t InRouterAdvertisements;     /**< Number of ICMP Router Advertisement messages received. */
   uint32_t InNeighborSolicits;         /**< Number of ICMP Neighbor Solicit messages received. */
   uint32_t InNeighborAdvertisements;   /**< Number of ICMP Neighbor Advertisement messages received. */
   uint32_t InRedirects;                /**< Number of Redirect messages received. */
   uint32_t InGroupMembQueries;         /**< Number of ICMPv6 Group Membership Query messages received. */
   uint32_t InGroupMembResponses;       /**< Number of ICMPv6 Group Membership Response messages received. */
   uint32_t InGroupMembReductions;      /**< Number of ICMPv6 Group Membership Reduction messages received. */
   uint32_t OutMsgs;                    /**< Total sent icmps, including errors. */
   uint32_t OutErrors;                  /**< ICMP Layer errors ONLY. */
   uint32_t OutDestUnreachs;            /**< Number of ICMP Destination Unreachable messages sent. */
   uint32_t OutAdminProhibs;            /**<  Number of ICMP dest unreachable/communication administratively prohibited messages sent. */
   uint32_t OutTimeExcds;               /**< Number of ICMP Time Exceeded messages sent. */
   uint32_t OutParmProblems;            /**< Number of ICMP Parameter Problem messages sent. */
   uint32_t OutPktTooBigs;              /**< Number of ICMP Packet Too Big messages sent. */
   uint32_t OutEchos;                   /**< Number of ICMP Echo (request) messages sent. */
   uint32_t OutEchoReplies;             /**< Number of ICMP Echo Reply messages sent. */
   uint32_t OutRouterSolicits;          /**< Number of ICMP Router Solicitation messages sent. */
   uint32_t OutRouterAdvertisements;    /**< Number of ICMP Router Advertisement messages sent. */
   uint32_t OutNeighborSolicits;        /**< Number of ICMP Neighbor Solicitation messages sent. */
   uint32_t OutNeighborAdvertisements;  /**< Number of ICMP Neighbor Advertisement messages sent. */
   uint32_t OutRedirects;               /**< Number of Redirect messages sent. */
   uint32_t OutGroupMembQueries;        /**< Number of ICMPv6 Group Membership Query messages sent. */
   uint32_t OutGroupMembResponses;      /**< Number of ICMPv6 Group Membership Response messages sent.*/
   uint32_t OutGroupMembReduction;      /**< Number of ICMPv6 Group Membership Reduction messages sent. */
}qapi_icmp6_stats_t;

/** Memory assigned according to a specified protocol.
 */
typedef struct nwstats_s {

	qapi_nwstats_prot_t prot; /**< Protocol for which statistics must be fetched. */

	union {
		   
		   qapi_ipv4_stats_t * ip;
		   /**< IPv4 statistics. */
		   qapi_ipv6_stats_t * ip6;
		   /**< IPv6 statistics. */
		   qapi_tcp_stats_t * tcp;
		   /**< TCP statistics. */
		   qapi_udp_stats_t * udp;
		   /**< UDP statistics. */
		   qapi_icmp_stats_t * icmp;
		   /**< ICMPv4 statistics. */
		   qapi_icmp6_stats_t * icmp6;
		   /**< ICMPv6 statistics. */

		}u_prot_stats; /**< Buffer to store statistics of a protocol. */
}nwstats_t;

extern void clr_icmp_stats(int ifind, nwstats_t * data);
extern void get_icmp_stats(int ifind, nwstats_t * data);
extern void clr_udp_stats(int ifind, nwstats_t * data);
extern void get_udp_stats(int ifind, nwstats_t * data);


#ifdef  QAPI_TXM_MODULE     // USER_MODE_DEFS

#define qapi_getnwstats(a,b,c,d)              ((int)           (_txm_module_system_call12)(TXM_QAPI_SOCKET_NWSTATS           , (ULONG) a, (ULONG) b, (ULONG) c, (ULONG) d, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0))

#else

UINT qapi_Net_Handler(UINT id, UINT a1, UINT a2, UINT a3, UINT a4, UINT a5, UINT a6, UINT a7, UINT a8, UINT a9, UINT a10, UINT a11, UINT a12);

/** @ingroup qapi_getnwstats
 *   Returns network statistics or clears statistics for the user provided protocol type
 *   based on the operation asked by the user. 

 * @param[in] ifname  Interface name.
 * @param[in] prot    Specifies the protocol for which statstics are required.
 * @param[in] op      Specifies the operation which must be performed on network statistics.
 * @param[out] buf    Pointer to memory location where network statstics are stored. \n
 *                    Memory allocates/deallocates information for buf: \n
 *                    - Application calling this QAPI must explicitly allocate memory to buf.
 *                    - Application must also allocate memory to substructure in nwstats_t or buf,
 *                      i.e., u_prot_stats for IPv4/IPv6/TCP/UDP/ICMP/ICMPv6 protocol.
 *                    - Application must free both allocated memories after calling this QAPI.

 * @return
 * On success, #QAPI_OK. \n
 * On error, #QAPI_ERROR.

 * Additional errors: \n
 * - ENOTCONN : Socket is not in connected state.
 * - ENOMEM: No memory available.
 */

qapi_Status_t qapi_getnwstats(char * ifname , qapi_nwstats_prot_t prot , qapi_nwstats_op_t op , nwstats_t * buf );

#endif

/** @} */ /* end_addtogroup qapi_getnwstats */
#endif


