/*
MIT License

Copyright (c) 2024

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#pragma once

#include <string>
#include <wtypes.h>

/**
 * @brief Converts the given parameter to string literal.
 * 
 * @param var The parameter to stringify.
 * @return The string literal of the given parameter.
*/
#define VAR_NAME(var) (#var)

namespace GP4MemLib {

	/**
	 * @class MemUtils
	 * @brief Utility class for memory operations, including converting addresses to strings,
	 * patching memory, and rerouting functions.
	 */
	class MemUtils {
	public:
		/**
		 * @brief Converts a DWORD memory address to its string representation in hexadecimal.
		 *
		 * @param address The memory address to convert.
		 * @return The string representation of the address in hexadecimal.
		 */
		static std::string dwordToString(DWORD address);

		/**
		 * @brief Converts a pointer to its string representation in hexadecimal.
		 *
		 * @param address The pointer to convert.
		 * @return The string representation of the pointer.
		 */
		static std::string ptrToString(LPVOID address);

		/**
		 * @brief Patches the memory at a specific address with a given byte sequence.
		 *
		 * @param address The memory address to patch.
		 * @param patch The byte sequence to write to the memory address.
		 * @param size The size of the patch.
		 */
		static void patchAddress(LPVOID address, LPBYTE patch, SIZE_T size);

		/**
		 * @brief Redirects the execution flow to a target function by inserting a jump at the specified address.
		 *
		 * @param jumpToAddress The address where the jump should occur.
		 * @param targetFunction The address of the function to reroute to.
		 * @param functionName Optional. The name of the function to reroute (used for debugging).
		 */
		static void rerouteFunction(DWORD jumpToAddress, DWORD targetFunction, std::string functionName = "");

	};

	/**
	 * @class RegUtils
	 * @brief Utility class for saving and restoring volatile CPU registers (EAX, ECX, EDX).
	 */
	class RegUtils {
	public:

		/**
		 * @brief Saves the volatile CPU registers (EAX, ECX, EDX).
		 */
		static void saveVolatileRegisters();

		/**
		 * @brief Restores the volatile CPU registers (EAX, ECX, EDX).
		 */
		static void restoreVolatileRegisters();

	private:
		/**
		 * @brief Static variables to hold the values of the volatile registers (EAX, ECX, EDX) when saved.
		 */
		static DWORD eaxVar; ///< Holds the value of the EAX register.
		static DWORD ecxVar; ///< Holds the value of the ECX register.
		static DWORD edxVar; ///< Holds the value of the EDX register.
	};

} // namespace GP4MemLib
