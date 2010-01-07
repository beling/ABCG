#include "LevelsProvider.h"

#include "libs/osdir.h"

#include <algorithm>

#ifndef LEVELS_DIRECORY
#define LEVELS_DIRECORY "." PATH_SEPARATOR "levels"
#endif

void LevelsProvider::scan() {
	episodes.clear();
	Episode* current;
	oslink::directory epis(LEVELS_DIRECORY);
	while (epis) {
		std::string episode_name = epis.next();
		if (episode_name.empty() || episode_name[0] == '.') continue;
		oslink::directory level(std::string(LEVELS_DIRECORY) + PATH_SEPARATOR + episode_name);
		current = 0;
		while (level) {
			std::string level_name = level.next();
			if (level_name.empty() || level_name[0] == '.') continue;
			if (!current) {
				episodes.push_back(Episode(episode_name));
				current = &episodes.back();
			}
			current->levels.push_back(level_name);
		}
		if (current) //was 1 or more level
			std::sort(current->levels.begin(), current->levels.end());
	}
	std::sort(episodes.begin(), episodes.end());
}

LevelsProvider::LevelsProvider()
{
	scan();
}

bool LevelsProvider::level_exist(int episode_nr, int level_nr) const {
	return episode_nr >= 0 && level_nr >= 0 &&
		   episode_nr < episodes_len() &&
		   level_nr < episode(episode_nr).levels_len();
	
}

std::string LevelsProvider::file_name(int episode_nr, int level_nr) const {
	return std::string(LEVELS_DIRECORY) + PATH_SEPARATOR +
		   episode(episode_nr).name + PATH_SEPARATOR +
		   episode(episode_nr).level(level_nr);
}

int LevelsProvider::next_level_id(int episode_nr, int level_nr) const {
	if (level_exist(episode_nr, level_nr + 1)) return id(episode_nr, level_nr + 1);
	if (level_exist(episode_nr+1, 0)) return id(episode_nr+1, 0);
	return id(0, 0);
}

int LevelsProvider::prev_level_id(int episode_nr, int level_nr) const {
	if (level_exist(episode_nr, level_nr - 1)) return id(episode_nr, level_nr - 1);
	if (episode_nr > 0) return id(episode_nr-1, episode(episode_nr-1).levels_len()-1);
	return id(episodes_len()-1, episode(episodes_len()-1).levels_len()-1);
}


