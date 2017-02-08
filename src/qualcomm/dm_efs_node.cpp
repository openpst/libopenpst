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
* @file dm_efs_node.cpp
* @class OpenPST::DmEfsNode
* @package openpst/libopenpst
* @brief Represents a file or dir retrieved from the diagnostic monitor EFS subsystem
*
* @author Gassan Idriss <ghassani@gmail.com>
*/

#include "qualcomm/dm_efs_node.h"

using namespace OpenPST::QC;

/**
* @brief DmEfsNode - Constructor
* @param QcdmSerial port
*/
DmEfsNode::DmEfsNode(std::string path, QcdmEfsReadDirResponse* data) :
	name(data->name),
	path(path),
	error(data->error),
	type(data->entryType),
	mode(data->mode),
	size(data->size),
	atime(data->atime),
	mtime(data->mtime),
	ctime(data->ctime)			
{
}

/**
* @brief ~DmEfsNode - Deconstructor
*/
DmEfsNode::~DmEfsNode()
{

}

/**
* @brief Copy Constructor
*/
DmEfsNode::DmEfsNode(const DmEfsNode& org) :
	name(org.name),
	path(org.path),
	error(org.error),
	type(org.type),
	mode(org.mode),
	size(org.size),
	atime(org.atime),
	mtime(org.mtime),
	ctime(org.ctime),
	children(org.children)
{

}

bool DmEfsNode::isDir()
{
	return type == kDiagEfsFileTypeDir;
}

bool DmEfsNode::isFile()
{
	return type == kDiagEfsFileTypeFile;
}

bool DmEfsNode::isLink()
{
	return type == kDiagEfsFileTypeLink;
}

bool DmEfsNode::isImmovable()
{
	return type == kDiagEfsFileTypeImmovable;
}
