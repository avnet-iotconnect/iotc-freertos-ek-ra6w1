/**
 * \addtogroup PLA_DRI_MEMORY
 * \{
 * \addtogroup HW_CACHE iCache Controller
 * \{
 * \brief iCache Controller DA1487x specific LLD API
 */

/**
 *****************************************************************************************
 *
 * @file hw_cache_da1487x.h
 *
 * @brief Definition of DA1487x specific API for the iCache Controller Low Level Driver.
 *
 * Copyright (C) 2020 Dialog Semiconductor.
 * This computer program includes Confidential, Proprietary Information
 * of Dialog Semiconductor. All Rights Reserved.
 *
 *****************************************************************************************
 */

#ifndef HW_CACHE_DA1487X_H_
#define HW_CACHE_DA1487X_H_

#if ((DEVICE_FAMILY == DA1487X) || (DEVICE_FAMILY == DA1640X))

#include <sdk_defs.h>

/*
 * ENUMERATION DEFINITIONS
 *****************************************************************************************
 */

/**
 * \brief Cacheable flash Region Sizes as defined in DA1487x datasheet
 *
 */

typedef enum {
        HW_CACHE_FLASH_REGION_SZ_256KB = 0,
        HW_CACHE_FLASH_REGION_SZ_512KB = 1,
        HW_CACHE_FLASH_REGION_SZ_1MB = 2,
        HW_CACHE_FLASH_REGION_SZ_2MB = 3,
        HW_CACHE_FLASH_REGION_SZ_4MB = 4,
        HW_CACHE_FLASH_REGION_SZ_8MB = 5,
        HW_CACHE_FLASH_REGION_SZ_16MB = 6,
        HW_CACHE_FLASH_REGION_SZ_32MB = 7,
        HW_CACHE_FLASH_REGION_SZ_64MB = 8,
        HW_CACHE_FLASH_REGION_SZ_128MB = 9,
        HW_CACHE_FLASH_REGION_SZ_INVALID,              /* Used as iteration terminator */
} HW_CACHE_FLASH_REGION_SZ;

typedef uint16_t flash_region_base_t;
typedef uint16_t flash_region_offset_t;

/* With a 64KB cacheable resolution (i.e. the target memory is addressed in blocks of 64KB),
 * and a size of 131072KB of the target memory (MEMORY_OQSPIC_SIZE) the maximum cacheable length
 * is 131072 = 131072KB / 64KB. In hex: 0x8000000 / 0x10000 = 0x800. */
#define HW_CACHE_CACHEABLE_RESOLUTION     0x10000                /* Blocks of 64KB in size */
#define HW_CACHE_CACHEABLE_LEN_MAX        MEMORY_OQSPIC_SIZE / HW_CACHE_CACHEABLE_RESOLUTION

#define HW_CACHE_FLASH_MIN_REGION_BASE         0xA00    // 0x1800
#define HW_CACHE_FLASH_MAX_REGION_BASE         0xDFF    //0x1FFF
#define HW_CACHE_FLASH_DEFAULT_REGION_BASE     (HW_CACHE_FLASH_MIN_REGION_BASE)

#define HW_CACHE_FLASH_MIN_REGION_OFFSET       0x0
#define HW_CACHE_FLASH_MAX_REGION_OFFSET       0xFFF
#define HW_CACHE_FLASH_DEFAULT_REGION_OFFSET   (HW_CACHE_FLASH_MIN_REGION_OFFSET)
#define HW_CACHE_FLASH_DEFAULT_REGION_SZ       HW_CACHE_FLASH_REGION_SZ_512KB

/*
 * FAMILY SPECIFIC GENERIC FUNCTIONALITY DEFINITIONS
 *****************************************************************************************
 */

/**
 * \brief Enables the iCache Controller
 *
 * The iCache Controller is enabled by setting the CACHERAM_MUX to '1'. This action enables
 * the corresponding HW block, letting the RAM memory of the block be visible only to the
 * iCache Controller for caching purposes.
 *
 */
