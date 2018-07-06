#include "susuprotocolODLPlib.h"

#ifdef DEBUG_ODLP
#define susuprotocolODLP_log(format, ...)  custom_log("ODLP", format, ##__VA_ARGS__)
#else
#define susuprotocolODLP_log(format, ...)
#endif

static unsigned char *HEADER;
static int HEADER_size=0;
int Setsize_Dynamic_Length_Protocol(unsigned char *__HEADER,int __HEADER_size)
{
    HEADER=__HEADER;
    HEADER_size=__HEADER_size;
    return __HEADER_size;
}
int Input_Dynamic_Length_Protocol(QUQE_T *Recdata,unsigned char data)
{
    static int * plen=0;
    if(!Recdata) Recdata=SeqQueue_Init(MAXPACKLENGTH*2,sizeof(unsigned char));
    SeqQueue_In(Recdata,&data);
    //    susuprotocolODLP_log( "Input_Dynamic_Length_Protocol, len is %d, data is [0x%02X]",SeqQueue_Len(Recdata),data);
    return (int)plen++;
}
int ReverseInput_Dynamic_Length_Protocol(QUQE_T *Recdata,unsigned char data)
{
    static int * plen=0;
    if(!Recdata)
        susuprotocolODLP_log("the quqe is not init!");
    SeqQueue_ReverseIn(Recdata,&data);
    //    susuprotocolODLP_log( "Input_Dynamic_Length_Protocol, len is %d, data is [0x%02X]",SeqQueue_Len(Recdata),data);
    return (int)plen++;
}

