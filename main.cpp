#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <map>

struct PasswordReport {
    std::string password;
    int score;
    std::string label;
    std::vector<std::string> suggestions;
};

bool hasLower(const std::string& s) {
    for (char c : s) {
        if (std::islower(static_cast<unsigned char>(c))) return true;
    }
    return false;
}

bool hasUpper(const std::string& s) {
    for (char c : s) {
        if (std::isupper(static_cast<unsigned char>(c))) return true;
    }
    return false;
}

bool hasDigit(const std::string& s) {
    for (char c : s) {
        if (std::isdigit(static_cast<unsigned char>(c))) return true;
    }
    return false;
}

bool hasSpecial(const std::string& s) {
    const std::string specials = "!@#$%^&*()-_=+[]{};:'\",.<>/?\\|`~";
    for (char c : s) {
        if (specials.find(c) != std::string::npos) return true;
    }
    return false;
}

bool hasSpace(const std::string& s) {
    for (char c : s) {
        if (std::isspace(static_cast<unsigned char>(c))) return true;
    }
    return false;
}

int computeScore(const std::string& pwd) {
    int score = 0;
    int length = static_cast<int>(pwd.size());

    if (length >= 6 && length <= 7) {
        score += 1;
    } else if (length >= 8 && length <= 10) {
        score += 2;
    } else if (length > 10) {
        score += 3;
    }

    if (hasLower(pwd)) score += 2;
    if (hasUpper(pwd)) score += 2;
    if (hasDigit(pwd)) score += 2;
    if (hasSpecial(pwd)) score += 2;

    if (hasSpace(pwd)) score -= 1;

    if (score < 0) score = 0;
    if (score > 10) score = 10;

    return score;
}

std::string getStrengthLabel(int score) {
    if (score <= 1) {
        return "Very Weak";
    } else if (score <= 3) {
        return "Weak";
    } else if (score <= 5) {
        return "Moderate";
    } else if (score <= 7) {
        return "Strong";
    } else {
        return "Very Strong";
    }
}

std::vector<std::string> getSuggestions(const std::string& pwd, int score) {
    std::vector<std::string> suggestions;
    int length = static_cast<int>(pwd.size());

    if (length < 8) {
        suggestions.push_back("Use at least 8 characters.");
    }
    if (!hasLower(pwd)) {
        suggestions.push_back("Add some lowercase letters.");
    }
    if (!hasUpper(pwd)) {
        suggestions.push_back("Add some uppercase letters.");
    }
    if (!hasDigit(pwd)) {
        suggestions.push_back("Include at least one digit.");
    }
    if (!hasSpecial(pwd)) {
        suggestions.push_back("Include at least one special character (e.g. @, #, !).");
    }
    if (hasSpace(pwd)) {
        suggestions.push_back("Avoid spaces in the password.");
    }

    if (suggestions.empty() && score >= 7) {
        suggestions.push_back("This looks like a reasonably strong password.");
    }

    return suggestions;
}

int main() {
    std::cout << "=== Password Strength Pattern Evaluator ===\n";
    std::cout << "Type a password to evaluate (or type 'exit' to finish).\n\n";

    std::vector<PasswordReport> reports;
    std::map<std::string, int> categoryCount;

    while (true) {
        std::cout << "Enter password: ";
        std::string pwd;
        std::getline(std::cin, pwd);

        if (!std::cin) {
            std::cout << "Input error. Exiting.\n";
            break;
        }

        if (pwd == "exit" || pwd == "EXIT") {
            break;
        }

        int score = computeScore(pwd);
        std::string label = getStrengthLabel(score);
        std::vector<std::string> suggestions = getSuggestions(pwd, score);

        PasswordReport report{pwd, score, label, suggestions};
        reports.push_back(report);
        categoryCount[label]++;

        std::cout << "\nResult:\n";
        std::cout << "  Score   : " << score << " / 10\n";
        std::cout << "  Strength: " << label << "\n";

        if (!suggestions.empty()) {
            std::cout << "  Suggestions:\n";
            for (const auto& s : suggestions) {
                std::cout << "   - " << s << "\n";
            }
        }

        std::cout << "\n";
    }

    if (!reports.empty()) {
        std::cout << "\n=== Summary ===\n";
        std::cout << "Total passwords evaluated: " << reports.size() << "\n";
        std::cout << "Category distribution:\n";
        for (const auto& entry : categoryCount) {
            std::cout << "  " << entry.first << ": " << entry.second << "\n";
        }
    } else {
        std::cout << "\nNo passwords were evaluated.\n";
    }

    std::cout << "\nThank you for using the evaluator.\n";
    return 0;
}
