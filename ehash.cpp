#include <bits/stdc++.h>
using namespace std;

//#define Bsize 2

typedef long double LD;
typedef long long ll;
#define pb push_back
#define REP(i,n) for (int i = 0; i < n; ++i)
#define FOR(i,a,b) for (int i = a; i < b; ++i)

#define FAST_IO ios::sync_with_stdio(0); cin.tie(0); cout.tie(0);

int Bsize;
class bucket{
    private:
        int *b;
        int local_depth;
        int position;
    public:
        bucket(int ld){
            b=(int*)malloc(Bsize*sizeof(int));
            position=0;
            local_depth=ld;
        }
        void insert(int x){if(position<Bsize)b[position++]=x;}
        bool isFull(){return position<Bsize ? 0: 1;}
        void prtbucket(){
            cout<<":  ";
            REP(i,position){
                cout<<b[i]<<" ";
            }cout<<endl;
        }
        void printbucket(){
            cout<<position<<" ";
            cout<<local_depth<<" ";
            //prtbucket();
            cout<<endl;
        }
        void erase(){position=0;}
        int* returndata(){return b;}
        int size(){return Bsize;}
        int getpos(){return position;}
        int getlocdep(){return local_depth;}
        void inclocdep(){++local_depth;}
        bool exists(int x){
            for(int i=0;i<=position;i++)
                if(b[i]==x)
                    return 1;
            return 0;
        }
        void del(int x){
            bool found=0;
            for(int i=0;i<=position;i++){
                if(b[i]==x){found=1;position--;}
                if(found){b[i]=b[i+1];}
            }
        }
};

class ehash{
    private:
        int size;
        bucket** dirs;
        int global_depth;
        list<bucket> b;
        
    public:
        ehash(int gd){
            global_depth=gd;
            size=1<<global_depth;
            dirs=(bucket**)malloc(sizeof(bucket*)*size);
            REP(i,size){
                bucket B(gd);
                b.pb(B);
                dirs[i]=&b.back();
            }
        }
        void prtdirbuckets(){REP(i,1<<global_depth){cout<<dirs[i];dirs[i]->prtbucket();}}
        void prtallbuckets(){for(auto i=b.begin();i!=b.end(); ++i){i->prtbucket();}}
        void printstatus(){
            cout<<global_depth<<endl;
            cout<<b.size()<<endl;
            for(auto i=b.begin();i!=b.end(); ++i){
               i->printbucket();
           }
        }
        int getgd(){return global_depth;}
        bool exists(int x){
            int loc = x%(1<<global_depth);
            return  dirs[loc]->exists(x);
        }
        void insert(int x){
            int loc = x%(1<<global_depth);
            if(global_depth>=20)return;
            if(!exists(x))
                if(dirs[loc]->isFull()){
                    bucket* B=dirs[loc];
                    if(B->getlocdep() == global_depth){
                        //double dir
                        ++global_depth;
                        if(global_depth>=20)return;
                        int new_size=size<<1;
                        dirs=(bucket**) realloc(dirs,new_size*sizeof(bucket*));
                        copy(dirs, dirs+size,dirs+size );
                        int newloc=size+loc;
                        size=new_size;
                        //new bucket
                        dirs[loc]->inclocdep();
                        bucket Bnew(B->getlocdep());
                        b.pb(Bnew);
                        dirs[newloc]=&b.back();
                        
                        //rearrange data
                        bucket Btmp=*B;
                        B->erase();
                        int* data=Btmp.returndata();
                        for(int i=0; i<Bsize; i++)
                            dirs[(*(data+i))%size]->insert(*(data+i)); //change this...
                        insert(x);
                    }else{
                         dirs[loc]->inclocdep();
                        bucket Bnew(B->getlocdep());
                        b.pb(Bnew);
                        int newloc=(1>>B->getlocdep())/2+x%(1<<(global_depth-1));
                        //if(x==1)cout<<"::"<<newloc<<" "<<loc<<" "<<endl;
                        int i=newloc;
                        int inc=(1<<B->getlocdep());
                        //dirs[newloc]=&b.back();
                        while(i<size){
                            if(i<size)dirs[i]=&b.back();
                            i+=inc;
                        }
                        //rearrange data
                        bucket Btmp=*B;
                        B->erase();
                        int* data=Btmp.returndata();
                        for(int i=0; i<Bsize; i++)
                            dirs[(*(data+i))%size]->insert(*(data+i));
                        insert(x);
                    }
                }else
                    dirs[loc]->insert(x);
            }
        void del(int x){
            int loc = x%(1<<global_depth);
            if(dirs[loc]->exists(x)){
                dirs[loc]->del(x);
            }

        }
};

signed main(){
    FAST_IO
    int gd,bsize;
    cin>>gd>>Bsize;
    ehash H1(gd);
    int n,x;
    while(cin>>n){
        switch(n){
            case 2: int x; cin>>x; H1.insert(x); break;
            case 3: int y; cin>>y; cout<<(H1.exists(y)?"Found":"Key not found")<<endl; break;
            case 4: int z; cin>>z; H1.del(z); break;
            case 5: H1.printstatus(); break;
            case 6: return 0; 
            case 7: H1.prtdirbuckets();break;
            default: cout<<".\n";break;
        }
    }
    
}
