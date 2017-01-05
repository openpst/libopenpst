/**
* LICENSE PLACEHOLDER
*
* @file dm_ssm.h
* @package OpenPST
* @brief QCDM definitions and structures for SSM feature
*
* @author Gassan Idriss <ghassani@gmail.com>
*/
#pragma once

#include "definitions.h"
#include "qualcomm/dm.h"

/**
* SSM Open File Request
*/
PACKED(typedef struct QcdmSsmOpenRequest{
	QcdmSubsysHeader header;
	uint32_t flags;       
	uint32_t mode;       
	char  path[0];  
}) QcdmSsmOpenRequest;

/**
* SSM Open File Response
*/
PACKED(typedef struct QcdmSsmOpenResponse{
	QcdmSubsysHeader header;
	uint32_t fd;       
	uint32_t error;  
}) QcdmSsmOpenResponse;


/**
* SSM Close File Request
*/
PACKED(typedef struct QcdmSsmCloseRequest{
	QcdmSubsysHeader header;
	uint32_t fd; 
}) QcdmSsmCloseRequest;

/**
* SSM Close File Response
*/
PACKED(typedef struct QcdmSsmCloseResponse{
	QcdmSubsysHeader header;     
	uint32_t error;  
}) QcdmSsmCloseResponse;


/**
* SSM Read File Request
*/
PACKED(typedef struct QcdmSsmReadRequest{
	QcdmSubsysHeader header;
	uint32_t fd; 
	uint32_t bytes; 
	uint32_t offset; 
}) QcdmSsmReadRequest;

/**
* SSM Read File Response
*/
PACKED(typedef struct QcdmSsmReadResponse{
	QcdmSubsysHeader header;
	uint32_t fd; 
	uint32_t offset; 
	uint32_t size; 
	uint32_t error;  
	uint8_t  data[0];
}) QcdmSsmReadResponse;

