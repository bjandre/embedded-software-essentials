
/*
** Copyright 2017 Benjamin J. Andre.
** All Rights Reserved.
**
** This Source Code Form is subject to the terms of the Mozilla
** Public License, v. 2.0. If a copy of the MPL was not distributed
** with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#ifndef ESE_APP_ANALYZE_DATA_H_
#define ESE_APP_ANALYZE_DATA_H_

#include <stdint.h>

#include "log-item.h"

/**
   Store results of data analysis
 */
typedef struct DataSummary {
    uint8_t num_received;
    uint32_t num_alphabetic;
    uint32_t num_numeric;
    uint32_t num_punctuation;
    uint32_t num_misc;
} data_summary_t;

/**
   initialize the data analysis
o
   \param[in,out] *data_summary pointer to a data_summary state
   \param[in] *item pointer to log item used for reporting
 */
void initialize_logger_data_analysis(data_summary_t *data_summary,
                                     log_item_t *item);

/**
   analyze a data event

   \param[in,out] *data_summary pointer to a data_summary state
   \param[in] *item pointer to log item used for reporting

 */
void analyze_logger_data_event(data_summary_t *data_summary, log_item_t *item);

/**
   Clear the counters in a data_summary struct.

   \param[in,out] *data_summary - pointer to a data_summary object that is to be
   cleared.

 */
void clear_data_summary(data_summary_t *data_summary);

/**
   Process the data contained in the byte, and increment the relevant field in
   the data summary type.

   Analysis is to classify each byte if input data according to what type of
   ascii character it is: alphabetic, numeric, punctuation, or misc.

   \param[in,out] *data_summary - pointer to a data_summary struct with the current
   analysis.
   \param[in] byte - data to add to the analysis

 */
void process_data(data_summary_t *data_summary, uint8_t byte);

/**
   Send the data analysis to the global logger.

   \param[in] *item - pointer to a log item object
   \param[in] *data_summary - pointer to a data_summary struct with the current
   analysis.

 */
void log_data_analysis(log_item_t *item, data_summary_t *data_summary);

#endif // ESE_APP_ANALYZE_DATA_H_
