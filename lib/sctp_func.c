#include "net.h"
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
int
sctp_get_number_streams(int sockfd, struct sockaddr *to, socklen_t tolen)
{
	 socklen_t stlen;
	 struct sctp_status status;
	 stlen = sizeof(status);
	 memset(&status, 0, stlen);

	 status.sstat_assoc_id = sctp_address_to_associd(sockfd, to, tolen);
	
	 Getsockopt(sockfd, IPPROTO_SCTP, SCTP_STATUS, &status, &stlen);

	 return status.sstat_outstrms;
}
