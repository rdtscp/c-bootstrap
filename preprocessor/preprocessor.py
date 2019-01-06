import sys
from subprocess import run


class Preprocessor:

    # Calls the clang++ preprocessor on an input,
    # and writes the output to an output
    def preprocess(self, inFile, outFile):
        if ".cpp" in inFile:
            # clang++ -std=c++14 -E compiler/main.cpp -o tempSource.cpp
            prepRes = run(["clang++", "-std=c++14",
                           "-E", inFile, "-o", outFile])
            if prepRes.returncode != 0:
                print("Failed to Preprocess file: " + inFile)
                exit()
        else:
            # clang++ -std=c++14 -E compiler/main.cpp -o tempSource.cpp
            prepRes = run(["clang", "-E", inFile, "-o", outFile])
            if prepRes.returncode != 0:
                print("Failed to Preprocess file: " + inFile)
                exit()


if __name__ == "__main__":
    print("Preprocessor should not be used as a standalone script.")
    inFile = sys.argv[1]
    outFile = sys.argv[2]
    print("In: " + inFile)
    print("Out: " + outFile)
    pp = Preprocessor()
    pp.preprocess(inFile, outFile)
