#include "gsm_parser.h"
#include <stdbool.h>
#include <string.h>
#include "gsm_state.h"

bool check_gsm_response_by_index(int idx, const char* response) {
    switch (idx) {
        case 0: // AT
            return parse_response_at(response);
        case 1: // ATI
            return parse_response_ati(response);
        case 2: // AT+CPIN?
            return parse_response_at_cpin(response);
        case 3: // AT+CSQ
            return parse_response_at_csq(response);
        case 4: // AT+CEREG?
            return parse_response_at_cereg(response);
        case 5: // AT+COPS
            return parse_response_at_cops(response);
        case 6: // AT+CGDCONT
            return parse_response_at_cgdcont(response);
        case 7: // AT+CGAUTH
            return parse_response_at_cgauth(response);
        case 8: // AT+CGACT
            return parse_response_at_cgact(response);
        case 9: // AT+CGPADDR
            return parse_response_at_cgpaddr(response);
        case 10: // ATD*99#
            return parse_response_atd99(response);
        // case 11: // +++
        //     return parse_response_plus(response);
        // case 12: // ATH
        //     return parse_response_ath(response);
        default:
            return false;
    }
}