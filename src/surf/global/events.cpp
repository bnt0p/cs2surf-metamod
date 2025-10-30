#include "events.h"

bool Surf::API::events::MapChange::ToJson(Json &json) const
{
	return json.Set("new_map", this->mapName);
}

bool Surf::API::events::MapInfo::FromJson(const Json &json)
{
	return json.Get("map", this->data);
}

bool Surf::API::events::PlayerJoin::ToJson(Json &json) const
{
	// clang-format off
	return json.Set("id", this->steamID)
		&& json.Set("name", this->name)
		&& json.Set("ip_address", this->ipAddress);
	// clang-format on
}

bool Surf::API::events::PlayerJoinAck::FromJson(const Json &json)
{
	return json.Get("is_banned", this->isBanned) && json.Get("preferences", this->preferences);
}

bool Surf::API::events::PlayerLeave::ToJson(Json &json) const
{
	// clang-format off
	return json.Set("id", this->steamID)
		&& json.Set("name", this->name)
		&& json.Set("preferences", this->preferences);
	// clang-format on
}

bool Surf::API::events::NewRecord::ToJson(Json &json) const
{
	// clang-format off
	return json.Set("player_id", this->playerID)
		&& json.Set("filter_id", this->filterID)
		&& json.Set("mode_md5", this->modeChecksum)
		&& json.Set("time", this->time)
		&& json.Set("styles", this->styles)
		&& json.Set("metadata", this->metadata);
	// clang-format on
}

bool Surf::API::events::NewRecord::StyleInfo::ToJson(Json &json) const
{
	return json.Set("style", this->name) && json.Set("checksum", this->checksum);
}

bool Surf::API::events::NewRecordAck::FromJson(const Json &json)
{
	if (!json.Get("record_id", this->recordId))
	{
		return false;
	}

	Json pbData;

	if (!json.Get("pb_data", pbData))
	{
		return true;
	}

	if (!pbData.Get("player_rating", this->playerRating))
	{
		return false;
	}

	if (pbData.Get("rank", this->overallData.rank))
	{
		if (!pbData.Get("points", this->overallData.points))
		{
			return false;
		}

		if (!pbData.Get("leaderboard_size", this->overallData.leaderboardSize))
		{
			return false;
		}
	}

	return true;
}

bool Surf::API::events::WantWorldRecordsForCache::ToJson(Json &json) const
{
	return json.Set("map_id", this->mapID);
}

bool Surf::API::events::WorldRecordsForCache::FromJson(const Json &json)
{
	return json.Get("records", this->records);
}

bool Surf::API::events::MapDetails::FromJson(const Json &json)
{
	return json.Get("id", this->id) && json.Get("name", this->name);
}

bool Surf::API::events::CourseDetails::FromJson(const Json &json)
{
	if (!(json.Get("id", this->id) && json.Get("name", this->name)))
	{
		return false;
	}

	std::string tier;

	if (!json.Get("tier", tier))
	{
		return false;
	}

	if (!Surf::API::Map::Course::Filter::DecodeTierString(tier, this->tier))
	{
		return false;
	}

	return true;
}

bool Surf::API::events::WantCourseTop::ToJson(Json &json) const
{
	// clang-format off
	return json.Set("map_name", this->mapName)
		&& json.Set("course", this->courseNameOrNumber)
		&& json.Set("mode", (u8)this->mode)
		&& json.Set("limit", this->limit)
		&& json.Set("offset", this->offset);
	// clang-format on
}

bool Surf::API::events::CourseTop::FromJson(const Json &json)
{
	// clang-format off
	return json.Get("map", this->map)
		&& json.Get("course", this->course)
		&& json.Get("overall", this->overall);
	// clang-format on
}

bool Surf::API::events::WantWorldRecords::ToJson(Json &json) const
{
	// clang-format off
	return json.Set("map", this->mapName)
		&& json.Set("course", this->courseNameOrNumber)
		&& json.Set("mode", (u8)this->mode);
	// clang-format on
}

bool Surf::API::events::WorldRecords::FromJson(const Json &json)
{
	// clang-format off
	return json.Get("map", this->map)
		&& json.Get("course", this->course)
		&& json.Get("overall", this->overall);
	// clang-format on
}

bool Surf::API::events::WantPersonalBest::ToJson(Json &json) const
{
	bool success = true;

	if (this->steamid64)
	{
		success &= json.Set("player", this->steamid64);
	}
	else
	{
		success &= json.Set("player", this->playerName);
	}

	success &= json.Set("map", this->mapName);
	success &= json.Set("course", this->courseNameOrNumber);
	success &= json.Set("mode", (u8)this->mode);
	success &= json.Set("styles", this->styles);

	return success;
}

bool Surf::API::events::PersonalBest::FromJson(const Json &json)
{
	// clang-format off
	return json.Get("player", this->player)
		&& json.Get("map", this->map)
		&& json.Get("course", this->course)
		&& json.Get("overall", this->overall);
	// clang-format on
}

bool Surf::API::events::WantPlayerRecords::ToJson(Json &json) const
{
	return json.Set("map_id", this->mapID) && json.Set("player_id", this->playerID);
}

bool Surf::API::events::PlayerRecords::FromJson(const Json &json)
{
	return json.Get("records", this->records);
}