int Output_Dynamic_Length_Protocol(QUQE_T *Recdata,unsigned char ** outp,int *len)
{
    static int ODLP=0;
    static int packlength=0;
    int quqelength,i;
    unsigned char *ptr=0,*pts=0,*recdatp=0;
    if(ODLP!=ODLP_Data_Finded)
    {
        quqelength=SeqQueue_Len(Recdata);
        if(quqelength<DEADLENGTH)
            return Not_reach_Deadlength;
        if(HEADER_size<=0)   return Not_Init_header;
        if(HEADER_size>SUBLENSTART)   return SUBLENSTART_error;
    }
    switch(ODLP)
    {
        case 0://寻找开头
            packlength=0;
            ptr=mymalloc(DEADLENGTH);
            if(ptr==0)  return Not_enough_mem;
            for(i=0;i<DEADLENGTH;i++)
            {
                recdatp=SeqQueue_Out(Recdata);
                ptr[i]=*recdatp;
                //susuprotocolODLP_log("Recdata length is %d , data is [%02X]",SeqQueue_Len(Recdata),ptr[i]);
            }
            if((pts=memmemcustom(ptr,DEADLENGTH,HEADER,HEADER_size))!=0)
            {
                //找到了开头
                susuprotocolODLP_log( "Output_Dynamic_Length_Protocol Header is finded the length is %d , ptr %X pts %X",(int)(ptr-pts),(int)ptr,(int)pts);
                ODLP=1;
                for(i=(int)(DEADLENGTH-(ptr-pts));i>(int)(ptr-pts);i--)
                {
                    ReverseInput_Dynamic_Length_Protocol(Recdata,ptr[i-1]);
                    //susuprotocolODLP_log("Recdata length is %d , data is [%02X]",SeqQueue_Len(Recdata),ptr[i]);
                }
            }
            else
            {
                //没有找到开头
                susuprotocolODLP_log( "Output_Dynamic_Length_Protocol Header is not find");
                ODLP=0;
                /*
                for(i=(int)(ptr-HEADER_size);i<(int)ptr;i++)
                {
                    Input_Dynamic_Length_Protocol(Recdata,ptr[i]);
                    //susuprotocolODLP_log("Recdata length is %d , data is [%02X]",SeqQueue_Len(Recdata),ptr[i]);
                }
                 */
            }
            myfree(ptr);
            break;
        case 1:
            susuprotocolODLP_log("11111111111111111111111111111111111111Recdata length is %d",SeqQueue_Len(Recdata));
            if(quqelength<SUBLENSTART+SUBLENLEN)
            {
                //长度小于可获取长度的长度,继续等待
                ODLP=1;
            }
            else
            {
                //长度合适了
                ODLP=2;
                pts=mymalloc(SUBLENSTART+SUBLENLEN);
                if(pts==0)  return Not_enough_mem;
                
                for(i=0;i<SUBLENSTART+SUBLENLEN;i++)
                {
                    recdatp=SeqQueue_Out(Recdata);
                    pts[i]=*recdatp;
                    //susuprotocolODLP_log("Recdata length is %d , data is [%02X]",SeqQueue_Len(Recdata),pts[i]);
                }
                packlength=u8dimtoint1(SUBLENLEN&0x80,pts+SUBLENSTART,SUBLENLEN);
                susuprotocolODLP_log( "Output_Dynamic_Length_Protocol DEADLENGTH is %d packlength is %d a 0x%02X b 0x%02X",DEADLENGTH,packlength,*(uint8_t *)(pts+SUBLENSTART),*(uint8_t *)(pts+SUBLENSTART+1));
                if(packlength<=MAXPACKLENGTH-DEADLENGTH)
                {
                    for(i=SUBLENSTART+SUBLENLEN;i>0;i--)
                    {
                        ReverseInput_Dynamic_Length_Protocol(Recdata,pts[i-1]);
                        //susuprotocolODLP_log("Recdata length is %d , data is [%02X]",SeqQueue_Len(Recdata),pts[i]);
                    }
                }
                else
                {
                    susuprotocolODLP_log("packlength is error, now the packlength is %d,the data below:",packlength);
                    printdim("error packlength the data is",pts,quqelength);
                    ODLP=0;
                    quqelength=SeqQueue_Len(Recdata);
                    for(i=0;i<quqelength;i++)
                    {
                        recdatp=SeqQueue_Out(Recdata);
                    }
                }
                myfree(pts);
                *len=DEADLENGTH+packlength;
                susuprotocolODLP_log( "Output_Dynamic_Length_Protocol len is %d packlength is %d quqe length is %d",*len,packlength,SeqQueue_Len(Recdata));
            }
            break;
        case 2:
            susuprotocolODLP_log( "quqelength is %d  DEADLENGTH %d packlength is %d",quqelength,DEADLENGTH,packlength);
            if(quqelength<DEADLENGTH+packlength)
            {
                //长度小于总包,继续等待
                ODLP=2;
            }
            else
            {
                //长度合适了
                ODLP=3;
                ptr=mymalloc(DEADLENGTH+packlength);
                if(ptr==0)  return Not_enough_mem;
                for(i=0;i<DEADLENGTH+packlength;i++)
                {
                    recdatp=SeqQueue_Out(Recdata);
                    if(recdatp==0)
                    {
                        printdim("ODLP=3 get a wrong array :", ptr, i);
                        myfree(pts);
                        ODLP=0;
                        break;
                    }
                    ptr[i]=*recdatp;
                }
                *outp=ptr;
                susuprotocolODLP_log("outp is 0x%08X ptr is 0x%08X ",(int)outp,(int)ptr);
                *len=DEADLENGTH+packlength;
                susuprotocolODLP_log( "quqelength is %d  DEADLENGTH %d packlength is %d",quqelength,DEADLENGTH,packlength);
            }
            break;
        case 3:
            ODLP=0;
            susuprotocolODLP_log( "333333333333333333333333 get a pack already 33333333333333333333  %d  %d  %d",quqelength,DEADLENGTH,packlength);
            *len=DEADLENGTH+packlength;
            break;
        default:
            susuprotocolODLP_log( "Output_Dynamic_Length_Protocol CMD error CMD is %d",(int)ODLP);
            ODLP=0;
            break;
    }
    return ODLP;
}
