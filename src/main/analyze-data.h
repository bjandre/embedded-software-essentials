
/*
** Copyright 2017 Benjamin J. Andre.
** All Rights Reserved.
**
** This Source Code Form is subject to the terms of the Mozilla
** Public License, v. 2.0. If a copy of the MPL was not distributed
** with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#ifndef ESE_MAIN_ANALYZE_DATA_H_
#define ESE_MAIN_ANALYZE_DATA_H_

#include <stdint.h>

#include <logger.h>

/**
   Store results of data analysis
 */
typedef struct DataSummary {
    uint32_t num_alphabetic;
    uint32_t num_numeric;
    uint32_t num_punctuation;
    uint32_t num_misc;
} data_summary_t;

/**
   clear_data_summary()

   Clear the counters in a data_summary struct.

   \param[in,out] *data_summary - pointer to a data_summary object that is to be
   cleared.

   \return none
 */
void clear_data_summary(data_summary_t *data_summary);

/**
   process_data()

   Process the data contained in the byte, and increment the relevant field in
   the data summary type.

   Analysis is to classify each byte if input data according to what type of
   ascii character it is: alphabetic, numeric, punctuation, or misc.

   \param[in,out] *data_summary - pointer to a data_summary struct with the current
   analysis.
   \param[in] byte - data to add to the analysis

   \return none
 */
void process_data(data_summary_t *data_summary, uint8_t byte);

/**
   log_data_analysis()

   Send the data analysis to the global logger.

   \param[in] *item - pointer to a log item object
   \param[in] *data_summary - pointer to a data_summary struct with the current
   analysis.

   \return none
 */
void log_data_analysis(log_item_t *item, data_summary_t *data_summary);

#endif // ESE_MAIN_ANALYZE_DATA_H_
