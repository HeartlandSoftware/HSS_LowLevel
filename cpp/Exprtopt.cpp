/**
 * ExprtOpt.h
 *
 * Copyright 2004-2023 Heartland Software Solutions Inc.
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

#ifdef _MSC_VER
#include <afxwin.h>
#endif
#include "Exprtopt.h"
#include <string.h>


static std::string encodestr(const TCHAR *str) {
	size_t i = 0, len = _tcslen(str);
	std::string encoded;
	for (; i < len; i++) {
		if (isalnum(str[i]))
			encoded += str[i];  
		else {  
			TCHAR buf[10];
#ifdef _MSC_VER
			_stprintf_s(buf, _T("%d"), (int)str[i]);
#else
            sprintf(buf, "%d", (int)str[i]);
#endif
			encoded += '#';                 
			encoded += buf;
			encoded += '#';
		}
	}               
	return encoded;
}


static std::string decodestr(const TCHAR *str) {  
	size_t i = 0, len = _tcslen(str);
	std::string decoded;
	for (; i < len; i++) {
		if (str[i] == '#') {
			int num;
			i++;
#ifdef _MSC_VER
			_stscanf_s(&str[i], _T("%d"), &num);
#else
            sscanf(&str[i], "%d", &num);
#endif
			decoded += (char)num;
			while (str[i] != '#')
				i++;
		} else	decoded += str[i];
	}
	return decoded;
}


ExportOptions::ExportOptions(const TCHAR *group_name) {
	std::string tmp, buf, defaultstr = encodestr(_T(","));
#ifdef _MSC_VER
	if ((group_name) && (AfxGetApp()))
		tmp = AfxGetApp()->GetProfileString(group_name, _T("Column Delimiter"), defaultstr.c_str());
	else
#endif
        tmp = defaultstr;
	buf = decodestr(tmp.c_str());
	SetExportDelimiter(buf.c_str());
}


void ExportOptions::SaveToIniFile(const TCHAR *group_name) const {
#ifdef _MSC_VER
	if (AfxGetApp()) {
		std::string tmp = encodestr(GetExportDelimiter());
		AfxGetApp()->WriteProfileString(group_name, _T("Column Delimiter"), tmp.c_str());
	}
#endif
}


void ExportOptions::SetExportDelimiter(const TCHAR *delimit) {
	Delimiter = delimit;
}


BOOL ExportOptions::ExportColumnAligned() const {
	return (Delimiter[0] == 0x01);
}


const TCHAR *ExportOptions::GetExportDelimiter() const {
	return Delimiter.c_str();
}


static const TCHAR expLoadFilter[] = _T("Text and CSV Files (*.txt;*.csv) | *.txt;*.csv|Text Files (*.txt) | *.txt|CSV Files (Comma Delimited)(*.csv) | *.csv|All Files (*.*) | *.*||");
static const TCHAR expSaveFilter[] = _T("CSV Files (Comma Delimited)(*.csv) | *.csv|Text Files (*.txt) | *.txt||");


const TCHAR *ExportOptions::GetImportFilter() const {
	return expLoadFilter;
}


const TCHAR *ExportOptions::GetExportFilter() const {
		return expSaveFilter;
}


const TCHAR *ExportOptions::GetExportExt(bool choice) const {
	if ((Delimiter == _T(",")) || (!choice))
		return _T("*.csv");
	return _T("*.txt");
}


