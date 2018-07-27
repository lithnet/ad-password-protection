

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.01.0622 */
/* at Tue Jan 19 14:14:07 2038
 */
/* Compiler settings for interface.idl:
    Oicf, W1, Zp8, env=Win64 (32b run), target_arch=AMD64 8.01.0622 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif /* __RPCNDR_H_VERSION__ */


#ifndef __interface_h_h__
#define __interface_h_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IRpcExec_INTERFACE_DEFINED__
#define __IRpcExec_INTERFACE_DEFINED__

/* interface IRpcExec */
/* [unique][version][uuid] */ 

/* [fault_status][comm_status] */ error_status_t Exec( 
    /* [in] */ handle_t Binding,
    /* [in] */ long lDataSize,
    /* [size_is] */ byte arData[  ],
    /* [out] */ long *plReturnSize,
    /* [size_is][size_is][out] */ byte **arReturn);



extern RPC_IF_HANDLE IRpcExec_v1_0_c_ifspec;
extern RPC_IF_HANDLE IRpcExec_v1_0_s_ifspec;
#endif /* __IRpcExec_INTERFACE_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


