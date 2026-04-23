#ifndef SKIP_LIST_HPP
#define SKIP_LIST_HPP

#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <climits>

template<typename T>
class SkipList {
private:
    struct Node {
        T value;
        std::vector<Node*> forward;
        
        Node(const T& val, int level) : value(val), forward(level + 1, nullptr) {}
    };
    
    Node* head;
    int maxLevel;
    int currentLevel;
    float probability;
    
    int randomLevel() {
        int level = 0;
        while ((rand() % 100) < (probability * 100) && level < maxLevel) {
            level++;
        }
        return level;
    }

public:
    SkipList() : maxLevel(16), currentLevel(0), probability(0.5) {
        srand(static_cast<unsigned int>(time(nullptr)));
        // Create a sentinel head node with a dummy value
        // We'll use the maximum level for the head node
        head = new Node(T(), maxLevel);
    }

    ~SkipList() {
        Node* current = head;
        while (current != nullptr) {
            Node* next = current->forward[0];
            delete current;
            current = next;
        }
    }

    // Insert a value into the skip list. If the value already exists, do nothing.
    void insert(const T& item) {
        std::vector<Node*> update(maxLevel + 1, nullptr);
        Node* current = head;
        
        // Find the position to insert
        for (int i = currentLevel; i >= 0; i--) {
            while (current->forward[i] != nullptr && current->forward[i]->value < item) {
                current = current->forward[i];
            }
            update[i] = current;
        }
        
        current = current->forward[0];
        
        // If the value already exists, do nothing
        if (current != nullptr && !(current->value < item) && !(item < current->value)) {
            return;
        }
        
        // Generate a random level for the new node
        int newLevel = randomLevel();
        
        // If the new level is higher than current level, update the current level
        if (newLevel > currentLevel) {
            for (int i = currentLevel + 1; i <= newLevel; i++) {
                update[i] = head;
            }
            currentLevel = newLevel;
        }
        
        // Create the new node
        Node* newNode = new Node(item, newLevel);
        
        // Update forward pointers
        for (int i = 0; i <= newLevel; i++) {
            newNode->forward[i] = update[i]->forward[i];
            update[i]->forward[i] = newNode;
        }
    }

    // Search for a value in the skip list
    bool search(const T& item) {
        Node* current = head;
        
        // Start from the highest level and move down
        for (int i = currentLevel; i >= 0; i--) {
            while (current->forward[i] != nullptr && current->forward[i]->value < item) {
                current = current->forward[i];
            }
        }
        
        current = current->forward[0];
        
        // Check if we found the item
        if (current != nullptr && !(current->value < item) && !(item < current->value)) {
            return true;
        }
        
        return false;
    }

    // Delete a value from the skip list. If the value does not exist in the skip list, do nothing.
    void deleteItem(const T& item) {
        std::vector<Node*> update(maxLevel + 1, nullptr);
        Node* current = head;
        
        // Find the node to delete
        for (int i = currentLevel; i >= 0; i--) {
            while (current->forward[i] != nullptr && current->forward[i]->value < item) {
                current = current->forward[i];
            }
            update[i] = current;
        }
        
        current = current->forward[0];
        
        // If the value doesn't exist, do nothing
        if (current == nullptr || current->value < item || item < current->value) {
            return;
        }
        
        // Update forward pointers
        for (int i = 0; i <= currentLevel; i++) {
            if (update[i]->forward[i] != current) {
                break;
            }
            update[i]->forward[i] = current->forward[i];
        }
        
        delete current;
        
        // Update current level if necessary
        while (currentLevel > 0 && head->forward[currentLevel] == nullptr) {
            currentLevel--;
        }
    }
};

#endif
