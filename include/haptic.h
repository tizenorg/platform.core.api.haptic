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




#ifndef __TIZEN_SYSTEM_HAPTIC_H__
#define __TIZEN_SYSTEM_HAPTIC_H__

#include <tizen_error.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup CAPI_SYSTEM_HAPTIC_MODULE
 * @{
 * @brief
 * This is a Haptic API of the System Service.
 * @}
 */

/**
 * @addtogroup CAPI_SYSTEM_HAPTIC_MODULE
 * @{
 */

/**
 * @brief Enumerations of feedback level for haptic.
 * @details Haptic level means vibration power (intensity).
 */
typedef enum
{
	HAPTIC_LEVEL_AUTO = -1,     /**< vibration level from settings */
	HAPTIC_LEVEL_0 = 0,         /**< vibration level 0 (silence) */
	HAPTIC_LEVEL_1 = 20,        /**< vibration level 1 (the lowest) */
	HAPTIC_LEVEL_2 = 40,        /**< vibration level 2 (low)*/
	HAPTIC_LEVEL_3 = 60,        /**< vibration level 3 (middle) */
	HAPTIC_LEVEL_4 = 80,        /**< vibration level 4 (high) */
	HAPTIC_LEVEL_5 = 100,       /**< vibration level 5 (the highest) */
} haptic_level_e;

/**
 * @brief Enumerations of error codes for the Haptic API.
 */
typedef enum
{
	HAPTIC_ERROR_NONE                  = TIZEN_ERROR_NONE,                     /**< Successful */
	HAPTIC_ERROR_INVALID_PARAMETER     = TIZEN_ERROR_INVALID_PARAMETER,      /**< Invalid parameter */
    HAPTIC_ERROR_NO_SUCH_FILE          = TIZEN_ERROR_NO_SUCH_FILE,           /**< No such file */
    HAPTIC_ERROR_NOT_SUPPORTED_FORMAT  = TIZEN_ERROR_SYSTEM_CLASS | 0x23,    /**< Not supported file format */
    HAPTIC_ERROR_NOT_INITIALIZED       = TIZEN_ERROR_SYSTEM_CLASS | 0x26,    /**< Not initialized */
    HAPTIC_ERROR_OPERATION_FAILED      = TIZEN_ERROR_SYSTEM_CLASS | 0x28,    /**< Operation failed  */
} haptic_error_e;


/**
 * @brief The structure type to contain the set of vibration data
 *        that used for generate a iteration of vibration.
 *
 * @see haptic_play_pattern()
 */
typedef struct
{
    int vibrator_index;           /**< Index of the vibrator */
    int time;              /**< Duration of the vibration in a iteration */
} haptic_vibration_iter_s;

/**
 * @brief Gets the number of the vibrators.
 *
 * @remarks The index zero is reserved meaning for all vibrators at a time.
 *
 * @param[out] vibrator_number A number of vibrators
 * 
 * @return 0 on success, otherwise a negative error value.
 * @retval #HAPTIC_ERROR_OPERATION_FAILED   Operation failed
 */
int haptic_get_count(int* vibrator_number);

/**
 * @brief Initializes haptic API.
 * 
 * @details Internally, it makes a connection to the vibrator.
 *
 * @remarks If this function is not called in advance, other functions will return #HAPTIC_ERROR_NOT_INITIALIZED.
 * @remarks Haptic API must be deinitialized by haptic_deinitialize().
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #HAPTIC_ERROR_NONE               Successful
 * @retval #HAPTIC_ERROR_OPERATION_FAILED   Operation failed
 *
 * @see haptic_deinitialize()
 */
int haptic_initialize(void);

/**
 * @brief Deinitializes haptic API.
 *
 * @details Internally, it disconnects the connection to vibrator.
 *
 * @return 0 on success, otherwise a negative error value.  
 * @retval #HAPTIC_ERROR_NONE               Successful
 * @retval #HAPTIC_ERROR_OPERATION_FAILED   Operation failed
 *
 * @see haptic_initialize()
 */
