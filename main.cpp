#include <Windows.h>
#include <thread>

namespace CustomFramerateDisplay {

    constexpr __int64 DEFAULT_BASE = 0x1E9DE1A0000;

    // Function to rebase the address relative to the current module's base address.
    __int64 rebaseAddress(__int64 address, __int64 base = DEFAULT_BASE) {
        return (address - base + reinterpret_cast<__int64>(GetModuleHandle(nullptr)));
    }

    // Function pointer to get the task scheduler's address.
    using GetTaskSchedulerFunc = __int64(__cdecl*)();
    GetTaskSchedulerFunc getTaskScheduler = reinterpret_cast<GetTaskSchedulerFunc>(rebaseAddress(0x1E9DEA3EA40));

    // Function to initialize and display the framerate.
    void displayFramerate() {
        __int64 taskSchedulerAddr = getTaskScheduler();

        // Retrieve the current framerate.
        double currentFramerate = 1 / *reinterpret_cast<double*>(taskSchedulerAddr + 0x158);

        // Display the framerate to the console.
        // This can be enhanced to use any custom logging mechanism.
        char buffer[100];
        sprintf_s(buffer, "Current Framerate: %f", currentFramerate);
        OutputDebugStringA(buffer);
    }

    // Entry point for the DLL.
    __int64 APIENTRY DllMain(HMODULE module, uint32_t reason, LPVOID) {
        if (reason == DLL_PROCESS_ATTACH) {
            // Run the displayFramerate function in a separate thread.
            std::thread(displayFramerate).detach();
        }
        return 1;
    }

} // namespace CustomFramerateDisplay
