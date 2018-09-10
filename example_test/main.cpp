#include <QCoreApplication>
#include "metainfo/declare_types_as_tuple.hpp"
#include <qdebug.h>

struct Point
{
    float x;
    float y;
    Point(float x = 0.f, float y = 0.f): x(x), y(y) {}

    DECL_TYPES_AS_TUPLE(Point, x, y)
};

struct Data
{
    int m_days;
    int m_hours;
    int m_minutes;
    std::string m_format;

    std::set<int> m_vec;
    std::map<int, int> m_map;
    std::deque<Point> m_vec1;

    DECL_TYPES_AS_TUPLE(Data, m_days, m_hours, m_minutes, m_format, m_vec, m_map, m_vec1)

    Data()
        : m_days(0), m_hours(0), m_minutes(0), m_format("dd or hh.mm")
    {
        for (int i = 0; i < 2; i++) {
            m_vec.insert(rand() % 22 + 10);
            m_map.insert(std::make_pair(rand() % 100, rand() % 100));
            m_vec1.push_back(Point(float(rand() % 100)/20.f, float(rand() % 100)/20.f));
        }
    }

};

namespace ns_game
{
    enum class e_game_type  : int8_t { none = -1 };
    enum class e_game_speed : int8_t { none = -1 };
    enum class e_game_limit : int8_t { none = -1 };
    enum class e_tournament_status : int8_t { none = -1 };
    enum class e_tournament_type : int8_t { none = -1 };
}

struct user_info {
    // methods
//    user_info() = default;
//    user_info(const user_info&) = default;
//    user_info& operator=(const user_info&) = default;
//    user_info& operator=(user_info&&) = default;
//    user_info(user_info&&) = default;

    // members

    int64_t _id{};
    std::string _nickname{};
    int32_t _country{};
    bool _is_finished{};
    bool _is_ticket{};
    int64_t _money{};
    int32_t _bounty_knocked_out_count{};
    int64_t _bounty_progr_money{};

    DECL_TYPES_AS_TUPLE(user_info
                      , _id
                      , _nickname
                      , _country
                      , _is_finished
                      , _is_ticket
                      , _money
                      , _bounty_knocked_out_count
                      , _bounty_progr_money)
};

struct tourn_reg_user_info {
    int64_t _id{};
    bool _is_finished{};

//    tourn_reg_user_info() = default;

    DECL_TYPES_AS_TUPLE(tourn_reg_user_info, _id, _is_finished)
};
struct rebuys_t {
    int64_t _id{};
    bool _enabled{};
    DECL_TYPES_AS_TUPLE(rebuys_t, _id, _enabled)
};
struct addon_t {
    int64_t _id{};
    bool _enabled{};
    DECL_TYPES_AS_TUPLE(addon_t, _id, _enabled)
};

struct tournament_stat_t {
    // methods
//    tournament_stat_t() = default;
//    tournament_stat_t(const tournament_stat_t&) = default;
//    tournament_stat_t& operator=(const tournament_stat_t&) = default;
//    tournament_stat_t& operator=(tournament_stat_t&&) = default;
//    tournament_stat_t(tournament_stat_t&&) = default;

    // members


    int64_t m_id{};
    int8_t m_seats_count{};
    int32_t m_max_players_count{};
    std::vector<tourn_reg_user_info> m_registered_users{};
    int32_t m_min_users_count{};
    int64_t m_main_tournament_id{};
    int64_t m_buyin{};
    int64_t m_fee{};
    int64_t m_big_blind{};
    int64_t m_round_duration{};
    int64_t m_start_stack{};
    int32_t m_prize_places_count{};
    int64_t m_common_prize{};
    int64_t m_bounty_prize{};
    ns_game::e_tournament_type m_types{};
    ns_game::e_game_type m_game_type{};
    ns_game::e_game_speed m_game_speed{};
    ns_game::e_game_limit m_game_limit{};
    std::string m_title{};
    ns_game::e_tournament_status m_status{};
    bool m_is_partner_tourn{};
    int64_t m_registration_start_time{};
    int64_t m_start_time{};
    int64_t m_finish_time{};
    int64_t m_late_registration_duration{};
    std::vector<int64_t> m_qualifiers_ids{};
    rebuys_t m_rebuys{};
    addon_t m_addon{};
    int64_t m_bounty{};
    int16_t m_current_level{};
    int64_t m_next_break_time{};
    int64_t m_current_level_finish_time{};
    std::vector<user_info> m_users{};
    std::vector<int64_t> m_tickets{};
    // operators
    DECL_TYPES_AS_TUPLE(tournament_stat_t
                      , m_id
                      , m_seats_count
                      , m_max_players_count
                      , m_registered_users
                      , m_min_users_count
                      , m_main_tournament_id
                      , m_buyin
                      , m_fee
                      , m_big_blind
                      , m_round_duration
                      , m_start_stack
                      , m_prize_places_count
                      , m_common_prize
                      , m_bounty_prize
                      , m_types
                      , m_game_type
                      , m_game_speed
                      , m_game_limit
                      , m_title
                      , m_status
                      , m_is_partner_tourn
                      , m_registration_start_time
                      , m_start_time
                      , m_finish_time
                      , m_late_registration_duration
                      , m_qualifiers_ids
                      , m_rebuys
                      , m_addon
                      , m_bounty
                      , m_current_level
                      , m_next_break_time
                      , m_current_level_finish_time
                      , m_users
                      , m_tickets)
};

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Point p1(-1.f, 2.1f);
    Point p2(4.7f, 2.1f);

    std::cout << "p1 = " << metafunction::to_json_string(p1) << std::endl;
    std::cout << "p2 = " << metafunction::to_json_string(p2) << std::endl;
    std::cout << "p1.get_member<0>() " << p1.get_member<0>() << std::endl;
    std::cout << "p2.member_name(1) " << p2.member_name(1) << std::endl;
    std::cout << "p1 != p2 " << (p1 != p2 ? "true" : "false") << std::endl;
    std::cout << "p1.get_not_equal_member_indices(p2) " << metafunction::to_json_string(p1.get_not_equal_member_indices(p2)) << std::endl;
    std::cout << "p2.get_not_equal_member_names(p1) " << metafunction::to_json_string(p2.get_not_equal_member_names(p1)) << std::endl;

    std::cout << metafunction::to_json_string(tournament_stat_t()) << std::endl;

    return a.exec();
}
