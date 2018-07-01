#include "susuClib_quqe.h"
////////////////////////////////以下是由链表实现的队列和栈/////////////////////////
static int lock;
SeqQueue_t *SeqQueue_tInit(int maxlen)//初始化队列
{  
    
    mylocknoinit();
    SeqQueue_t *q;
    if((q=(SeqQueue_t *)mymalloc(sizeof(SeqQueue_t)))!=0)
    {
//        result=(DATA_t *)mymalloc(sizeof(DATA_t));
        q->head=0;
        q->tail=0;
		q->length=0;
        q->maxlength=maxlen;
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
    DATA_t *resultp=0;
    
    mylocknoinit();
    resultp=&(q->data);
    while(q->length--)
    {
        resultp->point=q->head;
        q->head=((DATA_t *)q->head)->next;
        myfree(resultp->point);
    }
//    if(result!=NULL) myfree(result);
	if(q!=NULL) myfree(q);
}  
  
int SeqQueue_tIsEmpty(SeqQueue_t *q)//队列是否为空，空为0   
{  
    return (q->length<=0);
}  
  
int SeqQueue_tIsFull(SeqQueue_t *q)//队列是否为已满，满为1   
{  
    return q->maxlength?(q->length>=q->maxlength):0;
}  
  
int SeqQueue_tLen(SeqQueue_t *q)//返回队列长度   
{  
    return (q->length);  
}  
  
int SeqQueue_tIn(SeqQueue_t *q,DATA_t data)//入队   
{
    
    mylocknoinit();
    DATA_t * p;
    if(SeqQueue_tIsFull(q))
	{//判断队列q是否已满，满返回0
        printf("Queue is full!\n");
        myunlock();
        return 0;  
    }
    else
	{  
        p=(DATA_t *)mymalloc(sizeof(DATA_t));
        if(p==NULL)
        {
            printf("Can not get the mem,the get length is %d",sizeof(DATA_t));
            myunlock();
            return 0;
        }
        p->previous=q->tail;
        q->tail?((DATA_t *)q->tail)->next=p:0;
        q->tail=p;
        q->head=q->head?q->head:p;
        p->cell=data.cell;
        p->ptr=data.ptr;
        p->point=p;
        p->next=0;//printf("\n{0x%08X<-0x%08X->0x%08X}",(int)p->previous,(int)p->point,(int)p->next);
        q->length++;
        myunlock();
        return 1;  
    }  
}  
  
DATA_t *SeqQueue_tOut(SeqQueue_t *q)//出队   
{
    DATA_t *resultp=0;
    
    mylocknoinit();
    resultp=&(q->data);
    if(SeqQueue_tIsEmpty(q))  
    {  
        printf("Queue is empty!\n");//判断队列是否为空，空返回NULL
        myunlock();
        return NULL;  
    }  
    else
	{  
		q->length--;
        if((q->head==0)||(q->head->point==0))
        {
            printf("error q->head is NULL\n");
            myunlock();
            return NULL;
        }
        memcpy(resultp,q->head,sizeof(DATA_t));
        q->head=((DATA_t *)q->head)->next;
        myfree(resultp->point);
        q->head=q->length?q->head:0;
        q->tail=q->length?q->tail:0;
        myunlock();
        return resultp;
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
        return q->head;
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
        return q->tail;
    }
}
//获取指定位置的元素
DATA_t *SeqQueue_tPos(SeqQueue_t *q,int posn)
{
    
    mylocknoinit();
    DATA_t * p=q->head;
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
        while(posn--)
            p=(DATA_t *)p->next;
        myunlock();
        return p;
    }
}
//反向入队
int SeqQueue_tReverseIn(SeqQueue_t *q,DATA_t data)//反向入队
{
    
    mylocknoinit();
    DATA_t * p;
    if(SeqQueue_tIsFull(q))
    {//判断队列q是否已满，满返回0
        printf("Queue is full!\n");
        myunlock();
        return 0;
    }
    else
    {
        p=(DATA_t *)mymalloc(sizeof(DATA_t));
        if(p==NULL)
        {
            printf("Can not get the mem,the get length is %d",sizeof(DATA_t));
            myunlock();
            return 0;
        }
        p->next=q->head;
        q->head?((DATA_t *)q->head)->previous=p:0;
        q->head=p;
        q->tail=q->tail?q->tail:p;
        p->cell=data.cell;
        p->ptr=data.ptr;
        p->point=p;
        p->previous=0;//printf("{0x%08X<-0x%08X->0x%08X}",(int)p->previous,(int)p->point,(int)p->next);
        q->length++;
        myunlock();
        return 1;
    }
}
//反向出队
DATA_t *SeqQueue_tReverseOut(SeqQueue_t *q)//出队
{
    DATA_t *resultp=0;
    
    mylocknoinit();
    resultp=&(q->data);
    if(SeqQueue_tIsEmpty(q))
    {
        printf("Queue is empty!\n");//判断队列是否为空，空返回NULL
        myunlock();
        return NULL;
    }
    else
    {
        q->length--;
        memcpy(resultp,q->tail,sizeof(DATA_t));
        q->tail=((DATA_t *)q->tail)->previous;
        myfree(resultp->point);
        q->head=q->length?q->head:0;
        q->tail=q->length?q->tail:0;
        myunlock();
        return resultp;
    }
}

