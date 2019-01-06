import json
import os
import tempfile
from subprocess import run, DEVNULL
import argparse
from preprocessor.preprocessor import Preprocessor

ACC_BUILD_DIR = "./build"

parser = argparse.ArgumentParser(
    description='acc mini-c/c++ compiler.', usage="acc source [source ...]")

parser.add_argument('source', type=list, nargs='+',
                    help='Source files to compile.')

args = parser.parse_args()


def getSourceFiles():
    srcMap = {}
    for charArray in args.source:
        srcFile = "".join(charArray)
        print("Creating entry for({0})".format(srcFile))
        srcMap[srcFile] = {}
        srcMap[srcFile]["prep"] = ""
        srcMap[srcFile]["comp"] = ""
    return srcMap


def buildACC():
    print("Building ACC Compiler")
    if not os.path.exists(ACC_BUILD_DIR):
        os.makedirs(ACC_BUILD_DIR)

    os.chdir(ACC_BUILD_DIR)
    run(["cmake", "-DCMAKE_BUILD_TYPE=Debug", ".."],
        stdout=DEVNULL, stderr=DEVNULL)
    run(["make", "-j", "4"],
        stdout=DEVNULL, stderr=DEVNULL)
    os.chdir("..")
    print("Done\n")


def preprocessSourceFiles(srcMap, outDir):
    pp = Preprocessor()
    print("Preprocessing:")
    for srcFile, srcFiles in srcMap.items():
        srcFiles["prep"] = outDir + "/prep_" + srcFile.replace("/", "._")
        print("    {0}\n -> {1}".format(srcFile, srcFiles["prep"]))
        pp.preprocess(srcFile, srcFiles["prep"])
    print("Done\n")
    return srcMap


def compileSourceFiles(srcMap, outDir):
    print("Compiling:")
    for srcFile, srcFiles in srcMap.items():
        srcFiles["comp"] = outDir + "/comp_" + \
            srcFile.replace("/", "._").replace(".cpp", ".s")
        print("    {0} ({1})\n -> {2}".format(srcFile,
                                              srcFiles["prep"], srcFiles["comp"]))
        run(["./build/compiler/acc", srcFiles["prep"], srcFiles["comp"], "x86"])
        print("Done\n")
        return srcMap


def acc(srcMap):
    tmpDir = tempfile.TemporaryDirectory()
    tmpDirName = tmpDir.name
    try:
            # Build the c++ compiler.
        buildACC()
        # Preprocess the source.
        srcMap = preprocessSourceFiles(srcMap, tmpDirName)
        # Compile the preprocessed source.
        srcMap = compileSourceFiles(srcMap, tmpDirName)
    finally:
        print("Cleaning up temporary directory.")
        print(srcMap)
        tmpDir.cleanup()


srcMap = getSourceFiles()
acc(srcMap)
