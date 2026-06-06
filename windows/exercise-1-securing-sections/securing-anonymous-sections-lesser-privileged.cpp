#include <Windows.h>
#include <iostream>

int main() {
    
    std::cout << "PID of the current process: " << GetCurrentProcessId() << std::endl;
    
    DWORD dwHandleId;
    std::cout << "Please enter the value of the duplicated handle: " << std::endl;
    std::cin >> std::hex >> dwHandleId;
    HANDLE hHandle = (HANDLE)dwHandleId;
    std::cout << "Handle value entered: " << hHandle << std::endl;

    // step 3: duplicate the handle to the current process with read/write access
    HANDLE hCurrentProcess = GetCurrentProcess();
    HANDLE hDuplicatedHandle = nullptr;
    BOOL bDuplicate = DuplicateHandle(hCurrentProcess, hHandle, hCurrentProcess, &hDuplicatedHandle, FILE_MAP_READ | FILE_MAP_WRITE, FALSE, 0);
    if (!bDuplicate) {
        std::cerr << "Failed to duplicate handle: " << GetLastError() << std::endl;
        return 1;
    }

    LPVOID pMapView = MapViewOfFile(hDuplicatedHandle, FILE_MAP_WRITE, 0, 0, 0);
    if (pMapView == nullptr) {
        std::cerr << "Failed to map view of file: " << GetLastError() << std::endl;
        return 1;
    }
    std::cout << "Mapped view of file successfully!" << std::endl;

    // step 3: write some data to the section
    const char* message = "Hello from the lesser privileged process!";
    memcpy(pMapView, message, strlen(message) + 1);
    std::cout << "Data written to the section successfully!" << std::endl;

    // step 4: read data back from the section to verify it's correct
    char buffer[256];
    memcpy(buffer, pMapView, sizeof(buffer));
    std::cout << "Data read from the section: " << buffer << std::endl;

    return 0;
}