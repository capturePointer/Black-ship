#include "net.h"
sctp_assoc_t
sctp_address_to_associd(int sockfd, struct sockaddr *sa, socklen_t salen)
{
	 struct sctp_paddrparams sp;
	 int n;
     size_t len = sizeof(sp);
	 memeset(&sp, 0, sp);
	 memcpy(&sp.spp_address, sa, salen);
	 Sctp_opt_info(sockfd, 0 , SCTP_PEEER_ADDR_PARAMS, &sp, &n);
	 return sp.spp_assoc_id;
}
int
sctp_get_number_streams(int sockfd, struct sockaddr *to, socklen_t tolen)
{
	 int n;
	 struct sctp_status status;
	 memset(&status, 0, sizeof(status));

	 status.sstat_assoc_id = sctp_address_to_associd(sockfd, to, tolen);
	 Getsockopt(sockfd, IPPROTO_SCTP, SCTP_STATUS, &status, &n);

	 return status.sstat_outstrms;
}
