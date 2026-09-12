#include <mini/ini.h>
#include <spdlog/spdlog.h>

#include "ertransmogrify_config.hpp"

#include <sstream>
#include <string>

namespace fs = std::filesystem;

bool ertransmogrify::config::include_unobtained_armor = true;
bool ertransmogrify::config::include_cut_armor = true;
bool ertransmogrify::config::patch_grace_talk_script = true;
unsigned int ertransmogrify::config::initialize_delay = 0;
bool ertransmogrify::config::client_side_only = false;
std::vector<ertransmogrify::config::transmog_set_st> ertransmogrify::config::sets = {};
#if _DEBUG
bool ertransmogrify::config::debug = true;
#else
bool ertransmogrify::config::debug = false;
#endif

/**
 * Split a comma-separated list of up to 4 protector ids (head,chest,arms,legs). Missing or
 * unparsable entries default to -1 (bare/invisible).
 */
static ertransmogrify::config::transmog_set_st parse_set(const std::string &name_utf8,
                                                          const std::string &value) {
    ertransmogrify::config::transmog_set_st set;

    // Names in the ini are read as UTF-8/ASCII; widen them for use in the game's UTF-16 message
    // strings. This is a naive conversion, so non-ASCII set names aren't supported.
    set.name = std::wstring(name_utf8.begin(), name_utf8.end());

    int *fields[4] = {&set.head_protector_id, &set.chest_protector_id, &set.arms_protector_id,
                       &set.legs_protector_id};

    std::stringstream stream(value);
    std::string token;
    int field_index = 0;
    while (field_index < 4 && std::getline(stream, token, ',')) {
        try {
            *fields[field_index] = std::stoi(token);
        } catch (std::exception const &) {
            SPDLOG_WARN("Couldn't parse protector id \"{}\" for set \"{}\", defaulting to -1",
                        token, name_utf8);
            *fields[field_index] = -1;
        }
        field_index++;
    }

    return set;
}

void ertransmogrify::config::load(const fs::path &ini_path) {
    SPDLOG_INFO("Loading config from {}", ini_path.string());

    mINI::INIFile file(ini_path.string());
    mINI::INIStructure ini;
    if (file.read(ini) && ini.has("ertransmogrify")) {
        auto &config = ini["ertransmogrify"];

        if (config.has("include_unobtained_armor"))
            include_unobtained_armor = config["include_unobtained_armor"] != "false";

        if (config.has("include_cut_armor"))
            include_cut_armor = config["include_cut_armor"] != "false";

        if (config.has("patch_grace_talk_script"))
            patch_grace_talk_script = config["patch_grace_talk_script"] != "false";

        if (config.has("initialize_delay"))
            initialize_delay = stoi(config["initialize_delay"], nullptr, 10);

        if (config.has("client_side_only"))
            client_side_only = config["client_side_only"] != "false";

        if (config.has("debug")) debug = config["debug"] != "false";
    }

    sets.clear();
    if (ini.has("sets")) {
        for (auto &[name, value] : ini["sets"]) {
            sets.push_back(parse_set(name, value));
        }
    }

    SPDLOG_INFO("include_unobtained_armor = {}", include_unobtained_armor);
    SPDLOG_INFO("include_cut_armor = {}", include_cut_armor);
    SPDLOG_INFO("patch_grace_talk_script = {}", patch_grace_talk_script);
    SPDLOG_INFO("initialize_delay = {}", initialize_delay);
    SPDLOG_INFO("client_side_only = {}", client_side_only);
    if (debug) {
        SPDLOG_INFO("debug = true");
    }

    SPDLOG_INFO("Loaded {} saved set(s)", sets.size());
    for (auto &set : sets) {
        SPDLOG_INFO("  set: head={} chest={} arms={} legs={}", set.head_protector_id,
                    set.chest_protector_id, set.arms_protector_id, set.legs_protector_id);
    }
}
