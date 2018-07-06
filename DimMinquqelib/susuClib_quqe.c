#include "susuClib_quqe.h"
////////////////////////////////以下是由数组实现的队列和栈/////////////////////////
SeqQueue_t *SeqQueue_tInit(u16 maxlen,u8 unitlen)//初始化队列
{  
  mylocknoinit();  
	SeqQueue_t *q;
	unitlen=unitlen==0?1:unitlen;
    if((q=(SeqQueue_t *)quqemalloc(sizeof(SeqQueue_t)))!=0)
    {  
        q->head=0;  
        q->tail=0; 
				q->length=0;
        q->maxlength=maxlen;
        q->unitlength=unitlen;
				q->data=(void *)quqemalloc(maxlen*unitlen);
        myunlock();
        return q;  
    }  
    else  
		{
        myunlock();
        return NULL;  
		}
}		
  
void SeqQueue_tFree(SeqQueue_t *q)//释放队列   
{  
	mylocknoinit();  
	if(q->data!=NULL) quqefree(q->data);
	if(q!=NULL) quqefree(q);  
	myunlock();
}  
  
u16 SeqQueue_tIsEmpty(SeqQueue_t *q)//队列是否为空，空为0   
{  
    return (q->length<=0);
}  
  
u16 SeqQueue_tIsFull(SeqQueue_t *q)//队列是否为已满，满为1   
{  
    return (q->length>=q->maxlength);
}  
  
u16 SeqQueue_tLen(SeqQueue_t *q)//返回队列长度   
{  
    return (q->length);  
}  
  
u16 SeqQueue_tIn(SeqQueue_t *q,u8 *data)//入队   
{  
	mylocknoinit(); 
    if(SeqQueue_tIsFull(q))
	{//判断队列q是否已满，满返回0
        printf("Queue is full!\n");  
        myunlock();
        return 0;  
    }  
    else
	{  
        q->length++;
        memcpy((u8 *)q->data+q->tail*q->unitlength,data,q->unitlength);
				q->tail++;
				q->tail=q->tail>=q->maxlength?0:q->tail;
        myunlock();
        return 1;  
    }  
}  
  
void *SeqQueue_tOut(SeqQueue_t *q)//出队   
{  
  mylocknoinit();  
    void * result;
    if(SeqQueue_tIsEmpty(q))  
    {  
        printf("Queue is empty!\n");//判断队列是否为空，空返回NULL   
        myunlock();
        return NULL;  
    }  
    else
	{  
		q->length--;
		result=(u8 *)q->data+q->head*q->unitlength;
		q->head++;
		q->head=q->head>=q->maxlength?0:q->head;
		myunlock();
		return result;
    }  
}  
  
void *SeqQueue_tPeek(SeqQueue_t *q)//获取队头元素   
{  
  mylocknoinit();  
    if(SeqQueue_tIsEmpty(q))  
    {  
        printf("Queue is empty!\n");  
        myunlock();
        return NULL;  
    }  
    else
		{  
        myunlock();
        return (u8 *)q->data+q->head*q->unitlength;  
    }  
}
//获取队尾元素
void *SeqQueue_tTail(SeqQueue_t *q)
{
  mylocknoinit();  
    if(SeqQueue_tIsEmpty(q))
    {
        printf("Queue is empty!\n");
        myunlock();
        return NULL;
    }
    else
    {
        myunlock();
        return (u8 *)q->data+(q->tail==0?q->maxlength-1:q->tail-1)*q->unitlength;
    }
}
//获取指定位置的元素
void *SeqQueue_tPos(SeqQueue_t *q,u16 posn)
{
  mylocknoinit();  
    if(SeqQueue_tIsEmpty(q))
    {
        printf("Queue is empty!\n");
        myunlock();
        return NULL;
    }
    else if(posn>=q->length)
    {
        printf("Pos over the length!\n");
        myunlock();
        return NULL;
    }
    else
    {
        //printf(" [%d]->",(q->head+posn)>=q->maxlength?(q->head+posn-q->maxlength):(q->head+posn));
        myunlock();
        return (u8 *)q->data+((q->head+posn)>=q->maxlength?q->head+posn-q->maxlength:q->head+posn)*q->unitlength;
    }
}
//反向入队
u16 SeqQueue_tReverseIn(SeqQueue_t *q,u8 *data)//入队
{
  mylocknoinit();  
    if(SeqQueue_tIsFull(q))
    {//判断队列q是否已满，满返回0
        printf("Queue is full!\n");
        myunlock();
        return 0;
    }
    else
    {
        q->length++;
        q->head=q->head<=0?q->maxlength-1:q->head-1;
        memcpy((u8 *)q->data+q->head*q->unitlength,data,q->unitlength);
        myunlock();
        return 1;
    }
}
//反向出队
void *SeqQueue_tReverseOut(SeqQueue_t *q)//出队
{
  mylocknoinit();  
    void * result;
    if(SeqQueue_tIsEmpty(q))
    {
        printf("Queue is empty!\n");//判断队列是否为空，空返回NULL
        myunlock();
        return NULL;
    }
    else
    {
        q->length--;
        q->tail=q->tail<=0?q->maxlength-1:q->tail-1;
				result=(u8 *)q->data+q->tail*q->unitlength;
        myunlock();
        return result;
    }
}
