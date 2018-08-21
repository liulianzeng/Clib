#include "susuClib_base.h"
#include <stdarg.h>

static int lock=0;
static char StrNULL[]="";
int DEBUG_BASE_MEM=0;
void myclock(int *lock)
{
    int tmp = 1;
    while (tmp == 1)
    {
        tmp = *lock; // 如果 *lock 为 1，说明锁被占用，这时候会死循环。
        *lock = 1; // 无论锁有没有被占用，这里都将共享内存的值置 1.
    }
}
void mycunlock(int *lock)
{
    *lock=0;
}

#ifndef eax 
	#define myasmlock myclock
#else
void myasmlock(int *lock)
{
	__asm__ __volatile__ ("1:\n\t"
                          "movl $1, %%eax\n\t"
                          "lock xchg %%eax, %0\n\t" /* 将 lock 中的值和 eax 进行交换, 相当于 eax = lock, lock = 1 */
                          "test %%eax, %%eax\n\t"/* 判断 eax 是否为 1 */
                          "jnz 1b" /* 如果为 1 则跳到标记 1 的地方继续执行 */
                          ::"m"(*lock)
                          :"%eax"
                          );
}
#endif
void myasmunlock(int *lock)
{
    *lock = 0;
}
void printdim(char *header,void *dim,int len)
{
    mylock();
    size_t lenmal=strlen("Addr ptr = 0x%X || %s length is %d>>>>>")+10+strlen(header)+len*strlen(" FF");
    char *dispstart=mymalloc(lenmal);
    char *dispp=dispstart;
    memset(dispp,0,lenmal);
    sprintf(dispp,"Addr ptr = 0x%08X || %s length is %d>>>>>",(unsigned int)dispstart,header,len);
    dispp+=strlen(dispp);
    for(int j=0;j<len;j++)
    {
        sprintf(dispp," %02X",((unsigned char *)dim)[j]);
        dispp+=strlen(" FF");
    }
    printf(" %s\n",dispstart);
    myfree(dispstart);
    myunlock();
}
int GetStrSpaceBefore(char *src,int num,char *dst)
{
	mylock();
	int j=0,k=0;
	char *StrGetStrSpace;
	while((*src==' ')||(*src=='\t')||(*src=='='))src++;
	if(*src==0)
	{
		strcpy(dst,StrNULL);
		myunlock();
		return 0;
	}
	StrGetStrSpace=mymalloc(strlen(src));
	while(*src!=0)
	{
		if((*src!=' ')&&(*src!='\t')&&(*src!='='))
		{
			StrGetStrSpace[j++]=*src++;
		}
		else
		{
			if(k==num)
			{	
				StrGetStrSpace[j]=0;
				strcpy(dst,StrGetStrSpace);
				myunlock();
				myfree(StrGetStrSpace);
				return strlen(StrGetStrSpace);
			}
			else
			{
				while((*src==' ')||(*src=='\t')||(*src=='='))src++;
				j=0;k++;
			}
		}
	}
	if(k==num)
	{	
		StrGetStrSpace[j]=0;
		strcpy(dst,StrGetStrSpace);
		myunlock();
		myfree(StrGetStrSpace);
		return strlen(StrGetStrSpace);
	}
	else
	{
		strcpy(dst,StrNULL);
		myunlock();
		myfree(StrGetStrSpace);
		return 0;
	}
}
static char StrIntToStr[10];
char * IntToString(int num)
{
    mylock();
	int i=0,j=0,isNeg=0;
	
	char temp[10 + 2];

	if(num<0)
	{
		num*=-1;
		isNeg =1;
	}

	do
	{
		temp[i++] = (num%10)+'0';
		num /= 10;
	}while(num);

	if(isNeg)
		temp[i++]= '-';

	while(i>0)
		StrIntToStr[j++] = temp[--i];

	StrIntToStr[j]='\0';
    myunlock();
	return StrIntToStr;
}