__STATIC_INLINE void hw_cache_enable(void)
{
        REG_SET_BIT(CRG_TOP, SYS_CTRL_REG, CACHERAM_MUX);
        /* Wait until the CACHERAM_MUX=1 (because of the APB Bridge). */
        while (REG_GETF(CRG_TOP, SYS_CTRL_REG, CACHERAM_MUX) != 1) {}
}

/**
 * \brief Disables the iCache Controller
 *
 * The iCache Controller is disabled by setting the CACHERAM_MUX to '0'. This action disables
 * the corresponding HW block, bypassing the iCache Controller for all read requests
 * and letting the RAM memory of the block be visible in the entire memory space.
 *
 */
__STATIC_INLINE void hw_cache_disable(void)
{
        REG_CLR_BIT(CRG_TOP, SYS_CTRL_REG, CACHERAM_MUX);
        /* Wait until the CACHERAM_MUX=0 (because of the APB Bridge). */
        while (REG_GETF(CRG_TOP, SYS_CTRL_REG, CACHERAM_MUX) != 0) {}
}

/**
 * \brief Checks if the iCache Controller is enabled
 *
 * \return True if the iCache Controller is enabled, False otherwise.
 *
 *
 */
__STATIC_INLINE bool hw_cache_is_enabled(void)
{
        return REG_GETF(CRG_TOP, SYS_CTRL_REG, CACHERAM_MUX);
}


/**
 * \brief Enable flushing the iCache Controller (cache RAM cells) contents. For debugging only.
 */
__STATIC_INLINE void hw_cache_enable_flushing(void)
{
        REG_CLR_BIT(CACHE, CACHE_CTRL2_REG, CACHE_FLUSH_DISABLE);
}

/**
 * \brief Disable flushing the iCache Controller (cache RAM cells) contents. For debugging only.
 */
__STATIC_INLINE void hw_cache_disable_flushing(void)
{
        REG_SET_BIT(CACHE, CACHE_CTRL2_REG, CACHE_FLUSH_DISABLE);
}

/**
 * \brief Checks if the iCache Controller flushing is disabled. For debugging only.
 *
 * \return True if the iCache Controller flushing is disabled, False otherwise.
 *
 *
 */
__STATIC_INLINE bool hw_cache_is_flushing_disabled(void)
{
        return REG_GETF(CACHE, CACHE_CTRL2_REG, CACHE_FLUSH_DISABLE);
}

/**
 * \brief Check if the flushing process is complete
 *
 * \return True if flushing is complete, False if the iCache controller flushing is still
 *              in progress or there is no pending flushing termination indication as it
 *              will be cleared via a prior hw_cache_clear_flushed() call.
 */
__STATIC_INLINE bool hw_cache_is_flushed(void)
{
        return REG_GETF(CACHE, CACHE_CTRL2_REG, CACHE_FLUSHED);
}

/**
 * \brief Clear the indication that a prior flushing process is complete
 */
__STATIC_INLINE void hw_cache_clear_flushed(void)
{
        REG_CLR_BIT(CACHE, CACHE_CTRL2_REG, CACHE_FLUSHED);
}

/**
 * \brief Flush the cache contents
 *
 * Note: The very first flushing occurred after power on reset when the iCache Controller
 * is enabled for the first time by the booter.
 */
__STATIC_INLINE void hw_cache_flush(void)
{
        if (!hw_cache_is_flushing_disabled()) {
                hw_cache_disable();
                hw_cache_enable();
                /* Wait for the completion of the flushing process */
                while (hw_cache_is_flushed() == 0);

                /* Clear the indication that the flushing is complete */
                hw_cache_clear_flushed();
        }
}

#endif /* DA1487X */

#endif /* HW_CACHE_DA1487X_H_ */

/**
 * \}
 * \}
 */
