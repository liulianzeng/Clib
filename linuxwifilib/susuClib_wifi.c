#include "susuClib_wifi.h"
//////////////////////////////////获取网卡信息///////////////////////////////////////////////////////////
static int dhcpduseisc=1;

int GetNetCardCount(void)
{
	int nCount = 0;
	char szLine[512];
	FILE* f = fopen("/proc/net/dev", "r");
	if (f<=0)
	{
		fprintf(stderr, "Open /proc/net/dev failed!errno:%d\n", errno);
		return nCount;
	}
	fgets(szLine, sizeof(szLine), f);    /* eat line */
	fgets(szLine, sizeof(szLine), f);
	while(fgets(szLine, sizeof(szLine), f))
	{
		char szName[128] = {0};
		sscanf(szLine, "%s", szName);
		int nLen = strlen(szName);
		if (nLen <= 0)continue;
		if (szName[nLen - 1] == ':') szName[nLen - 1] = 0;
		if (strcmp(szName, "lo") == 0)continue;
		nCount++;
	}
	fclose(f);
	f = NULL;
	return nCount;
}
static char StrGetStrSpace[200];
char * GetStrsscanf(char *src,int num)
{
	sscanf(src, "%s", StrGetStrSpace);
	if(num>0)
		return GetStrsscanf(src+strlen(StrGetStrSpace)+1,num-1);
	return StrGetStrSpace;
}
int GetNetCardgatway(const char* szDevName)
{
	int nCount = 0;
	char szLine[512];
	char Gssb[200];
	FILE* f = fopen("/proc/net/route", "r");
	if (f<=0)
	{
		fprintf(stderr, "Open /proc/net/route failed!errno:%d\n", errno);
		return nCount;
	}
	fgets(szLine, sizeof(szLine), f);    /* eat line */
//	fgets(szLine, sizeof(szLine), f);
	while(fgets(szLine, sizeof(szLine), f))
	{
		char szName[128] = {0};
		sscanf(szLine, "%s", szName);
		int nLen = strlen(szName);
		if (nLen <= 0)continue;
		if (szName[nLen - 1] == ' ') szName[nLen - 1] = 0;
		if (strcmp(szName, szDevName) == 0)
		{
//******************************方法一  自取字符串*************************************
			GetStrSpaceBefore(szLine,3,Gssb);
			if(Gssb[0]!=0)
			{
				if(StringToInt(Gssb)==3)
				{
					GetStrSpaceBefore(szLine,2,Gssb);
					nCount= hex2dec(Gssb);
					break;
				}	
			}
/*///////////////////////////////方法二 系统sscanf取字符串///////////////////////////////////////////////////////			
			Gssb=GetStrsscanf(szLine,3);
			if(Gssb!=0)
			{
				if(StringToInt(Gssb)==3)
				{
					nCount= hex2dec(GetStrsscanf(szLine,2));
					break;
				}	
			}
*/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		}
	}
	fclose(f);
	f = NULL;
	return nCount;
}
char *GetNetCardDNS(const char* szDevName)
{
	int nCount = 0;
	char szLine[512];
	FILE* f = fopen("/etc/resolv.conf", "r");
	if (f<=0)
	{
		fprintf(stderr, "Open /etc/resolv.conf failed!errno:%d\n", errno);
		return "";
	}
	while(fgets(szLine, sizeof(szLine), f))
	{
		char szName[128] = {0};
		sscanf(szLine, "%s", szName);
		int nLen = strlen(szName);
		if (nLen <= 0)continue;
		if (szName[nLen - 1] == ' ') szName[nLen - 1] = 0;
		if (strcmp(szName, "nameserver") == 0)
		{
			sscanf(szLine+strlen(szName)+1, "%s", StrGetStrSpace);
			printf("%s",StrGetStrSpace);
			fclose(f);
			return StrGetStrSpace;
		}
	}
	fclose(f);
	f = NULL;
	return "";
}

