#include <windows.h>

#include <stdio.h>

#include <lm.h>




#pragma comment(lib, "Netapi32.lib")



int wmain(DWORD argc, WCHAR* lpszArgv[])

{

    PSHARE_INFO_502 BufPtr, p;

    PSHARE_INFO_1 BufPtr2, p2;

    NET_API_STATUS res;

    LPTSTR   lpszServer = NULL;

    DWORD er = 0, tr = 0, resume = 0, i,denied=0;



    switch (argc)

    {

    case 1:

        wprintf(L"Usage : RemoteShareEnum.exe <servername1> <servername2> <servernameX>\n");

        return 1;

    default:
        break;
        

    }


    wprintf(L"\n Share\tPath\tDescription\tCurrent Users\tHost\n\n");

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
            else if (res == ERROR_ACCESS_DENIED) {
                denied = 1;
            }

            else

            {

                wprintf(L"NetShareEnum() failed for server '%s'. Error code: % ld\n",lpszServer, res);

            }

        }

        while (res == ERROR_MORE_DATA);

        if (denied == 1) {
            do

            {

                res = NetShareEnum(lpszServer, 1, (LPBYTE*)&BufPtr2, -1, &er, &tr, &resume);


                if (res == ERROR_SUCCESS || res == ERROR_MORE_DATA)

                {

                    p2 = BufPtr2;

                    for (i = 1; i <= er; i++)

                    {

                        wprintf(L" % s\t % s\t % s\t\n", p2->shi1_netname, p2->shi1_remark,  lpszServer);


                        p2++;

                    }

                    NetApiBufferFree(BufPtr2);

                }

                else

                {

                    wprintf(L"NetShareEnum() failed for server '%s'. Error code: % ld\n", lpszServer, res);

                }

            }

            while (res == ERROR_MORE_DATA);
            denied = 0;
        }

        wprintf(L"-------------------------------------------------------------------------------------\n\n");
    }

    

    return 0;

}