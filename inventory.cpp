#include "inventory.hpp"

// This file implements the Inventory functions declared in inventory.hpp.

#include <algorithm>

void Inventory::addItem(const InventoryItem& newItem) {
    // Ignore invalid drops or purchases that contain no usable copies.
    if (newItem.quantity <= 0) {
        // Stop before the invalid item can be inserted into the inventory.
        return;
    }

    // Search existing stacks so another potion increases quantity instead of
    // creating a duplicate inventory row.
    for (InventoryItem& item : items) {
        if (item.name == newItem.name) {
            item.quantity += newItem.quantity;
            return;
        }
    }

    items.push_back(newItem);
}

const std::vector<InventoryItem>& Inventory::getItems() const {
    // Give the inventory panel read-only access to the real item list without
    // copying it or allowing the panel to modify it.
    return items;
}

const InventoryItem* Inventory::getSelectedItem() const {
    if (items.empty()) {
        return nullptr;
    }

    // Make a safe cursor position even if removing an item shortened the list.
    int safeIndex = std::clamp(
        selectedIndex,
        0,
        static_cast<int>(items.size()) - 1
    );

    return &items[safeIndex];
}

bool Inventory::consumeSelectedItem(InventoryItem& usedItem) {
    if (items.empty()) {
        return false;
    }

    selectedIndex = std::clamp(
        selectedIndex,
        0,
        static_cast<int>(items.size()) - 1
    );

    InventoryItem& selected = items[selectedIndex];

    if (selected.type != ItemType::Potion) {
        return false;
    }

    usedItem = selected;
    --selected.quantity;

    if (selected.quantity <= 0) {
        items.erase(items.begin() + selectedIndex);

        if (selectedIndex >= static_cast<int>(items.size())) {
            selectedIndex = std::max(0, static_cast<int>(items.size()) - 1);
        }
    }

    return true;
}

void Inventory::selectPrevious() {
    if (items.empty()) {
        return;
    }

    --selectedIndex;

    if (selectedIndex < 0) {
        selectedIndex = static_cast<int>(items.size()) - 1;
    }
}

void Inventory::selectNext() {
    if (items.empty()) {
        return;
    }

    ++selectedIndex;

    if (selectedIndex >= static_cast<int>(items.size())) {
        selectedIndex = 0;
    }
}

std::string itemTypeName(ItemType type) {
    // Convert the selected item's category into the word shown under INSPECT.
    switch (type) {
        case ItemType::Weapon:
            return "Weapon";
        case ItemType::Potion:
            return "Potion";
        case ItemType::Book:
            return "Book";
        case ItemType::Miscellaneous:
            return "Miscellaneous";
    }

    return "Unknown";
}
