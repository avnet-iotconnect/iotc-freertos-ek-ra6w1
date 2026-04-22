/***********************************************************************************************************************
 * Copyright [2020-2024] Renesas Electronics Corporation and/or its affiliates.  All Rights Reserved.
 *
 * This software and documentation are supplied by Renesas Electronics America Inc. and may only be used with products
 * of Renesas Electronics Corp. and its affiliates ("Renesas").  No other uses are authorized.  Renesas products are
 * sold pursuant to Renesas terms and conditions of sale.  Purchasers are solely responsible for the selection and use
 * of Renesas products and Renesas assumes no liability.  No license, express or implied, to any intellectual property
 * right is granted by Renesas. This software is protected under all applicable laws, including copyright laws. Renesas
 * reserves the right to change or discontinue this software and/or this documentation. THE SOFTWARE AND DOCUMENTATION
 * IS DELIVERED TO YOU "AS IS," AND RENESAS MAKES NO REPRESENTATIONS OR WARRANTIES, AND TO THE FULLEST EXTENT
 * PERMISSIBLE UNDER APPLICABLE LAW, DISCLAIMS ALL WARRANTIES, WHETHER EXPLICITLY OR IMPLICITLY, INCLUDING WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, AND NONINFRINGEMENT, WITH RESPECT TO THE SOFTWARE OR
 * DOCUMENTATION.  RENESAS SHALL HAVE NO LIABILITY ARISING OUT OF ANY SECURITY VULNERABILITY OR BREACH.  TO THE MAXIMUM
 * EXTENT PERMITTED BY LAW, IN NO EVENT WILL RENESAS BE LIABLE TO YOU IN CONNECTION WITH THE SOFTWARE OR DOCUMENTATION
 * (OR ANY PERSON OR ENTITY CLAIMING RIGHTS DERIVED FROM YOU) FOR ANY LOSS, DAMAGES, OR CLAIMS WHATSOEVER, INCLUDING,
 * WITHOUT LIMITATION, ANY DIRECT, CONSEQUENTIAL, SPECIAL, INDIRECT, PUNITIVE, OR INCIDENTAL DAMAGES; ANY LOST PROFITS,
 * OTHER ECONOMIC DAMAGE, PROPERTY DAMAGE, OR PERSONAL INJURY; AND EVEN IF RENESAS HAS BEEN ADVISED OF THE POSSIBILITY
 * OF SUCH LOSS, DAMAGES, CLAIMS OR COSTS.
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "r_wdog_w.h"
#include "bsp_api.h"
#include "bsp_cfg.h"

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

#define WDOG_W_OPEN                            (0X00574454ULL)

#define WDOG_W_PRV_WAIT_TIMEOUT_TICK           (0xFFFFU)
#define WDOG_W_PRV_CLC_DIV_RATIO               (320UL)
#define WDOG_W_REG_WEN_ENABLE                  (0x00000UL)
#define WDOG_W_CTRLREG_FREEZEEN_DISABLE        (0U)
#define WDOG_W_CTRLREG_FREEZEEN_ENABLE         (1U)
#define WDOG_W_CTRLREG_NMIRST_NMI              (0U)
#define WDOG_W_CTRLREG_NMIRST_RST              (1U)

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

#if defined(__ARMCC_VERSION) || defined(__ICCARM__)
typedef void (BSP_CMSE_NONSECURE_CALL * wdt_prv_ns_callback)(wdt_callback_args_t * p_args);
#elif defined(__GNUC__)
typedef BSP_CMSE_NONSECURE_CALL void (*volatile wdt_prv_ns_callback)(wdt_callback_args_t * p_args);
#endif
/***********************************************************************************************************************
 * Publis function prototypes
 **********************************************************************************************************************/

void bsp_wdt_freeze_cfg(void);

/***********************************************************************************************************************
 * Private function prototypes
 **********************************************************************************************************************/

static void      r_wdog_w_nmi_internal_callback(uint32_t * p_exception_args);
static fsp_err_t r_wdog_w_parameter_checking(wdog_w_instance_ctrl_t * const p_instance_ctrl,
                                            wdt_cfg_t const * const       p_cfg);
