// Logs Process

#include <unistd.h>

#include <array>
#include <boost/program_options.hpp>
#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <regex>
#include <set>
#include <string>

#include "version.h"

const std::basic_string<char> TERM_COLOR_CLEAR_LINE{"\33[2K"};
const std::basic_string<char> TERM_COLOR_NO_COLOR{"\033[0m"};
const std::basic_string<char> TERM_COLOR_BLACK{"\033[0;30m"};
const std::basic_string<char> TERM_COLOR_GRAY{"\033[1;30m"};
const std::basic_string<char> TERM_COLOR_RED{"\033[0;31m"};
const std::basic_string<char> TERM_COLOR_LIGHT_RED{"\033[1;31m"};
const std::basic_string<char> TERM_COLOR_GREEN{"\033[0;32m"};
const std::basic_string<char> TERM_COLOR_LIGHT_GREEN{"\033[1;32m"};
const std::basic_string<char> TERM_COLOR_BROWN{"\033[0;33m"};
const std::basic_string<char> TERM_COLOR_YELLOW{"\033[1;33m"};
const std::basic_string<char> TERM_COLOR_BLUE{"\033[0;34m"};
const std::basic_string<char> TERM_COLOR_LIGHT_BLUE{"\033[1;34m"};
const std::basic_string<char> TERM_COLOR_PURPLE{"\033[0;35m"};
const std::basic_string<char> TERM_COLOR_LIGHT_PURPLE{"\033[1;35m"};
const std::basic_string<char> TERM_COLOR_CYAN{"\033[0;36m"};
const std::basic_string<char> TERM_COLOR_LIGHT_CYAN{"\033[1;36m"};
const std::basic_string<char> TERM_COLOR_LIGHT_GRAY{"\033[1;37m"};
const std::basic_string<char> TERM_COLOR_WHITE{"\033[0;37m"};

static const std::string NAME_SOFTWARE = "Log process";

struct Args {
    bool flow = false;
    bool color = false;
    double seconde = 0.01;
};

void show_new_process(const std::set<uint64_t>& new_pids, const bool color);
void update_new_pids(const std::set<uint64_t>& old_pids,
                     const std::set<uint64_t>& curr_pids,
                     std::set<uint64_t>& new_pids);
void update_old_pids(const std::set<uint64_t>& curr_pids,
                     std::set<uint64_t>& old_pids);
void get_cmdline(uint64_t pid, std::string& cmdline);
void get_pids(std::set<uint64_t>& pids);
Args args_parser(int argc, char** argv);

Args args_parser(int argc, char** argv) {
    Args ret_args;

    boost::program_options::variables_map vm;
    boost::program_options::options_description parser(
        "**" + std::string(NAME_SOFTWARE) + "**" +
        " optio"
        "ns");
    parser.add_options()("help,h", "help message")("version,v", "version")(
        "flow,f",
        "continuously "
        "displays new "
        "processes")("color,c",
                     "continuously "
                     "displays new "
                     "processes")(
        "seconde,s", boost::program_options::value<double>(&ret_args.seconde),
        "waiting time "
        "between two "
        "analysis of "
        "the running "
        "processes "
        "(default : "
        "\"0.01s\")");
    try {
        boost::program_options::store(
            boost::program_options::parse_command_line(argc, argv, parser), vm);
        boost::program_options::notify(vm);
    } catch (const boost::program_options::error& ex) {
        throw std::runtime_error(std::string{} + __FILE__ + ":" +
                                 std::to_string(__LINE__) +
                                 " [-] "
                                 "error "
                                 "program_"
                                 "options " +
                                 ex.what());
    }

    if (vm.count("flow")) {
        ret_args.flow = true;
    }

    if (vm.count("color")) {
        ret_args.color = true;
    }

    if (vm.count("versio"
                 "n")) {
        version(NAME_SOFTWARE);
        exit(EXIT_SUCCESS);
    }

    if (vm.count("help")) {
        std::cout << parser << std::endl;
        exit(EXIT_SUCCESS);
    }

    return ret_args;
};

void get_pids(std::set<uint64_t>& pids) {
    std::regex str_expr("/proc/[0-9]+");

    std::string path = "/proc";
    for (const auto& entry : std::filesystem::directory_iterator(path)) {
        if (std::regex_match(entry.path().c_str(), str_expr)) {
            pids.insert(std::stoull(entry.path().filename()));
        }
    }
};

void get_cmdline(uint64_t pid, std::string& cmdline) {
    std::ifstream procFile;
    procFile.open("/proc/" + std::to_string(pid) + "/cmdline");
    getline(procFile, cmdline);
    procFile.close();

    for (uint64_t i = 0; i < cmdline.size(); i++) {
        if (cmdline[i] == 0) {
            cmdline[i] = ' ';
        }
    }
};

void update_old_pids(const std::set<uint64_t>& curr_pids,
                     std::set<uint64_t>& old_pids) {
    for (const auto& pid : old_pids) {
        if (!curr_pids.contains(pid)) {
            old_pids.erase(pid);
        }
    }
    for (const auto& pid : curr_pids) {
        if (!old_pids.contains(pid)) {
            old_pids.insert(pid);
        }
    }
};

void update_new_pids(const std::set<uint64_t>& old_pids,
                     const std::set<uint64_t>& curr_pids,
                     std::set<uint64_t>& new_pids) {
    for (const auto& pid : curr_pids) {
        if (!old_pids.contains(pid)) {
            new_pids.insert(pid);
        }
    }
};

void show_new_process(const std::set<uint64_t>& new_pids, const bool color) {
    for (const auto& pid : new_pids) {
        std::string cmdline;
        get_cmdline(pid, cmdline);
        const auto now = std::chrono::system_clock::now();
        const std::time_t t_c = std::chrono::system_clock::to_time_t(now);
        char
            timeString[std::size("yy"
                                 "yy"
                                 "-m"
                                 "m-"
                                 "dd"
                                 "Th"
                                 "h:"
                                 "mm"
                                 ":s"
                                 "s"
                                 "Z")];
        std::strftime(std::data(timeString), std::size(timeString),
                      "%FT%"
                      "TZ",
                      std::gmtime(&t_c));
        if (color) {
            std::cout << TERM_COLOR_YELLOW << timeString << TERM_COLOR_NO_COLOR
                      << " " << TERM_COLOR_LIGHT_BLUE << pid
                      << TERM_COLOR_NO_COLOR << " " << cmdline << std::endl;
        } else {
            std::cout << timeString << " " << pid << " " << cmdline
                      << std::endl;
        }
    }
};

int main(int argc, char** argv) {
    Args args = args_parser(argc, argv);
    std::set<uint64_t> old_pids;   // t - 1
    std::set<uint64_t> curr_pids;  // t
    std::set<uint64_t> new_pids;   // Append
                                   // PID in
                                   // old_pids

    // init
    get_pids(curr_pids);
    old_pids = curr_pids;
    new_pids = curr_pids;
    show_new_process(new_pids, args.color);
    new_pids.clear();
    usleep(static_cast<useconds_t>(args.seconde * 1000000));

    while (args.flow) {
        get_pids(curr_pids);
        update_new_pids(old_pids, curr_pids, new_pids);
        update_old_pids(curr_pids, old_pids);
        show_new_process(new_pids, args.color);
        new_pids.clear();
        usleep(static_cast<useconds_t>(args.seconde * 1000000));
    }
    return EXIT_SUCCESS;
}