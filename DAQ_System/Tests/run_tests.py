import os

os.system("cmake -S . -B ./build")
os.system("cmake --build ./build")
os.system("./build/daq-unit-tests")