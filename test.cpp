#include "test.h"

using namespace std;

//diferenciar constructor para server y cliente
udp::udp(int port){


    fd = socket(AF_INET, SOCK_DGRAM,0);
    check(fd,__FILE__,__LINE__);
    cout<<"socket en puerto "<<port<<endl;
    if(port!=-1)
    {
    sockaddr_in hint;
    hint.sin_family=AF_INET;
    hint.sin_port= htons(port);
    hint.sin_addr.s_addr = INADDR_ANY;

    check(bind(fd, (struct sockaddr *)&hint,sizeof(hint)),__FILE__,__LINE__);

    cout<< "Servidor escuchando en puerto "<<port<<endl;
    }
    else
        cout<<"cliente listo"<<endl;
}

bool udp::send(const char* ip ,int port, const char* send_string){

    struct sockaddr_in  servaddr;

    memset(&servaddr, 0, sizeof(servaddr));
       
 
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);
    inet_pton(AF_INET, ip, &servaddr.sin_addr.s_addr);
   
    check(sendto(fd, (const char *)send_string, strlen(send_string), MSG_CONFIRM, (const struct sockaddr *) &servaddr, sizeof(servaddr)),__FILE__,__LINE__);
    return 1;
}


bool udp::send( struct sockaddr_in  servaddr, const char* send_string){

    check(sendto(fd, (const char *)send_string, strlen(send_string), MSG_CONFIRM, (const struct sockaddr *) &servaddr, sizeof(servaddr)),__FILE__,__LINE__);
    return 1;
}
char* udp::recv(struct sockaddr_in  &cliaddr){//anadir int size para indicar los bytes a recibir

int size=1024;
    char str[INET_ADDRSTRLEN];
    socklen_t len, n;
   char buffer[1024]={"0"};
    len = sizeof(cliaddr);  //len is value/result
   
    n = recvfrom(fd, (char *)buffer, size, 
                0, ( struct sockaddr *) &cliaddr,
                &len);
/*
    inet_ntop(AF_INET,&cliaddr.sin_addr,str,INET_ADDRSTRLEN);
     cout<<"Mensaje recivido de: "<<str<<":"<<addr.sin_port<<endl;
  */
    check(n,__FILE__,__LINE__);
    return buffer;

}
bool udp::isServer()
{
if(port>0)
  return true;
return false;
}
//recv si es server, y cuenta bits recividos en el tiempo
//send envia continuamente al servidor 
void udp::test(const char* ip ,int port)
{
   struct sockaddr_in  addr;
     memset(&addr, 0, sizeof(addr));
if(isServer())//Recibir
{
cout<<"Server Waits msg"<<endl;
char str[INET_ADDRSTRLEN];
auto start = std::chrono::high_resolution_clock::now();
  auto stop = std::chrono::high_resolution_clock::now();
  
while(1)
{
    recv(addr);
    start = std::chrono::high_resolution_clock::now();
    for(int i;i<10;i++)
    {
        recv(addr);
        send(addr,hello);
    }
  
    
     stop = std::chrono::high_resolution_clock::now();
     auto duration = duration_cast<nanoseconds>(stop - start);
  
     double R =duration.count();
    double aux=(1024*8)/R*(100);//Mbits/s
cout<<aux<<" Mbits/s"<<endl;

}

}
else//Enviar
{
      
    while(1)
    {
       send(ip ,port,hello);
       sleep(2);
    
    
    
        for(int i;i<10;i++)
        {
            send(ip ,port,hello);
             sleep(2);
            recv(addr);
        }
    
    }
    

}
}

bool self_test(const char* local_ip)
{
cout<<"Running self test"<<endl;
    udp cli;
    udp serv(5000);
   
cout<<"server";
    std::thread servidor(&udp::test,serv,"0.0.0.0",0);
  

cout<<"peer"; 
   std::thread cliente(&udp::test,cli,local_ip,5000);
 
    servidor.join();
    cliente.join();

return 0;

}
int main() {

self_test("172.26.98.16");
 

    return 0;
}
