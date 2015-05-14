/*
--------------------------------------------------------------------------
	FILE  classic.c
--------------------------------------------------------------------------
*/

#include <include.h>
#include <osmifare.h>

MIFIRE_ORDER CLASSIC_Order;
MIFARE_ANSWER *CLASSIC_Answer;
MIFARE_ORDER_IN_TYPE CLASSIC_Type;
unsigned char aucMCardRevData[OSMIFARE_CLASSIC_BLOCK_LEN];

#define CLASSIC_ORDER_READ	    	0x30
#define CLASSIC_ORDER_WRITE	      	0xA0
#define CLASSIC_ORDER_INC	      	0xC1
#define CLASSIC_ORDER_DEC			0xC0
#define CLASSIC_ORDER_RESTORE		0xC2
#define CLASSIC_ORDER_TRANSFER		0xB0
#define CLASSIC_ORDER_AUTHA			0x60
#define CLASSIC_ORDER_AUTHB			0x61

unsigned char CLASSIC_ReadBlock( unsigned char ucBlockIndex,unsigned char *pucReadData );
unsigned char CLASSIC_WriteBlock( unsigned char ucBlockIndex,unsigned char *pucWriteData );
unsigned char CLASSIC_Increment( unsigned char ucBlockIndex,unsigned long ulValue );
unsigned char CLASSIC_Decrement( unsigned char ucBlockIndex,unsigned long ulValue );
unsigned char CLASSIC_Restore( unsigned char ucBlockIndex );
unsigned char CLASSIC_Transfer( unsigned char ucBlockIndex );

unsigned char CLASSIC_Auth( unsigned char ucAuthType,unsigned char ucSectorIndex,unsigned char ucKeyType,unsigned char *pucKey )
{

	CLASSIC_Order.order = OSMIFARE_CLASSIC_ORDER;
	CLASSIC_Order.pt_order_in = &CLASSIC_Type;
	if( ucKeyType==OSMIFARE_KEY_TYPE_A )
		CLASSIC_Type.classic_order.order_type = CLASSIC_ORDER_AUTHA;
	else
		CLASSIC_Type.classic_order.order_type = CLASSIC_ORDER_AUTHB;
	CLASSIC_Type.classic_order.ucBlockIndex = ucSectorIndex;
	CLASSIC_Type.classic_order.ucAuthType = ucAuthType;
	memcpy( CLASSIC_Type.classic_order.aucInData,pucKey,OSMIFARE_KEY_LEN );
	CLASSIC_Order.ptout = aucMCardRevData;
	CLASSIC_Answer = Os__MIFARE_command( &CLASSIC_Order );
	if( CLASSIC_Answer->drv_status != OSMIFARE_ERR_SUCCESS )
	{
	    return (CLASSIC_Answer->drv_status);
	}
	return( CLASSIC_Answer->card_status );
}

unsigned char CLASSIC_ReadBlock( unsigned char ucBlockIndex,unsigned char *pucReadData )
{

	CLASSIC_Order.order = OSMIFARE_CLASSIC_ORDER;
	CLASSIC_Order.pt_order_in = &CLASSIC_Type;
	CLASSIC_Type.classic_order.order_type = CLASSIC_ORDER_READ;
	CLASSIC_Type.classic_order.ucBlockIndex = ucBlockIndex;
	CLASSIC_Order.ptout = aucMCardRevData;
	CLASSIC_Answer = Os__MIFARE_command( &CLASSIC_Order );
	if( CLASSIC_Answer->drv_status != OSMIFARE_ERR_SUCCESS )
	{
	    return (CLASSIC_Answer->drv_status);
	}
	memcpy( pucReadData,aucMCardRevData,CLASSIC_Answer->Len );
	return( CLASSIC_Answer->card_status );
}

unsigned char CLASSIC_WriteBlock( unsigned char ucBlockIndex,unsigned char *pucWriteData )
{

	CLASSIC_Order.order = OSMIFARE_CLASSIC_ORDER;
	CLASSIC_Order.pt_order_in = &CLASSIC_Type;
	CLASSIC_Type.classic_order.order_type = CLASSIC_ORDER_WRITE;
	CLASSIC_Type.classic_order.ucBlockIndex = ucBlockIndex;
	memcpy( &CLASSIC_Type.classic_order.aucInData,pucWriteData,16 );
	CLASSIC_Order.ptout = aucMCardRevData;
	CLASSIC_Answer = Os__MIFARE_command( &CLASSIC_Order );
	if( CLASSIC_Answer->drv_status != OSMIFARE_ERR_SUCCESS )
	{
	    return (CLASSIC_Answer->drv_status);
	}
	return( CLASSIC_Answer->card_status );
}

