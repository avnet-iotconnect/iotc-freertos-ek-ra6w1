#ifndef BSP_OVERRIDE_H
#define BSP_OVERRIDE_H

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

/* Define overrides required for this MCU. */
#define BSP_OVERRIDE_TIMER_MODE_T
#define BSP_OVERRIDE_TIMER_EVENT_T
#define BSP_OVERRIDE_TIMER_SOURCE_DIV_T

//#define BSP_OVERRIDE_LPM_STANDBY_WAKE_SOURCE_T
#define BSP_OVERRIDE_LPM_DEEP_STANDBY_WAKE_SOURCE_T
#define BSP_OVERRIDE_LPM_SNOOZE_REQUEST_T
#define BSP_OVERRIDE_LPM_SNOOZE_END_T
#define BSP_OVERRIDE_LPM_SNOOZE_CANCEL_T
#define BSP_OVERRIDE_ADC_INCLUDE

#define BSP_OVERRIDE_ADC_EVENT_T

/** ADC callback event definitions  */
typedef enum e_adc_event
{
    ADC_EVENT_SCAN_COMPLETE,           ///< Normal/Group A scan complete
    ADC_EVENT_SCAN_COMPLETE_GROUP_B,   ///< Group B scan complete
    ADC_EVENT_SCAN_COMPLETE_GROUP_C,   ///< Group C scan complete
    ADC_EVENT_CALIBRATION_COMPLETE,    ///< Calibration complete
    ADC_EVENT_CONVERSION_COMPLETE,     ///< Conversion complete
    ADC_EVENT_CALIBRATION_REQUEST,     ///< Calibration requested
    ADC_EVENT_CONVERSION_ERROR,        ///< Scan error
    ADC_EVENT_OVERFLOW,                ///< Overflow occurred
    ADC_EVENT_LIMIT_CLIP,              ///< Limiter clipping occurred
    ADC_EVENT_FIFO_READ_REQUEST,       ///< FIFO read requested
    ADC_EVENT_FIFO_OVERFLOW,           ///< FIFO overflow occurred
    ADC_EVENT_THD_UNDER,               ///< Conversion result was below the threshold
    ADC_EVENT_THD_OVER,                ///< Conversion result exceeded the threshold
} adc_event_t;

#define BSP_OVERRIDE_ADC_RESOLUTION_T
/** ADC data resolution definitions */
typedef enum e_adc_resolution
{
    ADC_RESOLUTION_12_BIT = 0,         ///< 12 bit resolution
    ADC_RESOLUTION_10_BIT = 1,         ///< 10 bit resolution
    ADC_RESOLUTION_7_BIT  = 2,         ///<  7 bit resolution
    ADC_RESOLUTION_4_BIT  = 3,         ///<  4 bit resolution
} adc_resolution_t;

#define BSP_OVERRIDE_ADC_INFO_T
/** ADC Information Structure for Transfer Interface */
typedef struct st_adc_info
{
    void     * p_address;              ///< The address to start reading the data from
    uint32_t   length;                 ///< The total number of transfers to read
} adc_info_t;

#define BSP_OVERRIDE_WDT_TIMEOUT_T
/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/
typedef uint32_t wdt_timeout_t;

/** Timer operational modes */
typedef enum e_timer_mode
{
    TIMER_MODE_PERIODIC,               ///< Timer restarts after period elapses.
    TIMER_MODE_ONE_SHOT,               ///< Timer stops after period elapses.
    TIMER_MODE_PWM,                    ///< Timer generates square-wave PWM output.
    TIMER_MODE_EDGE_DETECT,            ///< Timer asynchronously counting up edges.
} timer_mode_t;

