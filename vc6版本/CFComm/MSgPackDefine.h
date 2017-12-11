
#ifndef MSGPACKDEFINE_H
#define MSGPACKDEFINE_H


const int MSG_DN_SIZE					= 35;	// Standardize DN Size //Bernard Tan
const int MSG_BODY_LENGTH				= 200;	//length of the Message body buf 
const int MSG_CDN_SIZE					= 8;	//CDN Buff length
const int MSG_GROUPID_SIZE				= 11;	//GroupID Buff length
const int MSG_CALLIDPK_SIZE				= 25;	//CallIDPK buff length
const int MSG_DNIS_SIZE					= 11;	//DNIS Buff length
const int MSG_AGENTID_SIZE              =  5;	//agentID buff length
const int MSG_ACDDN_SIZE				=  8;	//ACDDN buff length
const int MSG_TN_SIZE					=  3;	//All TN Buf Length


typedef struct 
{
	char cIdentifier;		//message sign
	int nMsgLen;		//message length(byte)
	int nMsgType;		//message type£¬subclass of the Merdian message£¬Please reference to MLMessageType.h
	int nDMType;		//message dest module type£ºACD¡¢Broker¡¢IVR etc¡£
	int nDMID;		//message dest module ID¡£
	int nSMType;		// module type of source module
	int nSMID;		//module id of source
	int nSecDMType ;	//module type of second module, which the message will be transfered to
	int nSecDModularID ;	//module ID of second module, which the message will be transfered to
	long lCardKey;
	char szMessageBody[ MSG_BODY_LENGTH];	// 600 bytes
}CFC_MESSAGE_PACK;

//Module type const define
const int DMODULARTYPE_CTI			= 1;//CTI DModularType
const int DMODULARTYPE_ACD			= 2;//ACD DModularType
const int DMODULARTYPE_IVR			= 3;//IVR DModularType
const int DMODULARTYPE_BROKER		= 4;//Broker DModularType
const int DMODULARTYPE_AGENT		= 5;//Agent DModularType
const int DMODULARTYPE_SUPERVISOR	= 6;//Supervisor  DModularType
const int DMODULARTYPE_DBROKER		= 7;//DbBroker DModularType


typedef struct 
{
	int nModularType;//message dest module type£ºACD¡¢Broker¡¢IVR etc¡£
	int nModularID;//message dest module ID, for example :DN

}CFC_NET_ABEND_TYPE;



/*****************************************************************************/
#endif



