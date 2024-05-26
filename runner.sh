rm AudioVisualizer
./build.sh
if [ $? -eq 0 ]; then
    ./AudioVisualizer $1
else
    echo "Build Failed"
fi