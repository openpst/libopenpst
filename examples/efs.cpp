#include "qualcomm/qcdm_serial.h"
#include "qualcomm/dm_efs_manager.h"
#include <iostream>
#include <string>

using namespace OpenPST::QC;
using OpenPST::Serial::SerialError;

std::ostream& operator<<(std::ostream& str, const QcdmEfsHelloResponse& r) {
	str << "Version: " << r.version << std::endl;
	str << "MaxVersion: " << r.maxVersion << std::endl;
	str << "MinVersion: " << r.minVersion << std::endl;
	str << "featureBits:" << r.featureBits << std::endl; 
}

std::ostream& operator<<(std::ostream& str, const QcdmEfsQueryResponse& r) {
	str << "maxFilenameLength: " << r.maxFilenameLength << std::endl;
	str << "mapPathnameLength: " << r.mapPathnameLength << std::endl;
	str << "maxSymlinkDepth: " << r.maxSymlinkDepth << std::endl;
	str << "maxFileSize: " << r.maxFileSize << std::endl;
	str << "maxDirectories: " << r.maxDirectories << std::endl;
	str << "maxMounts: " << r.maxMounts << std::endl;
}

std::ostream& operator<<(std::ostream& str, const DmEfsNode& e) {
	if(e.isDir()) str << "[DIR ] ";
	else if(e.isFile()) str << "[FILE] ";
	else if(e.isLink()) str << "[LINK] ";
	auto fmt = str.flags();
	str << std::setw(8) << std::setfill('0') << std::hex << e.mode;
	str.flags(fmt);
	str << " " << e.size << " " << e.name;
}

std::vector<std::string> split_str(const std::string& s, const std::string& delim, size_t max = std::numeric_limits<size_t>::max()) {
	std::vector<std::string> res;
	size_t offset = 0;
	do {
		auto pos = s.find(delim, offset);
		if (res.size() < max - 1 && pos != std::string::npos)
			res.push_back(s.substr(offset, pos - offset));
		else {
			res.push_back(s.substr(offset));
			break;
		}
		offset = pos + delim.size();
	} while (true);
	return res;
}

std::string join_str(std::vector<std::string>::const_iterator beg, std::vector<std::string>::const_iterator end, const std::string& delim) {
	std::string res;
	for (; beg != end; beg++) {
		if (!res.empty())
			res += delim;
		res += *beg;
	}
	return res;
}

using namespace std::string_literals;

void changedir(const std::string& path, const std::vector<DmEfsNode>& contents, std::string& cPath) {
	if(path == "..") {
		auto parts = split_str(cPath, "/"s);
		if(parts.size() > 1) parts.erase(parts.begin() + parts.size() - 1);
		cPath = join_str(parts.begin(), parts.end(), "/"s);
		if(cPath.empty()) cPath = "/";
	} else {
		bool found = false;
		for(auto& e: contents) {
			if(path == e.name && e.isDir()) {
				cPath += e.name;
				found = true;
				break;
			}
		}
		if(!found) std::cout << "Path not found" << std::endl;
	}
}

void get_entry(DmEfsManager& mgr, const std::string& path, const std::vector<DmEfsNode>& contents, const std::string& local) {
	for(auto& e: contents) {
		if(path == e.name && e.isFile()) {
			std::ofstream out(local, std::ios::binary);
			std::string p = e.path;
			if(p.back() != '/') p += "/";
			p += e.name;
			size_t s;
			{
				auto fp = mgr.open(p, O_RDONLY, 0x00);

				if (fp < 0) {
					std::cout << "open failed" << std::endl;
					return;
				}

				auto fileInfo = mgr.fstat(fp);
				if (fileInfo.error) {
					std::cout << "fstat error:" << fileInfo.error << std::endl;
					return;
				}
				if (fileInfo.size < 0) {
					std::cout << "fstat error: invalid filesize" << std::endl;
					return;
				}
				
				std::ofstream out(local.c_str(), std::ios::out | std::ios::binary | std::ios::trunc);
				if (!out.is_open()) {
					std::cout << "local open failed" << std::endl;
					return;
				}

				size_t read = 0;
				while(read != fileInfo.size) {
					auto data = mgr.read(fp, fileInfo.size - read, read);
					out.write(reinterpret_cast<char*>(&data[0]), data.size());
					read += data.size();
				}
				
				mgr.close(fp);
				s = read;
			}
			std::cout << "Wrote " << s << " bytes" << std::endl;
			return;
		}
	}
	std::cout << "Path not found" << std::endl;
}

