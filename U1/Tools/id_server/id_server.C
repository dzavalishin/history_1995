/*************************************************************************/
/* FTN_Serv - FTN Messages Identification Server for OS/2                */
/*                                                                       */
/* (C) 1995 by                                                           */
/*             Dmitry Zavalishin, 2:5020/32@FidoNet,  7:1130/32@FidoNet  */
/*             Kirill Lebedev,    2:5020/251@FidoNet, 7:1130/251@FidoNet */
/*             Pete Kvitek,       2:5020/6@FidoNet,   7:1130/6@FidoNet   */
/*             Yuri Safronov,     2:5020/204@FidoNet, 7:1130/204@FidoNet */
/*                                                                       */
/* Written by Kirill Lebedev, 1995, Tushino, Moscow, Russia.             */
/*                                                                       */
/* 30/08/95 - Kirill Lebedev, Version 0.90a.                             */
/*            First version publish into R50.TSC                         */
/*                                                                       */
/* 01/09/95 - Kirill Lebedev, Version 0.91a.                             */
/*            After 0xFFFFFFFF MsgId Server generate 0x00000000          */
/*                                                                       */
/* 19/09/95 - Kirill Lebedev, Version 0.95. /Prerelease/                 */
/*            Creating new counter for new address                       */
/*                                                                       */
/*                                 FREEWARE                              */
/*                               Public Domain                           */
/*************************************************************************/
#define INCL_BASE
#define INCL_DOSSEMAPHORES
#define INCL_DOSNMPIPES
#define INCL_DOSFILEMGR
#define INCL_DOSERRORS
#define INCL_DOSPROCESS
#define INCL_DOSEXCEPTIONS
#include <os2.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <io.h>
#include <fcntl.h>
#include <share.h>
#include <sys\stat.h>

#define FTN_SERV_VER_H 0
#define FTN_SERV_VER_L 95
#define FTN_SERV_VER_C '.'

#define INCREMENT 10
#define THREADS 10

#define dword unsigned int

#ifdef __BORLANDC__
#define _System _cdecl
#endif

ULONG _System ShutDownHandler( PEXCEPTIONREPORTRECORD       p1,
                               PEXCEPTIONREGISTRATIONRECORD p2,
                               PCONTEXTRECORD               p3,
                               PVOID                        pv );

APIRET CreateAddress(char *message);
APIRET FirstReadBase(void);
APIRET WriteBase(dword i);
APIRET EndWriteBase(void);

UCHAR *uchFileName=(UCHAR *)NULL;

typedef struct
{
  ULONG Offset;
  char *Address;
  dword MsgId;
  dword MsgIdInc;
} TypeMsgIdRecord;
TypeMsgIdRecord *MsgIdRecord;

#ifdef __IBMC__
void     _System NewThread(ULONG threadArg);
#else
void     NewThread(ULONG threadArg);
#endif
TID      threadID[512];
HPIPE    PipeHandle[255];
ULONG    threadFlags    = 0;
ULONG    stack_size     = 0;
USHORT   loopsize       = THREADS;
HFILE    hfFileHandle   = 0L;
ULONG    ulAction       = 0;
dword    AddressCounter = 0;
FILE    *LogFile        = (FILE *)NULL;
HMTX     hmtxMsgId      = 0;

