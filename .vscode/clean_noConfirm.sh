#!/bin/bash

if [ -z "$1" ] || ![ -e $1 ]; then
    echo "Please provide correct working directory."
    echo "Usage: $0 workingPath"
    exit 1
fi

workingPath="$1"
cd "$workingPath"

echo "Performed project clean at"
echo "$workingPath"

files=$(find "$workingPath" -type f \( -name "*.exe" -o -name "exe_*" -o -name "input.txt" \))

if [ -n "$files" ]; then
    echo "Deleting files:"
    echo "$files"
    rm -f $files
    echo "Deleted *.exe, *.dll, *.o, *.obj, *.pdb, *.ilk, *.out input.txt in $workingPath recursively."
else
    echo "No matching files to delete in $workingPath."
fi

# Remove directories
if [ -d "$workingPath/build" ]; then
    rm -rf "$workingPath/build" && echo "Removed directory $workingPath/build"
fi

exit 0