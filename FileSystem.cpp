#include "FileSystem.h"

void FileSystem::ProcessZip(const char* path) {
	unzFile zip = (unzOpen(path));
	if (!zip)
		return;

	mZipHandles.Push(zip);

	if (unzGoToFirstFile(zip) == UNZ_OK) {
		String2 key;

		do {
			unz_file_info file_info;
			char name[512];
			unzGetCurrentFileInfo(zip, &file_info, name, 511, NULL, 0, NULL, 0);
			char* slashf = (strrchr(name, '/')), * slashb = (strrchr(name, '\\'));
			char* base = (slashf) ? (slashf + 1) : (slashb) ? (slashb + 1) : name;
			key = base;

			// Con::Echo( " -- %s %s", name, mFiles.InsertUnique( name, file ) ? "ACCEPTED" : "REJECTED" );
			ZipFile* fileinfo = new ZipFile(zip, file_info.uncompressed_size, unzGetOffset(zip));
			if (!fileinfo || !mFiles.InsertUnique(key, fileinfo))
				delete fileinfo;
		} while (unzGoToNextFile(zip) == UNZ_OK);
	}
}

void FileSystem::Scan(const char* path, bool zip_scan) {
	String2 search_path;
	search_path.Assign("%s/*", path);

	WIN32_FIND_DATA find_file;
	HANDLE find = FindFirstFile(search_path.c_str(), &find_file);
	if (find == INVALID_HANDLE_VALUE) {
		return;
	}

	String2 key, item_path;
	do {
		item_path.Assign("%s/%s", path, find_file.cFileName);

		if (find_file.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
			if ((strcmp(find_file.cFileName, ".") != 0) && (strcmp(find_file.cFileName, "..") != 0)) {
				Scan(item_path.c_str(), zip_scan);
			}
		}
		else {
			if (zip_scan) {
				if (stristr(find_file.cFileName, ".zip"))
					ProcessZip(item_path.c_str());
			}
			else {
				key = find_file.cFileName;
				DiskFile* fileinfo = new DiskFile(item_path);
				if (!fileinfo || !mFiles.InsertUnique(key, fileinfo))
					delete fileinfo;
			}
		}
	} while (FindNextFile(find, &find_file) != 0);

	FindClose(find);
}