typedef enum e_timer_event
{
    TIMER_EVENT_CYCLE_END,                    ///< Requested timer delay has expired or timer has wrapped around
    TIMER_EVENT_PULSE_CNT_CYCLE_END,          ///< Timer underflow event (counter went under zero (0) value)
    TIMER_EVENT_SEQUENTIAL_CAPTURE_CYCLE_END, ///< Requested number of captures were triggered
    TIMER_EVENT_OVERFLOW,                     ///< Timer overflow event (counter reached 32-bit value)
    TIMER_EVENT_UNDERFLOW,                    ///< Timer underflow event (counter went under zero (0) value)
    TIMER_EVENT_GPIO1,                    ///< A capture has occurred on signal A
    TIMER_EVENT_GPIO2,                    ///< A capture has occurred on signal B
    TIMER_EVENT_GPIO3,                    ///< A capture has occurred on signal C
    TIMER_EVENT_GPIO4,                    ///< A capture has occurred on signal D
} timer_event_t;

typedef enum e_timer_source_div
{
    TIMER_SOURCE_DIV_1  =  0,    ///< Timer clock source divided by 1
    TIMER_SOURCE_DIV_2  =  1,    ///< Timer clock source divided by 2
    TIMER_SOURCE_DIV_3  =  2,    ///< Timer clock source divided by 3
    TIMER_SOURCE_DIV_4  =  3,    ///< Timer clock source divided by 4
    TIMER_SOURCE_DIV_5  =  4,    ///< Timer clock source divided by 5
    TIMER_SOURCE_DIV_6  =  5,    ///< Timer clock source divided by 6
    TIMER_SOURCE_DIV_7  =  6,    ///< Timer clock source divided by 7
    TIMER_SOURCE_DIV_8  =  7,    ///< Timer clock source divided by 8
    TIMER_SOURCE_DIV_9  =  8,    ///< Timer clock source divided by 9
    TIMER_SOURCE_DIV_10 =  9,    ///< Timer clock source divided by 10
    TIMER_SOURCE_DIV_11 = 10,    ///< Timer clock source divided by 11
    TIMER_SOURCE_DIV_12 = 11,    ///< Timer clock source divided by 12
    TIMER_SOURCE_DIV_13 = 12,    ///< Timer clock source divided by 13
    TIMER_SOURCE_DIV_14 = 13,    ///< Timer clock source divided by 14
    TIMER_SOURCE_DIV_15 = 14,    ///< Timer clock source divided by 15
    TIMER_SOURCE_DIV_16 = 15,    ///< Timer clock source divided by 16
    TIMER_SOURCE_DIV_17 = 16,    ///< Timer clock source divided by 17
    TIMER_SOURCE_DIV_18 = 17,    ///< Timer clock source divided by 18
    TIMER_SOURCE_DIV_19 = 18,    ///< Timer clock source divided by 19
    TIMER_SOURCE_DIV_20 = 19,    ///< Timer clock source divided by 20
    TIMER_SOURCE_DIV_21 = 20,    ///< Timer clock source divided by 21
    TIMER_SOURCE_DIV_22 = 21,    ///< Timer clock source divided by 22
    TIMER_SOURCE_DIV_23 = 22,    ///< Timer clock source divided by 23
    TIMER_SOURCE_DIV_24 = 23,    ///< Timer clock source divided by 24
    TIMER_SOURCE_DIV_25 = 24,    ///< Timer clock source divided by 25
    TIMER_SOURCE_DIV_26 = 25,    ///< Timer clock source divided by 26
    TIMER_SOURCE_DIV_27 = 26,    ///< Timer clock source divided by 27
    TIMER_SOURCE_DIV_28 = 27,    ///< Timer clock source divided by 28
    TIMER_SOURCE_DIV_29 = 28,    ///< Timer clock source divided by 29
    TIMER_SOURCE_DIV_30 = 29,    ///< Timer clock source divided by 30
    TIMER_SOURCE_DIV_31 = 30,    ///< Timer clock source divided by 31
    TIMER_SOURCE_DIV_32 = 31,    ///< Timer clock source divided by 32
} timer_source_div_t;
#endif
