/**
  ******************************************************************************
  * @file    demo_tcpip.c
  * @author  SIMCom OpenSDK Team
  * @brief   Source file of tcpip stack operation.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 SIMCom Wireless.
  * All rights reserved.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "simcom_api.h"
#include "stdlib.h"
#include "string.h"
#include "stdio.h"

#define SC_TCPIP_TEST_STACK_SIZE (1024*4)
#define SC_TCPIP_TEST_TASK_PRO 80
#define SC_MAX_STRING_LEN  64
#define SC_REMOTE_PORT_MIN               0
#define SC_REMOTE_PORT_MAX               65535
extern sMsgQRef simcomUI_msgq;
extern void PrintfOptionMenu(INT8* options_list[], int array_size);
extern void PrintfResp(INT8* format);



typedef enum{
    SC_TCPIP_DEMO_INIT                  = 1,
    SC_TCPIP_DEMO_TCP_CLIENT_OPEN       = 2,
    SC_TCPIP_DEMO_TCP_CLIENT_SEND       = 3,
    SC_TCPIP_DEMO_TCP_CLIENT_CLOSE      = 4,
    SC_TCPIP_DEMO_UDP_CLIENT_OPEN       = 5,
    SC_TCPIP_DEMO_UDP_CLIENT_SEND       = 6,
    SC_TCPIP_DEMO_UDP_CLIENT_CLOSE      = 7,
    SC_TCPIP_DEMO_TCP_SERVER_OPEN       = 8,
    SC_TCPIP_DEMO_TCP_SERVER_CLOSE      = 9,
    SC_TCPIP_DEMO_DEINIT                = 10,
    SC_TCPIP_DEMO_MAX                   = 99
}SC_FTPS_DEMO_TYPE;


INT8 *options_list[] = {
    " 1. Init",
    " 2. TCP client open",
    " 3. TCP client send",
    " 4. TCP client close",
    " 5. UDP client open",
    " 6. UDP client send",
    " 7. UDP client close",
    " 8. TCP server open",
    " 9. TCP server close",
    "10. DeInit",
    "99. back",
};

sMsgQRef tcpipUIResp_msgq;
INT32 pdp_id = 1;
INT32 tcpserver_localport= 0;
INT32 tcp_serverfd = 0;
struct SCipInfo localip_info = {TCPIP_PDP_INVALID, 0, {0}};
sTaskRef tcpserverRef;
static UINT8 simcomUIProcesserStack[1024 * 30];

/**
  * @brief  TCP server task.
  * @param  argv
  * @note   
  * @retval void
  */
void sc_tcpserver_task(void* argv)
{
    INT32 newfd = -1;
    INT32 backlog = 5;
    INT32 ret = -1;
    INT32 len = 0;
    UINT16 clientport = 0;
    INT8 revData[1024] = {0};
    INT8 res[100] = {0};
    SCsockAddrIn cliaddr;
    SCsockAddrIn sa;


    memset(&(sa), 0, sizeof(sa));

    sa.sin_addr.s_addr= SC_INADDR_ANY;
    sa.sin_family = SC_AF_INET;
    sa.sin_port   = sAPI_TcpipHtons(tcpserver_localport);

    if (sAPI_TcpipBind(tcp_serverfd, (const SCsockAddr *)&sa, sizeof(sa)) < 0)
    {
        sAPI_Debug("Bind local port fail errno[%d]",sAPI_TcpipGetErrno());
        return;
    }

    if (sAPI_TcpipListen(tcp_serverfd, backlog) < 0)
    {
        sAPI_Debug("listen fail errno[%d]",sAPI_TcpipGetErrno());
        return;
    }

    while (1)
    {
        newfd = sAPI_TcpipAccept(tcp_serverfd, (SCsockAddr *)&cliaddr, (UINT32 *)&len);
        sAPI_Debug("--- newfd[%d]",newfd);

        if(newfd < 0)
        {
            sAPI_Debug("accept fail ");
            sAPI_TaskSleep(1 * 200);
            continue;
        }

        while (1)
        {
            ret = sAPI_TcpipRecv(newfd, revData, sizeof(revData), 0);
            if(ret > 0)
            {
                sprintf(res,"\r\n TCP SERVER RECV FROM: ip[%s] port[%d] : ", sAPI_TcpipInet_ntoa(cliaddr.sin_addr.s_addr), clientport);
                PrintfResp(res);
                PrintfResp(revData);
                memset(revData, 0 ,strlen(revData));
            }
            else
            {
                break;
            }
        }
    }

}

