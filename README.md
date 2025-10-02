MFC Lua Calculator Project Setup Guide
This project is a simple MFC (C++) application that uses the Lua scripting language (v5.4) to handle the calculation logic (business logic).

1. System Requirements
- IDE: Visual Studio 2019 (VC16) or Visual Studio 2022 (VC17).
- Platform: Windows (Project is configured for x64).

2. Required Files
You need the pre-compiled Lua development library to link with this project.

- Clone/Download Code: Get the project code from this GitHub repository.
- Download LuaBinaries: Search for "Lua 5.4.0 Win64 vc16 lib" or go to the LuaBinaries website.
- Select Package: Download the lua-5.4.0_Win64_vc16_lib.zip file.

3. Project Configuration in Visual Studio
After downloading, you must extract the file and configure the paths in your Visual Studio project.

- Extract: Extract the downloaded .zip file to a fixed location (e.g., C:\Libs\Lua54). The folder structure must include include (for .h files) and lib (for .lib files).
- Open Project Properties: In VS, right-click the MFCbasic project → Properties.
- Ensure: Set Platform: x64 and Configuration: Debug (or Release).

A: Header Files (.h)
 - Setting	Location	Value (Example Path)
 - Additional Include Directories	C/C++ → General	C:\Libs\Lua54\include

B: Library Files (.lib)
 - Setting	Location	Value (Example Path)
 - Additional Library Directories	Linker → General	C:\Libs\Lua54\lib

 
C: Linker Input
 - Setting	Location	Value
 - Additional Dependencies	Linker → Input	lua54.lib; (The core library is sufficient)


4. Compile and Run
