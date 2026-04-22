/* From file in legacy repository (generated during build):
 * projects/rrq61x/rrq61x/RRQ61000-Debug/mem.ld */

        MEMORY
        {
                ROM (rx) : ORIGIN = (0x0), LENGTH = ((40 * 64 * 1024))
                RAM (rw) : ORIGIN = (0x20000000), LENGTH = ((703 * 1024)-(0x200B0000-(0x200A5000)))
                IPDRAM(rw): ORIGIN = 0x200A5000, LENGTH = ((0x200A8B00-1)-0x200A5000 +1)
                FRAGRAM(rw): ORIGIN = (((0x200AA500-1))+1), LENGTH = (0x200B0000-(((0x200AA500-1))+1)-0x400)
                lamac_memory (!rx) : ORIGIN = 0x61100000, LENGTH = 1024K
                machw_mib_memory (!rx) : ORIGIN = 0x60B00800, LENGTH = 4K
        }

/*
 * There will be a link error if there is not this amount of 
 * RAM free at the end. 
 */
_Minimum_RAM_Margin = 16;

PROVIDE (_RAM_Segment_End = ORIGIN(RAM) + LENGTH(RAM));
PROVIDE (_Heap5_Begin = __non_retention_slowram_end__ + _Minimum_RAM_Margin);
PROVIDE (_Heap5_Limit = _RAM_Segment_End - _Minimum_RAM_Margin);

/* From file in legacy repository (generated during build):
 * projects/rrq61x/rrq61x/RRQ61000-Debug/sections.ld */