int StringToInt(char *strSTI)  
{  

    mylock();
    char *p = strSTI;
      
    int nNUM = 0;  
      
    int bFlage = 1;  
      
    if (*p == '-')  
    {  
        bFlage = 0;  
        p++;  
    }  
    else if (*p == '+')  
    {  
        p++;  
    }  
      
    while (*p != '\0')  
    {  
        if ((*p < '0') || (*p > '9'))  
        {  
//            nNUM = 0;  
//            return nNUM;  
			break;
        }  
          
        nNUM = nNUM * 10 + (*p - '0');  
        p++;  
    }  
      
    if (!bFlage)  
    {  
        nNUM = 0 - nNUM;  
    }  
    
    myunlock();
    return nNUM;  
}  


/* 
 * 将字符转换为数值
 * */  
int c2i(char ch)  
{  
    mylock();
   // 如果是数字，则用数字的ASCII码减去48, 如果ch = '2' ,则 '2' - 48 = 2
    if((ch>='0')&&(ch<='9'))
    {
        myunlock();
        return ch - 48;
    }
    // 如果是字母，但不是A~F,a~f则返回
    if( ch < 'A' || (ch > 'F' && ch < 'a') || ch > 'z' )
    {
        myunlock();
        return -1;
    }
    // 如果是大写字母，则用数字的ASCII码减去55, 如果ch = 'A' ,则 'A' - 55 = 10
    // 如果是小写字母，则用数字的ASCII码减去87, 如果ch = 'a' ,则 'a' - 87 = 10
    //if(isalpha(ch))
    //        return isupper(ch) ? ch - 55 : ch - 87;
    if((ch<='f')&&(ch>='a'))
    {
        myunlock();
        return ch-'a'+10;
    }
    if((ch<='F')&&(ch>='A'))
    {
        myunlock();
        return ch-'A'+10;
    }
    myunlock();
    return -1;
}  
  
/* 
 * 功能：将十六进制字符串转换为整型(int)数值
 * */  
