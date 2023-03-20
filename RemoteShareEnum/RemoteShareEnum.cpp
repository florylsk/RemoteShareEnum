#include <windows.h>

#include <stdio.h>

#include <lm.h>




#pragma comment(lib, "Netapi32.lib")



int wmain(int argc, WCHAR* lpszArgv[])

{

    PSHARE_INFO_502 BufPtr, p;

    NET_API_STATUS res;

    LPTSTR   lpszServer = NULL;

    DWORD er = 0, tr = 0, resume = 0, i;



    switch (argc)

    {

    case 1:

        wprintf(L"Usage : RemoteShareEnum.exe <servername1> <servername2> <servernameX>\n");

        return 1;

    default:
        break;
        

    }


    wprintf(L"\n Share\tPath\tDescription\tCurrent Users\tServer\n\n");

    wprintf(L"-------------------------------------------------------------------------------------\n\n");

    for (DWORD iter = 1; iter <= argc-1; iter++) {
        lpszServer = lpszArgv[iter];
        do

        {

            res = NetShareEnum(lpszServer, 502, (LPBYTE*)&BufPtr, -1, &er, &tr, &resume);


            if (res == ERROR_SUCCESS || res == ERROR_MORE_DATA)

            {

                p = BufPtr;

                for (i = 1; i <= er; i++)

                {

                    wprintf(L" % s\t % s\t % s\t % u\t % s\t\n", p->shi502_netname, p->shi502_path, p->shi502_remark, p->shi502_current_uses, lpszServer);


                    p++;

                }

                NetApiBufferFree(BufPtr);

            }

            else

            {

                wprintf(L"NetShareEnum() failed for server '%s'. Error code: % ld\n",lpszServer, res);

            }

        }


        while (res == ERROR_MORE_DATA);
        wprintf(L"-------------------------------------------------------------------------------------\n\n");
    }

    

    return 0;

}