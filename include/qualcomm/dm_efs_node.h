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
* @file dm_efs_node.h
* @class OpenPST::DmEfsNode
* @package openpst/libopenpst
* @brief Represents a file or dir retrieved from the diagnostic monitor EFS subsystem
*
* @author Gassan Idriss <ghassani@gmail.com>
*/
#pragma once

#include "definitions.h"
#include <iostream>
#include <vector>
#include "qualcomm/dm_efs.h"

namespace OpenPST {
	namespace QC {
		class DmEfsNode {
			public:
				std::string name;
				std::string path;
				uint32_t error;
				int32_t type;
				int32_t mode;
				size_t size;
				time_t atime;
				time_t mtime;
				time_t ctime;
				std::vector<DmEfsNode> children;

				/**
				* @brief - Constructor
				*/
				DmEfsNode(std::string path, QcdmEfsReadDirResponse* data);

				/**
				* @brief - Constructor
				*/
				DmEfsNode(std::string path, std::string name);

				/**
				* @brief - Deconstructor
				*/
				~DmEfsNode();

				/**
				* @brief Copy Constructor
				*/
				DmEfsNode(const DmEfsNode& copy);

				/**
				* @brief isDir
				*
				* @return bool
				*/
				bool isDir() const;

				/**
				* @brief isFile
				*
				* @return bool
				*/
				bool isFile() const;

				/**
				* @brief isLink
				*
				* @return bool
				*/
				bool isLink() const;

				/**
				* @brief isImmovable
				*
				* @return bool
				*/
				bool isImmovable() const;

		};
	}
}
