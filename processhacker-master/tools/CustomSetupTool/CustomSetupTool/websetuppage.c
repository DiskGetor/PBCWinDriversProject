/*
 * Process Hacker Toolchain -
 *   project setup
 *
 * Copyright (C) 2017 dmex
 *
 * This file is part of Process Hacker.
 *
 * Process Hacker is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Process Hacker is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Process Hacker.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <setup.h>

NTSTATUS SetupDownloadWebSetupThread(
    _In_ PPH_SETUP_CONTEXT Context
    )
{
    ULONGLONG currentVersion = 0;
    ULONGLONG latestVersion = 0;
    PPH_STRING setupFileName;
    PH_IMAGE_VERSION_INFO versionInfo;

    if (!SetupQueryUpdateData(Context))
        goto CleanupExit;

    setupFileName = PhGetApplicationFileName();

    if (!PhInitializeImageVersionInfo(&versionInfo, PhGetString(setupFileName)))
        goto CleanupExit;

    currentVersion = ParseVersionString(versionInfo.FileVersion);

#ifdef FORCE_UPDATE_CHECK
    latestVersion = MAKE_VERSION_ULONGLONG(
        9999,
        9999,
        9999,
        0
        );
#else
    latestVersion = ParseVersionString(Context->WebSetupFileVersion);
#endif

    // Compare the current version against the latest available version
    if (currentVersion < latestVersion)
    {
        if (!UpdateDownloadUpdateData(Context))
            goto CleanupExit;
    }

    PostMessage(Context->DialogHandle, PSM_SETCURSELID, 0, IDD_DIALOG5);
    return STATUS_SUCCESS;

CleanupExit:

    PostMessage(Context->DialogHandle, PSM_SETCURSELID, 0, IDD_ERROR);
    return STATUS_FAIL_CHECK;
}

INT_PTR CALLBACK SetupWebSetup_WndProc(
    _In_ HWND hwndDlg,
    _In_ UINT uMsg,
    _Inout_ WPARAM wParam,
    _Inout_ LPARAM lParam
    )
{
    PPH_SETUP_CONTEXT context = NULL;

    if (uMsg == WM_INITDIALOG)
    {
        context = PhGetWindowContext(GetParent(hwndDlg), ULONG_MAX);
        PhSetWindowContext(hwndDlg, PH_WINDOW_CONTEXT_DEFAULT, context);
    }
    else
    {
        context = PhGetWindowContext(hwndDlg, PH_WINDOW_CONTEXT_DEFAULT);
    }

    if (context == NULL)
        return FALSE;

    switch (uMsg)
    {
    case WM_INITDIALOG:
        {
            SetupLoadImage(GetDlgItem(hwndDlg, IDC_PROJECT_ICON), MAKEINTRESOURCE(IDB_PNG1));
            SetupInitializeFont(GetDlgItem(hwndDlg, IDC_MAINHEADER), -17, FW_SEMIBOLD);
            //SetupInitializeFont(GetDlgItem(hwndDlg, IDC_SUBHEADER), -12, FW_NORMAL);
            SetupInitializeFont(GetDlgItem(hwndDlg, IDC_INSTALL_STATUS), -12, FW_SEMIBOLD);
            SetupInitializeFont(GetDlgItem(hwndDlg, IDC_INSTALL_SUBSTATUS), -12, FW_NORMAL);

            EnableThemeDialogTexture(hwndDlg, ETDT_ENABLETAB);
        }
        break;
    case WM_NOTIFY:
        {
            LPNMHDR header = (LPNMHDR)lParam;
            LPPSHNOTIFY pageNotify = (LPPSHNOTIFY)header;

            switch (pageNotify->hdr.code)
            {
            case PSN_SETACTIVE:
                {
                    context->MainHeaderHandle = GetDlgItem(hwndDlg, IDC_MAINHEADER);
                    context->StatusHandle = GetDlgItem(hwndDlg, IDC_INSTALL_STATUS);
                    context->SubStatusHandle = GetDlgItem(hwndDlg, IDC_INSTALL_SUBSTATUS);
                    context->ProgressHandle = GetDlgItem(hwndDlg, IDC_INSTALL_PROGRESS);

                    SetWindowText(context->MainHeaderHandle, L"Checking for newer websetup version...");
                    SetWindowText(context->StatusHandle, L"Requesting latest version...");
                    SetWindowText(context->SubStatusHandle, L"");

                    // Disable Next/Back buttons
                    PropSheet_SetWizButtons(context->DialogHandle, 0);

                    PhCreateThread2(SetupDownloadWebSetupThread, context);
                }
                break;
            }
        }
        break;
    }

    return FALSE;
}