int main(int argc, char *argv[])
{
  USHORT i=0;
  APIRET rc = NO_ERROR;
  dword  Version        = 0;
  ULONG    ulBytes       = 0;                
  PFNTHREAD pfnNewThread=&NewThread;
  EXCEPTIONREGISTRATIONRECORD RegRec  = {0};

 
if((LogFile=fdopen(sopen("FTN_Serv.Log",O_CREAT|O_APPEND|O_WRONLY,SH_DENYWR,S_IREAD|S_IWRITE),"at"))==NULL)
  {
    DosBeep(600,600);
    LogFile=fopen("NUL","w");
  }

  RegRec.ExceptionHandler = (ERR)ShutDownHandler;
  rc = DosSetExceptionHandler( &RegRec );
  if (rc != NO_ERROR)
  {
    fprintf(LogFile,"DosSetExceptionHandler error: return code = %u\n",rc);
    return 1;
  }

  if(argc>=2)
  {
    uchFileName=(UCHAR *)malloc(strlen(argv[1]));
    strcpy(uchFileName,argv[1]);
  }
  else
  {
    uchFileName=(UCHAR *)malloc(13);
    strcpy(uchFileName,"FTNMsgId.Dat");
  }
  rc = DosOpen(uchFileName,
               &hfFileHandle,
               &ulAction,
               sizeof(dword),
               FILE_ARCHIVED | FILE_NORMAL,
               OPEN_ACTION_CREATE_IF_NEW |
               OPEN_ACTION_OPEN_IF_EXISTS,
               OPEN_FLAGS_NOINHERIT |
               OPEN_FLAGS_NO_CACHE  |
               OPEN_SHARE_DENYWRITE |
               OPEN_ACCESS_READWRITE,     
               0L);                       
 
  if (rc != NO_ERROR)
  {
    fprintf(LogFile,"DosOpen error: return code = %u\n", rc);
    fclose(LogFile);
    return(1);
  }
  if(ulAction==2)
  {
    Version=FTN_SERV_VER_H*100+FTN_SERV_VER_L;
    rc = DosWrite (hfFileHandle,
                  &Version,   
                  sizeof(dword),
                  &ulBytes);
    if (rc != NO_ERROR)
    {
      fprintf(LogFile,"DosWrite error: return code = %u\n", rc);
      return 1;
    }
    AddressCounter=0;
    rc = DosWrite (hfFileHandle,
                  &AddressCounter,   
                  sizeof(dword),
                  &ulBytes);
    if (rc != NO_ERROR)
    {
      fprintf(LogFile,"DosRead error: return code = %u\n", rc);
      return 1;
    }
  }

  rc = DosCreateMutexSem("\\sem32\\MsgId",&hmtxMsgId,0,FALSE);
  if (rc != NO_ERROR)
  {
    fprintf(LogFile,"DosCreateMutexSem error: return code = %u\n", rc);
    fclose(LogFile);
    return(1);
  }
  rc = FirstReadBase();

  if (rc != NO_ERROR)
  {
    fprintf(LogFile,"FirstReadBase error: return code = %u\n", rc);
    fclose(LogFile);
    return(1);
  }

  threadFlags = 0;
  stack_size  = 1024;
  if(argc>=3)
  {
    loopsize=(USHORT) atoi(argv[2]);
  }
  for(i=1;i<loopsize+1;i++)
  {
    rc = DosCreateThread(&threadID[i], pfnNewThread, i,
                         threadFlags, stack_size);
    if (rc != 0)
    {
      fprintf(LogFile,"DosCreateThread error = %d\n",rc);
      fclose(LogFile);
      return(1);
    }
  }
  for(;;)
  {
    /* wait ShutDown request */
    DosSleep(60000);
  };
  rc = DosUnsetExceptionHandler( &RegRec );
  if (rc != NO_ERROR) 
  {
    fprintf(LogFile,"DosUnsetExceptionHandler error: return code = %u\n",rc);
    fclose(LogFile);
    return 1;
  }
  return NO_ERROR;
}

