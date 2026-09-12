#pragma once

#include <map>

namespace ertransmogrify {
namespace shop {
static constexpr unsigned int item_type_protector_begin = 0x10000000;
static constexpr unsigned int item_type_goods_begin = 0x40000000;
static constexpr unsigned int item_type_goods_end = 0x50000000;
static constexpr unsigned char protector_category_head = 0;
static constexpr unsigned char protector_category_chest = 1;
static constexpr unsigned char protector_category_arms = 2;
static constexpr unsigned char protector_category_legs = 3;
static constexpr long long bare_head_protector_id = 10000;
static constexpr long long bare_chest_protector_id = 10100;
static constexpr long long bare_arms_protector_id = 10200;
static constexpr long long bare_legs_protector_id = 10300;
static constexpr long long transmog_goods_start_id = 6900000;
static constexpr long long transmog_goods_end_id = 7000000;
// Deliberately placed just past transmog_goods_end_id (rather than inside the 6900000-7000000
// range) so it can never collide with a real protector's transmog goods id, no matter how high
// protector ids go in the future (get_transmog_goods_id_for_protector = 6900000 + id/100).
static constexpr long long transmog_set_goods_start_id = 7000000;
static constexpr long long transmog_head_shop_menu_id = 4000000;
static constexpr long long transmog_chest_shop_menu_id = 4100000;
static constexpr long long transmog_arms_shop_menu_id = 4200000;
static constexpr long long transmog_legs_shop_menu_id = 4300000;
static constexpr long long transmog_sets_shop_menu_id = 4400000;
static constexpr long long transmog_shop_max_size = 100000;

/**
 * DLC transformations. These protectors have no item names or icons, so they need an explicit
 * mapping to the goods that hold this data.
 */
extern const std::map<unsigned long long, unsigned long long>
    dlc_transformation_goods_by_protector_id;

/**
 * Hook the internal get_*_param() functions to return the new params used for the transmog shops
 */
void initialize();

/**
 * Remove transmog shop items from the player's inventory
 */
void remove_transmog_goods(signed char protector_category = -1);

/**
 * Add a transmog good to the player's inventory
 */
void add_transmog_good(unsigned long long protector_id);

/**
 * Apply a saved set (from config::sets) by its index: clears each armor slot, then equips the
 * transmog good for each of the set's 4 protector ids (skipping slots set to -1).
 */
void apply_transmog_set(size_t set_index);

inline bool is_invisible_protector_id(long long protector_id) {
    return protector_id == bare_head_protector_id || protector_id == bare_chest_protector_id ||
           protector_id == bare_arms_protector_id || protector_id == bare_legs_protector_id;
}

inline long long get_transmog_goods_id_for_protector(long long protector_id) {
    // Ignore armor IDs with upgrade level digits set, since these digits should be 0 in Elden Ring,
    // and transmog good IDs don't take them into account
    if (protector_id % 100 != 0) {
        return -1;
    }

    return transmog_goods_start_id + protector_id / 100;
}

inline long long get_protector_id_for_transmog_good(long long goods_id) {
    if (goods_id >= transmog_goods_start_id && goods_id < transmog_goods_end_id) {
        return 100 * (goods_id - transmog_goods_start_id);
    }

    return -1;
}

}
}
