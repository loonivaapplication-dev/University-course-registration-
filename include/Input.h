#ifndef INPUT_H
#define INPUT_H

#include <iostream>
#include <string>
#include <vector>

// A text value is safe to store if it has no '|' separator and no line break.
inline bool safeText(const std::string& value) {
    return value.find('|') == std::string::npos &&
           value.find('\n') == std::string::npos &&
           value.find('\r') == std::string::npos;
}

// Convert text such as "42" into a number. False if empty, too long or not all digits.
inline bool parseNumber(const std::string& text, int& value) {
    if (text.empty() || text.size() > 6) return false;
    int result = 0;
    for (size_t i = 0; i < text.size(); i++) {
        if (text[i] < '0' || text[i] > '9') return false;
        result = result * 10 + (text[i] - '0');
    }
    value = result;
    return true;
}

// Split "a|b|c" into {"a","b","c"}. Empty pieces are kept.
inline std::vector<std::string> splitFields(const std::string& line) {
    std::vector<std::string> fields;
    size_t start = 0;
    while (true) {
        size_t pos = line.find('|', start);
        if (pos == std::string::npos) {
            fields.push_back(line.substr(start));
            break;
        }
        fields.push_back(line.substr(start, pos - start));
        start = pos + 1;
    }
    return fields;
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
        if (parseNumber(line, value)) return true;
        std::cout << "Invalid input! Please enter a number.\n";
    }
}

#endif
