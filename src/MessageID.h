#ifndef _MESSAGEID_H_
#define _MESSAGEID_H_


enum MessageID
{
    MinValue = 99,
    
    GM_CONNECT_SC       = 100,
    GM_CONNECT_BC       = 101,
    GM_READY_CS         = 102,
    GM_READY_BC         = 103,
    GM_FRAME_CS         = 104,
    GM_FRAME_BC         = 105,
    GM_POSITION_BC      = 106,
    GM_BEGIN_BC         = 107,
    GM_GET_FRAME_CS     = 108,
    GM_GET_FRAME_SC     = 109,

    MaxValue,
};

#endif

