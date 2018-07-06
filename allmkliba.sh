cd baselib
./mkliba.sh
cp susuClib_base.liba ../susulibout/
cp susuClib_base.h ../susulibout/

cd ../DimMinquqelib
./mkliba.sh
cp susuClib_quqe.liba ../susulibout/
cp susuClib_quqe.h ../susulibout/

cd ../linuxwifilib
./mkliba.sh
cp susuClib_wifi.liba ../susulibout/
cp susuClib_wifi.h ../susulibout/

cd ../protocolODLPlib
./mkliba.sh
cp susuprotocolODLPlib.liba ../susulibout/
cp susuprotocolODLPlib.h ../susulibout/

cd ../lianbiaoquqelib
./mkliba.sh
cp susuClib_quqe.liba ../susulibout/
cp susuClib_quqe.h ../susulibout/

cd ../susucjson
./mkliba.sh
cp cJSON.liba ../susulibout/
cp cJSON.h ../susulibout/

cd ../susucjsonzhzc
./mkliba.sh
cp susucJSON.liba ../susulibout/
cp susucJSON.h ../susulibout/

cd ../susurtc
./mkliba.sh
cp susucJSON.liba ../susulibout/
cp susucJSON.h ../susulibout/