int hex2dec(char *hex)  
{  
    mylock();
        int len;
        int num = 0;  
        int temp;  
        int bits;  
        int i;  
          
        // 此例中 hex = "1de" 长度为3, hex是main函数传递的
        len = strlen(hex);  
  
        for (i=0, temp=0; i<len; i++, temp=0)  
        {  
                // 第一次：i=0, *(hex + i) = *(hex + 0) = '1', 即temp = 1
                // 第二次：i=1, *(hex + i) = *(hex + 1) = 'd', 即temp = 13
                // 第三次：i=2, *(hex + i) = *(hex + 2) = 'd', 即temp = 14
                temp = c2i( *(hex + i) );  
                // 总共3位，一个16进制位用 4 bit保存
                // 第一次：'1'为最高位，所以temp左移 (len - i -1) * 4 = 2 * 4 = 8 位
                // 第二次：'d'为次高位，所以temp左移 (len - i -1) * 4 = 1 * 4 = 4 位
                // 第三次：'e'为最低位，所以temp左移 (len - i -1) * 4 = 0 * 4 = 0 位
                bits = (len - i - 1) * 4;  
                temp = temp << bits;  
  
                // 此处也可以用 num += temp;进行累加
                num = num | temp;  
        }  
  
        // 返回结果
        myunlock();
        return num;  
}  
int StrToInt(char *str1)
{
    mylock();
    int value  = 0;
    int sign   = 1;
    int result = 0;
    if(NULL == str1)
    {
        myunlock();
        return -1;
    }
    if('-' == *str1)
    {
        sign = -1;
        str1++;
    }
    while(*str1)
    {
        value = value * 10 + *str1 - '0';
        str1++;
    }
    result = sign * value;
    myunlock();
    return result;
}
char * IntToStr(int value,char *n,char *str)
{
    mylock();
	memset(str,0,15);
	sprintf(str,n,value);
    myunlock();
	return str;
}
char * BcdToStr(int value,unsigned char n,char *str)//value 代表具体数值  n代表需产生多少位字符
{
  mylock();
	char *p,m;
	memset(str,0,15);
	p=str;
	for(;n>0;n--)
	{
		m=(value>>(4*n-4))&0x0f;
		if(m>9)
		*p++=m+'A';
		else
		*p++=m+'0';
			
	}
    myunlock();
	return str;
}
char * TelIntToTelStr(long long value,char *str)
{
    mylock();
	unsigned char i,len,*telNOp,telNO[7];
	char str1[20];
	if(value==0)
	{
		memset(str,' ',15);
        myunlock();
		return str;
	}
	telNOp=IntToBcd(value,14,&len,str1);
	for(i=0;i<7;i++)	telNO[i]=telNOp[i];
	memset(str,0,15);
	if(telNO[0])//说明是13位电话号码
	{
		str[0]=(telNO[0]&0x0f)+'0';
		str[1]=((telNO[1]&0xf0)>>4)+'0';
	}
	else
	{
		str[0]=' ';
		str[1]=' ';
	}
    str[2]=(telNO[1]&0x0f)+'0';
    str[3]=((telNO[2]&0xf0)>>4)+'0';
    str[4]=(telNO[2]&0x0f)+'0';
    str[5]=((telNO[3]&0xf0)>>4)+'0';
    str[6]=(telNO[3]&0x0f)+'0';
    str[7]=((telNO[4]&0xf0)>>4)+'0';
    str[8]=(telNO[4]&0x0f)+'0';
    str[9]=((telNO[5]&0xf0)>>4)+'0';
    str[10]=(telNO[5]&0x0f)+'0';
    str[11]=((telNO[6]&0xf0)>>4)+'0';
    str[12]=(telNO[6]&0x0f)+'0';
    myunlock();
	return str;
}
long long TelStrToTelInt(char *str2,char *str)
{
    mylock();
	if((str2[0]>='0')&&(str2[0]<='9'))
	{
		str[0]=str2[0]-'0';
		str[1]=((str2[1]-'0')<<4);
	}
	else
	{
		str[0]=0;
		str[1]=0;
	}
	str[1]=str[1]|(str2[3]-'0');
	str[2]=((str2[4]-'0')<<4)|(str2[5]-'0');
	str[3]=((str2[6]-'0')<<4)|(str2[7]-'0');
	str[4]=((str2[8]-'0')<<4)|(str2[9]-'0');
	str[5]=((str2[10]-'0')<<4)|(str2[11]-'0');
	str[6]=((str2[12]-'0')<<4)|(str2[13]-'0');
    myunlock();
	return BCDToInt((unsigned char *)str,7);
}

long long BCDToInt(unsigned char *str3,unsigned char num)
{
    mylock();
    long long value  = 0;
	unsigned char decplus = 0;
	if(str3[0]==0xff)
	{
		decplus=1;
		str3++;
		num--;
	}
 while(num--)
 {
	 value*=100;
	 value +=((*str3>>4)*10+(*str3&0x0f));
  str3++;
 }
	if(decplus==1)
		value=-value;
    myunlock();
	return value;
}
unsigned char ByteToBcd(unsigned char d)
{
	if(d>=100) return 0;
	return ((d/10))<<4|(d%10);
}
unsigned char *IntToBcd(long long value,unsigned char n,unsigned char *len,char *str)
{
  mylock();
	long long valuec;
	*len=n;
	n=((n-1)>>1);
	memset(str,0,15);
	valuec=value;
	if((value<0)&&(*len!=16))
	{
		valuec=-value;
		n+=1;
	}
	*len=n+1;
	do
	{
		str[n]=valuec%10;valuec/=10;
		str[n]|=(valuec%10)<<4;valuec/=10;
	}
	while(n--);
	if(value<0)
		str[0]=0xff;
    myunlock();
	return (unsigned char *)str;
}


unsigned char *SearchKey(unsigned char *source,unsigned char len,unsigned char *dest)
{
  mylock();
	unsigned char de[5],i,j=0xff;
	for(i=0;i<5;i++)
		if(dest[i]!=0)
        {
            de[i]=dest[i];
            j=i;
        }
		else
            de[i]=0;
	switch(j)
	{
		case 1:
            for(i=1;i<len;i++)	if((source[i]==de[1])&&(source[i-1]==de[0]))
            {
                myunlock();
                return (source+i);
            }
            if(i==len)
            {
                myunlock();
                return 0;
            }
			break;
		case 2:
			for(i=2;i<len;i++)
                if((source[i]==de[2])&&(source[i-1]==de[1])&&(source[i-2]==de[0]))
                {
                    myunlock();
                    return (source+i);
                }
			if(i==len)
            {
                myunlock();
                return 0;
            }
			break;
		case 3:
		default:
			break;
	}
    myunlock();
	return 0;
}

