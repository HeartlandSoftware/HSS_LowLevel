/**
 * comcodes.h
 *
 * Copyright 2006-2023 Heartland Software Solutions Inc.
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

#ifndef __HSS_COMCODES_H
#define __HSS_COMCODES_H

#include "types.h"

extern "C" HRESULT ComError(int _errno_);			// translates a C error return code to a (we think reasonable) COM HRESULT

#endif
