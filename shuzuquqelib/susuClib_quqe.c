#include "susuClib_quqe.h"
////////////////////////////////以下是由数组实现的队列和栈/////////////////////////
SeqQueue_t *SeqQueue_tInit(int maxlen)//初始化队列
{  
    SeqQueue_t *q;
    if((q=(SeqQueue_t *)quqemalloc(sizeof(SeqQueue_t)))!=0)
    {  
        q->head=0;  
        q->tail=0; 
		q->length=0;
        q->maxlength=maxlen;
		q->data=(DATA_t *)quqemalloc(sizeof(DATA_t)*q->maxlength);
        return q;  
    }  
    else  
        return NULL;  
}  
  
void SeqQueue_tFree(SeqQueue_t *q)//释放队列   
{  
    if(q->data!=NULL) quqefree(q->data);
	if(q!=NULL) quqefree(q);  
}  
  
int SeqQueue_tIsEmpty(SeqQueue_t *q)//队列是否为空，空为0   
{  
    return (q->length<=0);
}  
  
int SeqQueue_tIsFull(SeqQueue_t *q)//队列是否为已满，满为1   
{  
    return (q->length>=q->maxlength);
}  
  
int SeqQueue_tLen(SeqQueue_t *q)//返回队列长度   
{  
    return (q->length);  
}  
  
int SeqQueue_tIn(SeqQueue_t *q,DATA_t data)//入队   
{  
    if(SeqQueue_tIsFull(q))
	{//判断队列q是否已满，满返回0
        printf("Queue is full!\n");  
        return 0;  
    }  
    else
	{  
        q->length++;
        q->data[q->tail]=data;
		q->tail++;
		q->tail=q->tail>=q->maxlength?0:q->tail;
        return 1;  
    }  
}  
  
DATA_t *SeqQueue_tOut(SeqQueue_t *q)//出队   
{  
    DATA_t * result;
    if(SeqQueue_tIsEmpty(q))  
    {  
        printf("Queue is empty!\n");//判断队列是否为空，空返回NULL   
        return NULL;  
    }  
    else
	{  
		q->length--;
		result=&(q->data[q->head]);
		q->head++;
		q->head=q->head>=q->maxlength?0:q->head;
        return result;
    }  
}  
  
DATA_t *SeqQueue_tPeek(SeqQueue_t *q)//获取队头元素   
{  
    if(SeqQueue_tIsEmpty(q))  
    {  
        printf("Queue is empty!\n");  
        return NULL;  
    }  
    else
	{  
        return &(q->data[q->head]);  
    }  
}
//获取队尾元素
DATA_t *SeqQueue_tTail(SeqQueue_t *q)
{
    if(SeqQueue_tIsEmpty(q))
    {
        printf("Queue is empty!\n");
        return NULL;
    }
    else
    {
        return &(q->data[q->tail==0?q->maxlength-1:q->tail-1]);
    }
}
//获取指定位置的元素
DATA_t *SeqQueue_tPos(SeqQueue_t *q,int posn)
{
    if(SeqQueue_tIsEmpty(q))
    {
        printf("Queue is empty!\n");
        return NULL;
    }
    else if(posn>=q->length)
    {
        printf("Pos over the length!\n");
        return NULL;
    }
    else
    {
        //printf(" [%d]->",(q->head+posn)>=q->maxlength?(q->head+posn-q->maxlength):(q->head+posn));
        return &(q->data[(q->head+posn)>=q->maxlength?q->head+posn-q->maxlength:q->head+posn]);
    }
}
//反向入队
int SeqQueue_tReverseIn(SeqQueue_t *q,DATA_t data)//入队
{
    if(SeqQueue_tIsFull(q))
    {//判断队列q是否已满，满返回0
        printf("Queue is full!\n");
        return 0;
    }
    else
    {
        q->length++;
        q->head=q->head<=0?q->maxlength-1:q->head-1;
        q->data[q->head]=data;
        return 1;
    }
}
//反向出队
DATA_t *SeqQueue_tReverseOut(SeqQueue_t *q)//出队
{
    DATA_t * result;
    if(SeqQueue_tIsEmpty(q))
    {
        printf("Queue is empty!\n");//判断队列是否为空，空返回NULL
        return NULL;
    }
    else
    {
        q->length--;
        q->tail=q->tail<=0?q->maxlength-1:q->tail-1;
        result=&(q->data[q->tail]);
        return result;
    }
}
