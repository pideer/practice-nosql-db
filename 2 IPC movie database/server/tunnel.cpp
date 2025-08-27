#include <tunnel.h>

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
    createPipe();
    connect();
}

Tunnel::~Tunnel() {
#ifdef _WIN32
    CloseHandle(pipe);
#elif __linux__
    close(pipe);
    unlink(pipe_path);
#endif
    std::cout << "Server closed\n";
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
        this->~Tunnel();
        exit(1);
    }

    ssize_t numBytesWritten =
        write(pipe, wmsg.c_str(), (wcslen(wmsg.c_str()) + 1) * sizeof(wchar_t));
    bool result = numBytesWritten >= 0;
    close(pipe);
#endif

    if (result)
        std::wcout << "Number of bytes sent: " << numBytesWritten << std::endl;
    else
        std::wcout << "Failed to send data." << std::endl;
}

std::vector<std::string> Tunnel::receive() {
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
        this->~Tunnel();
        exit(1);
    }
    ssize_t numBytesRead = read(pipe, buffer, sizeof(buffer) - sizeof(wchar_t));
    bool result = numBytesRead >= 0;
    close(pipe);
#endif
    if (result) {
        buffer[numBytesRead / sizeof(wchar_t)] =
            '\0'; // null terminate the string
        // std::wcout << "Number of bytes read: " << numBytesRead << endl;
        std::wcout << "Message: " << buffer << std::endl;
        msg = buffer;
        if (msg == L"close")
            return {"close"};
        else if (msg == L"exit") {
            disconnect();
            connect();
            return std::vector<std::string>();
        }
    } else {
        std::wcout << "Failed to read data from the pipe." << std::endl;
        disconnect();
        connect();
        return std::vector<std::string>();
    }
    std::string cmd = ws2s(msg);

    // parse message
    std::string s;
    std::istringstream ss(cmd);
    std::vector<std::string> cmds;

    if (ss >> s) {
        cmds.push_back(s);
        s.clear();

        std::getline(ss, s);
        if (!s.empty()) {
            if (s[0] == ' ') s.erase(0, 1);
            cmds.push_back(s);
        }
    }

    return cmds;
}

void Tunnel::createPipe() {
    std::wcout << "Creating an instance of a named pipe..." << std::endl;
#ifdef _WIN32
    // Create a pipe to send data
    pipe = CreateNamedPipeW(
        L"\\\\.\\pipe\\my_pipe", // jsonDBPipe", // name of the pipe
        PIPE_ACCESS_DUPLEX,      // 1-way pipe -- send only
        PIPE_TYPE_BYTE,          // send data as a byte stream
        1,                       // only allow 1 instance of this pipe
        0,                       // no outbound buffer
        0,                       // no inbound buffer
        0,                       // use default wait time
        NULL                     // use default security attributes
    );
    if (pipe == NULL || pipe == INVALID_HANDLE_VALUE) {
        std::wcout << "Failed to create outbound pipe instance.";
        // look up error code here using GetLastError()
        system("pause");
        this->~Tunnel();
        exit(1);
    }
#elif __linux__
    mkfifo("/tmp/my_pipe", 0666);
#else
#warning no platform defined
#endif
}

void Tunnel::connect() {
#ifdef _WIN32
    // This call blocks until a client process connects to the pipe
    BOOL result = ConnectNamedPipe(pipe, NULL);
    if (!result) {
        std::wcout << "Failed to make connection on named pipe." << std::endl;
        // look up error code here using GetLastError()
        CloseHandle(pipe); // close the pipe
        system("pause");
        this->~Tunnel();
        exit(1);
    }
#endif // _WIN32
    std::wcout << "Listening..." << std::endl;
}

bool Tunnel::disconnect() {
    std::wcout << "disconnecting client...\n";
#ifdef _WIN32
    return DisconnectNamedPipe(pipe);
#elif __linux__
    return true;
#endif
}
