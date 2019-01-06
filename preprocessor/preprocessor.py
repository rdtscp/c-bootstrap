from subprocess import call


class Preprocessor:

    # Calls the clang++ preprocessor on an input,
    # and writes the output to an output
    def preprocess(self, inFile, outFile):
        # clang++ -std=c++14 -E compiler/main.cpp -o tempSource.cpp
        call(["clang++", "-std=c++14", "-E",
              inFile, "-o", outFile])
        return outFile


if __name__ == "__main__":
    print("Preprocessor cannot be used as a standalone script.")
