import sys
from subprocess import call


class Preprocessor:

    # Calls the clang++ preprocessor on an input,
    # and writes the output to an output
    def preprocess(self, inFile, outFile):
        if ".cpp" in inFile:
            # clang++ -std=c++14 -E compiler/main.cpp -o tempSource.cpp
            call(["clang++", "-std=c++14", "-E", inFile, "-o", outFile])
        else:
            # clang++ -std=c++14 -E compiler/main.cpp -o tempSource.cpp
            call(["clang", "-E", inFile, "-o", outFile])


if __name__ == "__main__":
    print("Preprocessor should not be used as a standalone script.")
    inFile = sys.argv[1]
    outFile = sys.argv[2]
    print("In: " + inFile)
    print("Out: " + outFile)
    pp = Preprocessor()
    pp.preprocess(inFile, outFile)
