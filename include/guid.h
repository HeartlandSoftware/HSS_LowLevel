/**
 * guid.h
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

#ifdef _MSC_VER
#include <guiddef.h>
#else
#include <cstring>

typedef struct _GUID {
    unsigned long  Data1;
    unsigned short Data2;
    unsigned short Data3;
    unsigned char  Data4[8];
} GUID;

#ifndef EXTERN_C
#ifdef __cplusplus
#define EXTERN_C extern "C"
#else
#define EXTERN_C extern
#endif //__cplusplus
#endif //EXTERN_C

#ifdef INITGUID
#define DEFINE_GUID(name, l, w1, s2, b1, b2, b3, b4, b5, b6, b7, b8) \
    EXTERN_C const GUID name = { l, w1, w2, { b1, b2, b3, b4, b5, b6, b7, b8 } }
#else
#define DEFINE_GUID(name, l, w1, s2, b1, b2, b3, b4, b5, b6, b7, b8) \
    EXTERN_C const GUID name
#endif

#ifdef __cplusplus
inline int InlineIsEqualGUID(const GUID& rguid1, const GUID& rguid2)
{
    return (
        ((unsigned long*)&rguid1)[0] == ((unsigned long*)&rguid2)[0] &&
        ((unsigned long*)&rguid1)[1] == ((unsigned long*)&rguid2)[1] &&
        ((unsigned long*)&rguid1)[2] == ((unsigned long*)&rguid2)[2] &&
        ((unsigned long*)&rguid1)[3] == ((unsigned long*)&rguid2)[3]
    );
}

inline int IsEqualGUID(const GUID& rguid1, const GUID& rguid2)
{
    return !memcmp(&rguid1, &rguid2, sizeof(GUID));
}

inline bool operator==(const GUID& guidOne, const GUID& guidOther)
{
    return !!IsEqualGUID(guidOne, guidOther);
}

inline bool operator!=(const GUID& guidOne, const GUID& guidOther)
{
    return !IsEqualGUID(guidOne, guidOther);
}
#else
#define InlineIsEqualGUID(rguid1, rguid2) \
        (((unsigned long*)&rguid1)[0] == ((unsigned long*)&rguid2)[0] &&
        ((unsigned long*)&rguid1)[1] == ((unsigned long*)&rguid2)[1] &&
        ((unsigned long*)&rguid1)[2] == ((unsigned long*)&rguid2)[2] &&
        ((unsigned long*)&rguid1)[3] == ((unsigned long*)&rguid2)[3])
#define IsEqualGUID(rguid1, rguid2) (!memcmp(rguid1, rguid2, sizeof(GUID)))
#endif

#endif