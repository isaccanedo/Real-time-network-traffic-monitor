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

#include "setting.h"
#include "settingfilter.h"
#include "stringutils.h"

#include <istream>
#include <ostream>
#include <sstream>

using namespace std;

Setting::~Setting()
{
    for(list<SettingFilter*>::const_iterator itFilter = m_filters.begin(); itFilter != m_filters.end(); ++itFilter)
        delete *itFilter;
    m_filters.clear();
}

void Setting::pushFilter(SettingFilter* filter)
{
    if(!filter)
        return;

    m_filters.push_back(filter);
}

void Setting::popFilter()
{
    if(m_filters.empty())
        return;

    delete m_filters.back();
    m_filters.pop_back();
}

SettingFilter* Setting::findFilterWithId(const string& id)
{
    for(list<SettingFilter*>::const_iterator itFilter = m_filters.begin(); itFilter != m_filters.end(); ++itFilter)
    {
        if((*itFilter)->getId() == id)
            return *itFilter;
    }

    return 0;
}

const SettingFilter* Setting::findFilterWithId(const string& id) const
{
    for(list<SettingFilter*>::const_iterator itFilter = m_filters.begin(); itFilter != m_filters.end(); ++itFilter)
    {
        if((*itFilter)->getId() == id)
            return *itFilter;
    }

    return 0;
}

string Setting::getThroughFilter() const
{
    string valueCopy = m_value;

    for(list<SettingFilter*>::const_reverse_iterator itFilter = m_filters.rbegin(); itFilter != m_filters.rend(); ++itFilter)
    {
        if((*itFilter)->filterRead(valueCopy) == SettingFilter::ResultSuccessQuit)
            break;
    }

    return valueCopy;
}

bool Setting::setThroughFilter(const string& value)
{
    string valueCopy = value;

    for(list<SettingFilter*>::iterator itFilter = m_filters.begin(); itFilter != m_filters.end(); ++itFilter)
    {
        SettingFilter::FilterResult result = (*itFilter)->filterWrite(valueCopy);
        if(result == SettingFilter::ResultSuccessQuit)
            break;
        else if(result == SettingFilter::ResultFailure)
            return false;
    }

    m_value = valueCopy;
    return true;
}

istream& operator>>(istream& in, Setting& setting)
{
    while(!in.eof())
    {
        // extract line from stream
        string line;
        getline(in, line);

        if(in.fail())
            return in;

        // strip whitespace
        line = trim(line);

        // skip empty lines
        if(line.empty())
            continue;

        // skip comments
        if(line[0] == '#')
            continue;

        // split line into id and value
        vector<string> words = splitQuoted(line, "=");

        if(words.size() < 2 || words[0].empty())
        {
            in.setstate(ios_base::failbit);
            return in;
        }

        setting.setId(words[0]);
        setting.setThroughFilter(words[1]);

        break;
    }

    return in;
}

ostream& operator<<(ostream& out, const Setting& setting)
{
    out << setting.getId() << "=\"" << setting.getThroughFilter() << "\"" << endl;
    return out;
}


