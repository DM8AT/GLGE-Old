# Install GLGE (currently just linux)

import os, glob

# install librarys
#os.system("sudo apt update && sudo apt install libglew-dev libsdl2-dev")

# install SDL2 headers
if False:
    os.system("wget https://github.com/libsdl-org/SDL/releases/download/release-2.28.5/SDL2-2.28.5.tar.gz")
    os.system("tar -xzvf SDL2-2.28.5.tar.gz")
    try:
        os.makedirs("include/SDL2")
    except: pass
    os.system("cp SDL2-2.28.5/include/* include/SDL2")
    os.system("rm -r SDL2-2.28.5 SDL2-2.28.5.tar.gz*")

# compile glge
os.system("bash createSharedObject.sh")

# extract all GLGE header files
headers = glob.glob("src/GLGE/*.h*")
headers += glob.glob("src/GLGE/CML/*.h*")

# copy all headers in a folder called bin/include
if not os.path.isdir("bin/include/GLGE/CML"):
    os.makedirs("bin/include/GLGE/CML")

for o in headers:
    os.system("cp "+o+" bin/include/"+o[4:])

# check wich directory to install GLGE to
print("If a sudo prompt appears, it is required for moving the .so to the /usr/lib folder")
if os.path.isdir("/usr/lib/x86_64-linux-gnu"):
    os.system("sudo mv bin/libGLGE.so /usr/lib/x86_64-linux-gnu")
    print("installed libGLGE.so to /usr/lib/x86_64-linux-gnu")
if os.path.isdir("/usr/lib64/x86_64-linux-gnu"):
    os.system("sudo mv bin/libGLGE.so /usr/lib64/x86_64-linux-gnu")
    print("installed libGLGE.so to /usr/lib64/x86_64-linux-gnu")

# copy the include/GLGE directory to /usr/include
os.system("sudo cp -r bin/include/GLGE /usr/include/")