void NewThread(ULONG threadArg)
{
  CHAR     PipeName[256] = "\\PIPE\\FTNMSGID";
  ULONG    ulBytes       = 0;                
  CHAR     message[256]  = "" ;              
  APIRET   rc            = NO_ERROR;         
  APIRET   rcDosWrite    = NO_ERROR;        
  HMTX     hmtxMsgId     = 0;
  dword    i             = 0;
  unsigned found         = 0;

  rc = DosOpenMutexSem("\\sem32\\MsgId",&hmtxMsgId);
  if (rc != NO_ERROR)
  {
    fprintf(LogFile,"DosOpenMutexSem error: return code = %u\n",rc);
  }
  rc = DosCreateNPipe(PipeName,              
                      &PipeHandle[threadArg],
                      NP_ACCESS_DUPLEX,
                      NP_WAIT |
                      NP_TYPE_MESSAGE |
                      NP_READMODE_MESSAGE |
                      loopsize,
                      sizeof(message),
                      sizeof(message),
                      200L);          
  if (rc != NO_ERROR)
  {
    fprintf(LogFile,"DosCreateNPipe error: return code = %u\n",rc);
    return;
  }

  do
  { 
    rc = DosConnectNPipe(PipeHandle[threadArg]);
    if (rc != NO_ERROR)
    {
      fprintf(LogFile,"DosConnectNPipe error: return code = %u\n",rc);
      return;
    }
    rcDosWrite=NO_ERROR;
    while( (rcDosWrite==NO_ERROR)&&
           ((rc = DosRead(PipeHandle[threadArg],
                          message,
                          sizeof(message),
                          &ulBytes))==NO_ERROR)&&
           (ulBytes>0))

    {
      found=0;
      rc = DosRequestMutexSem(hmtxMsgId,SEM_INDEFINITE_WAIT);
      if (rc != NO_ERROR)
      {
        fprintf(LogFile,"DosRequestMutexSem error: return code = %u\n",rc);
      }
      rc = NO_ERROR;
      do
      {
        for(i=0;i!=AddressCounter;i++)
        {
          if(!strcmp(message,MsgIdRecord[i].Address))
          {
            found=1;
            break;
          }
        }
        if(found==0)
        {
          rc=CreateAddress(message);
        }
      } while((found!=1)&&(rc==NO_ERROR));
      if(rc == NO_ERROR)
      {
        sprintf(message,"%08X",MsgIdRecord[i].MsgId);
        if(MsgIdRecord[i].MsgId!=0xFFFFFFFF)
        {
          MsgIdRecord[i].MsgId++;
        }
        else
        {
          MsgIdRecord[i].MsgId=0x00000001;
        }
        if(MsgIdRecord[i].MsgIdInc==MsgIdRecord[i].MsgId)
        {
          rc=WriteBase(i);
          if (rc != NO_ERROR)
          {
            fprintf(LogFile,"WriteBase error: return code = %u\n",rc);
          }
        } 
      }
      else
      {
        strcpy(message,"!ERROR!!");
      }
      rc = DosReleaseMutexSem(hmtxMsgId);
      if (rc != NO_ERROR)
      {
        fprintf(LogFile,"DosReleaseMutexSem error: return code = %u\n",rc);
      }
      ulBytes=0;
      rcDosWrite = DosWrite(PipeHandle[threadArg],
                    message,
                    strlen(message)+1,
                    &ulBytes);
      if (rcDosWrite != NO_ERROR)
      {
        fprintf(LogFile,"DosWrite error: return code = %u\n",rcDosWrite);
      } 
    };
    DosSleep(10L);
    rc = DosDisConnectNPipe(PipeHandle[threadArg]);
    if (rc != NO_ERROR)
    {
      fprintf(LogFile,"DosDisConnectNPipe error: return code = %u\n",rc);
    }
  } while(rc==0); 
  rc = DosCloseMutexSem(hmtxMsgId);
  if (rc != NO_ERROR)
  {
    fprintf(LogFile,"DosCloseMutexSem error: return code = %u\n",rc);
  }
  rc = DosClose(PipeHandle[threadArg]);
  if (rc != NO_ERROR)
  {
    fprintf(LogFile,"DosClose error: return code = %u\n",rc);
  }
  return;
}

APIRET CreateAddress(char *message)
{
  ULONG  ulBytes        = 0;
  ULONG  ulLocal        = 0;
  APIRET rc             = NO_ERROR;
  dword  AddrLen        = 0;
  dword  MsgId          = 0;
  dword  i              = 0;

  rc = EndWriteBase();
  if (rc != NO_ERROR)
  {
    fprintf(LogFile,"DosSetFilePtr error: return code = %u\n", rc);
    return 1;
  }
  rc = DosSetFilePtr (hfFileHandle,
                      0L,          
                      FILE_END,  
                      &ulLocal);
  if (rc != NO_ERROR)
  {
    fprintf(LogFile,"DosSetFilePtr error: return code = %u\n", rc);
    return 1;
  }
  AddrLen=strlen(message)+1;
  rc = DosWrite (hfFileHandle,
                &AddrLen,   
                sizeof(dword),
                &ulBytes);
  if (rc != NO_ERROR)
  {
    fprintf(LogFile,"DosWrite error: return code = %u\n", rc);
    return 1;
  }
  rc = DosWrite(hfFileHandle,
                message,   
                AddrLen,
                &ulBytes);
  if (rc != NO_ERROR)
  {
    fprintf(LogFile,"DosWrite error: return code = %u\n", rc);
    return 1;
  }
  ulLocal=0L;
  MsgId=0x00000000;
  rc = DosWrite(hfFileHandle,
                &MsgId,   
                sizeof(dword),
                &ulBytes);
  if (rc != NO_ERROR)
  {
    fprintf(LogFile,"DosWrite error: return code = %u\n", rc);
    return 1;
  }
  rc = DosSetFilePtr (hfFileHandle,
                      sizeof(dword),          
                      FILE_BEGIN,  
                      &ulLocal);
  if (rc != NO_ERROR)
  {
    fprintf(LogFile,"DosSetFilePtr error: return code = %u\n", rc);
    return 1;
  }
  AddressCounter++;
  rc = DosWrite(hfFileHandle,
                &AddressCounter,   
                sizeof(dword),
                &ulBytes);
  if (rc != NO_ERROR)
  {
    fprintf(LogFile,"DosWrite error: return code = %u\n", rc);
    return 1;
  }
  for(i=0;i!=AddressCounter-1;i++)
  {
    free(MsgIdRecord[i].Address);
  }
  if(MsgIdRecord!=NULL)
  {
    free(MsgIdRecord);
  }
  rc = FirstReadBase();
  return(rc);
}

