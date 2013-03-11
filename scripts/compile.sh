#! /bin/sh

# clear emerg.log file
echo "----------------------------" && 
echo "clear emerg.log file" && 
echo "----------------------------" && 
echo > /home/agostino/Scrivania/emerg.log && 

# cd driver directory
echo "----------------------------" && 
echo "cd driver directory" && 
echo "----------------------------" && 
cd /home/agostino/Scrivania/Dropbox/TESI/LinuxWireless/project_git/compat-wireless-3.5.4-1-s/ && 

#set only rtl818x driver driver
echo "----------------------------" && 
echo "set only rtl818x driver" && 
echo "----------------------------" && 
sudo ./scripts/driver-select rtl818x &&  #only one time

#compile
echo "----------------------------" && 
echo "compile" && 
echo "----------------------------" && 
sudo make
