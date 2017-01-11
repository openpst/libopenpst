/**
* LICENSE PLACEHOLDER
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
				bool isDir();

				/**
				* @brief isFile
				*
				* @return bool
				*/
				bool isFile();

				/**
				* @brief isLink
				*
				* @return bool
				*/
				bool isLink();

				/**
				* @brief isImmovable
				*
				* @return bool
				*/
				bool isImmovable();

		};
	}
}
