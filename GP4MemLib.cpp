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

#include "GP4MemLib.h"
#include <iomanip>
#include <sstream>
#include <windows.h>

namespace GP4MemLib {

	std::string MemUtils::dwordToString(DWORD address) {

		std::ostringstream outputString;

		outputString.str(std::string());
		outputString << "0x" << std::hex << std::setw(sizeof(DWORD) * 2) << std::setfill('0') << address;

		return outputString.str();

	}

	std::string MemUtils::ptrToString(LPVOID address) {

		return dwordToString(PtrToUlong(address));
	}

	void MemUtils::patchAddress(LPVOID address, LPBYTE patch, SIZE_T size) {
		DWORD oldProtect;

		std::string addressString = ptrToString(address);

		if (VirtualProtect(address, size, PAGE_EXECUTE_READWRITE, &oldProtect)) {

			memcpy(address, patch, size);
			VirtualProtect(address, size, oldProtect, &oldProtect);

			OutputDebugStringA(("Memory patched successfully at address " + addressString + "\n").c_str());
		}
		else {
			OutputDebugStringA(("Error while patching address " + addressString + "\n").c_str());
		}
	}

	void MemUtils::rerouteFunction(DWORD jumpToAddress, DWORD targetFunction, std::string functionName)
	{
		BYTE jmpCode[5] = { 0xe9, 0x0, 0x0, 0x0, 0x0 };

		// Offset to jump into the re-routed function
		DWORD jumpOffset = targetFunction - jumpToAddress - 5;

		// Append the jump offset to the jmp asm instruction code
		memcpy(&jmpCode[1], &jumpOffset, sizeof(DWORD));

		OutputDebugStringA(("Rerouting starting at address " + dwordToString(jumpToAddress) + "\n").c_str());

		if (functionName == "")
			functionName = "target function";

		OutputDebugStringA(("Address of " + functionName + ": " + dwordToString(targetFunction) + "\n").c_str());

		// Patch memory to jump
		patchAddress((LPVOID)jumpToAddress, (LPBYTE)&jmpCode, sizeof(jmpCode));

	}

	// Definition of static variables in the RegUtils class
	DWORD RegUtils::eaxVar = 0;
	DWORD RegUtils::ecxVar = 0;
	DWORD RegUtils::edxVar = 0;

	// Save the volatile CPU registers
	__declspec(naked) void RegUtils::saveVolatileRegisters() {

		__asm {
			mov eaxVar, EAX
			mov ecxVar, ECX
			mov edxVar, EDX
			ret
		}
	}

	// Restore the volatile CPU registers
	__declspec(naked) void RegUtils::restoreVolatileRegisters() {

		__asm {
			mov EAX, eaxVar
			mov ECX, ecxVar
			mov EDX, edxVar
			ret
		}
	}

} // namespace GP4MemLib