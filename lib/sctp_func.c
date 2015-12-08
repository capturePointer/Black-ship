#include "sailfish.h"
// LEGAGY CODE works for BSD

//The SCTP_PEER_ADDR_PARAMS 
//socket option in the sctp_address_to_associd() 
//function cannot be used to learn the association id, at least not on linux.
sctp_assoc_t
sctp_address_to_associd(int sockfd, struct sockaddr *sa, socklen_t salen)
{
	 struct				sctp_paddrparams sp;
	 socklen_t palen =	sizeof(struct sctp_paddrparams);

	 memset(&sp, 0 ,palen);
	 memcpy(&sp.spp_address, sa, salen);

     //sctp_opt_info like getsockopt
	 Sctp_opt_info(sockfd, 0 , SCTP_PEER_ADDR_PARAMS, &sp, &palen);

	 return sp.spp_assoc_id;
}
//function updated to work with linux

int 
sctp_get_number_streams_bsd(int sockfd, struct sockaddr *to, 
								socklen_t tolen)
{
	 socklen_t		stlen;
	 struct			sctp_status status;
	 stlen		=	sizeof(status);

	 memset(&status, 0, stlen);
	 status.sstat_assoc_id = sctp_address_to_associd(sockfd,to,tolen);
	 Getsockopt(sockfd, IPPROTO_SCTP, SCTP_STATUS, &status, &stlen);

	 return status.sstat_outstrms;
}

//int
//sctp_get_number_streams(int sockfd, struct sockaddr *to, socklen_t tolen, 
//						struct sctp_sndrcvinfo *sri)
//{
int
sctp_get_number_streams(int sockfd, struct sctp_sndrcvinfo *sri)
{
	 size_t stlen;
	 // sctp structure that holds our number of streams
	 struct sctp_status status;
	 stlen = sizeof(status);
	 memset(&status, 0, sizeof(status));
     //for linux it works like this
	 status.sstat_assoc_id = sri->sinfo_assoc_id;
	
		/**
		*   for bsd it's something like this  
		*	status.stat_assoc_id = sctp_address_to_associd(sockfd, to, tolen);
		*
		*/

	 Getsockopt(sockfd, IPPROTO_SCTP, SCTP_STATUS, &status, (socklen_t*)&stlen);

	 return status.sstat_outstrms;
}
/**
 * The function first zeros out the sctp_initmsg structure
 * This change assures tgat tge setsockopt call will not unintentionally change
 * any other values.The function also sets nstrs filed to the number of streams
 * it would like to request.Next it sets the socket option with the initial
 * message parameters
 *
 * An alternative to setting a socket option would be to use the sendmsg 
 * function and provice ancillary data to request different stream
 * parameters from the default.This type of ancillary data is only
 * effective on the one-to-many socket interface
 */
void
sctp_set_number_streams(int sockfd, struct sctp_initmsg *initm, int nstrs,int maxatmts)
{

	initm->sinit_num_ostreams	=	(__u16) nstrs;
	initm->sinit_max_instreams	=	(__u16) nstrs;
	initm->sinit_max_attempts	=	(__u16) maxatmts;

	 Setsockopt( sockfd, IPPROTO_SCTP, SCTP_INITMSG, &initm, sizeof(initm));
}

/**
 * This code is under testing..
 *
 */

/*
void
sct_set_hearbeat(int sock, struct sctp_paddrparams *heartbeat, int time, size_t pathmaxrxt)
{
	 size_t n = sizeof(*heartbeat);

	 heartbeat->spp_flags = SPP_HB_ENABLE;
	 heartbeat->spp_hbinterval = time;
	 heartbeat->spp_pathmaxrxt = pathmaxrxt;

	 // Set the socket options
	 Setsockopt(sock,SOL_SCTP, SCTP_PEER_ADDR_PARAMS, heartbeat, (socklen_t)n);
}

*/

/*
int
sctp_get_heartbeat(int sock, struct sctp_paddrparams *heartbeat)
{
	size_t n = sizeof(*heartbeat);

	Getsockopt(sock, IPPROTO_SCTP, SCTP_PEER_ADDR_PARAMS, &heartbeat,(socklen_t *)&n);
	 printf("Heart beat configured at %d\n",heartbeat->spp_hbinterval);
	
	return heartbeat->spp_hbinterval;
}
*/


