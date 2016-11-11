#include <errno.h>
#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cmocka.h>

#include <lib/net.h>

static void ip4_checksum_test(void **state)
{
	(void)state;
#define CHECKSUM_IP 5
	uint16_t raw_iphdr[] = {
		0x4500, 0x0028, 0x772b, 0x4000,
		0x4006, 0x0000, 0xc0a8, 0x0165,
		0x6819, 0x0b06
	};

	uint16_t result = 0x8e78;

	struct iphdr iphdr;
	memset(&iphdr, 0, sizeof(iphdr));
	memcpy(&iphdr, raw_iphdr, sizeof(raw_iphdr));

	// ivp4 test header checksum with the checksum field 0
	uint16_t csum = ip4_checksum(&iphdr);
	assert_int_equal(csum, result);

	raw_iphdr[CHECKSUM_IP] = result;
	memset(&iphdr, 0, sizeof(iphdr));
	memcpy(&iphdr, raw_iphdr, sizeof(raw_iphdr));

	// ipv4 test header checksum verify with the checksum field included
	csum = ip4_checksum(&iphdr);
	assert_int_equal(csum, 0);
}

static void tcp4_checksum_test(void **state)
{
	(void)state;

	uint8_t raw_iphdr[] = {
		0x45, 0x00, 0x00, 0x28, 0xd4, 0x31, 0x40, 0x00,
		0xff, 0x06, 0x23, 0xe7, 0xc0, 0xa8, 0x01, 0x65,
		0xc0, 0xa8, 0x01, 0x01
	};

	uint8_t raw_tcphdr[] = {
		0xb3, 0x5e, 0x00, 0x50, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x50, 0x02, 0xff, 0xff,
		0x00, 0x00, 0x00, 0x00
	};

#define CHECKSUM_HIGH_BIT 17
#define CHECKSUM_LOW_BIT 18

	struct iphdr iphdr;
	struct tcphdr tcphdr;
	memset(&iphdr, 0, sizeof(iphdr));
	memset(&tcphdr, 0, sizeof(tcphdr));

	memcpy(&iphdr, raw_iphdr, sizeof(raw_iphdr));
	memcpy(&tcphdr, raw_tcphdr, sizeof(raw_tcphdr));

	uint16_t result = 0x7d78;

	// ipv4 test tcp header checksum with the checksum field 0
	uint16_t csum = tcp4_checksum(&iphdr, &tcphdr);
	assert_int_equal(csum, result);
	
	// ipv4 test tcp header when checksum field is included
	raw_tcphdr[CHECKSUM_HIGH_BIT] = 0x7d;
	raw_tcphdr[CHECKSUM_LOW_BIT] = 0x78;

	memset(&tcphdr, 0, sizeof(tcphdr));
	memcpy(&tcphdr, raw_tcphdr, sizeof(raw_tcphdr));
	csum = tcp4_checksum(&iphdr, &tcphdr);
	assert_int_equal(csum, 0);
}

static void addr_if_test(void **state)
{
	(void)state;
	in_addr_t addr = addr_if("lo", AF_INET);
	assert_int_not_equal(addr, 0);

	addr = addr_if("other-interface", AF_INET);
	assert_int_equal(addr, 0);
}

int main(void)
{
	cmocka_set_message_output(CM_OUTPUT_STDOUT);
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(ip4_checksum_test),
		cmocka_unit_test(tcp4_checksum_test),
		cmocka_unit_test(addr_if_test),
	};

	return cmocka_run_group_tests(tests, NULL, NULL);
}
