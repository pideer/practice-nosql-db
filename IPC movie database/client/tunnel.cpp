#include "tunnel.h"

std::string ws2s(const std::wstring &wstr) {
    using convert_typeX = std::codecvt_utf8<wchar_t>;
    std::wstring_convert<convert_typeX, wchar_t> converterX;

    return converterX.to_bytes(wstr);
}
std::wstring s2ws(const std::string &str) {
    using convert_typeX = std::codecvt_utf8<wchar_t>;
    std::wstring_convert<convert_typeX, wchar_t> converterX;

    return converterX.from_bytes(str);
}

Tunnel::Tunnel() {
#ifdef _WIN32
    pipe =
        CreateFileW(L"\\\\.\\pipe\\my_pipe",      // Pipe name
                    GENERIC_READ | GENERIC_WRITE, // Desired access (read-only)
                    FILE_SHARE_READ | FILE_SHARE_WRITE, // Share mode
                    NULL,                  // Security attributes (default)
                    OPEN_EXISTING,         // Opens existing pipe
                    FILE_ATTRIBUTE_NORMAL, // File attributes
                    NULL                   // No template file
        );
    if (pipe == INVALID_HANDLE_VALUE) {
        std::wcout << "Failed to connect to pipe." << std::endl;
        // look up error code here using GetLastError()
        system("pause");
        this->~Tunnel();
        exit(1);
    }
#elif __linux__
#endif
    std::cout << "Connected to Server!\n\n";
}

Tunnel::~Tunnel() {
#ifdef _WIN32
    CloseHandle(pipe);
#elif __linux__
    close(pipe);
    // unlink(pipe_path);
#endif
    std::cout << "Disconnected.\n";
}

void Tunnel::send(std::string msg) {
    // std::wcout << "Sending data to pipe..." << std::endl;
    std::wstring wmsg = s2ws(msg);
#ifdef _WIN32
    const wchar_t *data = wmsg.c_str();
    DWORD numBytesWritten = 0;
    BOOL result = WriteFile(
        pipe,                           // handle to our outbound pipe
        data,                           // data to send
        wcslen(data) * sizeof(wchar_t), // length of data to send (bytes)
        &numBytesWritten,               // will store actual amount of data sent
        NULL                            // not using overlapped IO
    );
#elif __linux__
    pipe = open(pipe_path, O_WRONLY);
    if (pipe == -1) {
        std::wcout << L"Failed to open pipe for writing." << std::endl;
        perror("Error");
        system("pause");
    }

    ssize_t numBytesWritten =
        write(pipe, wmsg.c_str(), (wcslen(wmsg.c_str()) + 1) * sizeof(wchar_t));
    bool result = numBytesWritten >= 0;
    close(pipe);
#endif
    if (!result) std::wcout << "Failed to send data." << std::endl;
}

std::string Tunnel::receive() {
    wchar_t buffer[128];
    std::wstring msg;
#ifdef _WIN32
    DWORD numBytesRead = 0;
    BOOL result =
        ReadFile(pipe,
                 buffer, // the data from the pipe will be put here
                 127 * sizeof(wchar_t), // number of bytes allocated
                 &numBytesRead, // this will store number of bytes actually read
                 NULL           // not using overlapped IO
        );
#elif __linux__
    pipe = open(pipe_path, O_RDONLY);
    if (pipe == -1) {
        std::wcout << L"Failed to open pipe for writing." << std::endl;
        perror("Error");
        system("pause");
    }
    ssize_t numBytesRead = read(pipe, buffer, sizeof(buffer) - sizeof(wchar_t));
    close(pipe);

    bool result = numBytesRead >= 0;
#endif
    if (result) {
        buffer[numBytesRead / sizeof(wchar_t)] =
            '\0'; // null terminate the string
        // std::wcout << "Number of bytes read: " << numBytesRead << endl;
        msg = buffer;
    } else {
        std::wcout << "Failed to read data from the pipe." << std::endl;
        return std::string();
    }
    return ws2s(msg);
}