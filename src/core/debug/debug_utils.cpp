#include "core/debug/debug_utils.h"
#include <iostream>
#include <cstdlib>
#include <execinfo.h>
#include <csignal>

namespace engine {
namespace core {
namespace debug {

// Signal handler function
void signalHandler(int signal) {
    std::cerr << "\n==============================================" << std::endl;
    std::cerr << "CRASH DETECTED: Received signal " << signal 
              << " (" << getSignalName(signal) << ")" << std::endl;
    
    // Print the stack trace
    printStackTrace();
    
    std::cerr << "==============================================" << std::endl;
    
    // Exit the program
    exit(signal);
}

void printStackTrace() {
    // Get the stack trace (up to 100 frames)
    void* callstack[100];
    int frames = backtrace(callstack, 100);
    
    std::cerr << "Stack trace:" << std::endl;
    
    // Print the stack trace
    char** symbols = backtrace_symbols(callstack, frames);
    if (symbols) {
        for (int i = 0; i < frames; i++) {
            std::cerr << "  " << symbols[i] << std::endl;
        }
        free(symbols);
    } else {
        std::cerr << "  Failed to get stack symbols" << std::endl;
    }
}

std::string getSignalName(int signal) {
    switch (signal) {
        case SIGSEGV: return "SIGSEGV (Segmentation Fault)";
        case SIGABRT: return "SIGABRT (Abort)";
        case SIGILL: return "SIGILL (Illegal Instruction)";
        case SIGFPE: return "SIGFPE (Floating Point Exception)";
        case SIGBUS: return "SIGBUS (Bus Error)";
        default: return "Unknown Signal";
    }
}

void initDebugUtils() {
    // Register signal handlers
    signal(SIGSEGV, signalHandler);
    signal(SIGABRT, signalHandler);
    signal(SIGILL, signalHandler);
    signal(SIGFPE, signalHandler);
    signal(SIGBUS, signalHandler);
    
    std::cout << "Debug utilities initialized" << std::endl;
}

}}} // namespace engine::core::debug
