#define INCL_DOSFILEMGR       /* DOS File Manager values */
#define INCL_DOSNMPIPES       /* DOS Named Pipes values */
#define INCL_DOSSEMAPHORES    /* DOS Semaphore values */
#define INCL_DOSERRORS        /* DOS Error values */
#include <os2.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(VOID)
{
  APIRET   rc                     = NO_ERROR;   /* Return code */
  CHAR     message[256]           = "";         /* Message buffer */

/*  CHAR     outmsg[256]            = "";         Output message buffer */
/*  CHAR     inmsg[256]             = "";         Input message buffer */
  HFILE    PipeHandle             = NULLHANDLE; /* Pipe handle */
  PIPEINFO PipeBuffer[4]          = {{0}};
  struct   _AVAILDATA  BytesAvail = {0};
  UCHAR    Buffer[200]            = {0};
  ULONG    bytes                  = 0;
  ULONG    Action                 = 0;
  PIPESEMSTATE infobuf[3]         = {{0}};

  ULONG    PipeState              = 0;
  ULONG    HandType               = 0;
  ULONG    FlagWord               = 0;
  ULONG    BytesRead              = 0;

  rc = DosOpen("\\PIPE\\FTNMSGID", &PipeHandle, &Action, 0, 0, FILE_OPEN,
               OPEN_ACCESS_READWRITE | OPEN_SHARE_DENYREADWRITE |
               OPEN_FLAGS_FAIL_ON_ERROR, NULL);
  if (rc == ERROR_PIPE_BUSY)
    if (DosWaitNPipe("\\PIPE\\FTNMSGID", -1))
    {
      rc = DosOpen("\\PIPE\\FTNMSGID", &PipeHandle, &Action, 0, 0, FILE_OPEN,
                   OPEN_ACCESS_READWRITE | OPEN_SHARE_DENYREADWRITE |
                   OPEN_FLAGS_FAIL_ON_ERROR, NULL);
    }

  if (rc != NO_ERROR)
  {
    printf("DosOpen error: error code = %u\n", rc);
    return 1;
  }
  else
    printf("Connected to HOST\n");

  rc = DosSetNPHState(PipeHandle, NP_WAIT);
  if (rc != NO_ERROR)
  {
    printf("DosSetNPHState error: error code = %u\n", rc);
    return 1;
  }

  printf("Enter message to send to PIPEHOST: ");

  fflush(NULL);  /* Cause above printf to show on display */
  gets(message);

  rc = DosWrite(PipeHandle, message, strlen(message), &bytes);
  if (rc != NO_ERROR)
  {
    printf("DosWrite error: error code = %u\n", rc);
    return 1;
  }

  rc = DosRead(PipeHandle, message, sizeof(message), &bytes);
  if (rc != NO_ERROR)
  {
    printf("DosRead error: error code = %u\n", rc);
    return 1;
  }

  printf("\nMessage received from PIPEHOST was: %s\n\n", message);
  printf("Enter message to send to PIPEHOST: ");

  fflush(NULL);  /* Cause above printf to show on display */
  gets(message);

  rc = DosWrite(PipeHandle, message, strlen(message), &bytes);
  if (rc != NO_ERROR)
  {
    printf("DosWrite error: error code = %u\n", rc);
    return 1;
  }

  rc = DosRead(PipeHandle, message, sizeof(message), &bytes);
  if (rc != NO_ERROR)
  {
    printf("DosRead error: error code = %u\n", rc);
    return 1;
  }

  printf("\nMessage received from PIPEHOST was: %s\n\n", message);
  rc = DosClose(PipeHandle);
  printf("...Disconnected\n");
  return NO_ERROR;
}
/*

   ***** OR ******

  rc = DosCallNPipe("\\PIPE\\FTNMSGID",
                    outmsg,
                    strlen(outmsg),
                    inmsg,
                    sizeof(inmsg),
                    &bytes,
                    30000L);
  if (rc != NO_ERROR)
  {
    printf("DosCallNPipe error: error code = %u\n", rc);
    return 1;
  }
  else
  {
    printf("\nMessage received from PIPEHOST: %s\n\n", inmsg);
  }

  return NO_ERROR;
}

*/
