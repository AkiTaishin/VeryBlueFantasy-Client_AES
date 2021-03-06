/// <summary>
/// サーバーに接続してCFormationを取得する
/// JsonからCFormationを取り出せるようにする。
/// </summary>
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#include <fstream>
#include <stdio.h>
#include <winsock2.h>
#include "scene.h"
#include "manager.h"
#include "clientInfo.h"
#include <list>
#include <string>
#include <iostream>
#include "login.h"

/// <summary>
/// まず登録されているユーザーなのかLoadで確認する
/// 登録されていないユーザーだった場合はSaveを呼び出す
/// とりあえずsaveつくる
/// </summary>

CLIENT_INFO CClientInfo::LoadClientInfo(CLIENT_INFO senddata)
{
	WSADATA wsaData;
	struct sockaddr_in server;
	SOCKET sock;
	char buf[2048];

	// winsock2の初期化
	WSAStartup(MAKEWORD(2, 0), &wsaData);

	// ソケットの作成
	sock = socket(AF_INET, SOCK_STREAM, 0);

#pragma region 接続先指定用構造体の準備

	server.sin_family = AF_INET;
	server.sin_port = htons(8080);
	server.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

#pragma endregion

	// サーバに接続
	connect(sock, (struct sockaddr *)&server, sizeof(server));

#pragma region リクエスト送信

	CLIENT_INFO sendData;
	sendData = senddata;

	// dataが送れていない
	std::stringstream data;
	{
		cereal::JSONOutputArchive outArchive(data);
		outArchive(cereal::make_nvp("data", sendData));
	}

	// HTTPで「/」をリクエストする文字列を生成
	memset(buf, 0, sizeof(buf));
	_snprintf(buf, sizeof(buf), "POST /pleaseLoadClientInfo HTTP/1.0\r\nContent-Length:%d\r\nContent-Type:application/json\r\n\r\n", (int)strlen(data.str().c_str()));

	std::string newBuf = buf;

	newBuf = newBuf + data.str();

	// HTTPリクエスト送信
	int n = send(sock, newBuf.c_str(), (int)strlen(newBuf.c_str()), 0);
	if (n < 0) {
		printf("send : %d\n", WSAGetLastError());
	}

#pragma endregion

#pragma region データ受信

	CLIENT_INFO loadInfo;

	// サーバからデータを受信
	// 暗号化するべきポイント！！！@todo
	memset(buf, 0, sizeof(buf));
	int len = sizeof(buf);
	int nn = recv(sock, buf, len, 0);

	// bufからJsonDataを取り出す
	std::string getJsonData = buf;
	int startJsonData = getJsonData.find("[");
	// 新規ユーザーだった場合
	if (startJsonData == -1)
	{
		return loadInfo;
	}

	getJsonData = getJsonData.substr(startJsonData);

	// 事前にm_RegisterNumberに登録ユーザー数が格納されているのでループ回数はm_RegisterNumberにすればよい
	std::stringstream work(getJsonData);
	cereal::JSONInputArchive inArchive(work);
	{
		inArchive(loadInfo);	
	}

#pragma endregion

	// winsock2の終了処理
	WSACleanup();

	return loadInfo;
}

void CClientInfo::SaveClientInfo(CLIENT_INFO save)
{
	WSADATA wsaData;
	struct sockaddr_in server;
	SOCKET sock;
	char buf[2048];

	// winsock2の初期化
	WSAStartup(MAKEWORD(2, 0), &wsaData);

	// ソケットの作成
	sock = socket(AF_INET, SOCK_STREAM, 0);

#pragma region 接続先指定用構造体の準備

	server.sin_family = AF_INET;
	server.sin_port = htons(8080);
	server.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

#pragma endregion

	// サーバに接続
	connect(sock, (struct sockaddr *)&server, sizeof(server));

#pragma region リクエスト送信

	CLIENT_INFO saveData;
	saveData = save;
	std::stringstream sendData;
	{
		cereal::JSONOutputArchive outArchive(sendData);
		outArchive(cereal::make_nvp("data", saveData));
	}

	// HTTPで「/」をリクエストする文字列を生成
	memset(buf, 0, sizeof(buf));
	_snprintf(buf, sizeof(buf), "POST /pleaseSaveClient HTTP/1.0\r\nContent-Length:%d\r\nContent-Type:application/json\r\n\r\n", (int)strlen(sendData.str().c_str()));

	std::string newBuf = buf;

	newBuf = newBuf + sendData.str();

	// HTTPリクエスト送信
	int n = send(sock, newBuf.c_str(), (int)strlen(newBuf.c_str()), 0);
	if (n < 0) {
		printf("send : %d\n", WSAGetLastError());
	}

#pragma endregion

#pragma region データ受信

	// サーバからデータを受信
	memset(buf, 0, sizeof(buf));
	int len = sizeof(buf);
	int nn = recv(sock, buf, len, 0);

#pragma endregion

	// winsock2の終了処理
	WSACleanup();
}