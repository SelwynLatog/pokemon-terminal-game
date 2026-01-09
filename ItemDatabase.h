#ifndef ITEM_DATABASE_H
#define ITEM_DATABASE_H

#include <string>
#include <vector>

using namespace std;

/*****************************************************************************
 * CONSTANTS
 *****************************************************************************/
namespace ItemConfig {
    // Default starter inventory quantities
    constexpr int DEFAULT_POKEBALL_COUNT = 5;
    constexpr int DEFAULT_POTION_COUNT = 3;
    constexpr int DEFAULT_SUPER_POTION_COUNT = 1;
    
    // Healing amounts
    constexpr int POTION_HEAL = 20;
    constexpr int SUPER_POTION_HEAL = 50;
    
    // Shop prices
    constexpr int PRICE_POKEBALL = 200;
    constexpr int PRICE_POTION = 300;
    constexpr int PRICE_SUPER_POTION = 700;
}

/*****************************************************************************
 * ENUMS
 *****************************************************************************/
enum class ItemType {
    HEALING,
    POKEBALL
};

/*****************************************************************************
 * ITEM STRUCT
 *****************************************************************************/
struct Item {
    string itemName;
    ItemType itemType;
    int healAmount;
    int quantity;

    Item() 
        : itemName("")
        , itemType(ItemType::HEALING)
        , healAmount(0)
        , quantity(0) 
    {}

    Item(const string& name, ItemType type, int heal, int qty)
        : itemName(name)
        , itemType(type)
        , healAmount(heal)
        , quantity(qty) 
    {}

    static vector<Item> createDefaultItemSet() {
        return {
            Item("POKEBALL",     ItemType::POKEBALL, 0,                          ItemConfig::DEFAULT_POKEBALL_COUNT),
            Item("POTION",       ItemType::HEALING,  ItemConfig::POTION_HEAL,       ItemConfig::DEFAULT_POTION_COUNT),
            Item("SUPER POTION", ItemType::HEALING,  ItemConfig::SUPER_POTION_HEAL, ItemConfig::DEFAULT_SUPER_POTION_COUNT)
        };
    }
};

/*****************************************************************************
 * SHOP ITEM STRUCT
 *****************************************************************************/
struct ShopItem {
    string name;
    ItemType type;
    int healAmount;
    int price;

    ShopItem(const string& n, ItemType t, int heal, int p)
        : name(n)
        , type(t)
        , healAmount(heal)
        , price(p) 
    {}
};

/*****************************************************************************
 * POKEMART INVENTORY
 *****************************************************************************/
static inline vector<ShopItem> getPokemartInventory() {
    return {
        ShopItem("POKEBALL",     ItemType::POKEBALL, 0,                            ItemConfig::PRICE_POKEBALL),
        ShopItem("POTION",       ItemType::HEALING,  ItemConfig::POTION_HEAL,       ItemConfig::PRICE_POTION),
        ShopItem("SUPER POTION", ItemType::HEALING,  ItemConfig::SUPER_POTION_HEAL, ItemConfig::PRICE_SUPER_POTION)
    };
}

#endif // ITEM_DATABASE_H