#include "member_name.hpp"


namespace metafunction {
namespace detail {
/** ------------------------------------------------------------------------------------------- */
std::vector<std::string> wrap_members(const char* str) {
    std::vector<std::string> member_names;
    std::string memName;
    for (const char* it = str; *it != 0; it++) {
        if (*it == ',') {
            member_names.push_back(memName);
            memName.resize(0);
#if CUT_m_FROM_NAMES
        } else if (*it != ' '&& !(memName.empty() && (((*it == 'm') && *(it+1) == '_') || (it != str && *(it-1) == 'm' && *it == '_')))) {
#else
        } else if (*it != ' ') {
#endif
            memName += *it;
        }
    }
    member_names.push_back(memName);
    return member_names;
}
/** ------------------------------------------------------------------------------------------- */
} //namespace detail
} //namespace metafunction
