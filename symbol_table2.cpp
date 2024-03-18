#include<iostream>
#include<string.h>
#define hashsize 100
using namespace std;
/*
1) Hashing technique is used here.
2) closed addressing technique known as bucketing/chaining is used.
3) Hash table never fills up, we can always add more elements to the chain.
4) Less sensitive to hash function.
5) Used Linked list of hashtables to define a scope whenever we encounter new scope.
*/
class node
{
    public:
        string key;
        string value;
        node*next;
        node()
        {
            next=NULL;
        }
        ~node()
        {
            if(next)
            {
                delete next;
                next=NULL;
            }
        }
};
/* Harshtable using bucketing/chaining */
class hashtable{
    node**arr1;
    public:
        hashtable()
        {
            arr1=new node*[hashsize+1];
            for(int i=0;i<=hashsize;i++)arr1[i]=NULL;
        }
        int hash(string s)
        {
            int i=0,h=0,d=1,b=7;
            while(i<s.size())
            {
                h=h+s[i]*d;
                d=(d*b)%hashsize;
                h=h%hashsize;
                i++;
            }
            return h%hashsize;
        }
        void insert(string k,string v)
        {
            int hashvalue=hash(k);
            node*val=new node;
            val->key=new char[100];
            val->value=new char[100];
            val->value=v;
            val->key=k;
            val->next=arr1[hashvalue];
            arr1[hashvalue]=val;
        }
        pair<bool,string> search(string k)
        {
            pair<bool,string>retval;
            retval.second="";
            int hashvalue=hash(k),flag=0;
            node*nptr=arr1[hashvalue];
            while(nptr&&!flag)
            {
                if(nptr->key==k)
                {
                    retval.second=nptr->value;
                    flag=1; 
                }
                nptr=nptr->next;
            }
            retval.first=flag;
            return retval;
        }
        void delete1(string k)
        {
            int hashvalue=hash(k),flag=0;
            node*nptr=arr1[hashvalue];
            if(nptr->key==k)
            {
                node*temp=nptr->next;
                free(nptr);
                arr1[hashvalue]=temp;
            }
            else
            {
                while(nptr->next&&nptr->next->key!=k)
                {
                    nptr=nptr->next;
                }
                if(nptr->next)
                {
                    node*temp=nptr->next;
                    nptr->next=temp->next;
                    free(temp);
                }
                else
                {
                    printf("Element not found\n");
                }
            }
        }
        void display()      
        {
            for(int i=0;i<hashsize;i++)
            {
                if(arr1[i])
                {
                    node*nptr=arr1[i];
                    while(nptr)
                    {
                        printf("%s ",nptr->value);
                        nptr=nptr->next;
                    }
                    printf("\n");
                }
            }
        }
        ~hashtable()
        {
            for(int i=0;i<=hashsize;i++)
            {
                if(arr1[i])
                {
                    delete arr1[i];
                    arr1[i]=NULL;
                }
            }
        }
};
/* node of linked list of scope that consist of hashtable for symbols */
class scopenode{
    public:
        hashtable h;
        scopenode*next; 
};
int main()
{
    FILE*fp=fopen("input.txt","r");
    char arr[10000]={'\0'};
    int flag=0;
    while(flag==0)
    {
        char c=getc(fp);
        if(c!=EOF)
        {
            char ar[2]={c,'\0'};
            strcat(arr,ar);
        }
        else 
        {
            flag=1;
        }
    }
    fclose(fp);
    scopenode*head=NULL;
    for(int i=0;i<strlen(arr);i++)
    {
        while(arr[i]==' '||arr[i]=='\t')
        {
            i++;
        }
        if(arr[i]=='b')
        {
            if(head)
            {
                scopenode*temp=new scopenode;
                temp->next=head;
                head=temp;
            }
            else
            {
                head=new scopenode;
                head->next=NULL;
            }
            while(arr[i]!='\n')i++;
        }
        else if(arr[i]=='a')
        {
            if(head)
            {
                string key,value;
                int j=i+7;
                while(arr[j]!=' ')
                {
                    key.push_back(arr[j]);
                    j++;
                }
                j++;
                while(arr[j]!='\n'&&arr[j]!=' ')
                {
                    value.push_back(arr[j]);
                    j++;
                }
                head->h.insert(key,value);
                while(arr[i]!='\n')
                {
                    i++;
                }
            }
            else
            {
                cout<<"Given varaivle isn't declared in any block\n";
                exit(0);
            }
        }
        else if(arr[i]=='p')
        {
            int flag=0;
            int k=i+6;
            string key;
            while(arr[k]!='\n'&&arr[k]!=' ')
            {
                key.push_back(arr[k]);
                k++;
            }
            scopenode*nptr=head;
            pair<int,string>ans;
            ans.first=0;
            ans.second="";
            while(nptr&&ans.first==0)
            {
                ans=nptr->h.search(key);
                if(ans.first)
                {
                    cout<<ans.second<<endl;
                }
                nptr=nptr->next;
            }
            if(ans.first==0)
            {
                cout<<"error: '"<<key<<"' was not declared in this scope\n";
            }
            while(arr[i]!='\n')
            {
                i++;
            }
        }
        else
        {
            if(head)
            {
                scopenode*temp=head->next;
                delete head;
                head=temp;
            }
            else
            {
                cout<<"No 'begin' for corresponding 'end'\n";
                exit(0);
            }
            while(arr[i]!='\n'&&i<strlen(arr))
            {
                i++;
            }
        }
    }
    if(head)
    {
        cout<<"No 'end' for corresponding 'begin'\n";
        scopenode*nptr=head;
        while(nptr)
        {
            head=nptr->next;
            free(nptr);
            nptr=head;
        }
    }
    fclose(fp);
    return 0;
}
/*bool codeverifier(char*arr)
{
    bool retval=0,cnt=0;
    for(int i=0;i<strlen(arr);i++)
    {
        while(arr[i]==' ')i++;
        if(arr[i]=='a'&&arr[i+1]=='s'&&arr[i+2]=='s'&&arr[i+3]=='i'&&arr[i+4]=='g'&&arr[i+5]=='n'&&arr[i+6]==' ')
        {
            int j=i+7;
            while(arr[j]!=' ')
        }
    }
}*/
/*
begin
    assign x 10
    assign y 20
    assign z 30
        begin
            assign y 40
            assign z 50
                begin
                    assign x 60
                    assign z 70
                        begin 
                            assign x 80
                            assign y 90
                            print x
                            print y
                            print z
                        end
                    print x
                    print y
                    print z
                end
            print x
            print y
            print z
        end
    print x
    print y
    print z
end
begin
    assign a 1
    assign b 2
    begin
        assign a 3
        assign c 4
        print b
        begin
            assign c 5
            print b
            print a
            print c
        end
        print a
        print c
    end
    print a
end
*/