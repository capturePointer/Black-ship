#include "net.h"
// LEGAGY CODE works for BSD
sctp_assoc_t
sctp_address_to_associd(int sockfd, struct sockaddr *sa, socklen_t salen)
{
	 struct sctp_paddrparams sp;
	 socklen_t palen = sizeof(struct sctp_paddrparams);
	 
	 memset(&sp, 0 ,palen);
	 memcpy(&sp.spp_address, sa, salen);
	 
	 Sctp_opt_info(sockfd, 0 , SCTP_PEER_ADDR_PARAMS, &sp, &palen);
	 
	 return sp.spp_assoc_id;
}
//function updated to work with linux

int sctp_get_number_streams_bsd(int sockfd, struct sockaddr *to, 
								socklen_t tolen)
{
	 socklen_t stlen;
	 struct sctp_status status;
	 stlen = sizeof(status);
	 memset(&status, 0, stlen);
	 status.sstat_assoc_id = sctp_address_to_associd(sockfd,to,tolen);
	 Getsockopt(sockfd, IPPROTO_SCTP, SCTP_STATUS, &status, &stlen);

	 return status.sstat_outstrms;
}

int
sctp_get_number_streams(int sockfd, struct sockaddr *to, socklen_t tolen, 
						struct sctp_sndrcvinfo *sri)
{
	 socklen_t stlen;
	 struct sctp_status status;
	 stlen = sizeof(status);
	 memset(&status, 0, stlen);
     //for linux it works like this
	 status.sstat_assoc_id = sri->sinfo_assoc_id;
	 //for bsd it's something like this
	    // status.stat_assoc_id = sctp_address_to_associd(sockfd, to, tolen);
	 Getsockopt(sockfd, IPPROTO_SCTP, SCTP_STATUS, &status, &stlen);

	 return status.sstat_outstrms;
}