int haptic_deinitialize(void);

/**
 * @brief Vibrates during the specified time with a constant intensity.
 * @details
 * This function can be used to start monotonous vibration for specified time.\n
 * Default value of intensity is used.\n
 *
 * @remark
 * level is intensity variation of vibration. it has a value from 0 to 100.\n
 * level -1 is reserved for auto changing to saved variable in the settings.
 *
 * @param[in] vibrator_index	The index of the vibrator.\n
 * 							The index of the first vibrator is 1.\n
 *							0 is reserved for every vibrators at once.
 * @param[in] duration_ms	The play duration in milliseconds
 * @param[in] level         The amount of the intensity variation
 *
 * @return 0 on success, otherwise a negative error value.  
 * @retval #HAPTIC_ERROR_NONE               Successful
 * @retval #HAPTIC_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #HAPTIC_ERROR_OPERATION_FAILED   Operation failed
 * @retval #HAPTIC_ERROR_NOT_INITIALIZED    Not initialized
 *
 * @see haptic_stop_device()
 * @see haptic_vibrate_file()
 * @see haptic_get_count()
 */
int haptic_vibrate_monotone(int vibrator_index, int duration_ms, int level);

/**
 * @brief Stops the current vibration which is being played.
 * @details This function can be used to stop vibration started by haptic_vibrate_file(), or haptic_vibrate_monotone().
 *
 * @remarks You must use haptic_stop_pattern() to stop the vibration pattern from haptic_play_pattern();
 *
 * @param[in] vibrator_index	The index of the vibrator.\n
 * 							The index of the first vibrator is 1.\n
 *							0 is reserved for every vibrators at once.
 *
 * @return 0 on success, otherwise a negative error value.  
 * @retval #HAPTIC_ERROR_NONE               Successful
 * @retval #HAPTIC_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #HAPTIC_ERROR_OPERATION_FAILED   Operation failed
 * @retval #HAPTIC_ERROR_NOT_INITIALIZED    Not initialized
 *
 * @see haptic_vibrate_file()
 * @see haptic_vibrate_monotone()
 * @see haptic_get_count()
 */
int haptic_stop_device(int vibrator_index);

/**
 * @brief Starts playing the pattern of continuous vibration data.
 * @details
 * Each vibration data has index of vibrator and duration time. \n
 * Vibration data plays continuously in order of specific array. \n
 * This function returns the ID of playing session if it succeeds.
 * This ID can be used to stop playing iterations.
 *
 * @remark
 * level is intensity variation of vibration. it has a value from 0 to 100.\n
 * level -1 is reserved for auto changing to saved variable in the settings.
 *
 * @param[in]   pattern         The array of the vibration data
 * @param[in]   pattern_size    The number of the vibration data
 * @param[in]   count           The number of the Iteration that include playing every vibrations
 * @param[in]   level           The amount of the intensity variation in every vibrations
 * @param[out]  id              The ID of the vibration pattern
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #HAPTIC_ERROR_NONE               Successful
 * @retval #HAPTIC_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #HAPTIC_ERROR_OPERATION_FAILED   Operation failed
 * @retval #HAPTIC_ERROR_NOT_INITIALIZED    Not initialized
 *
 * @see haptic_stop_pattern()
 */
int haptic_play_pattern(haptic_vibration_iter_s* pattern, int pattern_size, int count, int level, int* id);

/**
 * @brief Stops playing the pattern which is being played.
 *
 * @remarks haptic_stop_device() can not stop pattern, 
 * because playing process calls the vibration procedure in each vibration data.
 *
 * @param[in] id    The ID of playing session from haptic_play_pattern() 
 *
 * @return 0 on success, otherwise a negative error value.  
 * @retval #HAPTIC_ERROR_NONE               Successful
 * @retval #HAPTIC_ERROR_INVALID_PARAMETER  Invalid parameter
 *
 * @see haptic_play_pattern()
 */
int haptic_stop_pattern(int id);
/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif  // __TIZEN_SYSTEM_HAPTIC_H__