unsigned char CLASSIC_Increment( unsigned char ucBlockIndex,unsigned long ulValue )
{

	CLASSIC_Order.order = OSMIFARE_CLASSIC_ORDER;
	CLASSIC_Order.pt_order_in = &CLASSIC_Type;
	CLASSIC_Type.classic_order.order_type = CLASSIC_ORDER_INC;
	CLASSIC_Type.classic_order.ucBlockIndex = ucBlockIndex;
	memcpy( &CLASSIC_Type.classic_order.aucInData,&ulValue,4 );
	CLASSIC_Order.ptout = aucMCardRevData;
	CLASSIC_Answer = Os__MIFARE_command( &CLASSIC_Order );
	if( CLASSIC_Answer->drv_status != OSMIFARE_ERR_SUCCESS )
	{
	    return (CLASSIC_Answer->drv_status);
	}
	return( CLASSIC_Answer->card_status );
}

unsigned char CLASSIC_Decrement( unsigned char ucBlockIndex,unsigned long ulValue )
{

	CLASSIC_Order.order = OSMIFARE_CLASSIC_ORDER;
	CLASSIC_Order.pt_order_in = &CLASSIC_Type;
	CLASSIC_Type.classic_order.order_type = CLASSIC_ORDER_DEC;
	CLASSIC_Type.classic_order.ucBlockIndex = ucBlockIndex;
	memcpy( &CLASSIC_Type.classic_order.aucInData,&ulValue,4 );
	CLASSIC_Order.ptout = aucMCardRevData;
	CLASSIC_Answer = Os__MIFARE_command( &CLASSIC_Order );
	if( CLASSIC_Answer->drv_status != OSMIFARE_ERR_SUCCESS )
	{
	    return (CLASSIC_Answer->drv_status);
	}
	return( CLASSIC_Answer->card_status );
}

unsigned char CLASSIC_Transfer( unsigned char ucBlockIndex )
{

	CLASSIC_Order.order = OSMIFARE_CLASSIC_ORDER;
	CLASSIC_Order.pt_order_in = &CLASSIC_Type;
	CLASSIC_Type.classic_order.order_type = CLASSIC_ORDER_TRANSFER;
	CLASSIC_Type.classic_order.ucBlockIndex = ucBlockIndex;
	CLASSIC_Order.ptout = aucMCardRevData;
	CLASSIC_Answer = Os__MIFARE_command( &CLASSIC_Order );
	if( CLASSIC_Answer->drv_status != OSMIFARE_ERR_SUCCESS )
	{
	    return (CLASSIC_Answer->drv_status);
	}
	return( CLASSIC_Answer->card_status );
}

unsigned char CLASSIC_Restore( unsigned char ucBlockIndex )
{

	CLASSIC_Order.order = OSMIFARE_CLASSIC_ORDER;
	CLASSIC_Order.pt_order_in = &CLASSIC_Type;
	CLASSIC_Type.classic_order.order_type = CLASSIC_ORDER_RESTORE;
	CLASSIC_Type.classic_order.ucBlockIndex = ucBlockIndex;
	CLASSIC_Order.ptout = aucMCardRevData;
	CLASSIC_Answer = Os__MIFARE_command( &CLASSIC_Order );
	if( CLASSIC_Answer->drv_status != OSMIFARE_ERR_SUCCESS )
	{
	    return (CLASSIC_Answer->drv_status);
	}
	return( CLASSIC_Answer->card_status );
}

unsigned char Data_XOR( unsigned char ucInitValue,unsigned char *pucInData,unsigned char ucLen )
{
	unsigned char ucI,ucLRC;
	
	ucLRC = ucInitValue;
	for( ucI=0;ucI<ucLen;ucI++ )
		ucLRC ^= pucInData[ucI];
	pucInData[ucI] = ucLRC;
	
	return ucLRC;
} 