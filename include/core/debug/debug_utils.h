#pragma once
#include <csignal>
#include <string>
#include <iostream>

namespace engine {
namespace core {
namespace debug {

// Add function declarations for the missing functions
void printStackTrace();
std::string getSignalName(int signal);

// Remove the inline implementation and just declare the function
void initDebugUtils();

// Signal handler function
void signalHandler(int signal);

}}} // namespace engine::core::debug
