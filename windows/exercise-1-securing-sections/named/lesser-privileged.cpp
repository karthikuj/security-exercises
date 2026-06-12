#include <Windows.h>
#include <iostream>

int main() {
    // step 1: open the named section without security attributes
    HANDLE hSection = OpenFileMappingW(FILE_MAP_ALL_ACCESS, FALSE, L"NoSecurityAttributes");
    if (hSection == nullptr) {
        std::cerr << "Failed to open file mapping: " << GetLastError() << std::endl;
        return 1;
    }
    std::cout << "Opened named section without security attributes successfully!" << std::endl;

    // step 2: open the named section with security attributes
    HANDLE hSectionWithSA = OpenFileMappingW(FILE_MAP_ALL_ACCESS, FALSE, L"WithSecurityAttributes");
    if (hSectionWithSA == nullptr) {    
        std::cerr << "Failed to open file mapping with security attributes: " << GetLastError() << std::endl;
        return 1;
    }
    std::cout << "Opened named section with security attributes successfully!" << std::endl;

    return 0;
}