static void      r_wdog_w_freeze (void);
static void      r_wdog_w_unfreeze (void);

#if WDOG_W_CFG_NMI_SUPPORTED
static void r_wdog_w_nmi_initialize(wdog_w_instance_ctrl_t * const p_instance_ctrl, wdt_cfg_t const * const p_cfg);
#endif

static fsp_err_t r_wdog_w_reload_timeout_value(void);

void bsp_wdt_freeze_cfg(void);

/***********************************************************************************************************************
 * ISR prototypes
 **********************************************************************************************************************/
#if WDOG_W_CFG_NMI_SUPPORTED
void NMI_HandlerC(uint32_t * p_exception_args);
#endif

/***********************************************************************************************************************
 * Private global variables
 **********************************************************************************************************************/

/* WDT timeout value. */
static uint32_t g_wdog_w_timeout = WDOG_W_TIMEOUT_MAX;

/** Global pointer to control structure for use by the NMI callback.  */
static volatile wdog_w_instance_ctrl_t * gp_wdt_ctrl           = NULL;

/***********************************************************************************************************************
 * Global variables
 **********************************************************************************************************************/

/** Watchdog implementation of WDOG_W Driver  */
const wdt_api_t g_wdt_on_wdog_w =
{
    .open        = R_WDOG_W_Open,
    .refresh     = R_WDOG_W_Refresh,
    .statusGet   = R_WDOG_W_StatusGet,
    .statusClear = R_WDOG_W_StatusClear,
    .counterGet  = R_WDOG_W_CounterGet,
    .timeoutGet  = R_WDOG_W_TimeoutGet,
    .callbackSet = R_WDOG_W_CallbackSet,
};

/*******************************************************************************************************************//**
 * @addtogroup WDOG_W
 * @{
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Functions
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * Configure and start the WDT. Implements @ref wdt_api_t::open.
 *
 * This function should only be called once. The subsequent calls will have no effect.
 *
 * Example:
 * @snippet r_wdog_w_example.c R_WDOG_W_Open
 *
 * @retval FSP_SUCCESS              WDT successfully configured.
 * @retval FSP_ERR_ASSERTION        Null pointer, or one or more configuration options is invalid.
 * @retval FSP_ERR_ALREADY_OPEN     Module is already open.  This module can only be opened once.
 * @retval FSP_ERR_IN_USE           Watchdog timer value was not written due to busy.
 **********************************************************************************************************************/