int u8dimtoint1(unsigned char littleend,unsigned char * dimstart,unsigned char len)
{
    mylock();
    int result=0;
    if(littleend)
        while(len--)    result+=((*dimstart++)<<8);
    else
        while(len--)    result=*dimstart+++((result)<<8);
    myunlock();
    return result;
}

typedef struct Malloc_Point_t
{
    size_t size;
    void *Point;
    void *NextPoint;
    void *Data;
}Malloc_t;
/*
void * mycalloc (size_t count,size_t size)
{
	return MYCALLOC_IN(count,size);
}
void * mymalloc (size_t size)
{
	return MYMALLOC_IN(size);
}
void myfree (void *ptr)
{
	MYFREE_IN(ptr);
}
void myfreeall (void)
{
	MYFREEALL_IN();
}*/
static Malloc_t MaStart={0,NULL,NULL};
static Malloc_t *MaP=&MaStart;
static int mymallocTimes=0;
static int myfreeTimes=0;
void * mycallocwithinfo (size_t count,size_t size,int gs,...)
{
    mylocknoinit();
    char *info="";
    int line=0;
		mymallocTimes=mymallocTimes;
    if(gs==2)
    {
        va_list ap;
        va_start(ap, gs);
        info=va_arg(ap, char*);
        line=va_arg(ap, int);
        va_end(ap);
    }
    void *p;
    Malloc_t *Mar;
	if(MaStart.Data==NULL)
    {
        MaStart.Data=(Malloc_t *)Malloc_input(sizeof(Malloc_t));
		MaStart.Point=&MaStart;
		MaStart.size=sizeof(Malloc_t);
        MaP=&MaStart;
//        printf("xxxxxxxxxxxxxxxxxxxxxxxxxxx======%s=%d===first get the mem!\n",info,line);
    }
    p=Calloc_input(count,size);
    if(p==NULL)
    {
        printf("=============================%s=%d===Memery Can not be calloc,there is no space or too much shard!\n",info,line);
        myunlock();
        return NULL;
    }
    MaP->Point=MaP;
    MaP->Data=p;
    MaP->NextPoint=(Malloc_t *)Malloc_input(sizeof(Malloc_t));
    if(MaP->NextPoint==NULL)
    {
        printf("=============================%s=%d===Memery Can not be calloc,there is no space or too much shard!\n",info,line);
        myunlock();
        return NULL;
    }
    MaP->size=size*count;
    Mar=MaP;
    MaP=MaP->NextPoint;
    MaP->Data=NULL;
if (DEBUG_BASE_MEM)
    printf("<-<-<-<-<-<-<-<-|------%s-%d---mymalloc Point is 0x%X size %d Data is 0x%X Times is %d\n",info,line,(int)Mar->Point,Mar->size,(int)Mar->Data,++mymallocTimes);

    myunlock();
    return Mar->Data;

}

