#include<iostream>
using namespace std;
class Iterator
{
private:
int releaseIteratorAfterIteration;
Iterator *iterator;
public:
Iterator()
{
this->releaseIteratorAfterIteration=1;
this->iterator=NULL;
}
Iterator(Iterator *iterator)
{
this->releaseIteratorAfterIteration=1;
this->iterator=iterator;
}
Iterator(const Iterator &other)
{
this->releaseIteratorAfterIteration=1;
this->iterator=other.iterator;
}
virtual ~Iterator()
{
if(this->releaseIteratorAfterIteration==1) delete this->iterator;
}
void setReleaseIteratorAfterIteration(int releaseIteratorAfterIteration)
{
this->releaseIteratorAfterIteration=releaseIteratorAfterIteration;
}
Iterator & operator=(const Iterator &other)
{
this->iterator=other.iterator;
return *this;
}
virtual int hasMoreElement()
{
if(this->iterator!=NULL) return this->iterator->hasMoreElement();
return 0;
}
virtual int next()
{
if(this->iterator!=NULL) return this->iterator->next();
return 0;
}
};
class Iterable
{
public:
virtual Iterator getIterator()=0;
};
class TMList:public Iterable //Abstract Class
{
public:
virtual void add(int data,bool *success)=0;
virtual void insertAt(int index,int data,bool *success)=0;
virtual int get(int index,bool *success)const=0;
virtual int getSize()const=0;
virtual void update(int index,int data,bool *success)=0;
virtual int removeAt(int index,bool *success)=0;
virtual void removeAll()=0;
virtual void clear()=0;
};

// TMArrayList Implementation Starts

class TMArrayList:public TMList
{
private:
char allocationFlag;
int **ptr;
int capacity;
int size;
bool addRow();
public:




class TMArrayListIterator:public Iterator
{
private:
int **ptr;
int index;
int size;
public:
TMArrayListIterator(int **ptr,int size)
{
this->ptr=ptr;
this->index=0;
this->size=size;
}
TMArrayListIterator(const TMArrayListIterator &other)
{
this->ptr=other.ptr;
this->index=other.index;
this->size=other.size;
}
TMArrayListIterator & operator=(const TMArrayListIterator &other)
{
this->ptr=other.ptr;
this->index=other.index;
this->size=other.size;
return *this;
}
int hasMoreElement()
{
return this->index<this->size;
}
int next()
{
if(this->index==this->size) return 0;
int rowIndex=this->index/10;
int columnIndex=this->index%10;
int data=this->ptr[rowIndex][columnIndex];
this->index++;
return data;
}
};
Iterator getIterator()
{
TMArrayListIterator *tmArrayListIterator;
tmArrayListIterator=new TMArrayListIterator(this->ptr,this->size);
Iterator k(tmArrayListIterator);
k.setReleaseIteratorAfterIteration(0);
return k;
}
TMArrayList();
TMArrayList(int bufferSize);
TMArrayList(const TMArrayList &other);
TMArrayList(const TMList &other);
virtual ~TMArrayList();
TMArrayList & operator=(const TMArrayList &other);
TMArrayList & operator=(const TMList &other);
void operator+=(const TMArrayList &other);
void operator+=(const TMList &other);
TMArrayList operator+(const TMArrayList &other);
TMArrayList operator+(const TMList &other);
void add(int data,bool *success);
void insertAt(int index,int data,bool *success);
int get(int index,bool *success)const;
int getSize()const;
void update(int index,int data,bool *success);
int removeAt(int index,bool *success);
void removeAll();
void clear();
};

bool TMArrayList::addRow()
{
if(this->capacity%10==0)
{
int numberOfPointers=this->capacity/10;
int **tmp=new int*[numberOfPointers+10];
if(tmp==NULL) return false;
for(int e=0;e<numberOfPointers;e++) tmp[e]=ptr[e];
delete [] ptr;
ptr=tmp;
}
int i=this->capacity/10;
this->ptr[i]=new int[10];
this->capacity=this->capacity+10;
return true;
}

TMArrayList::TMArrayList()
{
this->allocationFlag=0;
this->ptr=new int*[10];
this->ptr[0]=new int[10];
this->capacity=10;
this->size=0;
}

