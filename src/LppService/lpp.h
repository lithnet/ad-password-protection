

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.01.0622 */
/* at Tue Jan 19 14:14:07 2038
 */
/* Compiler settings for lpp.idl:
    Oicf, W1, Zp8, env=Win64 (32b run), target_arch=AMD64 8.01.0622 
    protocol : all , ms_ext, app_config, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */



/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 500
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif /* __RPCNDR_H_VERSION__ */


#ifndef __lpp_h__
#define __lpp_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __passwordfilter_INTERFACE_DEFINED__
#define __passwordfilter_INTERFACE_DEFINED__

/* interface passwordfilter */
/* [explicit_handle][strict_context_handle][unique][version][uuid] */ 

int GetPasswordFilterResult( 
    /* [in] */ handle_t IDL_handle,
    /* [max_is][string][in] */ const wchar_t *username,
    /* [max_is][string][in] */ const wchar_t *fullname,
    /* [max_is][string][in] */ const wchar_t *password);



extern RPC_IF_HANDLE passwordfilter_v1_0_c_ifspec;
extern RPC_IF_HANDLE passwordfilter_v1_0_s_ifspec;
#endif /* __passwordfilter_INTERFACE_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


