#include <stdio.h> 
#include <stdlib.h>
#include <sys/types.h>
#include <iostream>
#include <string>
#include <fstream>
#define total_processes 10
using namespace std;
int n=0;
int pid_temp; 
class process{
    public:
    int pid=0, AT = 0, BT = 0 ,CT = 0, TAT = 0, WT = 0, RT = 0,BT_temp = 0;
};

process p[total_processes];
void sort_array(process p[100]){
	process temp;
	int i,j;
		for(i=1;i<total_processes;i++)
			for(j=0;j<total_processes-i;j++){
				if(p[j].AT > p[j+1].AT){
					temp = p[j];
					p[j] = p[j+1];
					p[j+1] = temp;
			}
		}
}

void sort_array_BT(process p[100], int k){
    
	process temp;
	int i,j;
		for(i=1;i<k;i++)
			for(j=0;j<k-i;j++){
				if(p[j].BT > p[j+1].BT){
					temp = p[j];
					p[j] = p[j+1];
					p[j+1] = temp;
			}
		}
}

void processes_CT_info(process h, int l){
	pid_temp = h.pid;
	p[pid_temp].CT = l;
}

void processes_RT_info(process h, int l){
	pid_temp = h.pid;
	p[pid_temp].RT = l-p[pid_temp].AT;
}



class process_creator{
    public:
    process_creator(process p[total_processes]){
        int i=0;
        int o=0;
        while (i<total_processes){
            p[i].BT= ( rand() % 31);
            p[i].AT= ( rand() % 71);
            p[i].BT_temp= p[i].BT;
            i=i+1;
        }
        
        sort_array(p);
        while (o<total_processes){
            p[o].pid = o;
            o++;
        }
        
    }
    
};

class scheduler {
    public:
    process readyqueue[1000];
    int size=5;
    int front = 0;
    int rear = 0;
    int length=0;
    
    
void enqueue(process i){
      readyqueue[rear] =i;
      rear=rear+1;
      length=length+1;
   }
   
void dequeue() {
      int j = 0;
      for(j= 0;j < rear -1; j++){
          readyqueue[j] = readyqueue[j+1];
      }
      rear--;
      length--;
   }
   
void display() 
    { 
        int i; 
        if (front == rear) { 
            cout<<"\nQueue is Empty\n"; 
            return; 
        } 
  
        for (i = front; i < rear; i++) { 
            cout<<readyqueue[i].BT<<" ";
        } 
        cout<<"\n";
        return; 
    } 
   
};

class simulatorFCFS{
    public:
    int comp=0;
    int i;
    int burst_left;
    int last_pid;
    simulatorFCFS(process p[100],scheduler FCFS){
        
        burst_left=p[0].BT;
        int i=0;
        while(comp<total_processes){
            
            
            while(FCFS.length!=5 && (n<total_processes)){
                if(p[n].AT<=i){
                    FCFS.enqueue(p[n]);
                    cout<<"\nqueued process with arrival time: "<<(FCFS.readyqueue[FCFS.rear-1]).AT<<" at time "<<i<<"\n";
                   n++;
                }
                else{
                    break;
                }
            }
            
            if(burst_left==0 && FCFS.length!=0){
                last_pid=FCFS.readyqueue[FCFS.front].pid;
                processes_CT_info(FCFS.readyqueue[FCFS.front], i);
                
                FCFS.dequeue();
                if (FCFS.length>0){
                burst_left=FCFS.readyqueue[FCFS.front].BT;
                }
                else{
                 burst_left=p[n].BT;
                }
                cout<<"complete and dequeued process with pid: "<<last_pid <<" at time "<<i<<"\n";
                comp=comp+1;
                
            }
            
            
            if(FCFS.length!=0){
                burst_left--;
                if(burst_left==0 && FCFS.length>1){
                    processes_RT_info(FCFS.readyqueue[FCFS.front+1], i+1);
                }
            }
            i++;
        }
    }
};


class simulatorSRTF{
    public:
    int comp=0;
    int i;
    int burst_left;
    int last_pid;
    int last_SRTFlength;
    