void * mymallocwithinfo (size_t size,int gs,...)
{
    mylocknoinit();
    char *info="";
    int line=0;
    if(gs==2)
    {
        va_list ap;
        va_start(ap, gs);
        info=va_arg(ap, char*);
        line=va_arg(ap, int);
        va_end(ap);
    }
    void *p;
    Malloc_t *Mar;
	if(MaStart.Data==NULL)
    {
        MaStart.Data=(Malloc_t *)Malloc_input(sizeof(Malloc_t));
		MaStart.Point=&MaStart;
        MaStart.size=sizeof(Malloc_t);
        MaP=&MaStart;
        printf("xxxxxxxxxxxxxxxxxxxxxxxxxxx======%s====first get the mem!\n",info);
    }
    p=Malloc_input(size);
    if(p==NULL)
    {
        printf("=============================%s=%d===Memery Can not be malloc,there is no space or too much shard!\n",info,line);
        myunlock();
        return NULL;
    }
    MaP->Point=MaP;
    MaP->Data=p;
    MaP->NextPoint=(Malloc_t *)Malloc_input(sizeof(Malloc_t));
    if(MaP->NextPoint==NULL)
    {
        printf("=============================%s=%d===Memery Can not be malloc,there is no space or too much shard!\n",info,line);
        myunlock();
        return NULL;
    }
    MaP->size=size;
    Mar=MaP;
    MaP=MaP->NextPoint;
    MaP->Data=NULL;
if (DEBUG_BASE_MEM)
    printf("<-<-<-<-<-<-<-<-|------%s-%d---mymalloc Point is 0x%X size %d Data is 0x%X Times is %d\n",info,line,(int)Mar->Point,Mar->size,(int)Mar->Data,++mymallocTimes);

    myunlock();
    return Mar->Data;

}

