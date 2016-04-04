#include "stdafx.h"
#include "MessageServer.h"
#include "VisionStruct.h"
MessageServer::MessageServer()
{
}
MessageServer::~MessageServer()
{
}
void MessageServer::ServerProcess(int revLen)
{
    if (revLen>0)
    {
        //Tmtv_AskInfo msgData;
        //::EnterCriticalSection(&m_section);
        //::LeaveCriticalSection(&m_section);
        Tmtv_AskInfo* pMsgData = (Tmtv_AskInfo*)pBuffer;
        if (pMsgData->CheckCode== TMTV_CHECKCODE)
        {
            switch (pMsgData->Asktype)
            {
            case Tmtv_AskInfo::TMTV_ADDCAM:
                break;
            case Tmtv_AskInfo::TMTV_DELCAM:
                break;
            case Tmtv_AskInfo::TMTV_STARTCAM:
                break;
            case Tmtv_AskInfo::TMTV_STOPCAM:
                break;
            case Tmtv_AskInfo::TMTV_GETCAM:
                break;
            case Tmtv_AskInfo::TMTV_SETCAM:
                break;
            default:
                break;
            }
        }
    }
}
