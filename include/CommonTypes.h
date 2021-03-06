#ifndef COMMTYPE
#define COMMTYPE

#include <time.h>
#include <string.h>
#include <string>
#include <fstream>
#include <iostream>  


#include <list>
#include <map>
#include <vector>
#include <sys/time.h>
#include <unordered_map>
#include <iostream>
#include <string>
#include <stdio.h>
#include <vector>

#include <boost/mpi.hpp>
#include "repast_hpc/AgentId.h"
#include "repast_hpc/RepastProcess.h"
#include "repast_hpc/Utilities.h"
#include "repast_hpc/Properties.h"
#include "repast_hpc/initialize_random.h"
#include "repast_hpc/SVDataSetBuilder.h"
#include "repast_hpc/Point.h"

#include "repast_hpc/Schedule.h"
#include "repast_hpc/SharedContext.h"
#include "repast_hpc/AgentRequest.h"
#include "repast_hpc/TDataSource.h"
#include "repast_hpc/SVDataSet.h"
#include "repast_hpc/SharedDiscreteSpace.h"
#include "repast_hpc/GridComponents.h"

#include "json/json.h"



#define MAX_MSG_LEN 256
#define AGENT_MSGQUEUE_LEN 100
#define MAX_GROUP_CNT 500


using namespace std;


struct _MessageHead
{
    repast::AgentId receiverID;
    repast::AgentId senderID;
    int msgType;
    int bodyLength;
} ;

typedef struct _MessageInfo
{
    _MessageHead msgHead;
    unsigned char information[MAX_MSG_LEN - sizeof(_MessageHead)];
} MessageInfo;


typedef struct _Order {
    int orderNumber;
    repast::AgentId agentID;    //Sender's ID
    bool direction;             //Buy or Sell
    int productID;              //ProductID of a financial product
    float price;
    int count;
    int orderType;         //0-limit order with storage 1-limit order with send back
    int rank;
    timeval timeStamp;
} Order;

/*
typedef struct _OrderConfirm {
    int orderNumber;
    repast::AgentId agentID;    
    bool direction;             //Buy or Sell
    int productID;              //ProductID of a financial product
    float price;
    int restCount;
    int operation;              //0-store in orderbook, 1-send back
    timeval timeStamp;
} OrderConfirm;
*/

typedef struct _Trade {
    int resultType;             //0- Trade   1- OrderConfirm
    int orderNumber;
    repast::AgentId agentID;
    repast::AgentId counterPartyID;
    int productID;
    bool direction;             //Buy or Sell
    float price;           
    int count;                  //The count of trade or the rest of CrderConfirm
    int operation;              //0-store in orderbook, 1-send back 
    int marketRank;
    timeval timeStamp;
} TradeResult;


typedef struct _investorInfo {
    int investorID;
    int rank;       
    int stockID;
    int holding;
}InvestorInfo;

typedef struct _MarketInfo {
    int stockID;
    float lastPrice;
    float high;
    float low;
    timeval lastTradeTime;
    long volume;
    double turnover;
    int updateTimes = 0;
} MarketInfo;

typedef struct _OrderList {
    float price;
    list<Order> orders;
} OrderList;

typedef struct _OrderBook {
    int stockID;
    list<OrderList> BuyList;
    list<OrderList> SellList;
} OrderBook;

typedef struct _Holding {
    int productID;
    int count;
    int freezeCount;
    int holdingType;        //0-normal 
    timeval lastUpdateTime;
} Holding;

typedef struct _Contract {
    int resultType;             //1- Trade   2- OrderConfirm
    int orderNumber;
    repast::AgentId agentID;
    repast::AgentId counterPartyID;
    int productID;
    bool direction;             //Buy or Sell
    float price;           
    int count;                  //The count of trade or the rest of CrderConfirm
    int operation;              //0-store in orderbook, 1-send back 
    int marketRank;
    timeval timeStamp;          //Contract Built Date
} Contract;


typedef struct _CommInfo
{
    int groupNum;
    int pubRank;
    MPI_Comm comm;
    MPI_Request request;
} CommInfo;

typedef struct _MsgRoundBuf
{
    int head = 0;
    int tail = 0;
    MessageInfo info[AGENT_MSGQUEUE_LEN];
    int pushInfo(MessageInfo *info) 
    {
        int point = (head + 1) % AGENT_MSGQUEUE_LEN;
        if (point != tail)
        {
            info[point] = *info;
            head = point;  
            return 1;
        }
        else return 0;
    };

} MsgRoundBuf ;

#endif