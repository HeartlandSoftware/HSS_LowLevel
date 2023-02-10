/**
 * hss_propagate_const.h
 *
 * Copyright 2020-2023 Heartland Software Solutions Inc.
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

#if __has_include(<experimental/propagate_const>)
#include <experimental/propagate_const>
#else //__has_include(<experimental/propagate_const>)
//if the system doesn't have the experimental package here is a third party implementation
//https://github.com/jbcoe/propagate_const/blob/master/propagate_const.h
#include <propagate_const.h>
#endif
