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

# get the username
user = os.getlogin()
# setup the path header
pathHead = "/usr/local/"
# say if sudo is used
sudo = "sudo "

# compile glge
print("compiling GLGE to an shared object. This may take some time ... ")
os.system("bash createSharedObject.sh")

# extract all GLGE header files
headers = glob.glob("src/GLGE/*.h*")
headers += glob.glob("src/GLGE/CML/*.h*")

# print that sudo is needed for creation and movement of the files
print("sudo is needed to create and move the files to the destination folders")

# copy all headers in a folder called bin/include
if not os.path.isdir(pathHead + "include/GLGE/CML"):
    print("creating directory " + pathHead + "include/GLGE/CML" + " because it didn't exist")
    os.system(sudo + "mkdir " + pathHead + "include/GLGE/CML")

for o in headers:
    print("copying header " + o + " to " + pathHead + "include/"+o[4:])
    os.system(sudo + "cp "+o+" " + pathHead + "include/"+o[4:])

# check wich directory to install GLGE to
if os.path.isdir(pathHead + "lib"):
    print("moving libGLGE.so to " + pathHead + "lib")
    os.system(sudo + "mv bin/libGLGE.so " + pathHead + "lib")
    print("installed libGLGE.so to " + pathHead + "lib")