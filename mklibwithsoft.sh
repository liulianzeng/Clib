PATHCC=d:/micoder/compiler/arm-none-eabi-5_4-2016q2-20160622/win32/bin/arm-none-eabi-gcc.exe
PATHAR=d:/micoder/compiler/arm-none-eabi-5_4-2016q2-20160622/win32/bin/arm-none-eabi-ar.exe
#PATHCC=D:/Keil_v5/ARM/ARMCC/Bin/ArmCC.exe
#PATHAR=D:/Keil_v5/ARM/ARMCC/Bin/ArmAR.exe
cd baselib
$PATHCC -c susuClib_base.c -o susuClib_base.o
$PATHAR rcs -o susuClib_base.liba susuClib_base.o
cp susuClib_base.liba ../susulibout/
cp susuClib_base.h ../susulibout/

cd ../DimMinquqelib
$PATHCC -c susuClib_quqe.c -o susuClib_quqe.o
$PATHAR rcs -o susuClib_quqe.liba susuClib_quqe.o
cp susuClib_quqe.liba ../susulibout/
cp susuClib_quqe.h ../susulibout/

#cd ../linuxwifilib
#$PATHCC -c susuClib_wifi.c -o susuClib_wifi.o
#$PATHAR rcs -o susuClib_wifi.liba susuClib_wifi.o
#cp susuClib_wifi.liba ../susulibout/
#cp susuClib_wifi.h ../susulibout/

cd ../protocolODLPlib
$PATHCC -c susuprotocolODLPlib.c -o susuprotocolODLPlib.o
$PATHAR rcs -o susuprotocolODLPlib.liba susuprotocolODLPlib.o
cp susuprotocolODLPlib.liba ../susulibout/
cp susuprotocolODLPlib.h ../susulibout/

cd ../lianbiaoquqelib
$PATHCC -c susuClib_quqe.c -o susuClib_quqe.o
$PATHAR rcs -o susuClib_quqe.liba susuClib_quqe.o
#cp susuClib_quqe.liba ../susulibout/
#cp susuClib_quqe.h ../susulibout/

cd ../susucjson
$PATHCC -c cJSON.c -o cJSON.o
$PATHAR rcs -o cJSON.liba cJSON.o
cp cJSON.liba ../susulibout/
cp cJSON.h ../susulibout/

cd ../susucjsonzhzc
$PATHCC -c cJSON.c -o cJSON.o
$PATHCC -c cJSON_Utils.c -o cJSON_Utils.o
$PATHAR rcs -o cJSON.liba cJSON.o cJSON_Utils.o
cp cJSON.liba ../susulibout/
cp *.h ../susulibout/

cd ../susurtc
$PATHCC -c susuunixrtc.c -o susuunixrtc.o
$PATHAR rcs -o susuunixrtc.liba susuunixrtc.o
cp susuunixrtc.liba ../susulibout/
cp susuunixrtc.h ../susulibout/



