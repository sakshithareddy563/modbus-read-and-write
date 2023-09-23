#include<modbus/modbus-tcp.h>
#include<iostream>
#include<map>
#include<unistd.h>
#include<vector>
#include<cstdlib>
using namespace std;

// int argc;
// char **argv;

modbus_t* create_ctx()
{
    const char* ip = "127.0.0.1";
    int port = 502;
    int slave = 2;
    modbus_t *ctx = modbus_new_tcp(ip, port);
    int s = modbus_set_slave(ctx, slave);
    
    if(s==-1)
    {
        cout<<"unable to set slave, quitting\n";
        exit(0);
    }
    return ctx;
}

modbus_t* establish_connection()
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

uint16_t* split_num(int nb, uint64_t random_val)
{
    static uint16_t arr[INT_MAX];
    if(random_val>65535 && nb<2)
    {
        cout<<"failed to split"<<endl;
    }
    else if(random_val>4294967295 && nb<3)
    {
        cout<<"failed to split"<<endl;
    }
    else if(random_val>281474976710655 && nb<4)
    {
        cout<<"failed to split"<<endl;
    }
    else
    {
    
        for(int i=0;i<nb;i++)
        {
            arr[i] = (uint16_t) (random_val >> 16) & 0xFF;       // left shift and right shift
        }
    }
    
    return arr;
}
 
void write(const map<int,int>&addr, modbus_t* ctx)
{
    for(auto it=addr.begin(); it!=addr.end(); it++)
    {
        uint64_t random_val = rand();
        uint16_t* val = split_num(it->second, random_val);
        int wc = modbus_write_registers(ctx, it->first, it->second, val);
        
        if(wc!=it->second)
            cout<<"failed to write with wc code: "<<wc<<endl; 
        else
            cout<<"wrote to register: "<<it->first<<" with value "<<*val<<endl;
    }
}

int main(int argc, char **argv)
{
    modbus_t* ctx = establish_connection();

    map<int,int>addr;
    for(int i=0;i<100;i+=2)
    {
        addr[i]=2;
    }

    addr[505] = 4;
    addr[510]=6;
    // write function call
    while(true)
    {
        write(addr, ctx);
    }

    modbus_close(ctx);
    modbus_free(ctx);
}