APIRET FirstReadBase(void)
{
  ULONG  ulBytesRead    = 0;
  ULONG  ulWrote        = 0;
  ULONG  ulLocal        = 0;
  APIRET rc             = NO_ERROR;
  dword  AddrLen        = 0;
  dword  i              = 0;
  dword  Version        = 0;

  rc = DosSetFilePtr (hfFileHandle,
                      0L,          
                      FILE_BEGIN,  
                      &ulLocal);

  rc = DosRead (hfFileHandle,
                &Version,   
                sizeof(dword),
                &ulBytesRead);
  if (rc != NO_ERROR)
  {
    fprintf(LogFile,"DosRead error: return code = %u\n", rc);
    return 1;
  }

  if(Version<FTN_SERV_VER_H*100+FTN_SERV_VER_L)
  {
    fprintf(LogFile,"Old version base...\n");
    return 1;
  }
  else if(Version>FTN_SERV_VER_H*100+FTN_SERV_VER_L)
  {
    fprintf(LogFile,"Old *.EXE file or damaged base...\n");
    return 1;
  }
  AddressCounter=0;
  rc = DosRead (hfFileHandle,
                &AddressCounter,   
                sizeof(dword),
                &ulBytesRead);
  if (rc != NO_ERROR)
  {
    fprintf(LogFile,"DosRead error: return code = %u\n", rc);
    return 1;
  }

  MsgIdRecord=(TypeMsgIdRecord *)malloc(sizeof(TypeMsgIdRecord)*AddressCounter);

  for(i=0;i!=AddressCounter;i++)
  {
    rc = DosRead (hfFileHandle,
                  &AddrLen,   
                  sizeof(dword),
                  &ulBytesRead);
    if (rc != NO_ERROR)
    {
      fprintf(LogFile,"DosRead error: return code = %u\n", rc);
      return 1;
    }
    MsgIdRecord[i].Address=(char *)malloc(sizeof(char)*AddrLen);
    rc = DosRead (hfFileHandle,
                  MsgIdRecord[i].Address,   
                  AddrLen,
                  &ulBytesRead);
    if (rc != NO_ERROR)
    {
      fprintf(LogFile,"DosRead error: return code = %u\n", rc);
      return 1;
    }
    ulLocal=0L;
    rc = DosSetFilePtr (hfFileHandle,
                        0L,          
                        FILE_CURRENT,  
                        &ulLocal);
    if (rc != NO_ERROR)
    {
      fprintf(LogFile,"DosSetFilePtr error: return code = %u\n", rc);
      return 1;
    }
    MsgIdRecord[i].Offset=ulLocal;


    rc = DosRead (hfFileHandle,
                  &MsgIdRecord[i].MsgId,   
                  sizeof(dword),
                  &ulBytesRead);
    if (rc != NO_ERROR)
    {
      fprintf(LogFile,"DosRead error: return code = %u\n", rc);
      return 1;
    }
  }
  for(i=0;i!=AddressCounter;i++)
  {
    rc = DosSetFilePtr (hfFileHandle,
                        MsgIdRecord[i].Offset,          
                        FILE_BEGIN,  
                        &ulLocal);
    if (rc != NO_ERROR)
    {
      fprintf(LogFile,"DosSetFilePtr error: return code = %u\n", rc);
      return 1;
    }
    if(MsgIdRecord[i].MsgId<=0xFFFFFFFF-INCREMENT)
    {
      MsgIdRecord[i].MsgIdInc=MsgIdRecord[i].MsgId+INCREMENT;
    }
    else
    {
      MsgIdRecord[i].MsgIdInc=INCREMENT-(0xFFFFFFFF-MsgIdRecord[i].MsgId);
    }
    rc = DosWrite (hfFileHandle,
                   (PVOID) &MsgIdRecord[i].MsgIdInc,
                   sizeof(dword),
                   &ulWrote);
    if (rc != NO_ERROR)
    {
      fprintf(LogFile,"DosWrite error: return code = %u\n", rc);
      return 1;
    }
  }  
  return NO_ERROR;
}

