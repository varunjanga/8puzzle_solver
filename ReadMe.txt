1.Install Qt (The following installs IDE.)
  sudo apt-get install qtcreator

2.Uninstall qt-at-spi (Found that this was leading to some errors so I removed it)
  sudo apt-get remove qt-at-spi

3.Run the command 
  qmake && make
  
4.Now you can start the program with the executable
  ./8puzzle 
  
