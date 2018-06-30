#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <sys/ioctl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <linux/if.h>
#include <arpa/inet.h>
#include <errno.h>

#include <unistd.h>  

typedef struct{
	char SOFTAPrun[8];
	char driver[32];
	char ssid[32];
	char hw_mode[8];
	char channel[8];
	char auth_algs[8];
	char wpa[8];
	char wpa_passphrase[32];
	char wpa_key_mgmt[32];
	char wpa_pairwise[8];
	char interface[32];
	char bridge[32];
	char ignore_broadcast_ssid[32];
	char macaddr_acl[8];
	char accept_mac_file[32];
	char deny_mac_file[32];
}HostAPdinfo;

typedef struct {
	int index;
	char SSID[32];
	char WPAWEP[16];
	char SSIDmac[20];
	int SignalLength;
	int frequency;
	int channel;
}SSIDAPInfo;

typedef struct {
	int index;
	char ip[20];
	char starts[64];
	char ends[64];
	char cltt[64];
	char binding_state[64];
	char next_binding_state[64];
	char rewind_binding_state[64];
	char mac[20];
	char uid[64];
	char client_hostname[64];
}STAlistInfo;

typedef struct 
{
	char ip[20];
	char netmask[20];
	char gateway[20];
	char domain[20];
	char mac[20];
	char BroadIP[20];
	char name[32];

	char SSID[128];
	int frequency;
	int SignalLength;
	char SSIDmac[20];
	
	int maxlistindex;
	SSIDAPInfo SSIDAP[200];
	STAlistInfo STAlist[200];
	HostAPdinfo HostAPd;
	char wifistatus[20];
}
wifiInfo;

int GetNetCardCount(void);
char * GetStrsscanf(char *src,int num);
int GetNetCardgatway(const char* szDevName);
char *GetNetCardDNS(const char* szDevName);
void IOctlNetInfo(const char* szDevName,wifiInfo * wlanx);
int NetwlanGetLinkInfo(const char* szDevName,wifiInfo * wlanx);
int NetwlanScanSSID(const char* szDevName,wifiInfo * wlanx);
int HostapdGetConfig(const char* szDevName,wifiInfo * wlanx);
int HostapdGetDhcplist(const char* szDevName,wifiInfo * wlanx);
int wifimodisinsmod(void);
int wifiwlan0disconnect(void);
int wifiwlan0connect(char * conf,char * ssid ,char * psw, char * wpa);
int wifiwlan0setnet(char *dhcp,char *ip,char *netmask,char *gateway);
int wifiwlan1setconn(char *item,char *val,wifiInfo * wlanx);
int wifiwlan1hostapdoff(void);
int wifiwlan1setconf(char *item,char *val);
int wifiwlan1setconfdefault(const char* szDevName,wifiInfo * wlanx);
int wifiwlan1setnet(char *item,char *val,const char* szDevName,wifiInfo * wlanx);


extern int GetStrSpaceBefore(char *src,int num,char *dst);
extern int StringToInt(char *strSTI);
extern int StrToInt(char *strSTI);
extern int c2i(char ch);
extern int hex2dec(char *hex);

#define __WLAN_SCAN_FIL "/tmp/wlan0.scanfile"
#define __WLAN_LIST_FIL "/tmp/wlan1.listfile"

#define __WLAN_HOSTAPD_CONFIG_FIL "/etc/hostapd.conf"
#define __WLAN_ISCDHCPSERVER_CONFIG_FIL "/etc/default/isc-dhcp-server"
#define __WLAN_HOSTAPD_ACCEPT_FIL "/etc/hostapd.accept"
#define __WLAN_HOSTAPD_DENY_FIL "/etc/hostapd.deny"
#define __WLAN_HOSTAPD_DHCPLIST_FIL "/var/lib/dhcp/dhcpd.leases"

#define MAXSTRLEN	255






























