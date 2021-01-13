/*
 * nload
 * real time monitor for network traffic
 * Copyright (C) 2001 - 2018 by Roland Riegel <feedback@roland-riegel.de>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

#include "devreader-linux-sys.h"

#include <fstream>
#include <string>
#include <list>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

DevReaderLinuxSys::DevReaderLinuxSys(const string& deviceName)
    : DevReader(deviceName)
{
}

DevReaderLinuxSys::~DevReaderLinuxSys()
{
}

bool DevReaderLinuxSys::isAvailable()
{
    struct stat sysStat;
    if(stat("/sys/class/net", &sysStat) < 0 || ! S_ISDIR(sysStat.st_mode))
        return false;

    return true;
}

list<string> DevReaderLinuxSys::findAllDevices()
{
    list<string> interfaceNames;
    DIR* sysDir = opendir("/sys/class/net");
    struct dirent* sysDirEntry = 0;
    struct stat sysStat;

    if(!sysDir)
        return interfaceNames;

    while((sysDirEntry = readdir(sysDir)))
    {
        string interfaceName(sysDirEntry->d_name);
        
        if(interfaceName[0] == '.')
            continue;

        if(stat(("/sys/class/net/" + interfaceName).c_str(), &sysStat) < 0 ||
           !S_ISDIR(sysStat.st_mode))
            continue;
                
        interfaceNames.push_back(interfaceName);
    }

    closedir(sysDir);

    return interfaceNames;
}

void DevReaderLinuxSys::readFromDevice(DataFrame& dataFrame)
{
    string devPath = "/sys/class/net/";
    devPath += m_deviceName;
    
    struct stat sysStat;
    if(stat(devPath.c_str(), &sysStat) < 0 || ! S_ISDIR(sysStat.st_mode))
        return;

    dataFrame.setTotalDataIn(readULongSysEntry("statistics/rx_bytes"));
    dataFrame.setTotalDataOut(readULongSysEntry("statistics/tx_bytes"));

    dataFrame.setTotalPacketsIn(readULongSysEntry("statistics/rx_packets"));
    dataFrame.setTotalPacketsOut(readULongSysEntry("statistics/tx_packets"));

    dataFrame.setTotalErrorsIn(readULongSysEntry("statistics/rx_errors"));
    dataFrame.setTotalErrorsOut(readULongSysEntry("statistics/tx_errors"));
    
    dataFrame.setTotalDropsIn(readULongSysEntry("statistics/rx_dropped"));
    dataFrame.setTotalDropsOut(readULongSysEntry("statistics/tx_dropped"));
    
    dataFrame.setValid(true);
}

unsigned long long DevReaderLinuxSys::readULongSysEntry(const string& entry)
{
    string sysEntryPath = "/sys/class/net/";
    sysEntryPath += m_deviceName;
    sysEntryPath += '/';
    sysEntryPath += entry;
    
    ifstream sysEntry(sysEntryPath.c_str());
    if(!sysEntry.is_open())
        return 0;

    unsigned long long num = 0;
    sysEntry >> num;
    if(sysEntry.fail())
        return 0;

    return num;
}


