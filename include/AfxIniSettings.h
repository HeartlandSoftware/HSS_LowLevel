/**
 * AfxIniSettings.h
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

#pragma once

#include <boost/property_tree/ptree.hpp>
#include <string>
#include <mutex>

class IniSettings
{
public:
    IniSettings(bool saveOnWrite = true);

    void WriteProfileInt(const std::string &group_name, const std::string &key, int value);
    void WriteProfileLong(const std::string &group_name, const std::string &key, unsigned long value);
    void WriteProfileLongLong(const std::string &group_name, const std::string &key, std::uint64_t value);
    void WriteProfileDouble(const std::string &group_name, const std::string &key, double value, std::string format);
    void WriteProfileString(const std::string &group_name, const std::string &key, std::string value);
    void WriteProfileBOOL(const std::string &group_name, const std::string &key, bool value);
    void WriteProfileBinary(const std::string &group_name, const std::string &key, std::uint8_t* data, std::uint32_t len);

    int GetProfileInt(const std::string &group_name, const std::string &key, int def);
    unsigned long GetProfileLong(const std::string &group_name, const std::string &key, unsigned long def);
    std::uint64_t GetProfileLongLong(const std::string &group_name, const std::string &key, std::uint64_t def);
    double GetProfileDouble(const std::string &group_name, const std::string &key, double def);
    std::string GetProfileString(const std::string &group_name, const std::string &key, std::string def);
    bool GetProfileBOOL(const std::string &group_name, const std::string &key, bool def);
    void GetProfileBinary(const std::string &group_name, const std::string &key, std::uint8_t** data, std::uint32_t* len);


    void WriteRegistryInt(const std::string &group_name, const std::string &key, int value);
    void WriteRegistryString(const std::string &group_name, const std::string &key, std::string value);

    int GetRegistryInt(const std::string &group_name, const std::string &key, int def);
    std::string GetRegistryString(const std::string &group_name, const std::string &key, std::string def);
    
    void Save();
    
    std::string GetPath();

private:
    boost::property_tree::ptree m_tree;
    bool m_saveOnWrite;
    std::string m_path;
    std::mutex m_mutex;
};

//hack the settings onto the AfxGetApp call for compatibility
extern IniSettings* AfxGetApp();
