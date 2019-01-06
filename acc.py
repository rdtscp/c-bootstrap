import json
import os
import tempfile
from subprocess import run, DEVNULL
import argparse
from preprocessor.preprocessor import Preprocessor

ACC_BUILD_DIR = "./compiler/build"

parser = argparse.ArgumentParser(
    description="acc mini-c/c++ compiler.", usage="acc source [source ...]")

parser.add_argument("source", type=list, nargs="+",
                    help="source files to compile.")

parser.add_argument("-o", "--output", type=str, default="./a.out", required=False,
                    help='binary output filepath.')

args = parser.parse_args()


def getSourceFiles():
    srcMap = {}
    for charArray in args.source:
        srcFile = "".join(charArray)
        srcMap[srcFile] = {}
        srcMap[srcFile]["prep"] = ""
        srcMap[srcFile]["comp"] = ""
        srcMap[srcFile]["obj"] = ""
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
    os.chdir("../..")
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
        compRes = run(["./compiler/build/acc", srcFiles["prep"],
                       srcFiles["comp"], "x86", "--opt"])
        if compRes.returncode != 0:
            print("Failed to Compile file: " + srcFile)
            exit()
    print("Done\n")
    return srcMap


def assembleSourceFiles(srcMap, outDir):
    print("Assembling:")
    for srcFile, srcFiles in srcMap.items():
        srcFiles["obj"] = outDir + "/obj_" + \
            srcFile.replace("/", "._").replace(".cpp", ".obj")
        print("    {0} ({1})\n -> {2}".format(srcFile,
                                              srcFiles["comp"], srcFiles["obj"]))
        # nasm -f macho x86.s
        asmRes = run(
            ["nasm", "-f", "macho", srcFiles["comp"], "-o", srcFiles["obj"]])
        if asmRes.returncode != 0:
            print("Failed to Assemble file: " + srcFile)
            exit()
    print("Done\n")
    return srcMap


def linkSourceFiles(srcMap, outDir):
    print("Linking:")
    for srcFile, srcFiles in srcMap.items():
        print("    Linking {0} ({1})".format(srcFile, srcFiles["obj"]))
        # ld -macosx_version_min 10.14 -lSystem -o x86 x86.o
        linkRes = run(["ld", "-macosx_version_min", "10.14",
                       "-lSystem", "-o", "./a.out", srcFiles["obj"]])
        if linkRes.returncode != 0:
            print("Failed to Link file: " + srcFile)
            exit()
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
        # Assemble the compiled source.
        srcMap = assembleSourceFiles(srcMap, tmpDirName)
        # Link the compiled source.
        srcMap = linkSourceFiles(srcMap, tmpDirName)
    finally:
        tmpDir.cleanup()


srcMap = getSourceFiles()
acc(srcMap)
