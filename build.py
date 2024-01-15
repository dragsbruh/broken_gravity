import os
import sys
import subprocess

PROJECT     = "gravity"
DEBUG       = not "--release" in sys.argv

SOURCE_DIR  = "src"
OUT_DIR     = "bin"
TMP_DIR     = os.path.join(OUT_DIR, "tmp")
INCLUDES    = [".", "include"]
EXTERNS     = ["raylib.dll"]

PRINT_COMMAND = not "--silent" in sys.argv and not "-s" in sys.argv
RUN_AFTER   = "--run" in sys.argv or "-r" in sys.argv

def discover_c_files(extension='.c'):
    detected = []
    for file in os.listdir(SOURCE_DIR):
        if file.endswith(file):
            detected.append(file)
    return detected

def compile_c_file(filename):
    fullpath = os.path.join(SOURCE_DIR, filename)
    outpath  = os.path.join(TMP_DIR, filename.removesuffix(".c") + ".o")
    command = ["gcc", "-c", "-o", outpath, fullpath]
    for include in INCLUDES:
        command.append(f"-I{include}")
    if DEBUG:
        command.append("-g")
    if PRINT_COMMAND:
        print(" ".join(command))
    result = subprocess.run(command)
    if result.returncode != 0:
        print(f"Error while compiling {fullpath}")
        print(f"Compiler exited with code: {result.returncode}")
        exit(result.returncode)
    return outpath

def link(object_files):
    outpath = os.path.join(OUT_DIR, PROJECT)
    command = ["gcc", "-o", outpath, *object_files, f"-L{OUT_DIR}", *[f"-l:{extern}" for extern in EXTERNS]]
    if PRINT_COMMAND:
        print(" ".join(command))
    result = subprocess.run(command)
    if result.returncode != 0:
        print(f"Error while linking object files")
        print(f"Linker exited with code: {result.returncode}")
        exit(result.returncode)
    return outpath

def main():
    os.makedirs(TMP_DIR, exist_ok=True)
    exepath = link([compile_c_file(file) for file in discover_c_files()]) + ".exe" # Looks very sus
    if RUN_AFTER:
        command = [exepath]
        result = subprocess.run(command)
        if result.returncode != 0:
            print(f"\nProgram exited with code: {result.returncode}")
            exit(result.returncode)
        else:
            print("\nProgram finished successfully")

if __name__ == "__main__":
    main()