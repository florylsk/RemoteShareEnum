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

    case 2:

        lpszServer = lpszArgv[1];

        break;

    default:

        wprintf(L"Usage : % s <servername>\n", lpszArgv[0]);

        return 1;

    }


    wprintf(L"Share               Local Path                           Uses    Descriptor\n");

    wprintf(L"------------------------------------------------------------------------------\n");



    do 

    {

        res = NetShareEnum(lpszServer, 502, (LPBYTE*)&BufPtr, -1, &er, &tr, &resume);


        if (res == ERROR_SUCCESS || res == ERROR_MORE_DATA)

        {

            p = BufPtr;



            wprintf(L"NetShareEnum() is OK!\n");



            for (i = 1; i <= er; i++)

            {

                wprintf(L" % -20s % -45s % -8u", p->shi502_netname, p->shi502_path, p->shi502_current_uses);



                if (IsValidSecurityDescriptor(p->shi502_security_descriptor))

                    wprintf(L"Yes\n");

                else

                    wprintf(L"No\n");

                p++;

            }




            NetApiBufferFree(BufPtr);

        }

        else

        {

            wprintf(L"NetShareEnum() failed!Error: % ld\n", res);

        }

    }


    while (res == ERROR_MORE_DATA);

    return 0;

}