fsp_err_t R_WDOG_W_Open (wdt_ctrl_t * const p_ctrl, wdt_cfg_t const * const p_cfg)
{
    wdog_w_instance_ctrl_t * p_instance_ctrl = (wdog_w_instance_ctrl_t *) p_ctrl;
    fsp_err_t               err;

    /* Check validity of the parameters */
    err = r_wdog_w_parameter_checking(p_instance_ctrl, p_cfg);
    FSP_ERROR_RETURN(FSP_SUCCESS == err, err);

    g_wdog_w_timeout = p_cfg->timeout;

    /* Unfreezing WDT. */
    SYS_WDOG->WATCHDOG_CTRL_REG_b.NMI_RST = WDOG_W_CTRLREG_NMIRST_NMI;
    r_wdog_w_unfreeze();

#if WDOG_W_CFG_NMI_SUPPORTED
    /* Register callback with BSP NMI ISR. */
    r_wdog_w_nmi_initialize(p_instance_ctrl, p_cfg);
#else
    /* Disable the WDT NMI. */
    SYS_WDOG->WATCHDOG_CTRL_REG_b.NMI_RST = WDOG_W_CTRLREG_NMIRST_RST;

    /* Eliminate toolchain warning when NMI output is not being used.  */
    FSP_PARAMETER_NOT_USED(r_wdog_w_nmi_internal_callback);
#endif

    err = r_wdog_w_reload_timeout_value();
    FSP_ERROR_RETURN(FSP_SUCCESS == err, err);

    p_instance_ctrl->wdt_open = WDOG_W_OPEN;

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Read timeout information for the watchdog timer. Implements @ref wdt_api_t::timeoutGet.
 *
 * @retval FSP_SUCCESS              WDT timeout information retrieved successfully.
 * @retval FSP_ERR_ASSERTION        Null Pointer.
 * @retval FSP_ERR_NOT_OPEN         Instance control block is not initialized.
 **********************************************************************************************************************/
fsp_err_t R_WDOG_W_TimeoutGet (wdt_ctrl_t * const p_ctrl, wdt_timeout_values_t * const p_timeout)
{
#if WDOG_W_CFG_PARAM_CHECKING_ENABLE
    wdog_w_instance_ctrl_t * p_instance_ctrl = (wdog_w_instance_ctrl_t *) p_ctrl;
    FSP_ASSERT(NULL != p_timeout);
    FSP_ASSERT(NULL != p_instance_ctrl);
    FSP_ERROR_RETURN(WDOG_W_OPEN == p_instance_ctrl->wdt_open, FSP_ERR_NOT_OPEN);
#else
    FSP_PARAMETER_NOT_USED(p_ctrl);
#endif

    /* Read the current timeout period value */
    p_timeout->timeout_clocks = g_wdog_w_timeout;

    /* Get the frequency of the clock supplying the watchdog */
    p_timeout->clock_frequency_hz = R_FSP_SystemClockHzGet(FSP_PRIV_CLOCK_WDT) / WDOG_W_PRV_CLC_DIV_RATIO;

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Set timeout value.
 *
 * @retval FSP_SUCCESS              WDT timeout value is set successfully.
 * @retval FSP_ERR_ASSERTION        Null Pointer.
 * @retval FSP_ERR_NOT_OPEN         Instance control block is not initialized.
 * @retval FSP_ERR_INVALID_SIZE     WDT timeout value is out of range.
 **********************************************************************************************************************/
fsp_err_t R_WDOG_W_TimeoutSet(wdt_ctrl_t * const p_ctrl, uint32_t timeout)
{
#if WDOG_W_CFG_PARAM_CHECKING_ENABLE
    wdog_w_instance_ctrl_t * p_instance_ctrl = (wdog_w_instance_ctrl_t *) p_ctrl;
    FSP_ASSERT(NULL != p_instance_ctrl);
    FSP_ERROR_RETURN(WDOG_W_OPEN == p_instance_ctrl->wdt_open, FSP_ERR_NOT_OPEN);
    FSP_ERROR_RETURN((WDOG_W_TIMEOUT_MIN <= timeout) && (WDOG_W_TIMEOUT_MAX >= timeout), FSP_ERR_INVALID_SIZE);
#else
    FSP_PARAMETER_NOT_USED(p_ctrl);
#endif

    /* Set the timeout value */
    g_wdog_w_timeout = timeout;

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Refresh the watchdog timer. Implements @ref wdt_api_t::refresh.
 *
 * Example:
 * @snippet r_wdog_w_example.c R_WDOG_W_Refresh
 *
 * @retval FSP_SUCCESS              WDT successfully refreshed.
 * @retval FSP_ERR_ASSERTION        p_ctrl is NULL.
 * @retval FSP_ERR_NOT_OPEN         Instance control block is not initialized.
 *
 * @note This function refresh the watchdog timer counter value with the timeout period configured when Open.
 **********************************************************************************************************************/
fsp_err_t R_WDOG_W_Refresh (wdt_ctrl_t * const p_ctrl)
{
#if WDOG_W_CFG_PARAM_CHECKING_ENABLE
    wdog_w_instance_ctrl_t * p_instance_ctrl = (wdog_w_instance_ctrl_t *) p_ctrl;
    FSP_ASSERT(NULL != p_instance_ctrl);
    FSP_ERROR_RETURN(WDOG_W_OPEN == p_instance_ctrl->wdt_open, FSP_ERR_NOT_OPEN);
#else
    FSP_PARAMETER_NOT_USED(p_ctrl);
#endif

    return r_wdog_w_reload_timeout_value();
}

/*******************************************************************************************************************//**
 * Read the WDT status flags. Implements @ref wdt_api_t::statusGet.
 * 
 * Example:
 * @snippet r_wdog_w_example.c R_WDOG_W_StatusGet
 *
 * @retval FSP_SUCCESS              WDT status successfully read.
 * @retval FSP_ERR_ASSERTION        Null pointer as a parameter.
 * @retval FSP_ERR_NOT_OPEN         Instance control block is not initialized.
 * @retval FSP_ERR_UNSUPPORTED      This function is only valid when NMI is supported.
 *
 * @note This function is only available in the following configurations and only provides
 *       @ref WDT_STATUS_UNDERFLOW flag.
 *       - NMI Support: Enabled
 *       - Reset Control: NMI Generated
 **********************************************************************************************************************/
fsp_err_t R_WDOG_W_StatusGet (wdt_ctrl_t * const p_ctrl, wdt_status_t * const p_status)
{
#if WDOG_W_CFG_NMI_SUPPORTED
 #if WDOG_W_CFG_PARAM_CHECKING_ENABLE
    wdog_w_instance_ctrl_t * p_instance_ctrl = (wdog_w_instance_ctrl_t *) p_ctrl;
    FSP_ASSERT(NULL != p_status);
    FSP_ASSERT(NULL != p_instance_ctrl);
    FSP_ERROR_RETURN(WDOG_W_OPEN == p_instance_ctrl->wdt_open, FSP_ERR_NOT_OPEN);
 #else
    FSP_PARAMETER_NOT_USED(p_ctrl);
 #endif

    if (SYS_WDOG->WATCHDOG_REG_b.WDOG_VAL_NEG)
    {
        *p_status = WDT_STATUS_UNDERFLOW;
    }
    else
    {
        *p_status = WDT_STATUS_NO_ERROR;
    }

    return FSP_SUCCESS;
#else
    FSP_PARAMETER_NOT_USED(p_ctrl);
    FSP_PARAMETER_NOT_USED(p_status);

    /* This function is only supported when the NMI is used. */
    FSP_RETURN(FSP_ERR_UNSUPPORTED);
#endif

}

/*******************************************************************************************************************//**
 * Clear the WDT status and error flags. Implements @ref wdt_api_t::statusClear.
 *
 * @retval FSP_ERR_UNSUPPORTED      This function is not supported.
 *
 * @note This function is not supported for RA6W1/RA6W2.
 **********************************************************************************************************************/
fsp_err_t R_WDOG_W_StatusClear (wdt_ctrl_t * const p_ctrl, const wdt_status_t status)
{
    FSP_PARAMETER_NOT_USED(p_ctrl);
    FSP_PARAMETER_NOT_USED(status);

    /* This function is not supported for RA6W1/RA6W2. */
    return FSP_ERR_UNSUPPORTED;
}

/*******************************************************************************************************************//**
 * Read the current count value of the WDT. Implements @ref wdt_api_t::counterGet.
 *
 * Example:
 * @snippet r_wdog_w_example.c R_WDOG_W_CounterGet
 *
 * @retval FSP_SUCCESS          WDT current count successfully read.
 * @retval FSP_ERR_ASSERTION    Null pointer passed as a parameter.
 * @retval FSP_ERR_NOT_OPEN     Instance control block is not initialized.
 * @retval FSP_ERR_IN_USE       The watchdog is write busy and cannot be read the current count value.
 **********************************************************************************************************************/
fsp_err_t R_WDOG_W_CounterGet (wdt_ctrl_t * const p_ctrl, uint32_t * const p_count)
{
#if WDOG_W_CFG_PARAM_CHECKING_ENABLE
    wdog_w_instance_ctrl_t * p_instance_ctrl = (wdog_w_instance_ctrl_t *) p_ctrl;
    FSP_ASSERT(NULL != p_count);
    FSP_ASSERT(NULL != p_instance_ctrl);
    FSP_ERROR_RETURN(WDOG_W_OPEN == p_instance_ctrl->wdt_open, FSP_ERR_NOT_OPEN);
#else
    FSP_PARAMETER_NOT_USED(p_ctrl);
#endif

    uint16_t timeout;

    /* The watchdog value cannot be read while watchdog is busy writing a new value */
    for (timeout = WDOG_W_PRV_WAIT_TIMEOUT_TICK; ((1U == SYS_WDOG->WATCHDOG_CTRL_REG_b.WRITE_BUSY) && timeout);
         timeout--)
    {
        /* Wait */
    }
    FSP_ERROR_RETURN(0 != timeout, FSP_ERR_IN_USE);

    *p_count = (uint32_t) SYS_WDOG->WATCHDOG_REG_b.WDOG_VAL;

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Updates the user callback and has option of providing memory for callback structure.
 * If p_context is set to NULL, exception argument is passed to the Callback instead of p_context.
 * Implements wdt_api_t::callbackSet
 *
 * @retval  FSP_SUCCESS                  Callback updated successfully.
 * @retval  FSP_ERR_ASSERTION            A required pointer is NULL.
 * @retval  FSP_ERR_NOT_OPEN             The control block has not been opened.
 **********************************************************************************************************************/
fsp_err_t R_WDOG_W_CallbackSet (wdt_ctrl_t * const          p_ctrl,
                               void (                    * p_callback)(wdt_callback_args_t *),
                               void * const          p_context,
                               wdt_callback_args_t * const p_callback_memory)
{
    wdog_w_instance_ctrl_t * p_instance_ctrl = (wdog_w_instance_ctrl_t *) p_ctrl;

#if WDOG_W_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(p_ctrl);
    FSP_ASSERT(p_callback);
    FSP_ERROR_RETURN(WDOG_W_OPEN == p_instance_ctrl->wdt_open, FSP_ERR_NOT_OPEN);
#endif

    /* Store callback and context */
    p_instance_ctrl->p_callback        = p_callback;
    p_instance_ctrl->p_context         = p_context;
    p_instance_ctrl->p_callback_memory = p_callback_memory;

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Freeze/unfreeze the watchdog timer.
 * 
 * Example:
 * @snippet r_wdog_w_example.c R_WDOG_W_Freeze
 *
 * @retval  FSP_SUCCESS                  Freeze watchdog timer successfully.
 * @retval  FSP_ERR_ASSERTION            A required pointer is NULL.
 * @retval  FSP_ERR_NOT_OPEN             The control block has not been opened.
 * @retval  FSP_ERR_NOT_ENABLED          Freezing watchdog timer is not allowed.
 * 
 * @note The watchdog timer can only be frozen in the following configurations. Otherwise, this function has no effect.
 *       - NMI Support: Enabled
 *       - Reset Control: NMI Generated
 **********************************************************************************************************************/
fsp_err_t R_WDOG_W_Freeze(wdt_ctrl_t * const p_ctrl, bool freeze)
{
#if WDOG_W_CFG_PARAM_CHECKING_ENABLE
    wdog_w_instance_ctrl_t * p_instance_ctrl = (wdog_w_instance_ctrl_t *) p_ctrl;
    FSP_ASSERT(p_ctrl);
    FSP_ERROR_RETURN(WDOG_W_OPEN == p_instance_ctrl->wdt_open, FSP_ERR_NOT_OPEN);
#else
    FSP_PARAMETER_NOT_USED(p_ctrl);
#endif

    if (WDOG_W_CTRLREG_NMIRST_NMI != SYS_WDOG->WATCHDOG_CTRL_REG_b.NMI_RST)
    {
        FSP_RETURN(FSP_ERR_NOT_ENABLED);
    }

    if (freeze)
    {
        r_wdog_w_freeze();
    }
    else
    {
        r_wdog_w_unfreeze();
    }

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @} (end addtogroup WDOG_W)
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Private Functions
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * Internal NMI ISR callback which calls the user provided callback passing the context provided by the user.
 *
 * @param[in]    p_exception_args   Pointer to exception argument.
 **********************************************************************************************************************/
static void r_wdog_w_nmi_internal_callback (uint32_t * p_exception_args)
{
    /* Call user registered callback */
    if (NULL != gp_wdt_ctrl)
    {
        if (NULL != gp_wdt_ctrl->p_callback)
        {
            wdt_callback_args_t   args;

            /* Store callback arguments in memory provided by user if available. */
            wdt_callback_args_t * p_args = gp_wdt_ctrl->p_callback_memory;
            if (NULL == p_args)
            {
                /* Store on stack */
                p_args = &args;
            }
            else
            {
                /* Save current arguments on the stack in case this is a nested interrupt. */
                args = *p_args;
            }

            /* When the input of callback argument is NULL,
             * pointer to exception argument is passed to the callback argument. */
            if (NULL == gp_wdt_ctrl->p_context)
            {
                p_args->p_context = (void *) p_exception_args;
            }
            else
            {
                p_args->p_context = gp_wdt_ctrl->p_context;
            }

            /* Call user callback. */
            gp_wdt_ctrl->p_callback(p_args);
            if (NULL != gp_wdt_ctrl->p_callback_memory)
            {
                /* Restore callback memory in case this is a nested interrupt. */
                *gp_wdt_ctrl->p_callback_memory = args;
            }
        }
    }
}

/*******************************************************************************************************************//**
 * Initialize the NMI.
 *
 * @param[in]    p_instance_ctrl   Pointer to instance control structure
 * @param[in]    p_cfg             Pointer to configuration structure
 **********************************************************************************************************************/
#if WDOG_W_CFG_NMI_SUPPORTED
static void r_wdog_w_nmi_initialize (wdog_w_instance_ctrl_t * const p_instance_ctrl, wdt_cfg_t const * const p_cfg)
{
    /* Initialize global pointer to WDT for NMI callback use.  */
    gp_wdt_ctrl                           = p_instance_ctrl;

    p_instance_ctrl->p_callback           = p_cfg->p_callback;
    p_instance_ctrl->p_context            = p_cfg->p_context;
    p_instance_ctrl->p_callback_memory    = NULL;

    /* Enable the WDT NMI. */
    SYS_WDOG->WATCHDOG_CTRL_REG_b.NMI_RST = WDOG_W_CTRLREG_NMIRST_NMI;
}
#endif

/*******************************************************************************************************************//**
 * Parameter checking function for WDT Open
 *
 * @param[in]    p_instance_ctrl   Pointer to instance control structure
 * @param[in]    p_cfg             Pointer to configuration structure
 *
 * @retval FSP_SUCCESS              WDT successfully configured.
 * @retval FSP_ERR_ASSERTION        Null pointer, or one or more configuration options is invalid.
 * @retval FSP_ERR_ALREADY_OPEN     Module is already open.  This module can only be opened once.
 * @retval FSP_ERR_INVALID_SIZE     Timeout value setting is out of range.
 **********************************************************************************************************************/
static fsp_err_t r_wdog_w_parameter_checking (wdog_w_instance_ctrl_t * const p_instance_ctrl,
                                             wdt_cfg_t const * const       p_cfg)
{
#if WDOG_W_CFG_PARAM_CHECKING_ENABLE

    /* Check that control and config structure pointers are valid. */
    FSP_ASSERT(NULL != p_cfg);
    FSP_ASSERT(NULL != p_instance_ctrl);
    FSP_ERROR_RETURN(WDOG_W_OPEN != p_instance_ctrl->wdt_open, FSP_ERR_ALREADY_OPEN);

    /* Check timeout parameter is supported by WDT. */

    /* Enum checking is done here because some enums in wdt_timeout_t are not supported by the WDT peripheral (they are
     * included for other implementations of the watchdog interface). */
    FSP_ERROR_RETURN((p_cfg->timeout >= WDOG_W_TIMEOUT_MIN) && (p_cfg->timeout <= WDOG_W_TIMEOUT_MAX), FSP_ERR_INVALID_SIZE);

 #if WDOG_W_CFG_NMI_SUPPORTED

    /* NMI output mode. */
    if (WDT_RESET_CONTROL_NMI == p_cfg->reset_control)
    {
        FSP_ASSERT(NULL != p_cfg->p_callback);
    }
    /* Reset ouput mode. */
    else if (WDT_RESET_CONTROL_RESET == p_cfg->reset_control)
    {
        FSP_ASSERT(NULL == p_cfg->p_callback);
    }
    /* Invalid parameter case. */
    else
    {
        FSP_ERROR_RETURN(false, FSP_ERR_ASSERTION);
    }

 #else
    FSP_ASSERT(WDT_RESET_CONTROL_RESET == p_cfg->reset_control);
    FSP_ASSERT(NULL == p_cfg->p_callback);
 #endif
#else
    FSP_PARAMETER_NOT_USED(p_instance_ctrl);
    FSP_PARAMETER_NOT_USED(p_cfg);
#endif

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Reload the watchdog timeout count value.
 * 
 * @retval FSP_SUCCESS          Watchdog timer value was written successfully.
 * @retval FSP_ERR_IN_USE       Watchdog timer value was not written due to busy.
 **********************************************************************************************************************/
static fsp_err_t r_wdog_w_reload_timeout_value (void)
{
    uint16_t timeout;

    /* Cannot write WATCHDOG_REG while WRITE_BUSY */
    for (timeout = WDOG_W_PRV_WAIT_TIMEOUT_TICK; ((1U == SYS_WDOG->WATCHDOG_CTRL_REG_b.WRITE_BUSY) && timeout);
         timeout--)
    {
        /* Wait */
    }
    FSP_ERROR_RETURN(0 != timeout, FSP_ERR_IN_USE);

    /* Write WATCHDOG_REG */
    SYS_WDOG->WATCHDOG_REG = (WDOG_W_REG_WEN_ENABLE << SYS_WDOG_WATCHDOG_REG_WDOG_WEN_Pos) |
                             (0U << SYS_WDOG_WATCHDOG_REG_WDOG_VAL_NEG_Pos) |
                             (g_wdog_w_timeout << SYS_WDOG_WATCHDOG_REG_WDOG_VAL_Pos);

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Freeze the watchdog timer.
 **********************************************************************************************************************/
static void r_wdog_w_freeze (void)
{
    SYS_WDOG->WATCHDOG_CTRL_REG_b.WDOG_FREEZE_EN = WDOG_W_CTRLREG_FREEZEEN_ENABLE;
    R_BSP_PeripheralFreeze(BSP_FREEZE_PERIPHERAL_SYS_WDOG);
}

/*******************************************************************************************************************//**
 * Unfreeze the watchdog timer.
 **********************************************************************************************************************/
static void r_wdog_w_unfreeze (void)
{
    SYS_WDOG->WATCHDOG_CTRL_REG_b.WDOG_FREEZE_EN = WDOG_W_CTRLREG_FREEZEEN_ENABLE;
    R_BSP_PeripheralUnFreeze(BSP_FREEZE_PERIPHERAL_SYS_WDOG);
    SYS_WDOG->WATCHDOG_CTRL_REG_b.WDOG_FREEZE_EN = WDOG_W_CTRLREG_FREEZEEN_DISABLE;
}

#if WDOG_W_CFG_NMI_SUPPORTED
/*******************************************************************************************************************//**
 * Non-maskable interrupt handler.
 * 
 * @param[in]    p_exception_args   Pointer to exception argument.
 **********************************************************************************************************************/
void NMI_HandlerC (uint32_t * p_exception_args)
{
    /* Wait for stabilization of WDT. */
    R_BSP_SoftwareDelay(100, BSP_DELAY_UNITS_MICROSECONDS);

    r_wdog_w_nmi_internal_callback(p_exception_args);
}
#endif

void bsp_wdt_freeze_cfg (void)
{
    /* Stub function - Do not remove !!!
     * This function should be compiled instead of the Weak bsp_wdt_freeze_cfg.
     * Details:
     * WDT is enabled by default.
     * In case wdt module does not include in the project, WDT should be disabled.
     * bsp_wdt_freeze_cfg weak function - will disable WDT in case wdt module is not included.
     * bsp_wdt_freeze_cfg normal function - will do nothing, hence WDT remains enabled in case wdt included. */
}
