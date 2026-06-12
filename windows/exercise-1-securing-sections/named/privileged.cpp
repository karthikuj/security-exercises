#include <Windows.h>
#include <iostream>
#include <sddl.h>

int main() {
    // step 1: create a named section with read/write access with no security attributes
    HANDLE hSection = CreateFileMappingW(INVALID_HANDLE_VALUE, nullptr, PAGE_READWRITE, 0, 4096, L"NoSecurityAttributes");
    if (hSection == nullptr) {
        std::cerr << "Failed to create file mapping: " << GetLastError() << std::endl;
        return 1;
    }

    // step 2 create a named section with read/write access with security attributes
    PSECURITY_DESCRIPTOR pSD = nullptr;
    if (!ConvertStringSecurityDescriptorToSecurityDescriptorW(
            L"D:(A;;GR;;;WD)(A;;GA;;;BA)",  // Everyone read; admins read/write
            SDDL_REVISION_1,
            &pSD,
            nullptr)) {
        std::cerr << "ConvertStringSecurityDescriptorToSecurityDescriptorW failed: "
                << GetLastError() << std::endl;
        return 1;
    }

    SECURITY_ATTRIBUTES sa = {};
    sa.nLength = sizeof(sa);
    sa.lpSecurityDescriptor = pSD;
    sa.bInheritHandle = FALSE;
    HANDLE hSectionWithSA = CreateFileMappingW(INVALID_HANDLE_VALUE, &sa, PAGE_READWRITE, 0, 4096, L"WithSecurityAttributes");
    if (hSectionWithSA == nullptr) {
        std::cerr << "Failed to create file mapping with security attributes: " << GetLastError() << std::endl;
        return 1;
    }

    std::cout << "Named sections created successfully!" << std::endl;
    std::cout << "Enter any key to exit..." << std::endl;
    std::cin.get(); // Wait for user input before exiting

    return 0;
}