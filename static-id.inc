/*
 * Copyright (C) 2023 DeviceBlack
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#if defined _StaticID_ByDW
	#endinput
#endif
#define _StaticID_ByDW

#include <a_samp>
#include <sscanf2>

#define INVALID_STATIC_PLAYER_ID	(0)

// ========= [ Funções Privadas ] =========== //
static DBResult:db_exec(DB:handle, const query[], bool:use_cache = false)
{
	static DBResult:res;
	res = db_query(handle, query);

	if(use_cache)
	{
		db_free_result(res);
		res = DBResult:0;
	}
	return res;
}

static enum PlayerID
{
	p_ID,
	p_Name[24]
};
static StaticInfo[MAX_PLAYERS][PlayerID];

static DB:sqlite, DBResult:result, query[128];

public OnGameModeInit()
{
	sqlite = db_open("id-fixo.db");
	db_exec(sqlite, "create table if not exists Players (Id integer primary key asc, Name text(24) not null unique);");
	
	#if defined GmInit_StaticID
		return GmInit_StaticID();
	#else
		return 1;
	#endif
}
#if defined ALS_OnGameModeInit
	#undef OnGameModeInit
#else
	#define ALS_OnGameModeInit
#endif
#define OnGameModeInit GmInit_StaticID
forward GmInit_StaticID();

public OnPlayerConnect(playerid)
{
	GetPlayerName(playerid, StaticInfo[playerid][p_Name], 24);
	
	Repeater:
	format(query, sizeof query, "select Id from Players where Name='%s';", StaticInfo[playerid][p_Name]);
	result = db_exec(sqlite, query, true);

	if(!db_num_rows(result))
	{
		format(query, sizeof query, "insert into Players (Name) values ('%s');", StaticInfo[playerid][p_Name]);
		db_exec(sqlite, query);
		goto Repeater;
	}
	
	StaticInfo[playerid][p_ID] = db_get_field_int(result);
	db_free_result(result);
	
	#if defined PlayerConnect_StaticID
		return PlayerConnect_StaticID(playerid);
	#else
		return 1;
	#endif
}
#if defined ALS_OnPlayerConnect
	#undef OnPlayerConnect
#else
	#define ALS_OnPlayerConnect
#endif
#define OnPlayerConnect PlayerConnect_StaticID
forward PlayerConnect_StaticID(playerid);

public OnPlayerDisconnect(playerid, reason)
{
	StaticInfo[playerid][p_ID] = INVALID_STATIC_PLAYER_ID;
	strdel(StaticInfo[playerid][p_Name], 0, 23);

	#if defined PlayerDisconnect_StaticID
		return PlayerDisconnect_StaticID(playerid, reason);
	#else
		return 1;
	#endif
}
#if defined ALS_OnPlayerDisconnect
	#undef OnPlayerDisconnect
#else
	#define ALS_OnPlayerDisconnect
#endif
#define OnPlayerDisconnect PlayerDisconnect_StaticID
forward PlayerDisconnect_StaticID(playerid, reason);

// ========= [ Funções Públicas ] =========== //
stock GetPlayerStaticID(playerid) return StaticInfo[playerid][p_ID];

stock GetPlayerStaticIDFromName(const name[], bool:offline_too = false)
{
	static id;
	if(offline_too)
	{
		format(query, sizeof query, "select Id from Players where Name='%s';", name);
		result = db_exec(sqlite, query, true);
		
		if(db_num_rows(result))
		{
			id = db_get_field_int(result);
			db_free_result(result);
		}
		else id = INVALID_STATIC_PLAYER_ID;
	}
	else
	{
		static playerid;
		sscanf(name, "U(-1)", playerid);
		id = ((playerid == -1) ? INVALID_STATIC_PLAYER_ID : StaticInfo[playerid][p_ID]);
	}
	return id;
}

stock GetPlayerNameFromStaticID(staticId, dest[], len = sizeof dest)
{
	format(query, sizeof query, "select Name from Players where Id='%d';", staticId);
	result = db_exec(sqlite, query, true);

	if(db_num_rows(result))
	{
		db_field_name(result, 0, dest, len);
		db_free_result(result);
	}
	else return 0;
	return 1;
}

stock GetPlayerIDFromStaticID(staticId)
{
	if(staticId)
	{
		for(new id = GetPlayerPoolSize(); id >= 0; id++)
			if(staticId == StaticInfo[id][p_ID]) return id;
	}
	return INVALID_PLAYER_ID;
}
