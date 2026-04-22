## iotc-freertos-ek-ra6w1

### Board Setup
- Type "setup" on the first terminal to set up WiFi.

```
Stop all services for the setting.
 Are you sure ? [Yes/No] : Y
 
Country Code List:
AD  AE  AF  AI  AL  AM  AN  AR  AS  AT  AU  AW  AZ  BA  BB  BD  BE  BF  BG  BH
BL  BM  BN  BO  BR  BS  BT  BY  BZ  CA  CF  CH  CI  CL  CN  CO  CR  CU  CX  CY
CZ  DE  DK  DM  DO  DZ  EC  EE  EG  ES  ET  EU  FI  FM  FR  GA  GB  GD  GE  GF
GH  GL  GP  GR  GT  GU  GY  HK  HN  HR  HT  HU  ID  IE  IL  IN  IR  IS  IT  JM
JO  JP  KE  KH  KN  KP  KR  KW  KY  KZ  LB  LC  LI  LK  LS  LT  LU  LV  MA  MC
MD  ME  MF  MH  MK  MN  MO  MP  MQ  MR  MT  MU  MV  MW  MX  MY  NG  NI  NL  NO
NP  NZ  OM  PA  PE  PF  PG  PH  PK  PL  PM  PR  PT  PW  PY  QA  RE  RO  RS  RU
RW  SA  SE  SG  SI  SK  SN  SR  SV  SY  TC  TD  TG  TH  TN  TR  TT  TW  TZ  UA
UG  UK  US  UY  UZ  VA  VC  VE  VI  VN  VU  WF  WS  YE  YT  ZA  ZW  ZZ

 COUNTRY CODE ? [Quit] (Default KR, ZZ : for debug) : US


SYSMODE(WLAN MODE) ?
        1. Station
        2. Soft-AP
        3. WiFi Direct
        4. WiFi Direct P2P GO Fixed
        5. Station & Soft-AP
 MODE ?  [1/2/3/4/5/Quit] (Default Station) : 1

BAND ?
        1. 2.4 GHz
        2. 5 GHz
        3. AUTO
 BAND ?  [1/2/3/Quit] (Default 3. AUTO) : 1
 
 Do you want to set advanced WiFi configuration ? [No/Yes/Quit] (Default No) : N

 Enable DPM (Dynamic Power Management) ? [Yes/No/Manual/Quit] : N

============================================
BAND        : 2.4 GHz
SSID        : liuxuxu24
Security    : WPA2PSK
ENCRYPTION  : AES(CCMP)
Password    : 3157063540
PMF         : Default
============================================
 WIFI CONFIGURATION CONFIRM ? [Yes/No/Quit] : Y

 IP Connection Type ? [Automatic IP/Static IP/Quit] : A


IP Connection Type: Automatic IP

 IP CONFIGURATION CONFIRM ? [Yes/No/Quit] : Y

 SNTP Client enable ? [Yes/No/Quit] : N
```
- Setup board jumpers as below.
![board screenshot](media/jumper.png)

### Project Setup
- Modify the file *"src/app_config.h"* to set up the /IOTCONNECT credentials.