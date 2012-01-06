/*
 * Copyright (c) 2011 Samsung Electronics Co., Ltd All Rights Reserved
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. 
 */




#ifndef __TIZEN_SYSTEM_HAPTIC_PRIVATE_H__
#define __TIZEN_SYSTEM_HAPTIC_PRIVATE_H__

#ifdef __cplusplus
extern "C" {
#endif

void _haptic_init();
void _haptic_deinit();
int _haptic_play_monotone(int dev, int duration, haptic_level_e level);
	
#ifdef __cplusplus
}
#endif

#endif
