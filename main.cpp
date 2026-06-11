#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <limits> //input clearing


// Empty lines ignored
std::map<std::string, std::string> loadVault() {
    std::map<std::string, std::string> vault;
    std::ifstream file("data.txt");
    std::string line;
    
    if (file.is_open()) {
        while (std::getline(file, line)) {
            if (line.empty()) continue; 
            
            size_t delimiter = line.find(':');
            if (delimiter != std::string::npos) {
                std::string label = line.substr(0, delimiter);
                std::string encryptedValue = line.substr(delimiter + 1);
                vault[label] = encryptedValue;
            }
        }
        file.close();
    }
    return vault;
}

int main() {
    std::map<std::string, std::string> vault = loadVault();
    int cryptographicSalt = 42;
    
    while (true) {
        std::cout << "\n========================================\n";
        std::cout << "     CRYPTVAULT C++ SECURITY CORE       \n";
        std::cout << "========================================\n";
        std::cout << "1. Add and Encrypt New Secret\n";
        std::cout << "2. Decrypt and Retrieve Secret\n";
        std::cout << "3. Exit System\n";
        std::cout << "Enter selection (1-3): ";
        
        int choice;
        // Protection against invalid entries
        if (!(std::cin >> choice)) {
            std::cout << "\n Invalid input! Please enter a number.\n";
            std::cin.clear(); // clear the error state
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // discard bad characters
            continue;
        }
        
        if (choice == 1) {
            std::string masterKey, label, plainText;
            
            std::cout << "Establish Master Verification Key: ";
            std::cin >> masterKey;
            std::cout << "Enter Secret Label (e.g., GitHub): ";
            std::cin >> label;
            std::cout << "Enter Secret Text to Encrypt: ";
            std::cin >> plainText;
            
            std::string cipherText = encryptDecrypt(plainText, masterKey, cryptographicSalt);
            vault[label] = cipherText;
            saveVault(vault);
            
            shredString(masterKey);
            shredString(plainText);
            
            std::cout << "\n Secret successfully encrypted and written to data.txt!\n";
            
        } else if (choice == 2) {
            std::string masterKey, label;
            
            std::cout << "Enter Master Verification Key: ";
            std::cin >> masterKey;
            std::cout << "Enter Secret Label to retrieve: ";
            std::cin >> label;
            
            if (vault.find(label) != vault.end()) {
                std::string cipherText = vault[label];
                std::string decryptedText = encryptDecrypt(cipherText, masterKey, cryptographicSalt);
                
                std::cout << "\n Decrypted Plaintext for '" << label << "': " << decryptedText << "\n";
                shredString(decryptedText);
            } else {
                std::cout << "\n Secret record label not found.\n";
            }
            shredString(masterKey);
            
        } else if (choice == 3) {
            std::cout << "Clearing system channels.\n";
            break;
        } else {
            std::cout << "Invalid choice. Select 1, 2, or 3.\n";
        }
    }
    return 0;
}