APIRET EndWriteBase(void)
{
  ULONG  ulWrote        = 0;
  ULONG  ulLocal        = 0;
  APIRET rc             = NO_ERROR;
  dword  i              = 0;

  for(i=0;i!=AddressCounter;i++)
  {
    rc = DosSetFilePtr (hfFileHandle,
                        MsgIdRecord[i].Offset,          
                        FILE_BEGIN,  
                        &ulLocal);
    if (rc != NO_ERROR)
    {
      fprintf(LogFile,"DosSetFilePtr error: return code = %u\n", rc);
      return 1;
    }
    rc = DosWrite (hfFileHandle,
                   (PVOID) &MsgIdRecord[i].MsgId,
                   sizeof(dword),
                   &ulWrote);
    if (rc != NO_ERROR)
    {
      fprintf(LogFile,"DosWrite error: return code = %u\n", rc);
      return 1;
    }
    free((void *)MsgIdRecord[i].Address);
  }
  if(MsgIdRecord!=NULL)
  {
    free(MsgIdRecord);
  }
  return NO_ERROR;
}

APIRET WriteBase(dword i)
{
  ULONG  ulWrote        = 0;
  ULONG  ulLocal        = 0;
  APIRET rc             = NO_ERROR;

  rc = DosSetFilePtr (hfFileHandle,
                      MsgIdRecord[i].Offset,          
                      FILE_BEGIN,  
                      &ulLocal);
  if (rc != NO_ERROR)
  {
    fprintf(LogFile,"DosSetFilePtr error: return code = %u\n", rc);
    return 1;
  }
  if(MsgIdRecord[i].MsgId<=0xFFFFFFFF-INCREMENT)
  {
    MsgIdRecord[i].MsgIdInc=MsgIdRecord[i].MsgId+INCREMENT;
  }
  else
  {
    MsgIdRecord[i].MsgIdInc=INCREMENT-(0xFFFFFFFF-MsgIdRecord[i].MsgId);
  }
  rc = DosWrite (hfFileHandle,
                 (PVOID) &MsgIdRecord[i].MsgIdInc,
                 sizeof(dword),
                  &ulWrote);
  if (rc != NO_ERROR)
  {
    fprintf(LogFile,"DosWrite error: return code = %u\n", rc);
    return 1;
  }
  return NO_ERROR;
}

ULONG _System ShutDownHandler( PEXCEPTIONREPORTRECORD       p1,
                               PEXCEPTIONREGISTRATIONRECORD p2,
                               PCONTEXTRECORD               p3,
                               PVOID                        pv )
{
  APIRET   rc            = NO_ERROR;         

  switch( p1->ExceptionNum)
  {
    case XCPT_PROCESS_TERMINATE:
    case XCPT_ASYNC_PROCESS_TERMINATE:
    {
      rc = EndWriteBase();
      if (rc != NO_ERROR)
      {
        fprintf(LogFile,"EndWriteBase error: return code = %u\n", rc);
        fclose(LogFile);
        return 1;
      }
      rc = DosClose(hfFileHandle);
      if (rc != NO_ERROR)
      {
        fprintf(LogFile,"DosClose error: return code = %u\n", rc);
        fclose(LogFile);
        return 1;
      }
      rc = DosCloseMutexSem(hmtxMsgId);
      if (rc != NO_ERROR)
      {
        fprintf(LogFile,"DosCloseMutexSem error: return code = %u\n", rc);
        fclose(LogFile);
        return 1;
      }
      free(uchFileName);
      fclose(LogFile);
      break;
    }
    default:;
  }
  return XCPT_CONTINUE_SEARCH;
}