TMArrayList::TMArrayList(int bufferSize)
{
this->allocationFlag=0;
if(bufferSize<=0)
{
this->ptr=new int*[10];
this->ptr[0]=new int[10];
this->capacity=10;
this->size=0;
}
else
{
int rows;
int numberOfPointers;
rows=bufferSize/10;
if(bufferSize%10!=0) rows++;
numberOfPointers=rows+(10-(rows%10));
this->ptr=new int*[numberOfPointers];
for(int e=0;e<rows;e++) this->ptr[e]=new int[10];
this->size=0;
this->capacity=rows*10;
}
}

TMArrayList::TMArrayList(const TMArrayList &other)
{
this->allocationFlag=0;
int bufferSize=other.size;
if(bufferSize<=0)
{
this->ptr=new int*[10];
this->ptr[0]=new int[10];
this->capacity=10;
this->size=0;
}
else
{
int numberOfPointers;
int rows;
rows=bufferSize/10;
if(bufferSize%10!=0) rows++;
numberOfPointers=rows+(10-(rows%10));
this->ptr=new int*[numberOfPointers];
for(int e=0;e<rows;e++)
{
this->ptr[e]=new int[10];
}
this->capacity=rows*10;
this->size=0;
}
bool succ;
for(int e=0;e<other.size;e++)
{
this->add(other.get(e,&succ),&succ);
}
}

TMArrayList::TMArrayList(const TMList &other)
{
bool k;
this->allocationFlag=0;
this->ptr=new int*[10];
this->ptr[0]=new int[10];
this->capacity=10;
this->size=0;
for(int i=0;i<other.getSize();i++)
{
this->add(other.get(i,&k),&k);
}
}

TMArrayList::~TMArrayList()
{
if(this->allocationFlag==0)
{
int rows=this->capacity/10;
for(int i=0;i<rows;i++) delete [] ptr[i];
delete [] ptr;
}
}

TMArrayList & TMArrayList::operator=(const TMArrayList &other)
{
if(other.allocationFlag==0)
{
this->size=0;
bool succ;
for(int e=0;e<other.getSize();e++)
{
this->add(other.get(e,&succ),&succ);
}
}
else
{
int rows=this->capacity/10;
int j;
for(j=0;j<rows;j++) delete [] ptr[j];
delete [] ptr;
this->ptr=other.ptr;
this->size=other.size;
this->capacity=other.capacity;
}
return *this;
}

TMArrayList & TMArrayList::operator=(const TMList &other)
{
this->clear();
bool succ;
for(int e=0;e<other.getSize();e++)
{
this->add(other.get(e,&succ),&succ);
}
return *this;
}

void TMArrayList::operator+=(const TMArrayList &other)
{
bool succ;
for(int e=0;e<other.getSize();e++)
{
this->add(other.get(e,&succ),&succ);
}
}

void TMArrayList::operator+=(const TMList &other)
{
bool succ;
for(int e=0;e<other.getSize();e++)
{
this->add(other.get(e,&succ),&succ);
}
}

TMArrayList TMArrayList::operator+(const TMArrayList &other)
{
TMArrayList k;
k.allocationFlag=1;
bool succ;
for(int i=0;i<this->getSize();i++) k.add(this->get(i,&succ),&succ);
for(int i=0;i<other.getSize();i++) k.add(other.get(i,&succ),&succ);
return k;
}

TMArrayList TMArrayList::operator+(const TMList &other)
{
TMArrayList k;
k.allocationFlag=1;
bool succ;
for(int i=0;i<this->getSize();i++) k.add(this->get(i,&succ),&succ);
for(int i=0;i<other.getSize();i++) k.add(other.get(i,&succ),&succ);
return k;
}

void TMArrayList::add(int data,bool *success)
{
if(success) *success=false;
if(this->capacity==this->size)
{
if(!addRow()) return;
}
int rowIndex=this->size/10;
int columnIndex=this->size%10;
this->ptr[rowIndex][columnIndex]=data;
this->size++;
if(success) *success=true;
}

void TMArrayList::insertAt(int index,int data,bool *success)
{
if(success) *success=false;
if(index<0 || index>this->size) return;
if(index==this->size)
{
this->add(data,success);
return;
}
bool succ;
int k=this->get(size-1,&succ);
this->add(k,&succ);
if(succ==false) return;
int j=size-3;
while(j>=index)
{
this->update(j+1,this->get(j,&succ),&succ);
j--;
}
this->update(index,data,&succ);
}

int TMArrayList::get(int index,bool *success) const
{
if(success) *success=false;
if(index<0 || index>=this->size) return 0;
int rowIndex=index/10;
int columnIndex=index%10;
if(success) *success=true;
return ptr[rowIndex][columnIndex];
}

