import os

os.system("cmake -G \"Ninja\" -S . -B ./build")
os.system("cmake --build ./build")
os.system("./build/daq-unit-tests")