GROUP(libgcc.a libc.a libm.a libnosys.a)
ENTRY(Reset_Handler)
__PllOnFuncAddr = 0x160;
SECTIONS
{
        .init_text :
        {
                KEEP(*(.isr_vector))
                KEEP(*(.application_vectors))
                . = __PllOnFuncAddr;
                __pllon_start__ = .;
                KEEP(*(.pllon))
                __pllon_section_end = .;
                . = 0x200;
                __Vectors_End = .;
                __Vectors_Size = __Vectors_End - __Vectors;
                *(text_reset*)
        } > ROM
     HW_MIB (NOLOAD):
     {
         _smachw_mib = . ;
         *(MACHWMIB)
         _emachw_mib = . ;
     } > machw_mib_memory
        .text :
        {
                *(EXCLUDE_FILE(*libnosys.a:sbrk.o
                               *libgcc.a:_aeabi_uldivmod.o
                               *libgcc.a:_muldi3.o
                               *libgcc.a:_dvmd_tls.o
                               *libgcc.a:bpabi.o
                               *libgcc.a:_udivdi3.o
                               *libgcc.a:_clzdi2.o
                               *libgcc.a:_clzsi2.o) .text*)
                . = ALIGN(4);
                __start_adapter_init_section = .;
                KEEP(*(adapter_init_section))
                __stop_adapter_init_section = .;
                __start_bus_init_section = .;
                KEEP(*(bus_init_section))
                __stop_bus_init_section = .;
                __start_device_init_section = .;
                KEEP(*(device_init_section))
                __stop_device_init_section = .;
                KEEP(*(.init))
                KEEP(*(.fini))
                *crtbegin.o(.ctors)
                *crtbegin?.o(.ctors)
                *(EXCLUDE_FILE(*crtend?.o *crtend.o) .ctors)
                *(SORT(.ctors.*))
                *(.ctors)
                *crtbegin.o(.dtors)
                *crtbegin?.o(.dtors)
                *(EXCLUDE_FILE(*crtend?.o *crtend.o) .dtors)
                *(SORT(.dtors.*))
                *(.dtors)
                . = ALIGN(4);
                PROVIDE_HIDDEN (__preinit_array_start = .);
                KEEP(*(.preinit_array))
                PROVIDE_HIDDEN (__preinit_array_end = .);
                . = ALIGN(4);
                PROVIDE_HIDDEN (__init_array_start = .);
                KEEP(*(SORT(.init_array.*)))
                KEEP(*(.init_array))
                PROVIDE_HIDDEN (__init_array_end = .);
                . = ALIGN(4);
                PROVIDE_HIDDEN (__fini_array_start = .);
                KEEP(*(SORT(.fini_array.*)))
                KEEP(*(.fini_array))
                PROVIDE_HIDDEN (__fini_array_end = .);
                *(.rodata*)
                KEEP(*(.eh_frame*))
        } > ROM
        .ARM.extab :
        {
                *(.ARM.extab* .gnu.linkonce.armextab.*)
        } > ROM
        __exidx_start = .;
        .ARM.exidx :
        {
                *(.ARM.exidx* .gnu.linkonce.armexidx.*)
        } > ROM
        __exidx_end = .;
        .copy.table :
        {
                . = ALIGN(4);
                __copy_table_start__ = .;
                LONG (__etext)
                LONG (__retention_ram_init_start__)
                LONG ((__retention_ram_init_end__ - __retention_ram_init_start__))
                LONG (__etext + ((__retention_ram_init_end__ - __retention_ram_init_start__)))
                LONG (__snc_section_start__)
                LONG ((__snc_section_end__ - __snc_section_start__))
                LONG (__etext + ((__retention_ram_init_end__ - __retention_ram_init_start__)) + ((__snc_section_end__ - __snc_section_start__)))
                LONG (__non_retention_ram_init_start__)
                LONG ((__non_retention_ram_init_end__ - __non_retention_ram_init_start__))
                __copy_table_end__ = .;
        } > ROM
        .zero.table :
        {
                . = ALIGN(4);
                __zero_table_start__ = .;
                LONG (__bss_start__)
                LONG (__bss_end__ - __bss_start__)
                LONG (__retention_ram_zi_start__)
                LONG (__retention_ram_zi_end__ - __retention_ram_zi_start__)
                __zero_table_end__ = .;
        } > ROM
        __etext = .;
        .retention_ram_uninit (NOLOAD) :
        {
                __retention_ram_uninit_start__ = .;
                ASSERT( . == ORIGIN(RAM), ".retention_ram_uninit section moved!");
                KEEP(*(nmi_info))
                KEEP(*(hard_fault_info))
                KEEP(*(oops_tag_info))
                KEEP(*(oops_stack_info))
                KEEP(*(oops_thread_info))
                KEEP(*(cont_fault_info))
                KEEP(*(retention_mem_uninit))
                ASSERT( . <= __retention_ram_uninit_start__ + (716),
                        "retention_ram_uninit section overflowed! Increase dg_configRETAINED_UNINIT_SECTION_SIZE.");
                . = __retention_ram_uninit_start__ + (716);
                __retention_ram_uninit_end__ = .;
        } > RAM
        .retention_ram_init : AT (0x0 +__etext)
        {
                __retention_ram_init_start__ = .;

                /* ALIGN(32) is required to put FreeRTOS kernel data in a
                 * dedicated Cortex-M33 MPU region. */
                . = ALIGN(32);
                __privileged_data_start__ = .;
                *(privileged_data)
                __privileged_data_end__ = .;
                . = ALIGN(32);

                *(text_retained)
                KEEP(*(.code_in_ram*))
                *libnosys.a:sbrk.o (.text*)
                *libgcc.a:_aeabi_uldivmod.o (.text*)
                *libgcc.a:_muldi3.o (.text*)
                *libmacsw.a:txl_he.o (.text*)
                *libmacsw.a:txl_cntrl.o (.text*)
                *libmacsw.a:txl_hwdesc.o (.text*)
                *libgcc.a:_dvmd_tls.o (.text*)
                *libgcc.a:bpabi.o (.text*)
                *libgcc.a:_udivdi3.o (.text*)
                *libgcc.a:_clzdi2.o (.text*)
                *libgcc.a:_clzsi2.o (.text*)
                *(privileged_data_init)
                *(.retention)
                *(vtable)
                *(retention_mem_init)
                *(retention_mem_const)
                *libg_nano.a:* (.data*)
                *libnosys.a:* (.data*)
                *libgcc.a:* (.data*)
                *libble_stack_da1469x.a:* (.data*)
                *crtbegin.o (.data*)
                *libmacsw.a:txl_he.o (.data*)
                KEEP(*(.jcr*))
                . = ALIGN(4);
                __retention_ram_init_end__ = .;
        } > RAM
        .retention_ram_zi (NOLOAD) :
        {
                __retention_ram_zi_start__ = .;
                *(privileged_data_zi)
                *(retention_mem_zi)
                *libg_nano.a:* (.bss*)
                *libnosys.a:* (.bss*)
                *libgcc.a:* (.bss*)
                *libble_stack_da1469x.a:* (.bss*)
                *crtbegin.o (.bss*)
                *(os_heap)
                . = ALIGN(8);
                __HeapBase = .;
                __end__ = .;
                end = __end__;
                KEEP(*(.heap*))
                __HeapLimit = .;
                __retention_ram_zi_end__ = .;
        } > RAM
        .stack_section (NOLOAD):
        {
                . = ALIGN(8);
                __StackLimit = .;
                /* Main stack */
                KEEP(*(.stack))
                . = ALIGN(8);
                __StackTop = .;
                PROVIDE(__stack = __StackTop);
                /* Thread stacks */
                KEEP(*(.stack*))
                . = ALIGN(8);
                __StackTopAll = .;
        } > RAM
        .snc_section : AT (0x0 + __etext + ((__retention_ram_init_end__ - __retention_ram_init_start__)))
        {
                __snc_section_start__ = .;
                . = ALIGN(4);
                KEEP(*(.snc_region*))
                . = ALIGN(4);
                __snc_section_end__ = .;
        } > RAM
        __non_retention_ram_start__ = .;
        .non_retention_ram_init : AT (0x0 + __etext + ((__retention_ram_init_end__ - __retention_ram_init_start__)) + ((__snc_section_end__ - __snc_section_start__)))
        {
                __non_retention_ram_init_start__ = .;
                . = ALIGN(4);
                *(EXCLUDE_FILE(*libg_nano.a:* *libnosys.a:* *libgcc.a:* *libble_stack_da1469x.a:* *crtbegin.o) .data*)
                *(LARAM*)
                . = ALIGN(4);
                __non_retention_ram_init_end__ = .;
        } > RAM
        .bss :
        {
                . = ALIGN(4);
                __bss_start__ = .;
                *(EXCLUDE_FILE(*libg_nano.a:* *libnosys.a:* *libgcc.a:* *libble_stack_da1469x.a:* *crtbegin.o) .bss*)
                *(COMMON)
                . = ALIGN(4);
                __bss_end__ = .;
        } > RAM
        __non_retention_ram_end__ = .;
        .non_retention_ram_uninit (NOLOAD) :
        {
                __non_retention_slowram_start__ = .;
                . = ALIGN(4);
                __slowram_start__ = .;
                *(slowram)
                . = ALIGN(4);
                __slowram_end__ = .;
                __non_retention_slowram_end__ = .;
        } > RAM
        .non_retention_fragram_uninit (NOLOAD) :
        {
                __non_retention_fragram_start__ = .;
                . = ALIGN(4);
                __fragram_start__ = .;
                *(fragram)
                . = ALIGN(4);
                __fragram_end__ = .;
                __non_retention_fragram_end__ = ORIGIN(FRAGRAM) + LENGTH(FRAGRAM) - 1;
        } > FRAGRAM
        .ipdregion_uninit (NOLOAD) :
        {
                __ipdregion_sram_start__ = .;
                . = ALIGN(4);
                __ipdregion_start__ = .;
                *(ipdregion)
                . = ALIGN(4);
                __ipdregion_end__ = .;
                __ipdregion_sram_end__ = ORIGIN(IPDRAM) + LENGTH(IPDRAM) - 1;
        } > IPDRAM
        __unused_ram_start__ = . + 1;
        ASSERT(__etext + (__retention_ram_init_end__ - __retention_ram_init_start__) + (__snc_section_end__ - __snc_section_start__) + (__non_retention_ram_init_end__ - __non_retention_ram_init_start__) <
                        ORIGIN(ROM) + ( 0x00280000 ), "ROM space overflowed")
        ASSERT(__fragram_end__ < (ORIGIN(FRAGRAM) + LENGTH(FRAGRAM)), "FRAGRAM space overflowed")
}

