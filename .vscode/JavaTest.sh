javac -d compileRuntime/bin 2023Java/**/*.java 2024Java/**/*.java utils/JavaDataModel/*.java
SECONDS=0
for javaRoot in *Java/; do
    echo "Processing: $javaRoot"
    yearName=${javaRoot%/}
    find "$javaRoot" -name "*.java" | while read -r javaFile; do
        className=$(echo "$javaFile" | sed -e 's/\.java$//' -e 's/\//./g' -e "s/$yearName\.//")
        dayname=$(echo "$className" | cut -d '.' -f 1)
        curPath=$(pwd)
        cd "$yearName/$dayname"
        java -cp "$curPath/compileRuntime/bin" $className
        cd "$curPath"
    done
done
echo "Total runtime: $SECONDS seconds"