int TMArrayList::getSize() const
{
return this->size;
}
void TMArrayList::update(int index,int data,bool *success)
{
if(success) *success=false;
if(index<0 || index>=this->size) return;
int rowIndex=index/10;
int columnIndex=index%10;
this->ptr[rowIndex][columnIndex]=data;
if(success) *success=true;
}

int TMArrayList::removeAt(int index,bool *success)
{
if(success) *success=false;
if(index<0 || index>=this->size) return 0;
bool succ;
int data=this->get(index,&succ);
int ep=size-1;
int j=index;
while(j<=ep)
{
this->update(j,this->get(j+1,&succ),&succ);
j++;
}
this->size--;
if(success) *success=true;
return data;
}

void TMArrayList::removeAll()
{
this->size=0;
}
void TMArrayList::clear()
{
this->size=0;
}

// TMForwardList Implementation Starts

class TMForwardList:public TMList
{
class TMNode
{
public:
int data;
TMNode *next;
TMNode()
{
this->next=NULL;
}
};
private:
char allocationFlag;
TMNode *start,*end;
int size;
public:
class TMForwardListIterator:public Iterator
{
private:
TMNode *ptr;
public:
TMForwardListIterator()
{
this->ptr=NULL;
}
TMForwardListIterator(TMNode *ptr)
{
this->ptr=ptr;
}
TMForwardListIterator(const TMForwardListIterator &other)
{
this->ptr=other.ptr;
}
TMForwardListIterator & operator=(const TMForwardListIterator &other)
{
this->ptr=other.ptr;
return *this;
}
int hasMoreElement()
{
return this->ptr!=NULL;
}
int next()
{
if(this->ptr==NULL) return 0;
int data=this->ptr->data;
this->ptr=this->ptr->next;
return data;
}
};
Iterator getIterator()
{
TMForwardListIterator *tmForwardListIterator;
tmForwardListIterator=new TMForwardListIterator(this->start);
Iterator k(tmForwardListIterator);
k.setReleaseIteratorAfterIteration(0);
return k;
}
TMForwardList();
TMForwardList(int bufferSize);
TMForwardList(const TMForwardList &other);
TMForwardList(const TMList &other);
virtual ~TMForwardList();
TMForwardList & operator=(const TMForwardList &other);
TMForwardList & operator=(const TMList &other);
void operator+=(const TMForwardList &other);
void operator+=(const TMList &other);
TMForwardList operator+(const TMForwardList &other);
TMForwardList operator+(const TMList &other);
void add(int data,bool *success);
void insertAt(int index,int data,bool *success);
int get(int index,bool *success)const;
int getSize()const;
void update(int index,int data,bool *success);
int removeAt(int index,bool *success);
void removeAll();
void clear();
};

TMForwardList::TMForwardList()
{
this->start=NULL;
this->end=NULL;
this->size=0;
this->allocationFlag=0;
}

TMForwardList::TMForwardList(int bufferSize)
{
this->start=NULL;
this->end=NULL;
this->size=0;
this->allocationFlag=0;
}

TMForwardList::TMForwardList(const TMForwardList &other)
{
this->start=NULL;
this->end=NULL;
this->size=0;
this->allocationFlag=0;
bool succ;
for(int e=0;e<other.getSize();e++) this->add(other.get(e,&succ),&succ);
}

TMForwardList::TMForwardList(const TMList &other)
{
this->start=NULL;
this->end=NULL;
this->size=0;
this->allocationFlag=0;
bool succ;
for(int e=0;e<other.getSize();e++) this->add(other.get(e,&succ),&succ);
}

TMForwardList::~TMForwardList()
{
if(this->allocationFlag==0) this->clear();
}

TMForwardList & TMForwardList::operator=(const TMForwardList &other)
{
this->clear();
if(other.allocationFlag==0)
{
bool succ;
for(int e=0;e<other.getSize();e++) this->add(other.get(e,&succ),&succ);
}
else
{
this->size=other.size;
this->start=other.start;
this->end=other.end;
}
return *this;
}

TMForwardList & TMForwardList::operator=(const TMList &other)
{
this->clear();
bool succ;
for(int e=0;e<other.getSize();e++) this->add(other.get(e,&succ),&succ);
return *this;
}

