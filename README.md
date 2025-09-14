# TTS-CC

---------------
The Compiler for Custom Languages By TTS Consulting LLC.

## Overview
TTS-CC is a custom language compiler developed by TTS Consulting LLC. It is designed to read, process, and compile source files written in the CPlus language.

## Features
- Loads and previews CPlus source files
- Modular design for lexer, parser, and code generation (coming soon)
- Command-line interface

## Getting Started

### Prerequisites
- GCC or Clang (for building)
- Make (optional, if using a Makefile)

### Build Instructions
1. Clone the repository:
   ```batch
   git clone https://github.com/TTSConsulting/TTS-CC.git
   cd TTS-CC\cplus-compiler
   ```
2. Compile the compiler:
   ```batch
   make
   ```

### Usage
Run the compiler with a source file:
```sh
tts-c+ <source.cplus>
```

## Example
```sh
tts-cc examples/hello.cplus
```

## Contributing
Contributions are welcome! Please open issues or submit pull requests.

## License
This project is licensed under the MIT License.

## Notes

- This is a work in progress. More features and improvements will be added over time.
- This will be built in C+ IDE.
- The compiler currently supports basic syntax and constructs. More complex features will be added in future releases.