//
int SeqQueue_tHeadPosnIn(SeqQueue_t *q,DATA_t data,int posn)//队首开始第n个插入队
{
    
    mylocknoinit();
    DATA_t *pp,*p=q->head;
    if(SeqQueue_tIsFull(q))
    {//判断队列q是否已满，满返回0
        printf("Queue is full!\n");
        myunlock();
        return 0;
    }
    else
    {
        if(posn==0)
        {
            myunlock();
            return SeqQueue_tReverseIn(q,data);
        }
        if(posn==q->length-1)
        {
            myunlock();
            return SeqQueue_tIn(q,data);
        }
        while(posn--)
            p=(DATA_t *)p->next;
        pp=(DATA_t *)mymalloc(sizeof(DATA_t));
        pp->previous=p;
        q->tail=q->tail?q->tail:pp;
        q->head=q->head?q->head:pp;
        pp->cell=data.cell;
        pp->ptr=data.ptr;
        pp->point=pp;
        pp->next=(DATA_t *)p->next;
        ((DATA_t *)p->next)->previous=pp;
        p->next=pp;//printf("{0x%08X<-0x%08X->0x%08X}",(int)p->previous,(int)p->point,(int)p->next);
        q->length++;
        myunlock();
        return 1;
    }
}

DATA_t *SeqQueue_tHeadPosnOut(SeqQueue_t *q,int posn)////队首开始第n个插出队
{
    DATA_t *resultp=0;
    
    mylocknoinit();
    resultp=&(q->data);
    DATA_t *p=q->head;
    if(SeqQueue_tIsEmpty(q))
    {
        printf("Queue is empty!\n");//判断队列是否为空，空返回NULL
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
        if(posn==0)
        {
            myunlock();
            return SeqQueue_tOut(q);
        }
        if(posn==q->length-1)
        {
            myunlock();
            return SeqQueue_tReverseOut(q);
        }
        while(posn--)
            p=(DATA_t *)p->next;
        q->length--;
        memcpy(resultp,p,sizeof(DATA_t));
        ((DATA_t *)p->previous)->next=p->next;
        ((DATA_t *)p->next)->previous=p->previous;
        myfree(p);
        q->tail=q->length?q->tail:0;
        myunlock();
        return resultp;
    }
}
int SeqQueue_tTailPosnIn(SeqQueue_t *q,DATA_t data,int posn)////队尾开始第n个插入队
{
    
    mylocknoinit();
    DATA_t *pp,*p=q->tail;
    if(SeqQueue_tIsFull(q))
    {//判断队列q是否已满，满返回0
        printf("Queue is full!\n");
        myunlock();
        return 0;
    }
    else
    {
        if(posn==0)
        {
            myunlock();
            return SeqQueue_tIn(q,data);
        }
        if(posn==q->length-1)
        {
            myunlock();
            return SeqQueue_tReverseIn(q,data);
        }
        while(posn--)
            p=(DATA_t *)p->previous;
        pp=(DATA_t *)mymalloc(sizeof(DATA_t));
        pp->next=p;
        q->tail=q->tail?q->tail:pp;
        q->head=q->head?q->head:pp;
        pp->cell=data.cell;
        pp->ptr=data.ptr;
        pp->point=pp;
        pp->previous=(DATA_t *)p->previous;
        ((DATA_t *)p->previous)->next=pp;
        p->previous=pp;
        p->previous=pp;//printf("{0x%08X<-0x%08X->0x%08X}",(int)p->previous,(int)p->point,(int)p->next);
        q->length++;
        myunlock();
        return 1;
    }
}

DATA_t *SeqQueue_tTailPosnOut(SeqQueue_t *q,int posn)////队尾开始第n个插出队
{
    DATA_t *resultp=0;
    
    mylocknoinit();
    resultp=&(q->data);
    DATA_t *p=q->tail;
    if(SeqQueue_tIsEmpty(q))
    {
        printf("Queue is empty!\n");//判断队列是否为空，空返回NULL
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
        if(posn==0)
        {
            myunlock();
            return SeqQueue_tReverseOut(q);
        }
        if(posn==q->length-1)
        {
            myunlock();
            return SeqQueue_tOut(q);
        }
        while(posn--)
            p=(DATA_t *)p->previous;
        q->length--;
        memcpy(resultp,p,sizeof(DATA_t));
        ((DATA_t *)p->previous)->next=p->next;
        ((DATA_t *)p->next)->previous=p->previous;
        myfree(p);
        q->tail=q->length?q->tail:0;
        myunlock();
        return resultp;
    }
}






