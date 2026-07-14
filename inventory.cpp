#include <ftxui/component/component.hpp>       // For UI components
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>              // For text, hbox, vbox
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

// Structure to represent an inventory item
struct Item {
    std::string name;
    int quantity;
};

// Inventory class
class Inventory {
public:
    void AddItem(const std::string& name, int qty) {
        if (qty <= 0) return; // Ignore invalid quantities
        for (auto& item : items) {
            if (item.name == name) {
                item.quantity += qty;
                return;
            }
        }
        items.push_back({name, qty});
    }

    void RemoveItem(const std::string& name, int qty) {
        if (qty <= 0) return;
        for (auto it = items.begin(); it != items.end(); ++it) {
            if (it->name == name) {
                it->quantity -= qty;
                if (it->quantity <= 0) {
                    items.erase(it);
                }
                return;
            }
        }
    }

    const std::vector<Item>& GetItems() const {
        return items;
    }

private:
    std::vector<Item> items;
};

int main() {
    using namespace ftxui;

    Inventory inventory;
    inventory.AddItem("Sword", 1);
    inventory.AddItem("Potion", 5);
    inventory.AddItem("Shield", 1);

    std::string new_item_name;
    std::string qty_str;
    std::string remove_item_name;
    std::string remove_qty_str;

    auto screen = ScreenInteractive::Fullscreen();

    // Renderer for inventory list
    auto inventory_renderer = Renderer([&] {
        Elements elements;
        elements.push_back(text("=== Inventory ===") | bold | center);
        for (const auto& item : inventory.GetItems()) {
            elements.push_back(text(item.name + " x" + std::to_string(item.quantity)));
        }
        if (inventory.GetItems().empty()) {
            elements.push_back(text("(Empty)") | dim);
        }
        return vbox(std::move(elements)) | border;
    });

    // Input fields for adding items
    auto add_item_input = Input(&new_item_name, "Item name");
    auto add_qty_input = Input(&qty_str, "Quantity");

    auto add_button = Button("Add Item", [&] {
        try {
            int qty = std::stoi(qty_str);
            if (!new_item_name.empty() && qty > 0) {
                inventory.AddItem(new_item_name, qty);
                new_item_name.clear();
                qty_str.clear();
            }
        } catch (...) {
            // Ignore invalid input
        }
    });

    // Input fields for removing items
    auto remove_item_input = Input(&remove_item_name, "Item name");
    auto remove_qty_input = Input(&remove_qty_str, "Quantity");

    auto remove_button = Button("Remove Item", [&] {
        try {
            int qty = std::stoi(remove_qty_str);
            if (!remove_item_name.empty() && qty > 0) {
                inventory.RemoveItem(remove_item_name, qty);
                remove_item_name.clear();
                remove_qty_str.clear();
            }
        } catch (...) {
            // Ignore invalid input
        }
    });

    // Layout
    auto layout = Container::Vertical({
        inventory_renderer,
        Container::Horizontal({ add_item_input, add_qty_input, add_button }),
        Container::Horizontal({ remove_item_input, remove_qty_input, remove_button }),
        Button("Quit", screen.ExitLoopClosure())
    });

    auto main_renderer = Renderer(layout, [&] {
        return vbox({
            inventory_renderer->Render(),
            hbox({ text("Add: "), add_item_input->Render(), add_qty_input->Render(), add_button->Render() }),
            hbox({ text("Remove: "), remove_item_input->Render(), remove_qty_input->Render(), remove_button->Render() }),
            separator(),
            hbox({ Button("Quit", screen.ExitLoopClosure())->Render() | center })
        }) | border;
    });

    screen.Loop(main_renderer);
    return 0;
}