/* From file in legacy repository (generated during build):
 * sdk/bsp/ldscripts/non_ble_projects/da1640x_Romcrypto.symbols */

/**************************************************
 *   Exported Symbols from Romcrypto.elf
 **************************************************/
	CC_PalVerifyCertValidity         = 0x0f042901 ; 	/* FUNC   GLOBAL  DEFAULT */
	CCCertFieldsParse                = 0x0f0403c5 ; 	/* FUNC   GLOBAL  DEFAULT */
	SBRT_SwVersionGet                = 0x0f044675 ; 	/* FUNC   GLOBAL  DEFAULT */
	CC_BsvOTPWordRead                = 0x0f04184d ; 	/* FUNC   GLOBAL  DEFAULT */
	ProcessAesCcm                    = 0x0f04353d ; 	/* FUNC   GLOBAL  DEFAULT */
	UTIL_X509VerifyPubKey            = 0x0f044b0d ; 	/* FUNC   GLOBAL  DEFAULT */
	SBRT_SHA256                      = 0x0f044605 ; 	/* FUNC   GLOBAL  DEFAULT */
	UTIL_MemCmp                      = 0x0f0447e1 ; 	/* FUNC   GLOBAL  DEFAULT */
	SBRT_CryptoImageInit             = 0x0f044349 ; 	/* FUNC   GLOBAL  DEFAULT */
	CC_BsvOTPWordWrite               = 0x0f04188d ; 	/* FUNC   GLOBAL  DEFAULT */
	SBRT_OTPWordRead                 = 0x0f044519 ; 	/* FUNC   GLOBAL  DEFAULT */
	CCCertSecDbgParse                = 0x0f0404ed ; 	/* FUNC   GLOBAL  DEFAULT */
	CC_BsvCoreClkGatingEnable        = 0x0f040f7d ; 	/* FUNC   GLOBAL  DEFAULT */
	RSA_PKA_SetRegsMapTab            = 0x0f043db9 ; 	/* FUNC   GLOBAL  DEFAULT */
	InitBsvAes                       = 0x0f042ec9 ; 	/* FUNC   GLOBAL  DEFAULT */
	BsvAes                           = 0x0f040041 ; 	/* FUNC   GLOBAL  DEFAULT */
	BsvCryptoImageInit               = 0x0f0400a9 ; 	/* FUNC   GLOBAL  DEFAULT */
	CC_BsvPrivModeSet                = 0x0f0419d9 ; 	/* FUNC   GLOBAL  DEFAULT */
	CC_BsvSecModeSet                 = 0x0f041cf5 ; 	/* FUNC   GLOBAL  DEFAULT */
	SBRT_HalWaitInterrupt            = 0x0f044401 ; 	/* FUNC   GLOBAL  DEFAULT */
	CC_BsvLcsGetAndInit              = 0x0f0415d9 ; 	/* FUNC   GLOBAL  DEFAULT */
	SBROM_CryptoFinishDriver         = 0x0f043fd9 ; 	/* FUNC   GLOBAL  DEFAULT */
	InitContextToSha256              = 0x0f043021 ; 	/* FUNC   GLOBAL  DEFAULT */
	CC_BsvSHA256                     = 0x0f041c8d ; 	/* FUNC   GLOBAL  DEFAULT */
	CCSbCalcPublicKeyHASH            = 0x0f0409cd ; 	/* FUNC   GLOBAL  DEFAULT */
	SBRT_RSA_PSS_Verify              = 0x0f044521 ; 	/* FUNC   GLOBAL  DEFAULT */
	UTIL_X509VerifySubjectName       = 0x0f044d21 ; 	/* FUNC   GLOBAL  DEFAULT */
	CCCommonKeyCertVerify            = 0x0f040985 ; 	/* FUNC   GLOBAL  DEFAULT */
	FreeHash                         = 0x0f042e39 ; 	/* FUNC   GLOBAL  DEFAULT */
	_RSA_PKA_ExecOperation           = 0x0f044d55 ; 	/* FUNC   GLOBAL  DEFAULT */
	SBRT_LcsGet                      = 0x0f0444e1 ; 	/* FUNC   GLOBAL  DEFAULT */
	NVM_SetSwVersion                 = 0x0f043239 ; 	/* FUNC   GLOBAL  DEFAULT */
	CC_BsvFatalErrorSet              = 0x0f040f95 ; 	/* FUNC   GLOBAL  DEFAULT */
	SBROM_CryptoInitDriver           = 0x0f044019 ; 	/* FUNC   GLOBAL  DEFAULT */
	CCSbVerifySignature              = 0x0f040c55 ; 	/* FUNC   GLOBAL  DEFAULT */
	FinishAesCcm                     = 0x0f042b69 ; 	/* FUNC   GLOBAL  DEFAULT */
	CC_BsvAesCcm                     = 0x0f040cb9 ; 	/* FUNC   GLOBAL  DEFAULT */
	UTIL_X509VerifyIssuerName        = 0x0f044aed ; 	/* FUNC   GLOBAL  DEFAULT */
	CCCommonContentCertVerify        = 0x0f040841 ; 	/* FUNC   GLOBAL  DEFAULT */
	InitBsvHash                      = 0x0f042f19 ; 	/* FUNC   GLOBAL  DEFAULT */
	CCSbSetNvCounter                 = 0x0f040bed ; 	/* FUNC   GLOBAL  DEFAULT */
	CC_BsvOemAssetProvisioningOpen   = 0x0f0418d9 ; 	/* FUNC   GLOBAL  DEFAULT */
	get_romcrypto_info               = 0x0f044e8d ; 	/* FUNC   GLOBAL  DEFAULT */
	SBRT_HalClearInterruptBit        = 0x0f0443f1 ; 	/* FUNC   GLOBAL  DEFAULT */
	CC_BsvSwVersionGet               = 0x0f0424b9 ; 	/* FUNC   GLOBAL  DEFAULT */
	SBRT_PubKeyHashGet               = 0x0f04451d ; 	/* FUNC   GLOBAL  DEFAULT */
	ProcessAes                       = 0x0f043269 ; 	/* FUNC   GLOBAL  DEFAULT */
	UTIL_X509GetSignature            = 0x0f0448b5 ; 	/* FUNC   GLOBAL  DEFAULT */
	CC_BsvRMAModeEnable              = 0x0f041bbd ; 	/* FUNC   GLOBAL  DEFAULT */
	UTIL_Asn1ReadItemVerifyTagFW     = 0x0f04476d ; 	/* FUNC   GLOBAL  DEFAULT */
	SB_HalClearInterruptBit          = 0x0f044679 ; 	/* FUNC   GLOBAL  DEFAULT */
	FinishHash                       = 0x0f042d0d ; 	/* FUNC   GLOBAL  DEFAULT */
	UTIL_ReverseWordsBuff            = 0x0f044891 ; 	/* FUNC   GLOBAL  DEFAULT */
	SBRT_ReverseMemCopy              = 0x0f0445c9 ; 	/* FUNC   GLOBAL  DEFAULT */
	ProcessBsvHash                   = 0x0f0439ad ; 	/* FUNC   GLOBAL  DEFAULT */
	NVM_GetSwVersion                 = 0x0f0431c5 ; 	/* FUNC   GLOBAL  DEFAULT */
	CC_HalWaitInterrupt              = 0x0f0427a5 ; 	/* FUNC   GLOBAL  DEFAULT */
	UTIL_ReverseMemCopy              = 0x0f044855 ; 	/* FUNC   GLOBAL  DEFAULT */
	SBRT_ImageLoadAndVerify          = 0x0f044409 ; 	/* FUNC   GLOBAL  DEFAULT */
	UTIL_X509VerifyStr               = 0x0f044c89 ; 	/* FUNC   GLOBAL  DEFAULT */
	SBRT_MemCmp                      = 0x0f0444e5 ; 	/* FUNC   GLOBAL  DEFAULT */
	CC_BsvLcsGet                     = 0x0f0415a1 ; 	/* FUNC   GLOBAL  DEFAULT */
	SB_HalWaitInterrupt              = 0x0f04468d ; 	/* FUNC   GLOBAL  DEFAULT */
	CCSbVerifyNvCounter              = 0x0f040c09 ; 	/* FUNC   GLOBAL  DEFAULT */
	SBROM_CryptoUpdateBlockDriver    = 0x0f0441f1 ; 	/* FUNC   GLOBAL  DEFAULT */
	FinishBsvAes                     = 0x0f042c11 ; 	/* FUNC   GLOBAL  DEFAULT */
	FreeAes                          = 0x0f042dd1 ; 	/* FUNC   GLOBAL  DEFAULT */
	SBRT_CryptoImageUpdate           = 0x0f044399 ; 	/* FUNC   GLOBAL  DEFAULT */
	SBRT_HalMaskInterrupt            = 0x0f0443f9 ; 	/* FUNC   GLOBAL  DEFAULT */
	FreeBsvHash                      = 0x0f042e09 ; 	/* FUNC   GLOBAL  DEFAULT */
	FinishBsvHash                    = 0x0f042c61 ; 	/* FUNC   GLOBAL  DEFAULT */
	certType2SubjectNames            = 0x0f045080 ; 	/* OBJECT GLOBAL  DEFAULT */
	RSA_PKA_SetRegsSizesTab          = 0x0f043e11 ; 	/* FUNC   GLOBAL  DEFAULT */
	CCCertValidateSWComps            = 0x0f04053d ; 	/* FUNC   GLOBAL  DEFAULT */
	CC_HalMaskInterrupt              = 0x0f042795 ; 	/* FUNC   GLOBAL  DEFAULT */
	ProcessBsvAes                    = 0x0f0436d9 ; 	/* FUNC   GLOBAL  DEFAULT */
	CC_BsvHashFinish                 = 0x0f040ff1 ; 	/* FUNC   GLOBAL  DEFAULT */
	get_romcrypto_date               = 0x0f044e85 ; 	/* FUNC   GLOBAL  DEFAULT */
	CC_DeviceCompleteDisable         = 0x0f0426a1 ; 	/* FUNC   GLOBAL  DEFAULT */
	FinishAes                        = 0x0f042b19 ; 	/* FUNC   GLOBAL  DEFAULT */
	CC_BsvPubKeyHashGet              = 0x0f041a45 ; 	/* FUNC   GLOBAL  DEFAULT */
	NVM_ReadHASHPubKey               = 0x0f0431ed ; 	/* FUNC   GLOBAL  DEFAULT */
	CC_BsvICVKeyLock                 = 0x0f04108d ; 	/* FUNC   GLOBAL  DEFAULT */
	CC_BsvSwVersionSet               = 0x0f0425b9 ; 	/* FUNC   GLOBAL  DEFAULT */
	InitHash                         = 0x0f04303d ; 	/* FUNC   GLOBAL  DEFAULT */
	RSA_PSS_Verify                   = 0x0f043f89 ; 	/* FUNC   GLOBAL  DEFAULT */
	CCCertLoadCertificate            = 0x0f04047d ; 	/* FUNC   GLOBAL  DEFAULT */
	CC_BsvOTPPrivateKeysErase        = 0x0f041665 ; 	/* FUNC   GLOBAL  DEFAULT */
	RSA_PKA_InitPka                  = 0x0f043d61 ; 	/* FUNC   GLOBAL  DEFAULT */
	BsvCryptoImageUpdate             = 0x0f040105 ; 	/* FUNC   GLOBAL  DEFAULT */
	BsvCryptoImageFinish             = 0x0f040089 ; 	/* FUNC   GLOBAL  DEFAULT */
	RSA_HW_PKI_PKA_FinishPKA         = 0x0f043d4d ; 	/* FUNC   GLOBAL  DEFAULT */
	RSA_HW_PKI_PKA_CopyDataFromPkaReg = 0x0f043c39 ; 	/* FUNC   GLOBAL  DEFAULT */
	CC_SbCertChainVerificationInit   = 0x0f042919 ; 	/* FUNC   GLOBAL  DEFAULT */
	CC_BsvKeyDerivation              = 0x0f0413b9 ; 	/* FUNC   GLOBAL  DEFAULT */
	CC_BsvHashInit                   = 0x0f04104d ; 	/* FUNC   GLOBAL  DEFAULT */
	SBRT_CryptoImageFinish           = 0x0f044329 ; 	/* FUNC   GLOBAL  DEFAULT */
	CC_BsvInit                       = 0x0f041275 ; 	/* FUNC   GLOBAL  DEFAULT */
	RSA_HW_PKI_PKA_CopyDataIntoPkaReg = 0x0f043c89 ; 	/* FUNC   GLOBAL  DEFAULT */
	CCCommonCertVerify               = 0x0f0406b9 ; 	/* FUNC   GLOBAL  DEFAULT */
	CC_HalClearInterruptBit          = 0x0f042769 ; 	/* FUNC   GLOBAL  DEFAULT */
	CC_BsvIcvAssetProvisioningOpen   = 0x0f041175 ; 	/* FUNC   GLOBAL  DEFAULT */
	LoadDigest                       = 0x0f0430a9 ; 	/* FUNC   GLOBAL  DEFAULT */
	CC_BsvSecureDebugSet             = 0x0f041d61 ; 	/* FUNC   GLOBAL  DEFAULT */
	LoadInitialDigest                = 0x0f04311d ; 	/* FUNC   GLOBAL  DEFAULT */
	UTIL_MemSet                      = 0x0f04482d ; 	/* FUNC   GLOBAL  DEFAULT */
	RSA_CalcExponent                 = 0x0f043ba5 ; 	/* FUNC   GLOBAL  DEFAULT */
	__romcrypto_ram_end__            = 0x200a8d08 ; 	/* NOTYPE GLOBAL  DEFAULT */
	CCCertGetUnsignedDataOffset      = 0x0f040465 ; 	/* FUNC   GLOBAL  DEFAULT */
	CC_HalWaitInterruptRND           = 0x0f0427b1 ; 	/* FUNC   GLOBAL  DEFAULT */
	CC_HalTerminate                  = 0x0f0427a1 ; 	/* FUNC   GLOBAL  DEFAULT */
	StoreDigest                      = 0x0f044695 ; 	/* FUNC   GLOBAL  DEFAULT */
	SB_HalMaskInterrupt              = 0x0f044685 ; 	/* FUNC   GLOBAL  DEFAULT */
	get_romcrypto_time               = 0x0f044e95 ; 	/* FUNC   GLOBAL  DEFAULT */
	ProcessHashDrvBsv                = 0x0f043af1 ; 	/* FUNC   GLOBAL  DEFAULT */
	FreeBsvAes                       = 0x0f042ded ; 	/* FUNC   GLOBAL  DEFAULT */
	CCSbCalcPublicKeyHASHAndCompare  = 0x0f0409e5 ; 	/* FUNC   GLOBAL  DEFAULT */
	CC_BsvHashUpdate                 = 0x0f041061 ; 	/* FUNC   GLOBAL  DEFAULT */
	SB_HalInit                       = 0x0f044681 ; 	/* FUNC   GLOBAL  DEFAULT */
	CC_BsvICVRMAFlagBitLock          = 0x0f0410f1 ; 	/* FUNC   GLOBAL  DEFAULT */
	InitAes                          = 0x0f042e79 ; 	/* FUNC   GLOBAL  DEFAULT */
	UTIL_MemCopy                     = 0x0f044815 ; 	/* FUNC   GLOBAL  DEFAULT */
	BsvHashAdaptor                   = 0x0f040165 ; 	/* FUNC   GLOBAL  DEFAULT */
	init_romcrypto_platform          = 0x0f044e9d ; 	/* FUNC   GLOBAL  DEFAULT */
	NVM_GetLCS                       = 0x0f0431c1 ; 	/* FUNC   GLOBAL  DEFAULT */
	CC_BsvSocIDCompute               = 0x0f0423c9 ; 	/* FUNC   GLOBAL  DEFAULT */
	CCSbImageLoadAndVerify           = 0x0f040a35 ; 	/* FUNC   GLOBAL  DEFAULT */
	UTIL_Asn1ReadItemVerifyTag       = 0x0f044709 ; 	/* FUNC   GLOBAL  DEFAULT */
	DischargeDigest                  = 0x0f042a79 ; 	/* FUNC   GLOBAL  DEFAULT */
	RSA_PSS_Decode                   = 0x0f043e45 ; 	/* FUNC   GLOBAL  DEFAULT */
	ProcessHash                      = 0x0f043a7d ; 	/* FUNC   GLOBAL  DEFAULT */
	CC_SbCertVerifySingle            = 0x0f042939 ; 	/* FUNC   GLOBAL  DEFAULT */
	UTIL_ReverseBuff                 = 0x0f044835 ; 	/* FUNC   GLOBAL  DEFAULT */
	__romcrypto_ram_start__          = 0x200a8d00 ; 	/* NOTYPE GLOBAL  DEFAULT */
	CC_HalInit                       = 0x0f042775 ; 	/* FUNC   GLOBAL  DEFAULT */

