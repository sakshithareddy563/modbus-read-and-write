#include<modbus/modbus-tcp.h>
#include<iostream>
#include<map>
#include<unistd.h>
using namespace std;

modbus_t* create_ctx()
{
    const char* ip = "127.0.0.1";
    int slave;
    int port;
    modbus_t *ctx = modbus_new_tcp(ip, port);
    int s = modbus_set_slave(ctx, slave);
    
    if(s==-1)
    {
        cout<<"unable to set slave, quitting\n";
        exit(0);
    }
    return ctx;
}

modbus_t* establish_connection(int slave, int port)
{   
    modbus_t* ctx = create_ctx();
    int c = modbus_connect(ctx);
    if(c==-1)
    {
        cout<<"unable to connect to slave, quitting\n";
        exit(0);
    }
    return ctx;
}

int read(const map<int,int>&mp, modbus_t* ctx, uint16_t* arr)
{
    for(auto itr=mp.begin(); itr!=mp.end(); itr++)
    {
        int rc = modbus_read_registers(ctx, itr->first, itr->second, arr);
        if(rc!=itr->second)
        {
            cout<<"failed to read"<<endl;
        }
    }
    return 0;
}

void print(uint16_t *values, int n)
{ 
    for(int i=0;i<n;i++)
    {
        cout<<values[i]<<endl;
    }
}

int main(int argc, char *argv[])
{
    modbus_t* ctx = establish_connection(stoi(argv[1]),stoi(argv[2]));

    uint16_t arr[100];
    map<int,int>mp;
    for(int i=0;i<100;i++)
    {
        mp[i]=2;

    }

    while(true)
    {
        read(mp, ctx, arr);
        print(arr, 100);
        sleep(1);
    }

    modbus_close(ctx);
    modbus_free(ctx);
}





  