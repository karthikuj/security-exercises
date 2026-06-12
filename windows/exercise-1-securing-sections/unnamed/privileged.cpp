#include <Windows.h>
#include <iostream>

int main() {
    
    // TODO: add a check to ensure process is running as admin, return if not.
    // step 1: create an anonymous section
    LPCWSTR sectionName = L"AnonymousSection"; // anonymous section
    HANDLE hSection = CreateFileMappingW(INVALID_HANDLE_VALUE, nullptr, PAGE_READWRITE, 0, 4096, sectionName);
    if (hSection == nullptr) {
        std::cerr << "Failed to create file mapping: " << GetLastError() << std::endl;
        return 1;
    }

    // step 2: map the section into the current process
    LPVOID pMapView = MapViewOfFile(hSection, FILE_MAP_ALL_ACCESS, 0, 0, 0);
    if (pMapView == nullptr) {
        std::cerr << "Failed to map view of file: " << GetLastError() << std::endl;
        CloseHandle(hSection);
        return 1;
    }

    // step 3: write some data to the section
    const char* message = "Hello from the privileged process!";
    memcpy(pMapView, message, strlen(message) + 1);
    std::cout << "Data written to the section successfully! \"" << message << "\"" << std::endl;

    // step 4: read data back from the section to verify it's correct
    char buffer[256];
    memcpy(buffer, pMapView, sizeof(buffer));
    // std::cout << "Data read from the section: " << buffer << std::endl;

    // step 5: get the low privileged process pid from user input
    DWORD dwPid;
    std::cout << "\nEnter the PID of the low privileged process: ";
    std::cin >> dwPid;

    // step 6: open a handle to the low privileged process
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
    if (hProcess == nullptr) {
        std::cerr << "Failed to open process: " << GetLastError() << std::endl;
        UnmapViewOfFile(pMapView);
        CloseHandle(hSection);
        return 1;
    }
    // std::cout << "Handle to target process opened successfully!" << std::endl;

    // step 7: duplicate a read-only handle to a less privileged process
    HANDLE hTargetHandle = nullptr;
    BOOL bDuplicate = DuplicateHandle(GetCurrentProcess(), hSection, hProcess, &hTargetHandle, FILE_MAP_READ, FALSE, 0);
    if (!bDuplicate) {
        std::cerr << "Failed to duplicate handle: " << GetLastError() << std::endl;
        CloseHandle(hProcess);
        UnmapViewOfFile(pMapView);
        CloseHandle(hSection);
        return 1;
    }
    std::cout << "Handle duplicated successfully! Handle: " << hTargetHandle << std::endl;

    std::cout << "Enter any key to read data from the section..." << std::endl;
    std::cin.get(); // wait for user input before reading data back
    std::cin.get();
    // step 8: read data back from the section to verify it's correct
    char buffer2[256];
    memcpy(buffer2, pMapView, sizeof(buffer2));
    std::cout << "Updated data by lesser privileged process: " << buffer2 << std::endl;

    return 0;
}