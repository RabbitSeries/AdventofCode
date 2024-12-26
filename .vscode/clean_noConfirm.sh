#!/bin/bash

if [ -z "$1" ]; then
    echo "Please provide working directory."
    echo "Usage: $0 workingPath"
    exit 1
fi

workingPath="$1"
cd "$workingPath" || exit 1

echo "Performed project clean at"
echo "$workingPath"

# Check if any files with the specified extensions exist, then delete them
# shopt -s nullglob  # To avoid errors if no files are found
files=$(find "$workingPath" -type f \( -name "*.exe" -o -name "exe_*" -o -name "*.dll" -o -name "*.o" -o -name "*.obj" -o -name "*.pdb" -o -name "*.ilk" \))

if [ -n "$files" ]; then
    echo "Deleting files:"
    echo "$files"
    rm -f $files
    echo "Deleted *.exe, *.dll, *.o, *.obj, *.pdb, *.ilk in $workingPath recursively."
else
    echo "No matching files to delete in $workingPath."
fi

# Remove directories
if [ -d "$workingPath/.vs" ]; then
    rm -rf "$workingPath/.vs" && echo "Removed directory $workingPath/.vs"
fi
if [ -d "$workingPath/.vscode/debug" ]; then
    rm -rf "$workingPath/.vscode/debug" && echo "Removed directory $workingPath/.vscode/debug"
fi
if [ -d "$workingPath/build" ]; then
    rm -rf "$workingPath/build" && echo "Removed directory $workingPath/build"
fi
if [ -d "$workingPath/obj" ]; then
    rm -rf "$workingPath/obj" && echo "Removed directory $workingPath/obj"
fi
if [ -d "$workingPath/bin" ]; then
    rm -rf "$workingPath/bin" && echo "Removed directory $workingPath/bin"
fi

# Clean AssemblyOutput if directory exists
if [ -d "$workingPath/AssemblyOutput" ]; then
    find "$workingPath/AssemblyOutput" -type f -exec rm -f {} \;
    echo "Cleaned files in $workingPath/AssemblyOutput"
else
    echo "$workingPath/AssemblyOutput does not exist"
fi

exit 0
