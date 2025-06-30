#ifndef NET_TEST_H
#define NET_TEST_H

#ifdef __cplusplus
extern "C" {
#endif

void test_tcp_connect_google(void);
void test_dns_resolution(void);
void test_http_request(void);

#ifdef __cplusplus
}
#endif

#endif // NET_TEST_H