/**
  * @brief  TCP Demo.
  * @param  void
  * @note   
  * @retval void
  */
void TcpipDemo(void)
{
    SC_STATUS status;
    UINT8 tcpip_pdpactive_flag[8] = {0};
    UINT8 tcp_client_flag = 0;
    UINT8 udp_client_flag = 0;
    UINT8 tcp_server_flag = 0;
    INT8 resp[100] = {0};
    UINT32 opt = 0;
    INT32 tcp_client_sockfd = -1;
    INT32 udp_client_sockfd = -1;
    INT32 reuseport = 1;
    INT32 ret = -1;
    SIM_MSG_T optionMsg ={0,0,0,NULL};
    SChostent *host_entry;
    INT8 *note = "\r\nPlease select an option to test from the items listed below.\r\n";

    while(1)
    {
        PrintfResp(note);
        PrintfOptionMenu(options_list,sizeof(options_list)/sizeof(options_list[0]));
        sAPI_MsgQRecv(simcomUI_msgq,&optionMsg,SC_SUSPEND);
        if(SRV_UART != optionMsg.msg_id)
        {
            sAPI_Debug("%s,msg_id is error!!",__func__);
            break;
        }

        sAPI_Debug("arg3 = [%s]",optionMsg.arg3);
        opt = atoi(optionMsg.arg3);
        sAPI_Free(optionMsg.arg3);

        switch(opt)
        {
            case SC_TCPIP_DEMO_INIT:
              {
                  if(!tcpip_pdpactive_flag[pdp_id])
                  {
                      if (SC_TCPIP_SUCCESS == sAPI_TcpipPdpActive(pdp_id, 1))
                      {
                          sAPI_Debug("pdp active successful");
                          tcpip_pdpactive_flag[pdp_id] = 1;
                          if (SC_TCPIP_SUCCESS != sAPI_TcpipGetSocketPdpAddr(pdp_id, 1, &localip_info))
                          {
                              sAPI_Debug("PDP get addr err");
                              sAPI_TcpipPdpDeactive(pdp_id,1);
                              PrintfResp("\r\nTCPIP init fail!\r\n");
                              break;
                          }
                          PrintfResp("\r\nTCPIP init successful\r\n");

                          if(TCPIP_PDP_IPV4 == localip_info.type)
                          {
                              memset(resp, 0, strlen(resp));
                              sprintf(resp,"\r\nPDPCTX type: IPV4,\r\nADDR: %s\r\n", sAPI_TcpipInet_ntoa(localip_info.ip4));
                              PrintfResp(resp);
                          }
                          else if(TCPIP_PDP_IPV6 == localip_info.type)
                          {
                              memset(resp, 0, strlen(resp));
                              char dststr[100] = {0};
                              sAPI_TcpipInet_ntop(SC_AF_INET6, localip_info.ip6, dststr, sizeof(dststr));
                              sprintf(resp,"\r\nPDPCTX type: IPV6,\r\nADDR: [%s]\r\n", dststr);
                              PrintfResp(resp);
                          }
                          else if(TCPIP_PDP_IPV4V6 == localip_info.type)
                          {
                              memset(resp, 0, strlen(resp));
                              char dststr[100] = {0};
                              sAPI_TcpipInet_ntop(SC_AF_INET6, localip_info.ip6, dststr, sizeof(dststr));
                              sprintf(resp,"\r\nPDPCTX type: IPV4V6,\r\nADDR: %s\r\nADDR: [%s]\r\n", sAPI_TcpipInet_ntoa(localip_info.ip4),dststr);
                              PrintfResp(resp);
                          }

                      }
                      else
                      {
                          PrintfResp("\r\nTCPIP Init Fail !\r\n");
                      }
                  }
                  else
                  {
                      PrintfResp("\r\nTCPIP Init Fail! You've done the Init.\r\n");
                  }
              }
              break;

            case SC_TCPIP_DEMO_TCP_CLIENT_OPEN:
              {
                  SCsockAddrIn server;
                  INT8 ip_address[SC_MAX_STRING_LEN] = {0};
                  INT32 remote_port = 0;

                  if(tcpip_pdpactive_flag[pdp_id])
                  {
                      if(0 == tcp_client_flag)
                      {
                          PrintfResp("\r\nPlease enter the server IP address to connect.\r\n");
                          if(SC_SUCCESS == sAPI_MsgQRecv(simcomUI_msgq,&optionMsg,SC_SUSPEND))
                          {
                              if(SRV_UART != optionMsg.msg_id)
                              {
                                  sAPI_Debug("msg_id is error!!\r\n");
                                  break;
                              }
                              sAPI_Debug("ADDR arg3 = [%s]",optionMsg.arg3);

                              memset(ip_address, 0, SC_MAX_STRING_LEN);
                              memcpy(ip_address, optionMsg.arg3, strlen(optionMsg.arg3) - strlen("\r\n"));

                              host_entry = sAPI_TcpipGethostbyname(ip_address);
                              if (host_entry == NULL)
                              {
                                  PrintfResp("\r\n Server ip addr err !\r\n");
                                  break;
                              }
                              else
                              {
                                  PrintfResp("\r\nPlease enter the server port to connect.\r\n");
                                  if(SC_SUCCESS == sAPI_MsgQRecv(simcomUI_msgq,&optionMsg,SC_SUSPEND))
                                  {
                                      if(SRV_UART != optionMsg.msg_id)
                                      {
                                          sAPI_Debug("msg_id is error!!");
                                          break;
                                      }

                                      sAPI_Debug("PORT arg3 = [%s]",optionMsg.arg3);
                                      remote_port =atoi((char *)optionMsg.arg3);
                                      if ((remote_port < SC_REMOTE_PORT_MIN) ||(remote_port > SC_REMOTE_PORT_MAX))
                                      {
                                        PrintfResp("\r\n Server ip port err !\r\n");
                                        break;
                                      }
                                  }
                                  else
                                  {
                                      break;
                                  }
                              }
                          }
                          else
                          {
                              break;
                          }

                          tcp_client_sockfd = sAPI_TcpipSocket(SC_AF_INET, SC_SOCK_STREAM, 0);
                          if(tcp_client_sockfd < 0)
                          {
                              sAPI_Debug("\r\nCreate socket err \r\n");
                              break;
                          }
                          else
                          {
                              sAPI_Debug("\r\ncreate socket successful\r\n");
                          }
                          server.sin_family = SC_AF_INET;
                          server.sin_port = sAPI_TcpipHtons(remote_port);
                          server.sin_addr.s_addr= *(UINT32 *)host_entry->h_addr_list[0];

                          if(0 != sAPI_TcpipConnect(tcp_client_sockfd,(SCsockAddr*)&server,sizeof(SCsockAddr)))
                          {
                              PrintfResp("\r\nConnect server fail !\r\n");
                              tcp_client_sockfd = -1;
                          }
                          else
                          {
                              tcp_client_flag = 1;
                              PrintfResp("\r\nConnect server successful !\r\n");
                          }
                      }
                      else
                      {
                          PrintfResp("\r\n You've already opened the TCP client !\r\n");
                      }
                  }
                  else
                  {
                      PrintfResp("You haven't done the Init !");
                  }
              }
              break;

            case SC_TCPIP_DEMO_TCP_CLIENT_SEND:
              {
                  INT8 recvbuf[1024] = {0};

                  if(1 == tcp_client_flag)
                  {
                      PrintfResp("\r\n Please enter the message you want to send. \r\n\r\n");
                      if(SC_SUCCESS == sAPI_MsgQRecv(simcomUI_msgq,&optionMsg,SC_SUSPEND))
                      {
                          if(SRV_UART != optionMsg.msg_id)
                          {
                              sAPI_Debug("msg_id is error!!\r\n");
                              break;
                          }
                          memset(recvbuf, 0, 1024);

                          sAPI_Debug("ADDR arg3 = [%s]",optionMsg.arg3);
                          strcpy(recvbuf, (char *)optionMsg.arg3);

                          ret = sAPI_TcpipSend(tcp_client_sockfd, recvbuf, strlen(recvbuf), 0);
                          if(ret < 0)
                          {
                              PrintfResp("\r\nTCP client send fail !\r\n");
                              break;
                          }
                          else
                          {
                              PrintfResp("\r\nTCP client send success !\r\n");
                              break;
                          }
                      }
                  }
                  else
                  {
                      PrintfResp("\r\nYou haven't opened the tcp client !\r\n");
                  }
              }
              break;

            case SC_TCPIP_DEMO_TCP_CLIENT_CLOSE:
              {
                  if(1 == tcp_client_flag)
                  {
                      if(tcp_client_sockfd >= 0)
                      {
                          if(0 == sAPI_TcpipClose(tcp_client_sockfd))
                          {
                              tcp_client_sockfd = -1;
                              tcp_client_flag = 0;
                              sAPI_Debug("\r\nClose tcp client success !\r\n");
                          }
                          else
                          {
                              sAPI_Debug("\r\nClose tcp client fail !\r\n");
                          }
                      }
                  }
                  else
                  {
                      PrintfResp("\r\nYou haven't opened the tcp client !\r\n");
                  }
              }
              break;

            case SC_TCPIP_DEMO_UDP_CLIENT_OPEN:
              {
                  if(tcpip_pdpactive_flag[pdp_id])
                  {
                      if(0 == udp_client_flag)
                      {
                          udp_client_sockfd = sAPI_TcpipSocket(SC_AF_INET, SC_SOCK_DGRAM, 0);
                          if(udp_client_sockfd < 0)
                          {
                              PrintfResp("\r\nUDP client fail !\r\n");
                          }
                          else
                          {
                              udp_client_flag = 1;
                              PrintfResp("\r\nUDP client open success !\r\n");
                          }
                      }
                      else
                      {
                          PrintfResp("\r\nYou've already opened the UDP client \r\n");
                      }
                  }
                  else
                  {
                      PrintfResp("You haven't done the Init");
                  }
              }
              break;

            case SC_TCPIP_DEMO_UDP_CLIENT_SEND:
              {
                  SCsockAddrIn server;
                  INT8 sendbuf[1024] = {0};
                  INT8 ip_address[SC_MAX_STRING_LEN] = {0};
                  INT8 serverport[20] = {0};
                  INT32 remote_port = 0;;

                  if(1 == udp_client_flag)
                  {
                      PrintfResp("\r\nPlease enter the server IP address to send to.\r\n");
                      if(SC_SUCCESS == sAPI_MsgQRecv(simcomUI_msgq,&optionMsg,SC_SUSPEND))
                      {
                          if(SRV_UART != optionMsg.msg_id)
                          {
                              sAPI_Debug("msg_id is error!!\r\n");
                              break;
                          }
                          memset(ip_address, 0, SC_MAX_STRING_LEN);

                          sAPI_Debug("ADDR arg3 = [%s]",optionMsg.arg3);
                          strcpy(ip_address, (char *)optionMsg.arg3);

                          host_entry = sAPI_TcpipGethostbyname(ip_address);
                          if (host_entry == NULL)
                          {
                              PrintfResp("\r\nServer ip addr err\r\n");
                              break;
                          }
                          else
                          {
                              PrintfResp("\r\nPlease enter the server port to send.\r\n");

                              if(SC_SUCCESS == sAPI_MsgQRecv(simcomUI_msgq,&optionMsg,SC_SUSPEND))
                              {
                                  if(SRV_UART != optionMsg.msg_id)
                                  {
                                      sAPI_Debug("msg_id is error!!");
                                      break;
                                  }

                                  sAPI_Debug("PORT arg3 = [%s]",optionMsg.arg3);
                                  memset(serverport, 0, sizeof(serverport));
                                  strcpy(serverport, (char *)optionMsg.arg3);
                                  remote_port =atoi(serverport);
                                  if ((remote_port < SC_REMOTE_PORT_MIN) ||(remote_port > SC_REMOTE_PORT_MAX))
                                  {
                                    PrintfResp("\r\nServer ip port err !\r\n");
                                    break;
                                  }
                                  else
                                  {
                                      PrintfResp("\r\nPlease enter the message you want to send.\r\n");

                                      if(SC_SUCCESS == sAPI_MsgQRecv(simcomUI_msgq,&optionMsg,SC_SUSPEND))
                                      {
                                          if(SRV_UART != optionMsg.msg_id)
                                          {
                                              sAPI_Debug("msg_id is error!!\r\n");
                                              break;
                                          }
                                          memset(sendbuf, 0, 1024);

                                          sAPI_Debug("ADDR arg3 = [%s]",optionMsg.arg3);
                                          strcpy(sendbuf, (char *)optionMsg.arg3);
                                      }
                                  }
                              }
                          }
                      }

                      server.sin_family = SC_AF_INET;
                      server.sin_port = sAPI_TcpipHtons(remote_port);
                      server.sin_addr.s_addr= *(UINT32 *)host_entry->h_addr_list[0];

                      ret = sAPI_TcpipSendto(udp_client_sockfd,sendbuf,strlen(sendbuf), 0, (SCsockAddr*)&server, sizeof(SCsockAddrIn));
                      if(ret < 0)
                      {
                          PrintfResp("\r\nUDP client send data fail !\r\n");
                      }
                      else
                      {
                          PrintfResp("\r\nUDP client send successful ! \r\n");
                      }
                  }
                  else
                  {
                      PrintfResp("You haven't opened the UDP client !");
                  }
              }
              break;

            case SC_TCPIP_DEMO_UDP_CLIENT_CLOSE:
              {
                  if(1 == udp_client_flag)
                  {
                      if(udp_client_sockfd >= 0)
                      {
                          if(0 == sAPI_TcpipClose(udp_client_sockfd))
                          {
                              udp_client_sockfd = -1;
                              udp_client_flag = 0;
                              PrintfResp("\r\nClose udp client success !\r\n");
                          }
                          else
                          {
                              PrintfResp("\r\nClose udp client fail !\r\n");
                          }
                      }
                  }
                  else
                  {
                      PrintfResp("\r\nYou haven't opened the udp client !\r\n");
                  }
              }
              break;


            case SC_TCPIP_DEMO_TCP_SERVER_OPEN:
              {
                  if(tcpip_pdpactive_flag[pdp_id])
                  {
                      if(!tcp_server_flag)
                      {
                          tcp_serverfd = sAPI_TcpipSocket(SC_AF_INET, SC_SOCK_STREAM, 0);
                          if(tcp_serverfd < 0)
                          {
                              sAPI_Debug("create socket err");
                              break;
                          }
#if 1
                          if(SC_SOCKET_ERROR == sAPI_TcpipIoctlsocket(tcp_serverfd, SC_FIONBIO, 1))
                          {
                              sAPI_Debug("\r\nTCP server open fail !\r\n");
                              break;
                          }
#endif
                          PrintfResp("\r\nPlease enter the server port .\r\n");
                          if(SC_SUCCESS == sAPI_MsgQRecv(simcomUI_msgq,&optionMsg,SC_SUSPEND))
                          {
                              if(SRV_UART != optionMsg.msg_id)
                              {
                                  sAPI_Debug("msg_id is error!!");
                                  break;
                              }

                              sAPI_Debug("PORT arg3 = [%s]",optionMsg.arg3);

                              tcpserver_localport =atoi((char *)optionMsg.arg3);
                              if ((tcpserver_localport < SC_REMOTE_PORT_MIN) ||(tcpserver_localport > SC_REMOTE_PORT_MAX))
                              {
                                PrintfResp("\r\n Server ip port err !\r\n");
                                break;
                              }
                          }

                          if(TCPIP_PDP_IPV4 == localip_info.type)
                          {
                              memset(resp, 0, strlen(resp));
                              sprintf(resp,"\r\nPDPCTX type: IPV4,\r\nADDR: %s\r\n", sAPI_TcpipInet_ntoa(localip_info.ip4));
                              PrintfResp(resp);
                          }
                          else if(TCPIP_PDP_IPV6 == localip_info.type)
                          {
                              memset(resp, 0, strlen(resp));
                              char dststr[100] = {0};
                              sAPI_TcpipInet_ntop(SC_AF_INET6, localip_info.ip6, dststr, sizeof(dststr));
                              sprintf(resp,"\r\nPDPCTX type: IPV6,\r\nADDR: [%s]\r\n", dststr);
                              PrintfResp(resp);
                          }
                          else if(TCPIP_PDP_IPV4V6 == localip_info.type)
                          {
                              memset(resp, 0, strlen(resp));
                              char dststr[100] = {0};
                              sAPI_TcpipInet_ntop(SC_AF_INET6, localip_info.ip6, dststr, sizeof(dststr));
                              sprintf(resp,"\r\nPDPCTX type: IPV4V6,\r\nADDR: %s\r\nADDR: [%s]\r\n", sAPI_TcpipInet_ntoa(localip_info.ip4),dststr);
                              PrintfResp(resp);
                          }

                          ret = sAPI_TcpipSetsockopt(tcp_serverfd, SC_SOL_SOCKET, SC_SO_REUSEADDR, &reuseport, sizeof(reuseport));
                          if(ret < 0)
                          {
                              sAPI_Debug("Setsockopt fail errno[%d]",sAPI_TcpipGetErrno());
                          }



                          status = sAPI_TaskCreate(&tcpserverRef,
                                                  simcomUIProcesserStack,
                                                  SC_TCPIP_TEST_STACK_SIZE,
                                                  SC_TCPIP_TEST_TASK_PRO,
                                                  "scTask_tcpip_server",
                                                  sc_tcpserver_task,
                                                  NULL);

                          if (SC_SUCCESS != status)
                          {
                              sAPI_TcpipClose(tcp_serverfd);
                              PrintfResp("\r\nTCP server open fail !\r\n");
                              break;
                          }
                          else
                          {
                              PrintfResp("\r\nTCP server open success !\r\n");
                              tcp_server_flag = 1;
                          }
                      }
                      else
                      {
                          PrintfResp("\r\nYou've already opened the TCP server \r\n");
                      }
                  }
                  else
                  {
                      PrintfResp("You haven't done the Init");
                  }
              }
              break;

            case SC_TCPIP_DEMO_TCP_SERVER_CLOSE:
              {
                  if(1 == tcp_server_flag)
                  {
                      if(tcp_serverfd >= 0)
                      {
                          if((0 == sAPI_TcpipClose(tcp_serverfd)) && (SC_SUCCESS == sAPI_TaskDelete(tcpserverRef)))
                          {
                              tcp_serverfd = -1;
                              tcp_server_flag = 0;
                              PrintfResp("\r\nClose tcp server success !\r\n");
                          }
                          else
                          {
                              PrintfResp("\r\nClose tcp server fail !\r\n");
                          }
                      }
                  }
                  else
                  {
                      PrintfResp("\r\nYou haven't opened the tcp server !\r\n");
                  }

              }
              break;


            case SC_TCPIP_DEMO_DEINIT:
              {
                  if(tcpip_pdpactive_flag[pdp_id])
                  {
                      if (SC_TCPIP_SUCCESS != sAPI_TcpipPdpDeactive(pdp_id,1))
                      {
                          PrintfResp("\r\nTCPIP deinit fail !\r\n");
                          break;
                      }
                      tcpip_pdpactive_flag[pdp_id] = 0;

                      if(1 == tcp_client_flag)
                      {
                          if(tcp_client_sockfd >= 0)
                          {
                              if(0 == sAPI_TcpipClose(tcp_client_sockfd))
                              {
                                  tcp_client_sockfd = -1;
                                  tcp_client_flag = 0;
                                  PrintfResp("\r\nClose tcp client success !\r\n");
                              }
                              else
                              {
                                  PrintfResp("\r\nClose tcp client fail !\r\n");
                              }
                          }
                      }

                      if(1 == udp_client_flag)
                      {
                          if(udp_client_sockfd >= 0)
                          {
                              if(0 == sAPI_TcpipClose(udp_client_sockfd))
                              {
                                  udp_client_sockfd = -1;
                                  udp_client_flag = 0;
                                  PrintfResp("\r\nClose udp client success !\r\n");
                              }
                              else
                              {
                                  PrintfResp("\r\nClose udp client fail !\r\n");
                              }
                          }
                      }

                      if(1 == tcp_server_flag)
                      {
                          if(tcp_serverfd >= 0)
                          {
                              if((0 == sAPI_TcpipClose(tcp_serverfd)) && (SC_SUCCESS == sAPI_TaskDelete(tcpserverRef)))
                              {
                                  tcp_serverfd = -1;
                                  tcp_server_flag = 0;
                                  PrintfResp("\r\nClose tcp server success !\r\n");
                              }
                              else
                              {
                                  PrintfResp("\r\nClose tcp server fail !\r\n");
                              }
                          }
                      }

                      PrintfResp("\r\nTCPIP deinit success !\r\n");
                  }
                  else
                  {
                      PrintfResp("\r\nTCPIP deinit fail! You haven't done the Init\r\n");
                  }
              }
              break;

            case SC_TCPIP_DEMO_MAX:
              {
                  if(tcpip_pdpactive_flag[pdp_id])
                  {
                      if (SC_TCPIP_SUCCESS != sAPI_TcpipPdpDeactive(pdp_id,1))
                      {
                          PrintfResp("\r\nTCPIP deinit fail !\r\n");
                          break;
                      }
                      tcpip_pdpactive_flag[pdp_id] = 0;

                      if(1 == tcp_client_flag)
                      {
                          if(tcp_client_sockfd >= 0)
                          {
                              if(0 == sAPI_TcpipClose(tcp_client_sockfd))
                              {
                                  tcp_client_sockfd = -1;
                                  tcp_client_flag = 0;
                                  PrintfResp("\r\nClose tcp client success !\r\n");
                              }
                              else
                              {
                                  PrintfResp("\r\nClose tcp client fail !\r\n");
                              }
                          }
                      }

                      if(1 == udp_client_flag)
                      {
                          if(udp_client_sockfd >= 0)
                          {
                              if(0 == sAPI_TcpipClose(udp_client_sockfd))
                              {
                                  udp_client_sockfd = -1;
                                  udp_client_flag = 0;
                                  PrintfResp("\r\nClose udp client success !\r\n");
                              }
                              else
                              {
                                  PrintfResp("\r\nClose udp client fail !\r\n");
                              }
                          }
                      }

                      if(1 == tcp_server_flag)
                      {
                          if(tcp_serverfd >= 0)
                          {
                              if((0 == sAPI_TcpipClose(tcp_serverfd)) && (SC_SUCCESS == sAPI_TaskDelete(tcpserverRef)))
                              {
                                  tcp_serverfd = -1;
                                  tcp_server_flag = 0;
                                  PrintfResp("\r\nClose tcp server success !\r\n");
                              }
                              else
                              {
                                  PrintfResp("\r\nClose tcp server fail !\r\n");
                              }
                          }
                      }
                      PrintfResp("\r\nTCPIP deinit success !\r\n");
                  }
                  PrintfResp("\r\n\r\nLeave TCP demo !\r\n");
                  return;
              }

            default :
              {
                  PrintfResp("\r\n\r\n\r\nInput err !\r\n");
                  break;
              }
        }
    }
}




