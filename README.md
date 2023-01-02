# microlang

## howto
To compile compiler:
1. Go to build directory
2. Run `cmake ..`
3. Run `make`

To compile source code from test/text.txt, in build directory:
1. Run `./main ../test/text.txt ./output.ll`, IR in output.ll will be generated
2. Go to project root
3. Run `./compile_ll.sh ./build/output.ll`, asm will be generated, compiled and linked
4. Now you can run compiled program `./build/output.ll.out`
