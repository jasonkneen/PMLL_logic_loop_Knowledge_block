#include "pmll.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <mutex>

// Constructor: Initializes the memory file and loads any existing memory
PMLL::PMLL(const std::string& file_name) : memory_file(file_name) {
    loadMemory();  // Load memory from the file on initialization
}

// Destructor: Ensures memory is saved when the object is destroyed
PMLL::~PMLL() {
    saveMemory();  // Save memory to the file when the object is destroyed
}

// Load memory from the file into the unordered_map
void PMLL::loadMemory() {
    std::lock_guard<std::mutex> lock(memory_mutex);  // Ensure thread safety
    
    std::ifstream infile(memory_file);
    if (infile.is_open()) {
        std::string key, value;
        while (std::getline(infile, key) && std::getline(infile, value)) {
            memory[key] = value;  // Populate the memory map with key-value pairs
        }
        infile.close();
    } else {
        std::cerr << "Error: Could not open memory file for reading.\n";
    }
}

// Save the current memory state to the file
void PMLL::saveMemory() {
    std::lock_guard<std::mutex> lock(memory_mutex);  // Ensure thread safety
    
    std::ofstream outfile(memory_file);
    if (outfile.is_open()) {
        for (const auto& pair : memory) {
            outfile << pair.first << '\n' << pair.second << '\n';  // Write each key-value pair
        }
        outfile.close();
    } else {
        std::cerr << "Error: Could not open memory file for writing.\n";
    }
}

// Add or update a memory entry
void PMLL::addMemory(const std::string& key, const std::string& value) {
    std::lock_guard<std::mutex> lock(memory_mutex);  // Ensure thread safety
    
    memory[key] = value;  // Add or update the key-value pair
}

// Retrieve a memory entry by key
std::string PMLL::getMemory(const std::string& key) const {
    std::lock_guard<std::mutex> lock(memory_mutex);  // Ensure thread safety
    
    auto it = memory.find(key);
    if (it != memory.end()) {
        return it->second;  // Return the value associated with the key
    }
    return "";  // Return an empty string if the key doesn't exist
}

// Clear all memory entries
void PMLL::clearMemory() {
    std::lock_guard<std::mutex> lock(memory_mutex);  // Ensure thread safety
    
    memory.clear();  // Clear the entire unordered_map
}

// Display all memory entries (for debugging purposes)
void PMLL::displayMemory() const {
    std::lock_guard<std::mutex> lock(memory_mutex);  // Ensure thread safety
    
    if (memory.empty()) {
        std::cout << "Memory is empty.\n";
    } else {
        for (const auto& pair : memory) {
            std::cout << "Key: " << pair.first << " | Value: " << pair.second << '\n';
        }
    }
}
