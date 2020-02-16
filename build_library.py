import os
import glob

for file in glob.glob("raytrace/*.cpp"):
    fname = file.split("/")[1].split(".cpp")[0]
    os.system(f"g++ -c raytrace/{fname}.cpp -fopenmp -o raytrace_o/{fname}.o -std=c++17 -Ofast")
    print("Built", fname)

os.system("ar rcs raytrace/libraytrace.a " + " ".join(glob.glob("raytrace_o/*")))
print("Created Static Library")