#ifndef ESE_MAIN_ANALYZE_DATA_H_
#define ESE_MAIN_ANALYZE_DATA_H_

#include <stdint.h>

#include <logger.h>

typedef struct DataSummary {
    uint32_t num_alphabetic;
    uint32_t num_numeric;
    uint32_t num_punctuation;
    uint32_t num_misc;
} data_summary_t;

/**
   clear_data_summary()

   Clear the counters in a data_summary struct.

   Params: *data_summary - pointer to a data_summary object that is to be
   cleared.

   Returns: none
 */
void clear_data_summary(data_summary_t *data_summary);

/**
   process_data()

   Process the data contained in the byte, and increment the relevant field in
   the data summary type.

   Analysis is to classify each byte if input data according to what type of
   ascii character it is: alphabetic, numeric, punctuation, or misc.

   Params: *data_summary - pointer to a data_summary struct with the current
   analysis.

   Returns: none
 */
void process_data(data_summary_t *data_summary, uint8_t byte);

/**
   log_data_analysis()

   Send the data analysis to the global logger.

   Params: *data_summary - pointer to a data_summary struct with the current
   analysis.

   Returns: none
 */
void log_data_analysis(log_item_t *item, data_summary_t *data_summary);

#endif // ESE_MAIN_ANALYZE_DATA_H_
