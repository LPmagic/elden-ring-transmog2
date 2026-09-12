#pragma once

#include <filesystem>
#include <string>
#include <vector>

namespace ertransmogrify {
namespace config {

/**
 * A saved outfit: a display name plus a protector id for each armor slot.
 * A value of -1 means "leave this slot as bare/invisible".
 */
struct transmog_set_st {
    std::wstring name;
    int head_protector_id{-1};
    int chest_protector_id{-1};
    int arms_protector_id{-1};
    int legs_protector_id{-1};
};

/**
 * Load user preferences from an .ini file
 */
void load(const std::filesystem::path &ini_path);

/**
 * Load saved outfits (the [sets] section) from a separate .ini file, e.g.
 * ertransmogrify_sets.ini. Kept separate from load() so sets can be edited/backed up/shared
 * independently of the main settings file. Replaces whatever was previously in `sets`.
 */
void load_sets(const std::filesystem::path &ini_path);

/**
 * User-defined saved outfits, loaded from the [sets] section of the ini file. Exposed so that
 * ertransmogrify_shop.cpp can build a shop menu listing them.
 */
extern std::vector<transmog_set_st> sets;

/**
 * Don't require owning an armor piece to unlock it for transmogrification
 */
extern bool include_unobtained_armor;

/**
 * Include cut content armor as transmogrification options (e.g. Brave's Battlewear)
 */
extern bool include_cut_armor;

/**
 * Add the "Tranmsogrify armor" option to the Site of Grace menu. This can be disabled by mod
 * authors to add their entrypoint for transmog.
 *
 * Note that this isn't in the default ertransmogrify.ini, because it's not intended to be changed
 * by mod players. Everyone who needs to use this already knows about it.
 */
extern bool patch_grace_talk_script;

/**
 * Delay (in milliseconds) before enabling the mod. This can help workaround a Seamless Co-op
 * compatibility issue where Seamless overrides some hooks if it initializes after this mod.
 */
extern unsigned int initialize_delay;

/**
 * Disables networking, for PVP reasons.
 */
extern bool client_side_only;

/**
 * Enables debug console
 */
extern bool debug;
};
};
