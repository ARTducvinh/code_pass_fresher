#include "gsm_parser.h"
#include <string.h>
#include <ctype.h>

void gsm_parser_trim(char* s) {
    if (!s) return;

    // Trim đầu
    char* start = s;
    while (*start && isspace((unsigned char)*start)) start++;

    // Trim cuối
    char* end = start + strlen(start);
    while (end > start && isspace((unsigned char)*(end - 1))) end--;
    *end = '\0';

    // Nếu có trim đầu, dời chuỗi
    if (start != s) memmove(s, start, end - start + 1);
}

bool gsm_parser_is_empty_line(const char* resp) {
    if (!resp) return true;

    while (*resp) {
        if (!isspace((unsigned char)*resp)) return false;
        resp++;
    }
    return true;
}

bool gsm_parser_is_ok(const char* resp) {
    if (!resp) return false;

    char buf[32];
    strncpy(buf, resp, sizeof(buf) - 1);
    buf[sizeof(buf) - 1] = '\0';

    gsm_parser_trim(buf);

    return (strcmp(buf, "OK") == 0);
}

bool gsm_parser_is_sim_error(const char* resp) {
    if (!resp) return false;

    char buf[64];
    strncpy(buf, resp, sizeof(buf) - 1);
    buf[sizeof(buf) - 1] = '\0';

    gsm_parser_trim(buf);

    return strstr(buf, "SIM not inserted") ||
           strstr(buf, "+CME ERROR: 10") ||
           strstr(buf, "SIM failure") ||
           strstr(buf, "SIM PIN required");
}

bool gsm_parser_is_error(const char* resp) {
    if (!resp || gsm_parser_is_empty_line(resp)) return false;

    if (gsm_parser_is_ok(resp)) return false;

    char buf[64];
    strncpy(buf, resp, sizeof(buf) - 1);
    buf[sizeof(buf) - 1] = '\0';

    gsm_parser_trim(buf);

    return strstr(buf, "ERROR") ||
           strstr(buf, "+CME ERROR") ||
           strstr(buf, "+CMS ERROR");
}

bool gsm_parser_is_connect(const char* resp) {
    if (!resp) return false;

    char buf[64];
    strncpy(buf, resp, sizeof(buf) - 1);
    buf[sizeof(buf) - 1] = '\0';

    gsm_parser_trim(buf);

    return strstr(buf, "CONNECT") ||
           strstr(buf, "+CGEV: EPS PDN ACT");
}

bool gsm_parser_has_prefix(const char* resp, const char* prefix) {
    if (!resp || !prefix) return false;

    char buf[64];
    strncpy(buf, resp, sizeof(buf) - 1);
    buf[sizeof(buf) - 1] = '\0';

    gsm_parser_trim(buf);

    return strncmp(buf, prefix, strlen(prefix)) == 0;
}
