# Ben's Console Graphics Library
1/7/2021

This C++ library provides objects for drawing simple text-based 2D grahics using the Windows Console API. 

## Setup

As this is a static library, BensConsoleGraphicsLibrary.lib and all header files must be included. At the moment, these can be retrieved from Bens-Console-Engine/Debug/ and Bens-Console-Engine/BensConsoleGraphicsLibrary/ respectively.

### Visual Studio
1. Download the aforementioned .lib and .h files
2. Right click on project in the solution explorer and select "properties"
3. Navigate to Configuration Properties>C/C++>General
    - Enter path to header files under "Additional Include Directories"
4. Navigate to Configuration Properties>Linker>General
    - Enter path to BensConsoleGraphicsLibrary.lib under "Additional Library Directories"
5. Navigate to Configuration Properties>Linker>Input
    - Add BensConsoleGraphicsLibrary.lib to "Additional Dependencies"