void TMForwardList::operator+=(const TMForwardList &other)
{
bool succ;
for(int e=0;e<other.getSize();e++) this->add(other.get(e,&succ),&succ);
}

void TMForwardList::operator+=(const TMList &other)
{
bool succ;
for(int e=0;e<other.getSize();e++) this->add(other.get(e,&succ),&succ);
}

TMForwardList TMForwardList::operator+(const TMForwardList &other)
{
TMForwardList k;
k.allocationFlag=1;
k+=(*this);
k+=(other);
return k;
}

TMForwardList TMForwardList::operator+(const TMList &other)
{
TMForwardList k;
k.allocationFlag=1;
k+=(*this);
k+=(other);
return k;
}

void TMForwardList::add(int data,bool *success)
{
if(success) *success=false;
TMNode *t;
t=new TMNode;
if(t==NULL) return;
t->data=data;
if(this->start==NULL)
{
this->start=this->end=t;
}
else
{
this->end->next=t;
this->end=t;
}
this->size++;
if(success) *success=true;
}

void TMForwardList::insertAt(int index,int data,bool *success)
{
if(success) *success=false;
if(index<0 || index>this->size) return;
if(index==this->size)
{
this->add(data,success);
return;
}
TMNode *t,*j,*gg;
int x;
gg=new TMNode;
if(gg==NULL) return;
gg->data=data;
for(x=0,t=this->start;x<index;x++)
{
j=t;
t=t->next;
}
if(this->start==t)
{
gg->next=this->start;
this->start=gg;
}
else
{
j->next=gg;
gg->next=t;
}
this->size++;
if(success) *success=true;
}

int TMForwardList::get(int index,bool *success) const
{
if(success) *success=false;
if(index<0 || index>=this->size) return 0;
TMNode *t;
int x;
x=0;
t=this->start;
while(x<index)
{
t=t->next;
x++;
}
int data=t->data;
if(success) *success=true;
return data;
}

int TMForwardList::getSize() const
{
return this->size;
}

void TMForwardList::update(int index,int data,bool *success)
{
if(success) *success=false;
if(index<0 || index>=this->size) return;
TMNode *t;
int x;
for(x=0,t=this->start;x<index;x++) t=t->next;
t->data=data;
if(success) *success=true;
}

int TMForwardList::removeAt(int index,bool *success)
{
if(success) *success=false;
if(index<0 || index>=this->size) return 0;
TMNode *t,*j;
int x;
t=this->start;
for(x=0;x<index;x++)
{
j=t;
t=t->next;
}
int data=t->data;
if(this->start==t && this->end==t)
{
this->start=NULL;
this->end=NULL;
} else if(this->start==t)
{
this->start=this->start->next;
} else if(this->end==t)
{
this->end=j;
this->end->next=NULL;
} else
{
j->next=t->next;
}
delete t;
this->size--;
if(success) *success=true;
return data;
}

void TMForwardList::removeAll()
{
this->clear();
}

void TMForwardList::clear()
{
TMNode *t;
while(this->start!=NULL)
{
t=this->start;
this->start=this->start->next;
delete t;
}
this->end=NULL;
this->size=0;
}

int main()
{
TMForwardList list1;
bool succ;
list1.add(100,&succ);
list1.add(123320,&succ);
list1.add(10330,&succ);
list1.add(1320,&succ);
list1.add(78,&succ);
list1.add(430,&succ);
list1.add(230,&succ);
Iterator iterator1=list1.getIterator();
Iterator iterator2=list1.getIterator();
Iterator iterator3=list1.getIterator();
Iterator iterator4=list1.getIterator();
Iterator iterator5=list1.getIterator();
cout<<iterator1.next()<<endl;
cout<<iterator2.next()<<endl;
cout<<iterator3.next()<<endl;
cout<<iterator4.next()<<endl;
cout<<iterator5.next()<<endl;
return 0;
}

int mainTMArrayList()
{
TMArrayList list1;
bool k;
for(int i=100;i<=190;i++) list1.add(i,&k);
cout<<"size of list1 "<<list1.getSize()<<endl;
TMArrayList list2;
for(int i=89;i<100;i++) list2.add(i,&k);
cout<<"size of list2 "<<list2.getSize()<<endl;
TMArrayList list3;
list3=list1+list2;
cout<<"Size of List3 : "<<list3.getSize()<<endl;
for(int i=0;i<list3.getSize();i++) cout<<list3.get(i,&k)<<" ";
return 0;
}
