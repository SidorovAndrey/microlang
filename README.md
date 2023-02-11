# microlang

Trying to follow [this](https://github.com/cpp-best-practices/cppbestpractices/blob/master/03-Style.md) coding standart

### howto
To compile compiler:
1. Go to build directory
2. Run `cmake ..`
3. Run `make`

To compile source code from test/text.txt, in build directory:
1. Run `./src/main ../test/text.txt ./output.ll`, IR in output.ll will be generated
2. Go to project root
3. Run `./compile_ll.sh ./build/output.ll`, asm will be generated, compiled and linked
4. Now you can run compiled program `./build/output.ll.out`

### TODO
- [ ] Refactor building AST function in `AST::buildTree`
- [ ] "front" should not depend on LLVM
- [ ] Write unit tests. Check doctest/Catch2/gtest (from google)/boosttest
- [ ] Implement all the binary and unary operations
- [x] Create logging approach
- [ ] Create error reporting approach
- [x] Update project structure, separate front-end and backend; introduce utils module (with logging and Result); use different files for each Expression type in different folder
- [ ] Update to use prefixes for modules in `#include`, e.g. `#include "front/AST.h"` instead of `#include "AST.h"`
- [ ] Implement "if"
- [ ] Implement "while"
- [ ] Implement functions
