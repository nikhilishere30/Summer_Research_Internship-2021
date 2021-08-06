#include<bits/stdc++.h>
using namespace std;

char state='F';
int ADW=0;
int Ws=100000;
int Wf=10000;
int Wm=10000;
bool stop_receiving=false,MIOS=false;
int Queue_threshold_value_L=3;
int Queue_length_at_time_t_l=1;
queue<int>q;
void write1(fstream& fout,fstream& fin)
{
    string line;
    while (fin) {

		// Read a Line from File
		getline(fin, line);
        
        fout<<line<<endl;
		// Print line in Console
		
	}
    

}                   
void sync_for_flushing()
{
    state='F';
    ADW=0;
}
void predictor(int write_request_size)
{
    if(state=='F')
    {
        if(ADW+write_request_size<Wf)
        state='F';

        else
        state='S';
    }
    else if(state=='M')
    {
        if(ADW+write_request_size<Wm)
        state='M';

        else
        state='S';
    }
    else if(state=='S')
    {
        ADW=max(0,ADW-Wm);
        state='M';
    }
}
void BCW(int write_request_size,fstream& fin,fstream& fout2)
{
    sync_for_flushing();
    
    if(state=='F')
    {
        
        write1(fout2,fin);
        fout2.seekg(0, ios::end);
         int y = fout2.tellg();
            cout<<"OUTPUT FROM HDD and size of output file - "<<y<<endl;
            sync_for_flushing();
        predictor(write_request_size);
    }   
    
    else if(state=='M')
    {
        write1(fout2,fin);
        predictor(write_request_size);
    }    
    
    else
    {
        if(state=='S')
        {
            stop_receiving=true;
            while(state=='S')
            {        
                ADW=ADW+64;                            //64 is the size of PS.
                predictor(64);
            }
            stop_receiving=false;
            ADW=0;
        }
        if(state=='M')
        {
            ADW=ADW+4;
            predictor(4);                             //4 is the size of PF.
        }
    }

    if(q.size()>0)
    {
        q.pop();
        Queue_length_at_time_t_l--;
    }
    
}

void MIXED_IO_SCHEDULER(int write_request_size,fstream& fin,fstream& fout,fstream& fout2)
{
    if(stop_receiving==false)
    {
        if(Queue_length_at_time_t_l>Queue_threshold_value_L&&(state=='F'||state=='M'))
        BCW(write_request_size,fin,fout2);

        else if(MIOS==true&&state=='F')  
        {
            BCW(write_request_size,fin,fout2);    
        }                                //If MIOS=true it means MIOS is enabled 
                                   //and if it is false it means it is disabled
        
        else
        {
            
            
            q.push(write_request_size);
            Queue_length_at_time_t_l++;
            write1(fout,fin);
            fout.seekg(0, ios::end);
            int y = fout.tellg();
            cout<<"OUTPUT FROM SSD and size of output file - "<<y<<endl;
            sync_for_flushing();
        }       
    }
    else
    {
       
        q.push(write_request_size);
        Queue_length_at_time_t_l++;
        write1(fout,fin);
        fout.seekg(0, ios::end);
         int y = fout.tellg();
            cout<<"OUTPUT FROM SSD and size of output file - "<<y<<endl;
            sync_for_flushing();
        sync_for_flushing();
    }    
}
int main()
{
    
    for(int i=0;i<10;i++)
    {
        fstream fout_SSD;
    fstream fout2_HDD;
    fstream fin;

	fin.open("sample.txt",ios::in);
    
    fout_SSD.open("sample1.txt",ios::app); 
    fout2_HDD.open("D:\hello.txt",ios::app);
    int y=0;
    ifstream in_file("sample.txt", ios::binary);
   in_file.seekg(0, ios::end);
   y = in_file.tellg();
MIXED_IO_SCHEDULER(y,fin,fout_SSD,fout2_HDD);
    }
    

}