void IOctlNetInfo(const char* szDevName,wifiInfo * wlanx)
{
	struct ifreq ifr;
	unsigned char mac[6];
	unsigned long nIP, nNetmask, nBroadIP, nGateway;
	int s = socket(AF_INET, SOCK_DGRAM, 0);
	if (s < 0)
	{
		fprintf(stderr, "Create socket failed!errno=%d", errno);
		return;
	}


	printf("%s:\n", szDevName);

	strcpy(ifr.ifr_name, szDevName);
	if (ioctl(s, SIOCGIFHWADDR, &ifr) < 0)
	{
		close(s);
		return;
	}
	memcpy(mac, ifr.ifr_hwaddr.sa_data, sizeof(mac));
	sprintf(wlanx->mac,"%02x:%02x:%02x:%02x:%02x:%02x",mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
	strcpy(ifr.ifr_name, szDevName);
	if (ioctl(s, SIOCGIFADDR, &ifr) < 0)
	{
		nIP = 0;
	}
	else
	{
		nIP = *(unsigned long*)&ifr.ifr_broadaddr.sa_data[2];
	}
	sprintf(wlanx->ip,"%s", inet_ntoa(*(struct in_addr*)&nIP));

	strcpy(ifr.ifr_name, szDevName);
	if (ioctl(s, SIOCGIFBRDADDR, &ifr) < 0)
	{
		nBroadIP = 0;
	}
	else
	{
		nBroadIP = *(unsigned long*)&ifr.ifr_broadaddr.sa_data[2];
	}
	sprintf(wlanx->BroadIP,"%s", inet_ntoa(*(struct in_addr*)&nBroadIP));

	strcpy(ifr.ifr_name, szDevName);
	if (ioctl(s, SIOCGIFNETMASK, &ifr) < 0)
	{
		nNetmask = 0;
	}
	else
	{
		nNetmask = *(unsigned long*)&ifr.ifr_netmask.sa_data[2];
	}
	sprintf(wlanx->netmask,"%s", inet_ntoa(*(struct in_addr*)&nNetmask));
	close(s);

	//gateway
	nGateway=GetNetCardgatway(szDevName);
	sprintf(wlanx->gateway,"%s", inet_ntoa(*(struct in_addr*)&nGateway));
	sprintf(wlanx->domain,"%s",GetNetCardDNS(szDevName));//DNS
	if(!strcmp(szDevName,"wlan0"))
		NetwlanGetLinkInfo(szDevName,wlanx);
}

int NetwlanGetLinkInfo(const char* szDevName,wifiInfo * wlanx)
{
		int nCount = 0;
		char szLine[512];
		char Linesub[200];
		char szName[128] = {0};
		FILE* f = fopen("/tmp/wlan0connect.tmp", "r");
		if (f<=0)
		{
			fprintf(stderr, "Open %s failed!errno:%d\n","/tmp/wlan0connect.tmp", errno);
			return 0;
		}
		while(fgets(szLine, sizeof(szLine), f))
		{
			int nL=GetStrSpaceBefore(szLine,0,szName);
			if (nL <= 0)	continue;
			if (szName[nL - 1] == '\n') szName[nL - 1] = 0;
			if (!strcmp(szName, "Connected"))
			{
				sprintf(wlanx->wifistatus,"%s","connected");
				if(GetStrSpaceBefore(szLine,2,Linesub)!=17)
					continue;
				sprintf(wlanx->SSIDmac,"%s",Linesub);
			}
			else 
			{
				GetStrSpaceBefore(szLine,0,szName);
				if(!strcmp(szName,"Not"))
				{
					sprintf(wlanx->wifistatus,"%s","notconnected");
				}
				else if(!strcmp(szName,"SSID:"))
				{
					nL = GetStrSpaceBefore(szLine,1,Linesub);
					if (Linesub[nL - 1] == '\n') Linesub[nL - 1] = 0;
					sprintf(wlanx->SSID,"%s",Linesub);
				}
				else if(!strcmp(szName,"freq:"))
				{
					nL = GetStrSpaceBefore(szLine,1,Linesub);
					if (Linesub[nL - 1] == '\n') Linesub[nL - 1] = 0;
					wlanx->frequency=StringToInt(Linesub);
				}
				else if(!strcmp(szName,"signal:"))
				{
					GetStrSpaceBefore(szLine,1,Linesub);
					wlanx->SignalLength=StringToInt(Linesub);
				}
			}
		}
		fclose(f);
		system("iw dev wlan0 link > /tmp/wlan0connect.tmp");
}


int NetwlanScanSSID(const char* szDevName,wifiInfo * wlanx)
{
	int nCount = 0;
	char szLine[512];
	char Linesub[200];
	int readfiletimes=0;
	char szName[128] = {0};
	int nL;
	char wlanScanSSIDcmd[MAXSTRLEN];
//	sprintf(wlanScanSSIDcmd,"iw dev wlan0 scan > %s &",__WLAN_SCAN_FIL);
//	system(wlanScanSSIDcmd);
//	sleep(5);
	FILE* f = fopen(__WLAN_SCAN_FIL, "r");
	if (f<=0)
	{
		fprintf(stderr, "Open %s failed!errno:%d\n",__WLAN_SCAN_FIL, errno);
//		fclose(f);
		wlanx->maxlistindex=0;
		return 2;
	}
	printf("NetwlanScanSSID\n");
	while(fgets(szLine, sizeof(szLine), f))
	{
//		readfiletimes++;
		int nLen = GetStrSpaceBefore(szLine,0,szName);
		if (nLen <= 0)continue;
		if (szName[nLen - 1] == ' ') szName[nLen - 1] = 0;
		if (strcmp(szName, "BSS") == 0)
		{
			if(GetStrSpaceBefore(szLine,1,Linesub)!=20)
				continue;
			Linesub[17]=0;
			nCount++;
			sprintf(wlanx->SSIDAP[nCount].SSIDmac,"%s",Linesub);
			wlanx->SSIDAP[nCount].index=nCount;
			printf("NO.%d AP\n",nCount);
		}
		else 
		{
			GetStrSpaceBefore(szLine,0,szName);

			if(!strcmp(szName,"SSID:"))
			{
				nL = GetStrSpaceBefore(szLine,1,Linesub);
				if (Linesub[nL - 1] == '\n') Linesub[nL - 1] = 0;
				sprintf(wlanx->SSIDAP[nCount].SSID,"%s",Linesub);
			}		
			else if(!strcmp(szName,"signal:"))
			{
				GetStrSpaceBefore(szLine,1,Linesub);
				wlanx->SSIDAP[nCount].SignalLength=StringToInt(Linesub);
			}
			else if(!strcmp(szName,"freq:"))
			{
				GetStrSpaceBefore(szLine,1,Linesub);
				wlanx->SSIDAP[nCount].frequency=StringToInt(Linesub);
			}
			else if(!strcmp(szName,"RSN:"))
			{
				sprintf(wlanx->SSIDAP[nCount].WPAWEP,"%s","WPA");
			}		
			else 
			{
				GetStrSpaceBefore(szLine,3,szName);
				if(!strcmp(szName,"channel"))
				{
					GetStrSpaceBefore(szLine,4,Linesub);
					wlanx->SSIDAP[nCount].channel=StringToInt(Linesub);
				}
			}
		}
	}
	fclose(f);
	wlanx->maxlistindex=nCount;
	if(nCount==0)
	{
		if(wifimodisinsmod()==0)
		{
			system("rm /etc/udev/rules.d/*");
			system("insmod /root/8188eu.ko");
			system("ifconfig wlan0 up");
		}
		else
		{
			system("ifconfig wlan0 up");
		}
	}
	f = NULL;
	sprintf(wlanScanSSIDcmd,"iw dev wlan0 scan > %s &",__WLAN_SCAN_FIL);
	system(wlanScanSSIDcmd);
	return 0;
}

int HostapdGetConfig(const char* szDevName,wifiInfo * wlanx)
{
	int nCount = 0;
	char szLine[512];
	char Linesub[200];
	FILE* f = fopen(__WLAN_HOSTAPD_CONFIG_FIL, "r");
	if (f<=0)
	{
		fprintf(stderr, "Open %s failed!errno:%d\n",__WLAN_HOSTAPD_CONFIG_FIL, errno);
		return 0;
	}
	while(fgets(szLine, sizeof(szLine), f))
	{
		int nLen = GetStrSpaceBefore(szLine,0,Linesub);
		if (nLen <= 1)	continue;
		if(!strcmp(Linesub,"driver"))
		{
			nLen = GetStrSpaceBefore(szLine,1,Linesub);
			if (Linesub[nLen - 1] == '\n') Linesub[nLen - 1] = 0;
			sprintf(wlanx->HostAPd.driver,"%s",Linesub);
		}
		else if(!strcmp(Linesub,"ssid"))
		{
			nLen = GetStrSpaceBefore(szLine,1,Linesub);
			if (Linesub[nLen - 1] == '\n') Linesub[nLen - 1] = 0;
			sprintf(wlanx->HostAPd.ssid,"%s",Linesub);
		}
		else if(!strcmp(Linesub,"hw_mode"))
		{
			nLen = GetStrSpaceBefore(szLine,1,Linesub);
			if (Linesub[nLen - 1] == '\n') Linesub[nLen - 1] = 0;
			sprintf(wlanx->HostAPd.hw_mode,"%s",Linesub);
		}
		else if(!strcmp(Linesub,"channel"))
		{
			nLen = GetStrSpaceBefore(szLine,1,Linesub);
			if (Linesub[nLen - 1] == '\n') Linesub[nLen - 1] = 0;
			sprintf(wlanx->HostAPd.channel,"%s",Linesub);
		}
		else if(!strcmp(Linesub,"auth_algs"))
		{
			nLen = GetStrSpaceBefore(szLine,1,Linesub);
			if (Linesub[nLen - 1] == '\n') Linesub[nLen - 1] = 0;
			sprintf(wlanx->HostAPd.auth_algs,"%s",Linesub);
		}
		else if(!strcmp(Linesub,"wpa"))
		{
			nLen = GetStrSpaceBefore(szLine,1,Linesub);
			if (Linesub[nLen - 1] == '\n') Linesub[nLen - 1] = 0;
			sprintf(wlanx->HostAPd.wpa,"%s",Linesub);
		}
		else if(!strcmp(Linesub,"wpa_passphrase"))
		{
			nLen = GetStrSpaceBefore(szLine,1,Linesub);
			if (Linesub[nLen - 1] == '\n') Linesub[nLen - 1] = 0;
			sprintf(wlanx->HostAPd.wpa_passphrase,"%s",Linesub);
		}
		else if(!strcmp(Linesub,"wpa_key_mgmt"))
		{
			nLen = GetStrSpaceBefore(szLine,1,Linesub);
			if (Linesub[nLen - 1] == '\n') Linesub[nLen - 1] = 0;
			sprintf(wlanx->HostAPd.wpa_key_mgmt,"%s",Linesub);
		}
		else if(!strcmp(Linesub,"wpa_pairwise"))
		{
			nLen = GetStrSpaceBefore(szLine,1,Linesub);
			if (Linesub[nLen - 1] == '\n') Linesub[nLen - 1] = 0;
			sprintf(wlanx->HostAPd.wpa_pairwise,"%s",Linesub);
		}
		else if(!strcmp(Linesub,"interface"))
		{
			nLen = GetStrSpaceBefore(szLine,1,Linesub);
			if (Linesub[nLen - 1] == '\n') Linesub[nLen - 1] = 0;
			sprintf(wlanx->HostAPd.interface,"%s",Linesub);
		}
		else if(!strcmp(Linesub,"bridge"))
		{
			nLen = GetStrSpaceBefore(szLine,1,Linesub);
			if (Linesub[nLen - 1] == '\n') Linesub[nLen - 1] = 0;
			sprintf(wlanx->HostAPd.bridge,"%s",Linesub);
		}
		else if(!strcmp(Linesub,"ignore_broadcast_ssid"))
		{
			nLen = GetStrSpaceBefore(szLine,1,Linesub);
			if (Linesub[nLen - 1] == '\n') Linesub[nLen - 1] = 0;
			sprintf(wlanx->HostAPd.ignore_broadcast_ssid,"%s",Linesub);
		}
		else if(!strcmp(Linesub,"macaddr_acl"))
		{
			nLen = GetStrSpaceBefore(szLine,1,Linesub);
			if (Linesub[nLen - 1] == '\n') Linesub[nLen - 1] = 0;
			sprintf(wlanx->HostAPd.macaddr_acl,"%s",Linesub);
		}
		else if(!strcmp(Linesub,"accept_mac_file"))
		{
			nLen = GetStrSpaceBefore(szLine,1,Linesub);
			if (Linesub[nLen - 1] == '\n') Linesub[nLen - 1] = 0;
			sprintf(wlanx->HostAPd.accept_mac_file,"%s",Linesub);
		}
		else if(!strcmp(Linesub,"deny_mac_file"))
		{
			nLen = GetStrSpaceBefore(szLine,1,Linesub);
			if (Linesub[nLen - 1] == '\n') Linesub[nLen - 1] = 0;
			sprintf(wlanx->HostAPd.deny_mac_file,"%s",Linesub);
		}
	}
	fclose(f);
	f = NULL;
	sprintf(wlanx->HostAPd.SOFTAPrun,"%s","off");
	f=popen("ps -e | grep \'hostapd\' | awk \'{print $1}\'","r");
	if(f<=0)
	{
		printf("\n\ncommand ps -e | grep \'hostapd\' | awk \'{print $1}\' can not run\n\n");
		return 0;
	}
	while(fgets(szLine, sizeof(szLine), f))
	{
		printf("hostapd pid is %s",szLine);
		sprintf(wlanx->HostAPd.SOFTAPrun,"%s","on");
	}
	pclose(f);
	return 0;
}

int HostapdGetDhcplist(const char* szDevName,wifiInfo * wlanx)
{
	int nCount = 0;
	char szLine[512];
	char Linesub[200];
	char *cnt;
	FILE* f = fopen(__WLAN_HOSTAPD_DHCPLIST_FIL, "r");
	if (f<=0)
	{
		fprintf(stderr, "Open %s failed!errno:%d\n",__WLAN_HOSTAPD_DHCPLIST_FIL, errno);
		return 0;
	}
	while(fgets(szLine, sizeof(szLine), f))
	{
		int nLen = GetStrSpaceBefore(szLine,0,Linesub);
		if (nLen <= 1)	continue;
		if(!strcmp(Linesub,"lease"))
		{
			nLen = GetStrSpaceBefore(szLine,1,Linesub);
			if (nLen<7) continue;
			nCount++;
			sprintf(wlanx->STAlist[nCount].ip,"%s",Linesub);
		}
		else if(!strcmp(Linesub,"starts"))
		{
			cnt=szLine;
			while((*cnt==' ')||(*cnt=='\t')||(*cnt=='='))cnt++;
			nLen = strlen(cnt);
			if (cnt[nLen - 1] == '\n') cnt[nLen - 2] = 0;
			strcpy(wlanx->STAlist[nCount].starts,cnt);
		}
		else if(!strcmp(Linesub,"ends"))
		{
			cnt=szLine;
			while((*cnt==' ')||(*cnt=='\t')||(*cnt=='='))cnt++;
			nLen = strlen(cnt);
			if (cnt[nLen - 1] == '\n') cnt[nLen - 2] = 0;
			strcpy(wlanx->STAlist[nCount].ends,cnt);
		}
		else if(!strcmp(Linesub,"cltt"))
		{
			cnt=szLine;
			while((*cnt==' ')||(*cnt=='\t')||(*cnt=='='))cnt++;
			nLen = strlen(cnt);
			if (cnt[nLen - 1] == '\n') cnt[nLen - 2] = 0;
			strcpy(wlanx->STAlist[nCount].cltt,cnt);
		}
		else if(!strcmp(Linesub,"binding"))
		{
			nLen = GetStrSpaceBefore(szLine,2,Linesub);
			if (Linesub[nLen - 1] == '\n') Linesub[nLen - 2] = 0;
			sprintf(wlanx->STAlist[nCount].binding_state,"%s",Linesub);
		}
		else if(!strcmp(Linesub,"next"))
		{
			cnt=szLine;
			while((*cnt==' ')||(*cnt=='\t')||(*cnt=='='))cnt++;
			nLen = strlen(cnt);
			if (cnt[nLen - 1] == '\n') cnt[nLen - 2] = 0;
			strcpy(wlanx->STAlist[nCount].next_binding_state,cnt);
		}
		else if(!strcmp(Linesub,"rewind"))
		{
			cnt=szLine;
			while((*cnt==' ')||(*cnt=='\t')||(*cnt=='='))cnt++;
			nLen = strlen(cnt);
			if (cnt[nLen - 1] == '\n') cnt[nLen - 2] = 0;
			strcpy(wlanx->STAlist[nCount].rewind_binding_state,cnt);
		}
		else if(!strcmp(Linesub,"hardware"))
		{
			nLen = GetStrSpaceBefore(szLine,2,Linesub);
			if (Linesub[nLen - 1] == '\n') Linesub[nLen - 2] = 0;
			sprintf(wlanx->STAlist[nCount].mac,"%s",Linesub);
		}
		else if(!strcmp(Linesub,"uid"))
		{
			cnt=szLine;
			while((*cnt==' ')||(*cnt=='\t')||(*cnt=='='))cnt++;
			nLen = strlen(cnt);
			if (cnt[nLen - 1] == '\n') cnt[nLen - 2] = 0;
			strcpy(wlanx->STAlist[nCount].uid,cnt);
		}
		else if(!strcmp(Linesub,"client-hostname"))
		{
			nLen = GetStrSpaceBefore(szLine,1,Linesub);
			if (Linesub[nLen - 1] == '\n') Linesub[nLen - 2] = 0;
			sprintf(wlanx->STAlist[nCount].client_hostname,"%s",Linesub);
		}
	}
	fclose(f);
	wlanx->maxlistindex=nCount+1;
	f = NULL;
	return 0;
}
int wifimodisinsmod(void)
{
	FILE *f;
	char szLine[512];
	char Linesub[32];
	int result=0;
	f=popen("lsmod | grep 8188eu","r");
	if(f<=0)
	{
		printf("\n\ncommand lsmod | grep 8188eu\n\n");
		return 0;
	}
	while(fgets(szLine, sizeof(szLine), f))
	{
		GetStrSpaceBefore(szLine,0,Linesub);
		if(!strcmp(Linesub,"8188eu"))
			result=1;
	}
	pclose(f);
	return result;
}
int wifiwlan0disconnect(void)
{
	FILE *f;
	char szLine[512],shellcmd[256];
	system("iw dev wlan0 disconnect");
	system("pkill -9 vsftpd");
	system("pkill -9 udhcpc");
	system("pkill -9 wpa_supplicant");
	system("rmmod 8188eu");
	return 0;
}

int wifiwlan0connect(char * conf,char * ssid ,char * psw, char * wpa)
{
	FILE *f;
	char szLine[512],shellcmd[1024];
	system("ifconfig wlan0 up");
	if((conf!=0)&&(conf[0]!=0))
	{
		sprintf(shellcmd,"echo \"wpa_supplicant -Dnl80211 -iwlan0 -c%s &\" > /tmp/netcodexec.sh",conf);
		system(shellcmd);
		system("echo \"udhcpc -i wlan0 &\" >> /tmp/netcodexec.sh");
		system("echo \"/usr/local/sbin/vsftpd &\" >> /tmp/netcodexec.sh");
		system("echo \"ntpdate ntp.ubuntu.com &\" >> /tmp/netcodexec.sh");
		system("chmod 777 /tmp/netcodexec.sh");
		system("/tmp/netcodexec.sh &");

	}
	else
	{
		if((strcmp(wpa,"wpa"))&&(strcmp(wpa,"WPA"))&&(strcmp(wpa,"WPA2"))&&(strcmp(wpa,"wpa2")))
		{
			if((psw==0)||(psw[0]==0))
				sprintf(shellcmd,"echo \"iw dev wlan0 connect %s &\" > /tmp/netcodexec.sh",ssid);
			else
			sprintf(shellcmd,"echo \"iw dev wlan0 connect %s key 0:%s &\" > /tmp/netcodexec.sh",ssid,psw);

			system(shellcmd);
			system("echo \"udhcpc -i wlan0 &\" >> /tmp/netcodexec.sh");
			system("echo \"/usr/local/sbin/vsftpd &\" >> /tmp/netcodexec.sh");
			system("echo \"ntpdate ntp.ubuntu.com &\" >> /tmp/netcodexec.sh");
			system("chmod 777 /tmp/netcodexec.sh");
			system("/tmp/netcodexec.sh &");
		}
		else
		{
			sprintf(szLine,"%s","/etc/wpa_supplicant.conf");
			conf=szLine;
			sprintf(shellcmd,"echo \"ctrl_interface=/var/run/wpa_supplicant\nctrl_interface_group=0\nap_scan=1\nnetwork={\n\tssid=\\\"%s\\\"\n\tkey_mgmt=WPA-EAP WPA-PSK IEEE8021X NONE\n\tpairwise=TKIP CCMP\n\tgroup=CCMP TKIP WEP104 WEP40\n\tpsk=\\\"%s\\\"\n}\" >  %s",ssid,psw,conf);
//			printf(shellcmd);
			system(shellcmd);
//			sprintf(shellcmd,"wpa_supplicant -Dnl80211 -iwlan0 -c%s",conf);
//			system(shellcmd);
//			system("udhcpc -i wlan0&");
//			system("/usr/local/sbin/vsftpd");
			sprintf(shellcmd,"echo \"wpa_supplicant -Dnl80211 -iwlan0 -c%s &\" > /tmp/netcodexec.sh",conf);
			system(shellcmd);
			system("echo \"udhcpc -i wlan0 &\" >> /tmp/netcodexec.sh");
			system("echo \"/usr/local/sbin/vsftpd &\" >> /tmp/netcodexec.sh");
			system("echo \"ntpdate ntp.ubuntu.com &\" >> /tmp/netcodexec.sh");
			system("chmod 777 /tmp/netcodexec.sh");
			system("/tmp/netcodexec.sh &");

		}
		
	}
	

}

int wifiwlan0setnet(char *dhcp,char *ip,char *netmask,char *gateway)
{
	char ipinfo[1024];
	if(!strcmp(dhcp,"dhcp"))
	{
		system("echo \"# interfaces(5) file used by ifup(8) and ifdown(8)\n# Include files from /etc/network/interfaces.d:\nsource-directory /etc/network/interfaces.d\nauto eth0\nallow-hotplug eth0\nauto wlan0\niface wlan0 inet dhcp\" > /etc/network/interfaces");
		system("/etc/init.d/networking restart");
		
	}
	else if(!strcmp(dhcp,"default"))
	{
		system("echo \"# interfaces(5) file used by ifup(8) and ifdown(8)\n# Include files from /etc/network/interfaces.d:\nsource-directory /etc/network/interfaces.d\nauto eth0\nallow-hotplug eth0\nauto wlan0\niface wlan0 inet dhcp\" > /etc/network/interfaces");
		system("echo \"\nnameserver 114.114.114.114\" > /etc/resolv.conf");
		system("/etc/init.d/networking restart");
		system("/etc/init.d/resolvconf restart");
		
	}
	else if(!strcmp(dhcp,"staticip"))
	{
		sprintf(ipinfo,"echo \"# interfaces(5) file used by ifup(8) and ifdown(8)\n# Include files from /etc/network/interfaces.d:\nsource-directory /etc/network/interfaces.d\nauto eth0\nallow-hotplug eth0\nauto wlan0\niface wlan0 inet static\naddress %s\nnetmask %s\ngateway %s\" > /etc/network/interfaces",ip,netmask,gateway);
		system(ipinfo);
		
	}
	else if(!strcmp(dhcp,"domain"))
	{
		sprintf(ipinfo,"echo \"nameserver %s\" > /etc/resolv.conf",ip);
		system(ipinfo);
	}
}
#define __WLAN_HOSTAPD_CONFIG_TMP "/tmp/hostapdtmp.tmp"

int wifiwlan1setconn(char *item,char *val,wifiInfo * wlanx)

{
	char cnt[1024];
	char ipa[20];
	int i;
	if(!strcmp(item,"SOFTAPrun"))
	{
		if(!strcmp(val,"on"))
		{
			if(wifimodisinsmod()==0)
			{
				system("rm /etc/udev/rules.d/*");
				system("insmod /root/8188eu.ko");
				system("ifconfig wlan0 up");
			}
			printf("\n\n\n\n  ifconfig wlan1 up  \n\n\n\n");
			system("echo \"ifconfig wlan1 up\" > /tmp/netcodexed.sh");
			if((wlanx->ip[0]==0)||(wlanx->ip[0]=='0'))
			{
				strcpy(wlanx->ip,"192.168.2.1");
				strcpy(wlanx->netmask,"255.255.255.0");
			}
			sprintf(cnt,"echo \"ifconfig wlan1 %s netmask %s up\" >> /tmp/netcodexed.sh",wlanx->ip,wlanx->netmask);
			system(cnt);
			if(dhcpduseisc)
			{
				memset(ipa,0,20);
				strcpy(ipa,wlanx->ip);
				for(i=strlen(wlanx->ip);i>0;i--)
				{
					if(ipa[i]=='.')
					{
						ipa[i+1]=0;
						break;
					}
				}
				printf("\n\nip is %s\n\n",wlanx->ip);
				printf("\n\nipa is %s\n\n",ipa);
				if(wlanx->domain[0]==0)
					sprintf(wlanx->domain,"%s","114.114.114.114");
				sprintf(cnt,"echo \"ddns-update-style none;\noption domain-name-servers %s, %s;\ndefault-lease-time 600;\nmax-lease-time 7200;\nlog-facility local7;\nsubnet %s0 netmask %s {\n\trange %s100 %s200;\n\toption routers %s;\n\toption domain-name-servers %s;\n}\" > /etc/dhcp/dhcpd.conf",
				wlanx->ip,
				wlanx->domain,
				ipa,
				wlanx->netmask,
				ipa,
				ipa,
				wlanx->ip,
				wlanx->domain);
				system(cnt);
				system("echo \"service isc-dhcp-server start\" >> /tmp/netcodexed.sh");
				system("echo \"sysctl net.ipv4.ip_forward=1\" >> /tmp/netcodexed.sh");
			}
			sprintf(cnt,"echo \"hostapd %s &\" >> /tmp/netcodexed.sh",__WLAN_HOSTAPD_CONFIG_FIL);
			system(cnt);
			if(dhcpduseisc) system("echo \"/etc/init.d/isc-dhcp-server restart\" >> /tmp/netcodexed.sh");
			else system("echo \"udhcpd -fS /etc/udhcpd.conf\" >> /tmp/netcodexed.sh");
			system("chmod 777 /tmp/netcodexed.sh");
			system("/tmp/netcodexed.sh");
		}
		else if(!strcmp(val,"off"))
		{
			wifiwlan1hostapdoff();
		}
	}
}
int wifiwlan1hostapdoff(void)
{
	FILE *f;
	char szLine[512],shellcmd[256];
	printf("\n\n\n\n wifiwlan1hostapdoff  \n\n\n\n");
	system("pkill -9 hostapd");
	system("pkill -9 dhcpd");
	system("pkill -9 udhcpd");
	system("ifconfig wlan1 down");
	return 0;

}
int wifiwlan1setconf(char *item,char *val)
{
	int nCount = 0;
	char szLine[512];
	char Linesub[200];
	FILE* ft=fopen(__WLAN_HOSTAPD_CONFIG_TMP, "w");
	if (ft<=0)
	{
		fprintf(stderr, "Open %s failed!errno:%d\n",__WLAN_HOSTAPD_CONFIG_TMP, errno);
		return 0;
	}
	FILE* f = fopen(__WLAN_HOSTAPD_CONFIG_FIL, "r");
	if (f<=0)
	{
		fprintf(stderr, "Open %s failed!errno:%d\n",__WLAN_HOSTAPD_CONFIG_FIL, errno);
		return 0;
	}
	while(fgets(szLine, sizeof(szLine), f))
	{
		int nLen = GetStrSpaceBefore(szLine,0,Linesub);
		if(!strcmp(Linesub,item))
		{
			sprintf(szLine,"%s=%s\n",item,val);
			fputs(szLine,ft);
			nCount++;
		}
		else
		{
			fputs(szLine,ft);
		}
	}
	if(nCount==0)
	{
		sprintf(szLine,"%s=%s\n",item,val);
		fputs(szLine,ft);
	}
	fclose(f);
	fclose(ft);
	f = NULL;ft=NULL;
	sprintf(szLine,"mv %s %s",__WLAN_HOSTAPD_CONFIG_TMP,__WLAN_HOSTAPD_CONFIG_FIL);
	system(szLine);
	if(dhcpduseisc==0)
		return 0;
	nCount=0;
	ft=fopen(__WLAN_ISCDHCPSERVER_CONFIG_FIL, "wr");
	if (ft<=0)
	{
		fprintf(stderr, "Open %s failed!errno:%d\n",__WLAN_ISCDHCPSERVER_CONFIG_FIL, errno);
		return 0;
	}
	while(fgets(szLine, sizeof(szLine), ft))
	{
//		if(szLine[0]=='#')	continue;
		int nLen = GetStrSpaceBefore(szLine,0,Linesub);
//		if (nLen <= 1)	continue;
		if(!strcmp(Linesub,"INTERFACES"))
		{
			sprintf(szLine,"%s=%s\n","INTERFACES","\"wlan1\"");
			fputs(szLine,ft);
			nCount++;
		}
		else
		{
			fputs(szLine,ft);
		}
	}
	if(nCount==0)
	{
		sprintf(szLine,"%s=%s\n","INTERFACES","\"wlan1\"");
		fputs(szLine,ft);
	}
	fclose(ft);ft=NULL;

	return 0;

}
int wifiwlan1setconfdefault(const char* szDevName,wifiInfo * wlanx)
{
	char szLine[512];
	char Linesub[200];
	int nCount = 0;
	FILE* ft=fopen(__WLAN_HOSTAPD_CONFIG_TMP, "w");
	if (ft<=0)
	{
		fprintf(stderr, "Open %s failed!errno:%d\n",__WLAN_HOSTAPD_CONFIG_TMP, errno);
		return 0;
	}
	fputs("interface=wlan1\n",ft);
	fputs("#bridge=br0\n",ft);
	fputs("driver=nl80211\n",ft);
	fputs("ssid=m1_device\n",ft);
	fputs("hw_mode=g\n",ft);
	fputs("channel=1\n",ft);
	fputs("auth_algs=1\n",ft);
	fputs("wpa=1\n",ft);
	fputs("wpa_passphrase=1234567890\n",ft);
	fputs("wpa_key_mgmt=WPA-PSK\n",ft);
	fputs("wpa_pairwise=CCMP\n",ft);
	fclose(ft);
	ft=NULL;
	sprintf(szLine,"mv %s %s",__WLAN_HOSTAPD_CONFIG_TMP,__WLAN_HOSTAPD_CONFIG_FIL);
	system(szLine);
	ft=fopen(__WLAN_ISCDHCPSERVER_CONFIG_FIL, "wr");
	if(ft<=0)
	{
		dhcpduseisc=0;
	}
	else
	{
		while(fgets(szLine, sizeof(szLine), ft))
		{
	//		if(szLine[0]=='#')	continue;
			int nLen = GetStrSpaceBefore(szLine,0,Linesub);
	//		if (nLen <= 1)	continue;
			if(!strcmp(Linesub,"INTERFACES"))
			{
				sprintf(szLine,"%s=%s\n","INTERFACES","\"wlan1\"");
				fputs(szLine,ft);
				nCount++;
			}
			else
			{
				fputs(szLine,ft);
			}
		}
		if(nCount==0)
		{
			sprintf(szLine,"%s=%s\n","INTERFACES","\"wlan1\"");
			fputs(szLine,ft);
		}
		fclose(ft);ft=NULL;
	}
	IOctlNetInfo(szDevName,wlanx);
	char SSIDAP[32]="M1_";
	char SSIDPSWAP[32]="HIKE*687467*899076";
	char SSIDT[10];
	SSIDT[0]=wlanx->mac[12]>'a'?wlanx->mac[12]-'a'+'0':wlanx->mac[12];
	SSIDT[1]=wlanx->mac[13]>'a'?wlanx->mac[13]-'a'+'0':wlanx->mac[13];
	SSIDT[2]=wlanx->mac[15]>'a'?wlanx->mac[15]-'a'+'0':wlanx->mac[15];
	SSIDT[3]=wlanx->mac[16]>'a'?wlanx->mac[16]-'a'+'0':wlanx->mac[16];
	SSIDT[4]=0;
	strcat(SSIDAP,SSIDT);
	wifiwlan1setnet("ssid",SSIDAP,"wlan1",wlanx);
	wifiwlan1setnet("wpa_passphrase",SSIDPSWAP,"wlan1",wlanx);
	wifiwlan1setnet("save","","wlan1",wlanx);
	printf("\n\n\n\n wifi wlan1 set net save wlan1 start ap\n\n\n\n");
	return 0;

}

int wifiwlan1setnet(char *item,char *val,const char* szDevName,wifiInfo * wlanx)
{
	printf("\n wifiwlan1setnet : %s  %s \n",item,val);
	if(wlanx->mac[0]==0)
	{
		IOctlNetInfo(szDevName,wlanx);
		HostapdGetConfig(szDevName,wlanx);
		HostapdGetDhcplist(szDevName,wlanx);
	}
		
	strcmp(item,"ip")?0:strcpy(wlanx->ip,val);
	strcmp(item,"netmask")?0:strcpy(wlanx->netmask,val);
	strcmp(item,"BroadIP")?0:strcpy(wlanx->BroadIP,val);
	strcmp(item,"gateway")?0:strcpy(wlanx->gateway,val);
	strcmp(item,"domain")?0:strcpy(wlanx->domain,val);
	strcmp(item,"mac")?0:strcpy(wlanx->mac,val);

	strcmp(item,"driver")?0:wifiwlan1setconf(item,val);//wlanx->HostAPd.driver);
	strcmp(item,"ssid")?0:wifiwlan1setconf(item,val);//wlanx->HostAPd.ssid);
	strcmp(item,"hw_mode")?0:wifiwlan1setconf(item,val);//wlanx->HostAPd.hw_mode);
	strcmp(item,"channel")?0:wifiwlan1setconf(item,val);//wlanx->HostAPd.channel);
	strcmp(item,"auth_algs")?0:wifiwlan1setconf(item,val);//wlanx->HostAPd.auth_algs);
	strcmp(item,"wpa")?0:wifiwlan1setconf(item,val);//wlanx->HostAPd.wpa);
	strcmp(item,"wpa_passphrase")?0:wifiwlan1setconf(item,val);//wlanx->HostAPd.wpa_passphrase);
	strcmp(item,"wpa_key_mgmt")?0:wifiwlan1setconf(item,val);//wlanx->HostAPd.wpa_key_mgmt);
	strcmp(item,"wpa_pairwise")?0:wifiwlan1setconf(item,val);//wlanx->HostAPd.wpa_pairwise);
	strcmp(item,"interface")?0:wifiwlan1setconf(item,val);//wlanx->HostAPd.interface);
	strcmp(item,"bridge")?0:wifiwlan1setconf(item,val);//wlanx->HostAPd.bridge);
	strcmp(item,"ignore_broadcast_ssid")?0:wifiwlan1setconf(item,val);//wlanx->HostAPd.ignore_broadcast_ssid);
	strcmp(item,"macaddr_acl")?0:wifiwlan1setconf(item,val);//wlanx->HostAPd.macaddr_acl);
	strcmp(item,"accept_mac_file")?0:wifiwlan1setconf(item,val);//wlanx->HostAPd.accept_mac_file);
	strcmp(item,"deny_mac_file")?0:wifiwlan1setconf(item,val);//wlanx->HostAPd.deny_mac_file);		

	if(!strcmp(item,"save"))
	{
		printf("\n\n=========================wlan1set save=====================\n\n");
		wifiwlan1setconn("SOFTAPrun","off",wlanx);
		
		wifiwlan1setconn("SOFTAPrun","on",wlanx);
	}
	if(!strcmp(item,"default"))
	{
		wifiwlan1setconn(item,val,wlanx);
	}

	if(!strcmp(item,"SOFTAPrun"))
	{
		wifiwlan1setconn(item,val,wlanx);
	}

}
//////////////////////////////////////////////////////////////////////////////////////////////////////