void myfreewithinfo(void *ptr,int gs,...)
{
    mylocknoinit();
    char *info="";
    int line=0;
    if(gs==2)
    {
        va_list ap;
        va_start(ap, gs);
        info=va_arg(ap, char*);
        line=va_arg(ap, int);
        va_end(ap);
    }
    Malloc_t *MaPfBak=NULL;
    Malloc_t PFrent;
	int Flag=0,i=0;
    for(Malloc_t *MaPf=&MaStart;MaPf->Data!=NULL;MaPf=MaPf->NextPoint,i++)
    {
        if (DEBUG_BASE_MEM==2)
            printf("-%s-%d-index %d -- Point is 0x%X size is size %d Data is 0x%X \n",info,line,i,(int)MaPf->Point,MaPf->size,(int)MaPf->Data);
        if((MaPf->Data)==ptr)
        {
            MaPfBak->NextPoint=MaPf->NextPoint;
            memcpy(&PFrent,MaPf,sizeof(Malloc_t));
            Flag=1;
            Free_input(MaPf->Data);
            MaPf->Data=NULL;
            Free_input(MaPf->Point);
            //MaPf->Point=NULL;
            if (DEBUG_BASE_MEM)
            printf("->->->->->->->->|------%s-%d---myfree  Point is 0x%X size is size %d Data is 0x%X Times is %d\n",info,line,(int)PFrent.Point,PFrent.size,(int)PFrent.Data,++myfreeTimes);

           break;
        }
            MaPfBak=MaPf;
    }
    if(Flag==0)
        printf("||||||||||||||||||||||||||||||%s|%d|||ptr = 0x%X Address Can not be free,because it has not malloc.myfree Run Times is -|- %d\n",info,line,(int)ptr,++myfreeTimes);
    myunlock();
}
void myfreeallwithinfo(int gs,...)
{
	mylocknoinit();
	char *info="";
	int line=0;
	if(gs==2)
	{
		va_list ap;
		va_start(ap, gs);
		info=va_arg(ap, char*);
		line=va_arg(ap, int);
		va_end(ap);
	}

	Malloc_t *MaPfBak=MaStart.NextPoint;
	Malloc_t PFrent;
	if(MaStart.Data==NULL)
	{
		myunlock();
		return;
	}
	for(Malloc_t *MaPf=MaStart.NextPoint;MaPf->Data!=NULL;MaPf=MaPfBak->NextPoint)
	{
		if((MaPf->Data)!=0)
		{
			MaPfBak->NextPoint=MaPf->NextPoint;
            memcpy(&PFrent,MaPf,sizeof(Malloc_t));
            Free_input(MaPf->Data);
            MaPf->Data=NULL;
       		Free_input(MaPf->Point);
            MaPf->Point=NULL;
if (DEBUG_BASE_MEM)
			printf("============->->->->->->->->|------%s-%d---allmyfree  Point is 0x%X size is size %d Data is 0x%X Times is %d\n",info,line,(int)PFrent.Point,PFrent.size,(int)PFrent.Data,++myfreeTimes);

		}
	}
	MaPfBak=&MaStart;
	memcpy(&PFrent,MaPfBak,sizeof(Malloc_t));
	Free_input(MaPfBak->Data);
	MaPfBak->Data=NULL;
	MaPfBak->Point=NULL;
//#ifdef DEBUG_BASE_MEM
	printf("============->->->->->->->->|------%s-%d---allmyfree  Point is 0x%X size is size %d Data is 0x%X Times is %d\n",info,line,(int)PFrent.Point,PFrent.size,(int)PFrent.Data,++myfreeTimes);
//#endif
	MaP=&MaStart;
	lock=lock;
  myunlock();

}
int char_is_hexnum(char ch)
{
	return (((ch>='0')&&(ch<='9'))||((ch>='A')&&(ch<='F'))||((ch>='a')&&(ch<='f')));
}
int char_is_decnum(char ch)
{
	return (((ch>='0')&&(ch<='9')));
}
int char_is_afAFnum(char ch)
{
	return (((ch>='A')&&(ch<='F'))||((ch>='a')&&(ch<='f')));
}
int char_is_afnum(char ch)
{
	return (((ch>='a')&&(ch<='f')));
}
int char_is_AFnum(char ch)
{
	return (((ch>='A')&&(ch<='F')));
}
int char_is_NULLnum(char ch)
{
	return (ch==0);
}
int hexstrtoarrayu8(char *hexstr,void *array,int len)
{
    mylocknoinit();
    int lenp=0,flag=0;
    unsigned char *ptr=array;
    if(len<=0)
    {
        myunlock();
        return lenp;
    }
//    kagoo_udp_log("clear start the string is %s",hexstr);
    while((!char_is_hexnum(*hexstr))&&(!char_is_NULLnum(*hexstr)))hexstr++;//kagoo_udp_log("clear %c",*hexstr++);//
    if(char_is_NULLnum(*hexstr))
    {
        myunlock();
        return lenp;
    }
    while((!char_is_NULLnum(*hexstr))&&(lenp<=len))
    {
        ptr[lenp]=flag?((ptr[lenp]<<4)|c2i(*hexstr++)):c2i(*hexstr++),flag++;
        if((!char_is_hexnum(*hexstr))||(flag==2))
        {
//            kagoo_udp_log("data [%x]",ptr[lenp]);
            lenp++;flag=0;
        }
        while((!char_is_hexnum(*hexstr))&&(!char_is_NULLnum(*hexstr)))hexstr++;//kagoo_udp_log("clear %c",*hexstr++);//
    }
    if(flag)    lenp++;
    myunlock();
    return lenp;
}
char * arrayu8tohexstr(void *array,int len,char *hexstr)
{
    mylock();
    char * dispp=hexstr;
    for(int j=0;j<len;j++)
    {
        sprintf(dispp," %02X",((unsigned char *)array)[j]);
        dispp+=strlen(" FF");
    }
    myunlock();
		return hexstr;
}
void *memmemcustom(void *start, unsigned int s_len, void *find, unsigned int f_len)
{
    mylocknoinit();
    char *p, *q;
    unsigned int len;
    p = start, q = find;
    len = 0;
    while((p - (char *)start + f_len) <= s_len)
    {
        while(*p++ == *q++)
        {
            len++;
            if(len == f_len)
            {
                myunlock();
                return(p - f_len);
            }
        }
        q = find;
        len = 0;
    }
    myunlock();
    return(NULL);
}
unsigned short CRC16Calculate(unsigned char * b, unsigned short startIndex, unsigned short endIndex) 
{
	mylocknoinit();
	unsigned short crc = 0xFFFF;
	unsigned short j,i;
	for (j = startIndex ; j <= endIndex; j++) 
	{
		crc = crc ^ ((b[j] + 256) % 256) ;
		for (i = 0; i < 8; i++) 
		{
			if ((crc & 0x0001) == 0x0001) 
			{
				crc = (crc >> 1) ^ 0xA001;
			} 
			else 
			{
				crc = crc >> 1;
			}
		}
	}
	myunlock();
	return crc;
}


