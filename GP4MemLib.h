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

/**
 * @def GP4MEMLIB_API
 * @brief API visibility specifier for GP4MEMLIB.
 * This macro is used to control symbol visibility when compiling as
 * a static or dynamic library.
 */
#ifdef GP4MEMLIB_STATIC
#define GP4MEMLIB_API 
#else
#ifdef GP4MEMLIB_EXPORTS
#define GP4MEMLIB_API __declspec(dllexport)
#else
#define GP4MEMLIB_API  __declspec(dllimport)
#endif
#endif

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
	class GP4MEMLIB_API MemUtils {
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

		/**
		 * @brief Converts any given value of type T to a byte pointer (LPBYTE).
		 *
		 * @tparam T The type of the input parameter.
		 * @param value The value to convert.
		 * @return A pointer to the byte representation of the value.
		 */
		template <typename T>
		static LPBYTE toBytes(const T& value) {
			return (LPBYTE)reinterpret_cast<const byte *>(std::addressof(value));
		}
		
		/**
		 * @brief Converts a DWORD address to a pointer of type T.
		 * 
		 * This function takes a memory address in the form of a DWORD and converts it into a pointer
		 * of the specified type T.
		 * 
		 * @tparam T The type of the pointer to be returned.
		 * @param address The DWORD address to be converted.
		 * @return A pointer of type T pointing to the memory location represented by the address.
		 */
		template <typename T>
		static T* addressToPtr(DWORD address) {
			return  reinterpret_cast<T*>(address);
		}

		/**
		 * @brief Retrieves the value of type T at a given DWORD address.
		 *
		 * This function dereferences the memory address provided and returns the value stored
		 * at that address as the specified type T.
		 *
		 * @tparam T The type of the value to be returned.
		 * @param address The DWORD address where the value is stored.
		 * @return The value of type T located at the memory address.
		 */
		template <typename T>
		static T addressToValue(DWORD address) {
			return  *reinterpret_cast<T*>(address);
		}

	};

	/**
	 * @class RegUtils
	 * @brief Utility class for saving and restoring volatile CPU registers (EAX, ECX, EDX).
	 */
	class GP4MEMLIB_API RegUtils {
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