/* From file in legacy repository (generated during build):
 * sdk/bsp/ldscripts/non_ble_projects/da1640x_Rommac.symbols
 */
/**************************************************
 *   Exported Symbols from mrom\rommac\ROM_release\Rommac.elf
 **************************************************/
 dpmtm_calc_dur256us              = 0x0f048045 ; 	/* FUNC   GLOBAL  DEFAULT */
    RomacStartup                     = 0x0f046005 ; 	/* FUNC   GLOBAL  DEFAULT */
    dpmtm_init_timer                 = 0x0f049a15 ; 	/* FUNC   GLOBAL  DEFAULT */
	romac_primitive                  = 0x200a9260 ; 	/* OBJECT GLOBAL  DEFAULT */
	dpmtm_update_topfunc             = 0x0f04a4ad ; 	/* FUNC   GLOBAL  DEFAULT */
	dpm_get_adj_period               = 0x0f04762d ; 	/* FUNC   GLOBAL  DEFAULT */
	dpm_preced_cb                    = 0x200a8d10 ; 	/* OBJECT GLOBAL  DEFAULT */
	dpm_preced_cb                    = 0x200a8d10 ; 	/* OBJECT GLOBAL  DEFAULT */
	timp_intf                        = 0x200a9280 ; 	/* OBJECT GLOBAL  DEFAULT */
	tim_calc_bcncnt                  = 0x0f04da1d ; 	/* FUNC   GLOBAL  DEFAULT */
	timp_reset_buf                   = 0x0f0537ad ; 	/* FUNC   GLOBAL  DEFAULT */
	tim_rtm_init                     = 0x0f051509 ; 	/* FUNC   GLOBAL  DEFAULT */
	timp_perf                        = 0x200a9284 ; 	/* OBJECT GLOBAL  DEFAULT */
	__rommac_ram_end__               = 0x200aa058 ; 	/* NOTYPE GLOBAL  DEFAULT */
	dpmtm_check_mainst               = 0x0f049649 ; 	/* FUNC   GLOBAL  DEFAULT */
	timprep_get_func_prep            = 0x0f0539d1 ; 	/* FUNC   GLOBAL  DEFAULT */
	timprep_get_next_prep_us         = 0x0f053b2d ; 	/* FUNC   GLOBAL  DEFAULT */
	dpmtm_update_next_timer          = 0x0f04a169 ; 	/* FUNC   GLOBAL  DEFAULT */
	dpmtm_add_tcpka_timer            = 0x0f049101 ; 	/* FUNC   GLOBAL  DEFAULT */
	timp_u_0                         = 0x200a9288 ; 	/* OBJECT GLOBAL  DEFAULT */
	dpmtm_update_timer               = 0x0f04a1f9 ; 	/* FUNC   GLOBAL  DEFAULT */
	romac_init_primitives            = 0x0f04c439 ; 	/* FUNC   GLOBAL  DEFAULT */
	dpm_get_wk_rtclk                 = 0x0f047b75 ; 	/* FUNC   GLOBAL  DEFAULT */
	dpmtm_add_fb1_timer              = 0x0f048f09 ; 	/* FUNC   GLOBAL  DEFAULT */
	dpmtm_add_timer                  = 0x0f0491f5 ; 	/* FUNC   GLOBAL  DEFAULT */
	dpmtm_add_udph_timer             = 0x0f049339 ; 	/* FUNC   GLOBAL  DEFAULT */
	dpmtm_chk_prio                   = 0x0f0496dd ; 	/* FUNC   GLOBAL  DEFAULT */
	dpmtm_del_all_timer              = 0x0f049765 ; 	/* FUNC   GLOBAL  DEFAULT */
	dpmtm_add_bufp_timer             = 0x0f048cb1 ; 	/* FUNC   GLOBAL  DEFAULT */
	dpmtm_dealloc_no                 = 0x0f0496f1 ; 	/* FUNC   GLOBAL  DEFAULT */
	dpmtm_push_topback               = 0x0f049e51 ; 	/* FUNC   GLOBAL  DEFAULT */
	dpmtm_check_topfunc              = 0x0f049695 ; 	/* FUNC   GLOBAL  DEFAULT */
	dpmtm_move_back                  = 0x0f049a79 ; 	/* FUNC   GLOBAL  DEFAULT */
	dpmtm_fire_timer                 = 0x0f049781 ; 	/* FUNC   GLOBAL  DEFAULT */
	dpm_wait_us                      = 0x0f048679 ; 	/* FUNC   GLOBAL  DEFAULT */
	__rommac_ram_start__             = 0x200a8d10 ; 	/* NOTYPE GLOBAL  DEFAULT */
	dpmtm_peek_timer                 = 0x0f049b0d ; 	/* FUNC   GLOBAL  DEFAULT */
	dpmtm_handle_timer               = 0x0f0499d9 ; 	/* FUNC   GLOBAL  DEFAULT */
	dpmtm_remove_completed_timer     = 0x0f049efd ; 	/* FUNC   GLOBAL  DEFAULT */
	dpmtm_is_empty                   = 0x0f049a45 ; 	/* FUNC   GLOBAL  DEFAULT */
	dpmtm_add_twt_timer              = 0x0f0492c9 ; 	/* FUNC   GLOBAL  DEFAULT */
	dpmtm_check_subst                = 0x0f04966d ; 	/* FUNC   GLOBAL  DEFAULT */
	dpmtm_remove_subst               = 0x0f049ff5 ; 	/* FUNC   GLOBAL  DEFAULT */
	timp_w_1                         = 0x200a9294 ; 	/* OBJECT GLOBAL  DEFAULT */
	dpmtm_remove_func                = 0x0f049f9d ; 	/* FUNC   GLOBAL  DEFAULT */
	dpmtm_prog_timer                 = 0x0f049bd1 ; 	/* FUNC   GLOBAL  DEFAULT */
	dpmtm_is_full                    = 0x0f049a5d ; 	/* FUNC   GLOBAL  DEFAULT */
	timp_set_period                  = 0x0f053849 ; 	/* FUNC   GLOBAL  DEFAULT */
	timp_check                       = 0x0f0534f1 ; 	/* FUNC   GLOBAL  DEFAULT */
	dpmtm_pop_timer                  = 0x0f049b39 ; 	/* FUNC   GLOBAL  DEFAULT */
	dpm_wait_clk                     = 0x0f04864d ; 	/* FUNC   GLOBAL  DEFAULT */
	timp_get_interface               = 0x0f0535f1 ; 	/* FUNC   GLOBAL  DEFAULT */
	dpmtm_search_no                  = 0x0f04a089 ; 	/* FUNC   GLOBAL  DEFAULT */
	timp_update                      = 0x0f053881 ; 	/* FUNC   GLOBAL  DEFAULT */
	dpmtm_calc_dur2clk               = 0x0f049401 ; 	/* FUNC   GLOBAL  DEFAULT */
	dpm_get_rtcnt                    = 0x0f0477ad ; 	/* FUNC   GLOBAL  DEFAULT */
	dpm_get_rtclk                    = 0x0f047769 ; 	/* FUNC   GLOBAL  DEFAULT */
	dpmtm_remove_mainst              = 0x0f049fc9 ; 	/* FUNC   GLOBAL  DEFAULT */
	dpmtm_calc_time_us               = 0x0f04956d ; 	/* FUNC   GLOBAL  DEFAULT */
	dpmtm_add_timchk_timer           = 0x0f049191 ; 	/* FUNC   GLOBAL  DEFAULT */
	timp_set_condition               = 0x0f0537ed ; 	/* FUNC   GLOBAL  DEFAULT */
	dpmtm_move_front                 = 0x0f049ac5 ; 	/* FUNC   GLOBAL  DEFAULT */
	dpmtm_search_elapsed             = 0x0f04a045 ; 	/* FUNC   GLOBAL  DEFAULT */
	dpmtm_check_id                   = 0x0f049625 ; 	/* FUNC   GLOBAL  DEFAULT */
	dpmtm_get_txcnt_timer            = 0x0f049971 ; 	/* FUNC   GLOBAL  DEFAULT */
	dpmtm_push_preced_topfunc        = 0x0f049d11 ; 	/* FUNC   GLOBAL  DEFAULT */
	dpmtm_add_tim_timer              = 0x0f049165 ; 	/* FUNC   GLOBAL  DEFAULT */
	pdpm                             = 0x200a926c ; 	/* OBJECT GLOBAL  DEFAULT */
	dpm_update_period                = 0x0f04856d ; 	/* FUNC   GLOBAL  DEFAULT */
	dpmtm_calc_dur2tick              = 0x0f049479 ; 	/* FUNC   GLOBAL  DEFAULT */
	dpmtm_add_fb0_timer              = 0x0f048e49 ; 	/* FUNC   GLOBAL  DEFAULT */
	dpmtm_add_tcpack_timer           = 0x0f049039 ; 	/* FUNC   GLOBAL  DEFAULT */
	timp_reset                       = 0x0f053765 ; 	/* FUNC   GLOBAL  DEFAULT */
	dpmtm_update_stclk               = 0x0f04a1ad ; 	/* FUNC   GLOBAL  DEFAULT */
	dpmtm_set_preced_cb              = 0x0f04a121 ; 	/* FUNC   GLOBAL  DEFAULT */
	init_romsys_primitives           = 0x0f04b135 ; 	/* FUNC   GLOBAL  DEFAULT */
	timp_set_backup_addr             = 0x0f0537c9 ; 	/* FUNC   GLOBAL  DEFAULT */
	dpmtm_add_setps_timer            = 0x0f049005 ; 	/* FUNC   GLOBAL  DEFAULT */
	timp_w_0                         = 0x200a9290 ; 	/* OBJECT GLOBAL  DEFAULT */
	timp_get_interface_data          = 0x0f05362d ; 	/* FUNC   GLOBAL  DEFAULT */
	dpmtm_precedence_timer           = 0x0f049b51 ; 	/* FUNC   GLOBAL  DEFAULT */
	timp_get_condition               = 0x0f0535c9 ; 	/* FUNC   GLOBAL  DEFAULT */
	dpmtm_remove_topfunc             = 0x0f04a021 ; 	/* FUNC   GLOBAL  DEFAULT */
	dpmtm_get_datacnt_timer          = 0x0f0498fd ; 	/* FUNC   GLOBAL  DEFAULT */
	timp_init                        = 0x0f053671 ; 	/* FUNC   GLOBAL  DEFAULT */
	mm_sec_machwkey_rec              = 0x0f04bd79 ; 	/* FUNC   GLOBAL  DEFAULT */
	timp_get_performance             = 0x0f053665 ; 	/* FUNC   GLOBAL  DEFAULT */
	timp_u_1                         = 0x200a928c ; 	/* OBJECT GLOBAL  DEFAULT */
	dpm_reset_apsync                 = 0x0f047efd ; 	/* FUNC   GLOBAL  DEFAULT */
	da16x_crc8ccitt                  = 0x0f0469ad ; 	/* FUNC   GLOBAL  DEFAULT */
        dpm_calc_next_twt                = 0x0f047161 ; 	/* FUNC   GLOBAL  DEFAULT */
	tim_calc_timcnt                  = 0x0f04da3d ; 	/* FUNC   GLOBAL  DEFAULT */
	dpmtm_add_ka_timer               = 0x0f048fc9 ; 	/* FUNC   GLOBAL  DEFAULT */
	mm_sec_machwkey_store            = 0x0f04be29 ; 	/* FUNC   GLOBAL  DEFAULT */
	dpmtm_check_func                 = 0x0f0495fd ; 	/* FUNC   GLOBAL  DEFAULT */
	dpmtm_push_topfunc               = 0x0f049ea5 ; 	/* FUNC   GLOBAL  DEFAULT */
	dpm_restart_rtclk                = 0x0f047f61 ; 	/* FUNC   GLOBAL  DEFAULT */
	dpmtm_add_tcpchk_timer           = 0x0f04909d ; 	/* FUNC   GLOBAL  DEFAULT */
	dpmtm_calc_dur2tu                = 0x0f0494e5 ; 	/* FUNC   GLOBAL  DEFAULT */
	dpmtm_alloc_no                   = 0x0f04939d ; 	/* FUNC   GLOBAL  DEFAULT */
	timp_get_interface_current       = 0x0f0535fd ; 	/* FUNC   GLOBAL  DEFAULT */
	dpmtm_calc_dur2us                = 0x0f04952d ; 	/* FUNC   GLOBAL  DEFAULT */
	dpmtm_push_timer                 = 0x0f049d6d ; 	/* FUNC   GLOBAL  DEFAULT */
	dpmtm_setup_timer                = 0x0f04a12d ; 	/* FUNC   GLOBAL  DEFAULT */
	dpm_reset                        = 0x0f047ed9 ; 	/* FUNC   GLOBAL  DEFAULT */
	romac_store_machwkey             = 0x0f04ca79 ; 	/* FUNC   GLOBAL  DEFAULT */
	romac_rem_machwkey               = 0x0f04ca4d ; 	/* FUNC   GLOBAL  DEFAULT */
	dpmtxb_init                      = 0x0f04a5c9 ; 	/* FUNC   GLOBAL  DEFAULT */
    rtos_setup_feat                  = 0x0f04cb15 ; 	/* FUNC   GLOBAL  DEFAULT */
    rtos_setup_func                  = 0x0f04cc3d ; 	/* FUNC   GLOBAL  DEFAULT */