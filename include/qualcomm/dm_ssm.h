/**
*
* (c) Gassan Idriss <ghassani@gmail.com>
* 
* This file is part of libopenpst.
* 
* libopenpst is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
* 
* libopenpst is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* 
* You should have received a copy of the GNU General Public License
* along with libopenpst. If not, see <http://www.gnu.org/licenses/>.
*
* @file dm_ssm.h
* @package openpst/libopenpst
* @brief QCDM definitions and structures for SSM feature
*
* @author Gassan Idriss <ghassani@gmail.com>
*/
#pragma once

#include "definitions.h"
#include "qualcomm/dm.h"

#ifndef NO_POD_PACKET_STRUCTURES
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

#endif