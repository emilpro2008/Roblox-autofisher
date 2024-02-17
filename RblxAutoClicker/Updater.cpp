#define _CRT_SECURE_NO_WARNINGS 1;

#include <filesystem>
#include <iostream>
#include <string>

#include "Updater.h"
#include "curl/curl.h"

void Updater::Update(std::string CurVer, std::string NewVer) {
	// Auto update on startup if new version is found
}

static int DownloadFile(const char* url, const char* dest) {
	if (std::filesystem::exists(dest)) {
		std::cout << "File already exists." << std::endl;
		return 1;
	}

	CURL* curl;
	FILE* fp;
	CURLcode res;
	curl = curl_easy_init();

	if (curl)
	{
		fp = fopen(dest, "wb");
		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
		res = curl_easy_perform(curl);
		curl_easy_cleanup(curl);
		fclose(fp);

		std::cout << "Downloaded file successfully." << std::endl;
		return 0;
	}
}