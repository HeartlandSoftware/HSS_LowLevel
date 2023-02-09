/**
 * AfxIniSettings.cpp
 *
 * Copyright 2018-2023 Heartland Software Solutions Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the license at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the LIcense is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "AfxIniSettings.h"

#include "types.h"
#ifdef _MSC_VER
#include <ShlObj.h>
#undef WriteProfileString
#undef GetProfileInt
#undef GetProfileString
#else
#include <unistd.h>
#include <pwd.h>
#endif
#include <sys/types.h>
#include "filesystem.hpp"

#include <boost/property_tree/info_parser.hpp>

namespace tree = boost::property_tree;


IniSettings* AfxGetApp()
{
    static IniSettings s_instance;

    return &s_instance;
}


IniSettings::IniSettings(bool saveOnWrite) :
    m_saveOnWrite(saveOnWrite)
{
    char* home;
    if ((home = getenv("HOME")) == nullptr)
    {
#ifdef _MSC_VER
        PWSTR path;
        SHGetKnownFolderPath(FOLDERID_Documents, 0, nullptr, &path);
        home = new char[MAX_PATH];
        wcstombs(home, path, MAX_PATH);
        CoTaskMemFree(path);
#else
        home = getpwuid(getuid())->pw_dir;
#endif
    }
    fs::path p = home;
    p /= ".prometheus";
    if (!fs::exists(p))
        fs::create_directory(p);
    p /= "settings.info";
    if (fs::exists(p))
        tree::info_parser::read_info(p.string(), m_tree);
    m_path = p.string();
#ifdef _MSC_VER
    delete[] home;
#endif
}

void IniSettings::Save()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    tree::info_parser::write_info(m_path, m_tree);
}

std::string IniSettings::GetPath()
{
    fs::path p = m_path;
    return p.parent_path().string();
}

void IniSettings::WriteProfileInt(const std::string &group_name, const std::string &key, int value)
{
    tree::ptree::path_type place = tree::path(group_name + "|" + key, '|');
    m_tree.put(place, value);
    if (m_saveOnWrite)
        Save();
}

void IniSettings::WriteProfileLong(const std::string &group_name, const std::string &key, unsigned long value)
{
    tree::ptree::path_type place = tree::path(group_name + "|" + key, '|');
    m_tree.put(place, value);
    if (m_saveOnWrite)
        Save();
}

void IniSettings::WriteProfileLongLong(const std::string &group_name, const std::string &key, std::uint64_t value)
{
    tree::ptree::path_type place = tree::path(group_name + "|" + key, '|');
    m_tree.put(place, value);
    if (m_saveOnWrite)
        Save();
}

void IniSettings::WriteProfileDouble(const std::string &group_name, const std::string &key, double value, std::string format)
{
    tree::ptree::path_type place = tree::path(group_name + "|" + key, '|');
    m_tree.put(place, value);
    if (m_saveOnWrite)
        Save();
}

void IniSettings::WriteProfileString(const std::string &group_name, const std::string &key, std::string value)
{
    tree::ptree::path_type place = tree::path(group_name + "|" + key, '|');
    m_tree.put(place, value);
    if (m_saveOnWrite)
        Save();
}

void IniSettings::WriteProfileBOOL(const std::string &group_name, const std::string &key, bool value)
{
    tree::ptree::path_type place = tree::path(group_name + "|" + key, '|');
    m_tree.put(place, value);
    if (m_saveOnWrite)
        Save();
}

void IniSettings::WriteProfileBinary(const std::string &group_name, const std::string &key, std::uint8_t* data, std::uint32_t len)
{
    std::string temp(reinterpret_cast<char *>(data), len);
    tree::ptree::path_type place = tree::path(group_name + "|" + key, '|');
    m_tree.put(place, temp);
    if (m_saveOnWrite)
        Save();
}

int IniSettings::GetProfileInt(const std::string &group_name, const std::string &key, int def)
{
    tree::ptree::path_type place = tree::path(group_name + "|" + key, '|');
    return m_tree.get(place, def);
}

unsigned long IniSettings::GetProfileLong(const std::string &group_name, const std::string &key, unsigned long def)
{
    tree::ptree::path_type place = tree::path(group_name + "|" + key, '|');
    return m_tree.get(place, def);
}

std::uint64_t IniSettings::GetProfileLongLong(const std::string &group_name, const std::string &key, std::uint64_t def)
{
    tree::ptree::path_type place = tree::path(group_name + "|" + key, '|');
    return m_tree.get(place, def);
}

double IniSettings::GetProfileDouble(const std::string &group_name, const std::string &key, double def)
{
    tree::ptree::path_type place = tree::path(group_name + "|" + key, '|');
    return m_tree.get(place, def);
}

std::string IniSettings::GetProfileString(const std::string &group_name, const std::string &key, std::string def)
{
    tree::ptree::path_type place = tree::path(group_name + "|" + key, '|');
    return m_tree.get(place, def);
}

bool IniSettings::GetProfileBOOL(const std::string &group_name, const std::string &key, bool def)
{
    tree::ptree::path_type place = tree::path(group_name + "|" + key, '|');
    return m_tree.get(place, def);
}

void IniSettings::GetProfileBinary(const std::string &group_name, const std::string &key, std::uint8_t** data, std::uint32_t* len)
{
    tree::ptree::path_type place = tree::path(group_name + "|" + key, '|');
    std::string temp = m_tree.get(place, "");
    *len = temp.length();
    if (temp.length() > 0)
    {
        *data = new std::uint8_t[temp.length()];
        std::copy(temp.begin(), temp.end(), *data);
    }
}

void IniSettings::WriteRegistryInt(const std::string &group_name, const std::string &key, int value)
{
    tree::ptree::path_type place = tree::path("registry|" + group_name + "|" + key, '|');
    m_tree.put(place, value);
    if (m_saveOnWrite)
        Save();
}

void IniSettings::WriteRegistryString(const std::string &group_name, const std::string &key, std::string value)
{
    tree::ptree::path_type place = tree::path("registry|" + group_name + "|" + key, '|');
    m_tree.put(place, value);
    if (m_saveOnWrite)
        Save();
}

int IniSettings::GetRegistryInt(const std::string &group_name, const std::string &key, int def)
{
    tree::ptree::path_type place = tree::path("registry|" + group_name + "|" + key, '|');
    return m_tree.get(place, def);
}

std::string IniSettings::GetRegistryString(const std::string &group_name, const std::string &key, std::string def)
{
    tree::ptree::path_type place = tree::path("registry|" + group_name + "|" + key, '|');
    return m_tree.get(place, def);
}