void del_entry(DmEfsManager& mgr, const std::string& path, const std::vector<DmEfsNode>& contents) {
	for(auto& e: contents) {
		if(path == e.name && e.isFile()) {
			std::string p = e.path;
			if(p.back() != '/') p += "/";
			p += e.name;
			mgr.unlink(p);
			return;
		}
	}
	std::cout << "Path not found" << std::endl;
}

void put_entry(DmEfsManager& mgr, const std::string& path, const std::string& local) {
	size_t s;
	auto fp = mgr.open(path, O_WRONLY | O_CREAT, 0x000081b6);
	if (fp < 0) {
		std::cout << "open failed" << std::endl;
		return;
	}

	std::ifstream in(local, std::ios::in | std::ios::binary);
	if (!in.is_open()) {
		std::cout << "local open failed" << std::endl;
		mgr.close(fp);
		return;
	}
	std::vector<char> buffer((std::istreambuf_iterator<char>(in)), (std::istreambuf_iterator<char>()));

	size_t written = 0;
	while(written != buffer.size()){
		written += mgr.write(fp, reinterpret_cast<uint8_t*>(buffer.data() + written), buffer.size() - written, written);
	}
	std::cout << "Uploaded " << written << " bytes" << std::endl;

	mgr.close(fp);
}

int main(int argc, char**argv) {

	if (argc < 2) {
		std::cout << "Usage: program [port] <args>" << std::endl;
		return 0;
	}

	bool use_afs = false;
	for(int i=2; i < argc; i++) {
		if("-a"s == argv[i] || "--afs"s == argv[i]) {
			use_afs = true;
		} else {
			std::cerr << "Unknown option " << argv[i] << std::endl;
			return 0;
		}
	}

	try {
		QcdmSerial port(argv[1], 3840, 150);
		DmEfsManager mgr(port);
		if(use_afs)
			mgr.setSubsystemId(kDiagSubsysEfsAlternate);

		auto info = mgr.hello();
		std::cout << info << std::endl;
		auto query = mgr.query();
		std::cout << query << std::endl;

		std::string cPath = "/";
		std::string line;

		do {
			auto contents =  mgr.readDir(cPath);
			std::cout << "Path: " << cPath << std::endl;
			std::cout << "[DIR ] .." << std::endl;
			for(auto& e : contents) {
				std::cout << e << std::endl;
			}
			std::cout << std::endl;
			std::cout << "> ";
			std::getline(std::cin, line);
			auto parts = split_str(line, " "s);
			if(parts[0] == "cd") {
				if(parts.size() < 2) { std::cout << "Missing args: cd [dir]" << std::endl; continue; }
				changedir(parts[1], contents, cPath);
			} else if(parts[0] == "get") {
				if(parts.size() < 2) { std::cout << "Missing args: get [remote] <localfile>" << std::endl; continue; }
				std::string local = parts[1];
				if(parts.size() > 2) local = parts[2];
				get_entry(mgr, parts[1], contents, local);
			} else if(parts[0] == "put") {
				if(parts.size() < 3) { std::cout << "Missing args: put [remote] [localfile]" << std::endl; continue; }
				put_entry(mgr, parts[1], parts[2]);
			} else if(parts[0] == "del" || parts[0] == "rm") {
				if(parts.size() < 2) { std::cout << "Missing args: del [file]" << std::endl; continue; }
				del_entry(mgr, parts[1], contents);
			} else if(parts[0] == "quit" || parts[0] == "q" || parts[0] == "exit") {
				break;
			}
			else {
				std::cout << "Unknown command" << std::endl;
			}
		} while(true);

		port.close();
	}
	catch(const std::exception& e) {
		std::cerr << e.what() << std::endl;
	}

	return 0;
}