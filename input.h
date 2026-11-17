#ifndef INPUT_H
#define INPUT_H

#include <iostream>
#include <string>

// A text value is safe to store if it has no '|' separator and no line break.
inline bool safeText(const std::string& value) {
    return value.find('|') == std::string::npos &&
           value.find('\n') == std::string::npos &&
           value.find('\r') == std::string::npos;
}

// Show a prompt and read one whole line. Returns false if input has ended (EOF).
inline bool readLine(const std::string& prompt, std::string& value) {
    std::cout << prompt;
    if (!std::getline(std::cin, value)) return false;
    if (!value.empty() && value[value.size() - 1] == '\r') value.erase(value.size() - 1);
    return true;
}

// Show a prompt and read a whole number. Asks again on bad input.
// Returns false only if input has ended (EOF).
inline bool readInt(const std::string& prompt, int& value) {
    std::string line;
    while (true) {
        if (!readLine(prompt, line)) return false;
        if (!line.empty() && line.size() <= 6) {
            bool ok = true;
            for (size_t i = 0; i < line.size(); i++) {
                if (line[i] < '0' || line[i] > '9') ok = false;
            }
            if (ok) {
                value = std::stoi(line);
                return true;
            }
        }
        std::cout << "Invalid input! Please enter a number.\n";
    }
}

#endif