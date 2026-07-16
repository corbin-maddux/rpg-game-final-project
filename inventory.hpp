#pragma once

// This header describes inventory item data and the Inventory class interface.

#include <string>
#include <vector>

enum class ItemType {
    // Each item receives one category so inventory input can distinguish
    // equippable weapons, usable potions, magic books, and inactive objects.
    Weapon,
    Potion,
    Book,
    Miscellaneous
};

struct InventoryItem {
    // These fields contain everything the inventory needs to list, inspect,
    // stack, consume, or equip one kind of item.
    std::string name;
    std::string description;
    ItemType type = ItemType::Miscellaneous;
    int quantity = 1;
    int manaRestore = 0;
    bool restoreFullHealth = false;
    bool restoreFullMana = false;

    // Bonuses used when an ItemType::Weapon is equipped.
    int weaponDamageBonus = 0;
    int weaponSpeedBonus = 0;
};

class Inventory {
public:
    // Adds a new stack or increases an existing stack with the same item name.
    void addItem(const InventoryItem& item);

    const std::vector<InventoryItem>& getItems() const;
    const InventoryItem* getSelectedItem() const;

    // Removes one of the selected consumable and copies it into usedItem.
    bool consumeSelectedItem(InventoryItem& usedItem);

    void selectPrevious();
    void selectNext();

private:
    // Only Inventory functions may directly change its item list and cursor,
    // preventing UI code from leaving selectedIndex outside the list.
    std::vector<InventoryItem> items;
    int selectedIndex = 0;
};

std::string itemTypeName(ItemType type);
