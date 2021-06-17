#ble scanner

If you like this code you have to look for zephyr rtos: https://www.zephyrproject.org/. I you still like the idea to try the code, you have to install the development kit. The correct way depends from you os. If you like windows and/or mac you are on your own. I have installed on my linux ubunt 20 and, I was to able develop this and other program. 

Please don't forget to evaluate the possible use of Visual Studio, or Platform.io, but I'm still on linux VIM and command line. 

So: 
a) install west development kit
b) start to compile and load the examples. I'm developing some scripts at: https://github.com/gvieri/myzephyrstuff
c) to compile the code I use: rm -rf ~/mybuilddir; west build -c -p auto -b nrf5340dk_nrf5340_cpunet ~/myprojectdir -d ~/mybuilddir
d) to load: west flash --build-dir ~/mybuilddir

IF I get trouble in loading: nrfjprog --recover. If problem persists: nrfjprog --recover -f NRF53 *if nrf53 is your development kit* 
Usually this solve the problems. Otherwise contact assistance on https://devzone.nordicsemi.com/ or contact the producer of your development kit. 

I really hope to have you too interest in this OS and on these chips. 
