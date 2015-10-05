Many members on 43oh have requested support for a full FAT system stack using the super-awesome Elm-Chan stack. This is my attempt at it. The software still needs work.
You can ask for support in this 43oh thread:


Hardware:
1. The 43oh SDCard BoosterPack
http://store.43oh.com/The-CardReader-SDCard-BoosterPack

2. MSP432 Launchpad
https://store.ti.com/msp-exp432p401r.aspx

Software:
1. Compiled with CCS 
6.1.0.00104 

2. MSPWare
Install http://www.ti.com/tool/MSPWARE
Make sure you have msp432p4xx_driverlib.lib in the root of your project or linked for the above install folder.

The software has only been tested with a 32MB card. This is Fat16. Cards 2Gb or greater should work just as well.

Thanks to the following software:
1. ElmChan FatFS
http://www.elm-chan.org/fsw/ff/00index_e.html

2. TI TivaWare for the SDCard example
http://www.ti.com/tool/sw-tm4c