    simulatorSRTF(process p[100],scheduler SRTF){
        
        
        int i=0;
        burst_left=p[0].BT;
        
        while(comp<total_processes){
            last_SRTFlength=SRTF.length;
            while(n<total_processes){
                if(SRTF.length<5 && p[n].AT<=i){
                    SRTF.enqueue(p[n]);
                    cout<<"\nqueued process with arrival time: "<<SRTF.readyqueue[SRTF.rear -1].AT<<" at time "<<i<<"\n";
                    n++;
                }
                else if(p[n].AT<=i && SRTF.length==5){
                    p[n].AT=p[n].AT-1; 
            }
                
                else{
                    break;
                }
            }
            
            if(SRTF.length>1 && SRTF.length!=last_SRTFlength){
                SRTF.readyqueue[SRTF.front].BT=burst_left;
                sort_array_BT(SRTF.readyqueue, SRTF.length);
                burst_left=SRTF.readyqueue[SRTF.front].BT;
                
            }
            
            if(burst_left==0 && SRTF.length!=0){
                last_pid = SRTF.readyqueue[SRTF.front].pid;
                processes_CT_info(SRTF.readyqueue[SRTF.front], i);
                SRTF.dequeue();
                if (SRTF.length>0){
                    
                    burst_left=SRTF.readyqueue[SRTF.front].BT;
                    
                }
                else{
                 burst_left=p[n].BT;
                }
                cout<<"complete and dequeued process: "<<last_pid<< " at time "<<i<<"\n";
             
                comp=comp+1;
                
            }
            if(SRTF.length!=0){
                burst_left--;
                if(burst_left==0 && SRTF.readyqueue[SRTF.front+1].BT==SRTF.readyqueue[SRTF.front+1].BT_temp &&SRTF.length>1){
                    processes_RT_info(SRTF.readyqueue[SRTF.front+1], i+1);  //i+1 is the start time if BT_temp==BT
                }
            }
            i++;
        }
    }
};

class simulatorRR{
    public:
    int comp=0;
    int i;
    int burst_left;
    int last_pid;
    int currentslice;
    
    simulatorRR(process p[100],scheduler RR, int timeslice){
        
        int current_slice=0;
        burst_left=p[0].BT;
        int i=0;
        
        while(comp<total_processes){
            while(RR.length!=5 && (n<total_processes)){
                if(p[n].AT<=i){
                    RR.enqueue(p[n]);
                    cout<<"\nqueued process with arrival time: "<<RR.readyqueue[RR.rear -1].AT<<" at time "<<i<<"\n";
                   n++;
                }
                else{
                    break;
                }
            }
            if(burst_left==0 && RR.length!=0){
                last_pid=RR.readyqueue[RR.front].pid;
                processes_CT_info(RR.readyqueue[RR.front], i);
                RR.dequeue();
                if (RR.length>0){
                    burst_left=RR.readyqueue[RR.front].BT;
                }
                else{
                 burst_left=p[n].BT;
                }
                cout<<"complete and dequeued process: "<<last_pid<< " at time "<<i<<"\n";
                current_slice=0;
                comp=comp+1;
                
            }
            
            if(RR.length>1 && current_slice==timeslice){
                RR.readyqueue[RR.front].BT=burst_left;
                
                RR.enqueue(RR.readyqueue[RR.front]);
                RR.dequeue();
                burst_left = RR.readyqueue[RR.front].BT;
                current_slice=0;
                
            }
            
            if(RR.length==1 && current_slice==timeslice){
                RR.readyqueue[RR.rear].BT = burst_left;
                current_slice=0;
            }
            
            if(RR.length!=0){
                burst_left--;
                if(burst_left==0 && RR.readyqueue[RR.front+1].BT==RR.readyqueue[RR.front+1].BT_temp && RR.length>1){
                    processes_RT_info(RR.readyqueue[RR.front+1], i+1); 
                }
                current_slice++;
            }
            i++;
        }
    }
};

int main(){
    process_creator pc(p);
    scheduler FCFS;
    scheduler SRTF;
    scheduler RR;
    string name;
    int timeslice;
    cout<<"Enter the name of the algorithm SRTF/FCFS/RR: "<<"\n";
    cin>>name;
    
    for(int i=0;i<total_processes;i++){
        cout<<"process id: "<<p[i].pid<<" "<<"Arrival Time: "<<p[i].AT<<" "<<"Burst Time: "<<p[i].BT<<endl;
    }
    if(name== "SRTF"){
    simulatorSRTF sim(p,SRTF);
    }
    else if(name=="FCFS"){
    simulatorFCFS sim(p,FCFS);
    }
    else if(name=="RR"){
        cout<<"\nEnter time quantum: ";
        cin>>timeslice;
        simulatorRR sim(p,RR,timeslice);
    }
    ofstream myfile;
    myfile.open ("Processes.txt");
    myfile << "|__Pid__|__Arrival Time__|__Burst Time___|Completion Time|_TurnAround Time_|__Waiting Time__|Response Time|\n";
    
    for(int i=0;i<total_processes;i++){
        myfile<<"|\t"<<p[i].pid<<"\t|\t\t"<<p[i].AT<<"\t\t |\t"<<"\t "<<p[i].BT<<"\t\t | \t\t"<<p[i].CT<<"\t\t |   \t"<<(p[i].CT)-(p[i].AT)<<"\t\t   |    \t"<<(p[i].CT-p[i].AT)-p[i].BT<<"\t\t|  \t"<<p[i].RT<<endl;
    }
    myfile.close();
    
    return 0;
}
