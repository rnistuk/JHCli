#include <gtest/gtest.h>
#include "../src/data_parser.h"

std::string tm_to_string(const struct tm &t) {
    std::stringstream ss;
    ss << t.tm_mon + 1 << "/" << t.tm_mday << "/" << (1900 + t.tm_year) - 2000;
    return ss.str();
}

TEST(data_parser, test_string_to_date) {
    std::stringstream tss{
            "|1/22/20|1/23/20|1/24/20|1/25/20|1/26/20|1/27/20|1/28/20|1/29/20|1/30/20|1/31/20|2/1/20|2/2/20|2/3/20|2/4/20|2/5/20|2/6/20|2/7/20|2/8/20|2/9/20|2/10/20|2/11/20|2/12/20|2/13/20|2/14/20|2/15/20|2/16/20|2/17/20|2/18/20|2/19/20|2/20/20|2/21/20|2/22/20|2/23/20|2/24/20|2/25/20|2/26/20|2/27/20|2/28/20|2/29/20|3/1/20|3/2/20|3/3/20|3/4/20|3/5/20|3/6/20|3/7/20|3/8/20|3/9/20|3/10/20|3/11/20|3/12/20|3/13/20|3/14/20|3/15/20|3/16/20|3/17/20|3/18/20|3/19/20|3/20/20|3/21/20|3/22/20|3/23/20|3/24/20|3/25/20|3/26/20|3/27/20|3/28/20|3/29/20|3/30/20|3/31/20|4/1/20|4/2/20|4/3/20|4/4/20|4/5/20|4/6/20|4/7/20|4/8/20|4/9/20|4/10/20|4/11/20|4/12/20|4/13/20|4/14/20|4/15/20|4/16/20|4/17/20|4/18/20|4/19/20"};
    while (tss) {
        std::string accepted;
        std::getline(tss, accepted,'|');
        if (accepted.empty()) {
            continue;
        }
        ASSERT_EQ(accepted, tm_to_string(string_to_date(accepted)));
    }
}

TEST(data_parser, test_dates_from_titles) {
    const std::vector<std::string> titles{"Province/State", "Country/Region", "Lat", "Long", "1/22/20", "1/23/20", "1/24/20", "1/25/20", "1/26/20"};
    const auto actual = dates_from_titles(titles);

    ASSERT_EQ(titles.size() - 4, actual.size());
    for (int i{0}; i < titles.size() - 4; ++i) {
        ASSERT_EQ(titles[i + 4], tm_to_string(actual[i]));
    }
}

TEST(data_parser, test_re_delimit_to_pipes) {
    std::string src{R"tag("don't,break",break, this, too, "but,not,this",last part)tag"};
    std::string accepted{R"tag("don't,break"|break| this| too| "but,not,this"|last part)tag"};

    ASSERT_EQ(accepted, re_delimit_to_pipes(src));
}

TEST(data_parser, test_parse_stream_to_lines) {
    std::vector<std::string> accepted{
            "Province/State|Country/Region|Lat|Long|1/22/20|1/23/20|1/24/20|1/25/20|1/26/20|1/27/20|1/28/20|1/29/20|1/30/20|1/31/20|2/1/20|2/2/20|2/3/20|2/4/20|2/5/20|2/6/20|2/7/20|2/8/20|2/9/20|2/10/20|2/11/20|2/12/20|2/13/20|2/14/20|2/15/20|2/16/20|2/17/20|2/18/20|2/19/20|2/20/20|2/21/20|2/22/20|2/23/20|2/24/20|2/25/20|2/26/20|2/27/20|2/28/20|2/29/20|3/1/20|3/2/20|3/3/20|3/4/20|3/5/20|3/6/20|3/7/20|3/8/20|3/9/20|3/10/20|3/11/20|3/12/20|3/13/20|3/14/20|3/15/20|3/16/20|3/17/20|3/18/20|3/19/20|3/20/20|3/21/20|3/22/20|3/23/20|3/24/20|3/25/20|3/26/20|3/27/20|3/28/20|3/29/20|3/30/20|3/31/20|4/1/20|4/2/20|4/3/20|4/4/20|4/5/20|4/6/20|4/7/20|4/8/20|4/9/20|4/10/20|4/11/20|4/12/20|4/13/20|4/14/20|4/15/20|4/16/20|4/17/20|4/18/20|4/19/20",
            "|Afghanistan|33.0|65.0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|1|1|1|1|1|1|1|1|1|2|2|2|2|2|2|5|5|10|10|10|15|18|18|29|32|32|32|32|32|40|43|54|99|112|131"
    };

    std::filesystem::path tmp = std::filesystem::temp_directory_path();
    tmp.append("test.txt");
    std::ofstream out(tmp.c_str());
    for (const auto &s : accepted) {
        out << s << std::endl;
    }
    out.close();

    std::ifstream in(tmp.c_str());
    const auto actual = parse_stream_to_lines(in);
    in.close();

    std::filesystem::remove(tmp);

    ASSERT_EQ(accepted.size(), actual.size());
    ASSERT_EQ(accepted[0], actual[0]);
    ASSERT_EQ(accepted[1], actual[1]);
}

TEST(data_parser, test_parse_titles) {
    const std::string title_line{"Province/State|Country/Region|Lat|Long|1/22/20|1/23/20|1/24/20|1/25/20"};
    const std::vector<std::string> accepted{"Province/State", "Country/Region", "Lat", "Long", "1/22/20", "1/23/20", "1/24/20", "1/25/20"};
    const auto actual = parse_titles(title_line);
    ASSERT_EQ(accepted.size(), actual.size());

    for (int i{0}; i < accepted.size(); ++i) {
        ASSERT_EQ(actual[i], actual[i]);
    }
}

TEST(data_parser, test_parse_regions) {
    const std::vector<std::string> titles{"Province/State", "Country/Region", "Lat", "Long", "1/22/20", "1/23/20",
                                          "1/24/20", "1/25/20"};
    data_lines lines{
            "|countryname|33.0|65.0|1|2|4|8",
            "provincename|country 02|41.1533|20.1683|0|0|0|0",
            "\"test,this\"|Algeria|28.0339|1.6596|0|0|0|0",
            "|Antigua and Barbuda|17.0608|-61.7964|0|0|0|0"
    };

    const auto actual{parse_regions(titles, lines)};
    ASSERT_EQ(lines.size(), actual.size());
    ASSERT_EQ("provincename", actual[1].province);
    ASSERT_EQ("Antigua and Barbuda", actual[3].country);
    ASSERT_EQ(8, actual[0].data[3].people);
}

TEST(data_parser, test_pop_front) {
    std::deque<std::string> q{"a", "b", "c"};
    auto actual = pop_front(q);

    ASSERT_EQ("a", actual);
    ASSERT_EQ(q.size(), 2);
}

TEST(data_parser, test_parse_argument) {
    {
        const char *in{"-province=\"British Columbia\""};
        std::string accepted_arg{"-province"};
        std::string accepted_val{"British Columbia"};
        const auto actual{parse_argument(in)};

        ASSERT_EQ(accepted_arg, actual.first);
        ASSERT_EQ(accepted_val, actual.second);
    }

    {
        const char *in{"-country=Canada"};
        std::string accepted_arg{"-country"};
        std::string accepted_val{"Canada"};
        const auto actual{parse_argument(in)};

        ASSERT_EQ(accepted_arg, actual.first);
        ASSERT_EQ(accepted_val, actual.second);
    }
}
